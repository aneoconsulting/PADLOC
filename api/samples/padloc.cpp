/**
 * \file padloc.cpp
 * \brief DynamoRIO client developped in the scope of INTERFLOP project
 * \details This file is the sample that DynamoRIO uses to access to our
 * modifications. It is the entry point to the whole PADLOC project.
 * As such, to use this sample on your binary, the simplest command line
 * to use is :
 * "drrun -c api/samples/libpadloc.so -- path/to/binary"
 *
 * \author Dylan Brasseur, Mickaël Teaudors, Yoann Valeri
 * \date 2019
 * \copyright Interflop
 */
#include <vector>
#include <string>

#include "dr_api.h"
#include "drreg.h"
#include "drmgr.h"
#include "drsyms.h"

#include "padloc/utils.hpp"
#include "padloc/padloc_operations.hpp"
#include "padloc/symbol_config.hpp"
#include "padloc/padloc_client.h"
#include "padloc/analyse.hpp"

/**
 * \brief Callback called when a module is loaded
 * 
 * \param drcontext DynamoRIO's context
 * \param module Module that has been loaded
 * \param loaded (Kept for backward compatibility, only set to true) 
 * tells if the module is fully loaded 
 */
static void module_load_handler(void *drcontext, const module_data_t *module,
                                bool loaded){
    /*
     * Sets if we need to instrument the module, based on the 
     * whitelist-blacklist
     */
    dr_module_set_should_instrument(module->handle,
                                    should_instrument_module(module));
}

/**
 * \brief Gather all the DynamoRIO packages and structure initializations
 * \details Contains the initialization of the following packages :
 *  - drsym
 *  - drmgr
 *  - drreg
 *  - Verrou
 */
static void api_initialisation(){
    drsym_init(0);
    drmgr_init();
    //Configure verrou in random rounding mode
    Interflop::verrou_prepare();
    drreg_options_t drreg_options;
    drreg_options.conservative = false;
    drreg_options.num_spill_slots = 1;
    drreg_options.struct_size = sizeof(drreg_options_t);
    drreg_options.do_not_sum_slots = true;
    drreg_options.error_callback = NULL;
    drreg_init(&drreg_options);
}

/**
 * \brief Gather all the TLS registers registration
 * \details Currently register 3 TLS fields, used for result,
 * GPR and FP registers.
 */
static void tls_register(){
    set_index_tls_result(drmgr_register_tls_field());
    set_index_tls_float(drmgr_register_tls_field());
    set_index_tls_gpr(drmgr_register_tls_field());
}

/**
 * \brief Callback called when the program exits
 */
static void event_exit(){
    //Unregistering the tls fields
    drmgr_unregister_tls_field(get_index_tls_result());
    drmgr_unregister_tls_field(get_index_tls_gpr());
    drmgr_unregister_tls_field(get_index_tls_float());
    //If we were generating the symbols, write the results to file
    if(get_symbol_mode() == PLC_SYMBOL_GENERATE){
        write_symbols_to_file();
    }
    //Exiting the api
    drreg_exit();
    drmgr_exit();
    drsym_exit();
    Interflop::verrou_end();
}

/**
 * \brief Callback called when a thread is created
 * 
 * \param dr_context Context of the created thread
 */
static void thread_init(void *dr_context){
    SET_TLS(dr_context, get_index_tls_result(), 
        dr_thread_alloc(dr_context, 8));
    SET_TLS(dr_context, get_index_tls_float(),
            dr_thread_alloc(dr_context,
                            AVX_512_SUPPORTED ? 64 * 32 
                                              : AVX_SUPPORTED ? 32 * 16 
                                                              : 16 * 16));
    SET_TLS(dr_context, get_index_tls_gpr(),
            dr_thread_alloc(dr_context, 17 * 8));
}


/**
 * \brief Callback called when a thread exits
 * \param dr_context Context of the exiting thread
 */
static void thread_exit(void *dr_context){
    dr_thread_free(dr_context, GET_TLS(dr_context, get_index_tls_result()), 8);
    dr_thread_free(dr_context, GET_TLS(dr_context, get_index_tls_float()),
                   AVX_512_SUPPORTED ? 64 * 32 : AVX_SUPPORTED ? 32 * 16 
                                                               : 16 * 16);
    dr_thread_free(dr_context, GET_TLS(dr_context, get_index_tls_gpr()),
                   17 * 8);
}

/**
 * \brief Callback called when a basic block is sent to the code cache
 * \details Used in the first phase of instrumentation
 * 
 * \param drcontext DynamoRIO's context
 * \param tag Unique tag of the basic block
 * \param bb Linked list of instructions of the basic block
 * \param for_trace True if this callback is called for trace creation
 * \param translating True if this callback is called for address translation
 * \return dr_emit_flags_t DR_EMIT_DEFAULT
 */
static dr_emit_flags_t app2app_bb_event(void *drcontext, void *tag,
                                        instrlist_t *bb, bool for_trace,
                                        bool translating){
    instr_t *instr, *next_instr;
    OPERATION_CATEGORY oc;
    //Checks if we need to instrument this basic bloc based on the whitelist-blacklist
    if(!needs_to_instrument(bb)){
        return DR_EMIT_DEFAULT;
    }
    static int nb = 0;
    for(instr = instrlist_first_app(bb); instr != NULL; instr = next_instr){
        oc = plc_get_operation_category(instr);
        bool registers_saved = false;
        bool should_continue = false;

        do{
            next_instr = instr_get_next_app(instr);

            if(plc_is_instrumented(oc)){
                //We need to instrument this instruction
                if(get_log_level() >= 1){
                    dr_printf("%d ", nb);
                    dr_print_instr(drcontext, STDOUT, instr, ": ");
                }
                ++nb;

                bool is_double = plc_is_double(oc);

                //If the registers haven't been saved yet
                if(!registers_saved){
                    insert_save_gpr_and_flags(drcontext, bb, instr);
                    insert_save_simd_registers(drcontext, bb, instr);
                }

                //Make the result tls point to the correct location
                insert_set_destination_tls(drcontext, bb, instr,
                                           GET_REG(DST(instr, 0)));

                //Setup the calling convention
                insert_set_operands(drcontext, bb, instr, instr, oc);

                if(!registers_saved){
                    //If it's the first instrumented instruction of the instrumented block, we need to setup rsp
                    insert_restore_rsp(drcontext, bb, instr);
                    translate_insert(
                            XINST_CREATE_sub(drcontext, OP_REG(DR_REG_XSP),
                                             OP_INT(32)), bb, instr);
                }
                registers_saved = true;
                //Insert the call to the function which corresponds to the instruction
                insert_call(drcontext, bb, instr, oc, is_double);
                oc = plc_get_operation_category(next_instr);
                should_continue = next_instr != nullptr &&
                                  plc_is_instrumented(oc);
                if(!should_continue){
                    //It's not a floating point operation
                    insert_restore_simd_registers(drcontext, bb, instr);
                    insert_restore_gpr_and_flags(drcontext, bb, instr);
                }
                // Remove original instruction
                instrlist_remove(bb, instr);
                instr_destroy(drcontext, instr);
                registers_saved = true;
            }
            instr = next_instr;
        }while(should_continue);
    }
    return DR_EMIT_DEFAULT;
}

/**
 * \brief Callback called when a basic block is sent to the code cache
 * \details Used when generating the symbols list
 * 
 * \param drcontext DynamoRIO's context
 * \param tag Unique tag of the basic block
 * \param bb Linked list of the instructions
 * \param for_trace True if this callback is called for trace creation
 * \param translating True if this callback is called for address translation
 * \param user_data Memory location in which to store data for the next instrumentation phase
 * \return dr_emit_flags_t DR_EMIT_DEFAULT
 */
static dr_emit_flags_t symbol_lookup_event(void *drcontext, void *tag,
                                           instrlist_t *bb, bool for_trace,
                                           bool translating,
                                           OUT void **user_data){
    bool already_found_fp_op = false;

    for(instr_t *instr = instrlist_first_app(bb);
        instr != NULL; instr = instr_get_next_app(instr)){
        OPERATION_CATEGORY oc = plc_get_operation_category(instr);
        if(plc_is_instrumented(oc)){
            if(!already_found_fp_op){
                already_found_fp_op = true;
                //Register the symbol as seen
                log_symbol(bb);
            }
            //We break the loop if we don't need to print all the found instructions
            if(get_log_level() >= 1){
                dr_print_instr(drcontext, STDERR, instr, "Found : ");
            }else{
                break;
            }
        }
    }
    return DR_EMIT_DEFAULT;
}

/**
 * \brief Gather all the DynamoRIO events registration
 * \details Register the :
 *  - exit event
 *  - thread init/exit event
 *  - instrumentation phase's function event
 *  - module load event
 *  - app2app phase's function event
 */
static void api_register(){
    // Define the functions to be called before exiting this client program
    dr_register_exit_event(event_exit);

    drmgr_register_thread_init_event(thread_init);
    drmgr_register_thread_exit_event(thread_exit);

    if(get_symbol_mode() == PLC_SYMBOL_GENERATE){
        drmgr_register_bb_instrumentation_event(symbol_lookup_event, NULL, NULL);
    }else{
        drmgr_register_module_load_event(module_load_handler);
        drmgr_register_bb_app2app_event(app2app_bb_event, NULL);
    }
}

/**
 * \brief Main of the client
 * 
 * \param id ID of the client
 * \param argc Number of client arguments
 * \param argv Array of client arguments
 */
DR_EXPORT void dr_client_main(client_id_t id, // client ID
                              int argc,
                              const char *argv[]){
    api_initialisation();

    /*
     * If the parsing returns true, that means the execution of the program
     * must stop. Either something went wrong, or one of the plugins detected
     * a command line option that should stop the program.
     */
    if(arguments_parser(argc, argv)){
        dr_printf("truc\n");
        dr_abort_with_code(0);
        return;
    }

    api_register();

    tls_register();

    if(get_log_level() >= 2){
        print_register_vectors();
    }else if(get_log_level() == 1){
        print_register_vectors_single();
    }
}