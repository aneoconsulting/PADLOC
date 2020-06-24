/**
	 * DynamoRIO client developped in the scope of INTERFLOP project 
 */

#include "dr_api.h"
#include "drmgr.h"

#include <forward_list>
#include <iostream>
#include <map>

static std::forward_list<reg_id_t> register_list;

static void event_exit(void);

//Function to treat each block of instructions to instrument
static dr_emit_flags_t event_basic_block( void *drcontext,        //Context
										 void *tag,              // Unique identifier of the block
										 instrlist_t *bb,        // Linked list of the instructions 
										 bool for_trace,         //TODO
										 bool translating);      //TODO

static void api_initialisation(){
    drmgr_init();
}

static void api_register(){
    // Define the functions to be called before exiting this client program
    dr_register_exit_event(event_exit);

    drmgr_register_bb_app2app_event(event_basic_block, NULL);
}

// Main function to setup the dynamoRIO client
DR_EXPORT void dr_client_main(  client_id_t id, // client ID
								int argc,   
								const char *argv[])
{
    api_initialisation();

    api_register();
}

static void vect_to_map(std::forward_list<reg_id_t> vect){
    std::map<reg_id_t, int> registers;

    while (!register_list.empty()){
        reg_id_t temp = register_list.front();
        auto it = registers.find(temp);
        if (it == registers.end()){
            registers.insert(std::pair<reg_id_t, int>(temp, 1));
        }else{
            it->second += 1;
        }
        register_list.pop_front();
    }

    std::cout << "List of registers and their use :\n";
    for (auto it=registers.begin(); it!=registers.end(); ++it){
        std::cout << get_register_name(it->first) << " => " << it->second << ",\n";
    }
    std::cout << '\n';
}

static void event_exit(){
    drmgr_exit();

    vect_to_map(register_list);
}

void register_list_add(int reg)
{   
    register_list.push_front((reg_id_t) reg);
}


static dr_emit_flags_t event_basic_block(void *drcontext, void* tag, instrlist_t *bb, bool for_trace, bool translating)
{
    instr_t *instr;

    for(instr = instrlist_first(bb); instr != NULL; instr = instr_get_next(instr))
    {
        //dr_print_instr(drcontext, STDOUT, instr , "before : ");
        int i;
        for(i = 0; i < instr_num_srcs(instr); ++i){
            opnd_t src = instr_get_src(instr, i);
            if(opnd_is_reg(src)){
                if((int)opnd_get_reg(src) != 0){
                    dr_insert_clean_call(drcontext, bb, instr, (void*) register_list_add, 
                        false, 1, OPND_CREATE_INT64((int)opnd_get_reg(src)));
                }
            }else if(opnd_is_base_disp(src)){
                if((int)opnd_get_base(src) != 0){
                    dr_insert_clean_call(drcontext, bb, instr, (void*) register_list_add, 
                        false, 1, OPND_CREATE_INT64((int)opnd_get_base(src)));
                }
                if((int)opnd_get_index(src) != 0){
                    dr_insert_clean_call(drcontext, bb, instr, (void*) register_list_add, 
                        false, 1, OPND_CREATE_INT64((int)opnd_get_index(src)));
                }
            }
        }
        for(i = 0; i < instr_num_dsts(instr); ++i){
            opnd_t dst = instr_get_dst(instr, i);
            if(opnd_is_reg(dst)){
                if((int)opnd_get_reg(dst) != 0){
                    dr_insert_clean_call(drcontext, bb, instr, (void*) register_list_add, 
                        false, 1, OPND_CREATE_INT64((int)opnd_get_reg(dst)));
                }
            }else if(opnd_is_base_disp(dst)){
                if((int)opnd_get_base(dst) != 0){
                    dr_insert_clean_call(drcontext, bb, instr, (void*) register_list_add, 
                        false, 1, OPND_CREATE_INT64((int)opnd_get_base(dst)));
                }
                if((int)opnd_get_index(dst) != 0){
                    dr_insert_clean_call(drcontext, bb, instr, (void*) register_list_add, 
                        false, 1, OPND_CREATE_INT64((int)opnd_get_index(dst)));
                }
            }
        }
        //dr_print_instr(drcontext, STDOUT, instr , "after : ");
    }
    return DR_EMIT_DEFAULT;
}