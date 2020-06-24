/**
 * \file analyse.cpp
 * \brief Backend analysis plugin source file. Part of the PADLOC project.
 *
 * \details This file analyses the current backend used in order to determine the
 * different registers used by it. The goal is to know what needs to be saved
 * by the frontend, so that the backend functions don't modify any register
 * used by the application in an unpredictable way. The registers checked for
 * GPRs and Floating points/SIMD registers.
 * 
 * Also, this file is used to check whether or not the problem regarding
 * implicit operands and their order by DynamoRIO is solved or not.
 * 
 * 
 * \author Brasseur Dylan, Teaudors Mickaël, Valeri Yoann
 * \date 2019
 * \copyright Interflop 
 */

#include <string.h>
#include <algorithm>
#include <fstream>
#include <iostream>

#include "analyse.hpp"
#include "drsyms.h"

#include "utils.hpp"

/**
 * This bool is used to know if DynamoRIO still puts implicit operands
 * at the end of the sources, rather than at their normal place.
 */
static bool NEED_SSE_INVERSE = false;

/**
 * This vector contains addresses and is used so that we don't follow each 
 * and every call when we already checked it once.
 */
static std::vector<app_pc> app_pc_vect;

/**
 * This vector contains the different GPR used by the backend.
 */
static std::vector<reg_id_t> gpr_reg;

/**
 * This vector contains all the XMM/YMM/ZMM used on X86, 
 * or the Q/D/S/B/H on AArch64, used by the backend.
 */
static std::vector<reg_id_t> float_reg;

bool get_need_sse_inverse(){
    return NEED_SSE_INVERSE;
}

void set_need_sse_inverse(bool new_value){
    NEED_SSE_INVERSE = new_value;
}

std::vector<reg_id_t> get_gpr_reg(){
    return gpr_reg;
}

std::vector<reg_id_t> get_float_reg(){
    return float_reg;
}

std::vector<reg_id_t> get_all_registers(){
    std::vector<reg_id_t> ret;

    for(auto reg: gpr_reg){
        if(std::find(ret.begin(), ret.end(), reg)
           == ret.end()){
            ret.push_back(reg);
        }
    }

    for(auto reg: float_reg){
        if(std::find(ret.begin(), ret.end(), reg)
           == ret.end()){
            ret.push_back(reg);
        }
    }

    return ret;
}

/**
 * \brief Helper function to print a certain number of tabs, used as way to
 * know when we follow a call through
 * 
 * \param tabs The number of tabs
 */
static void print_tabs(int tabs){
    for(int i = 0; i < tabs; ++i){
        dr_printf("\t");
    }
}

/**
 * \brief Prints the content of a vector of reg_id_t
 * \details Iterate over a vector of reg_id_t and prints the names
 * of each register, according to their value in the reg_id_t enum.
 * 
 * \param vect The vector to print
 */
static void print_vect(std::vector<reg_id_t> vect){
    for(auto i = vect.begin(); i != vect.end(); ++i){
        /* We use get_register_name to get the name of a register
         * by giving it the function it's number
         */
        dr_printf("%s", get_register_name(*i));
        if(i + 1 != vect.end()){
            dr_printf(", ");
        }
    }
}

void print_register_vectors(){
    dr_printf("List of gpr registers : \n\t");
    print_vect(gpr_reg);
    dr_printf("\nList of float registers : \n\t");
    print_vect(float_reg);
    dr_printf("\n");
}

void print_register_vectors_single(){
    dr_printf("Registers used by backend :\n");
    print_vect(gpr_reg);
    dr_printf(", ");
    print_vect(float_reg);
    dr_printf("\n");
}

/**
 * \brief Adds a reg_id_t to gpr_reg or float_reg according to it's status
 * \details Checks whether a given register is a GPR or a FP register.
 * In both cases, iterate through the corresponding vector and add reg to it
 * if necessary. At any point in time, the vectors are assured to be in one of
 * three states compared to reg :
 *      - a vector contains reg -> don't add reg to the vector
 *      - a vector element overlaps with reg -> remove the element and add reg
 *      - reg overlaps with a vector element -> don't add reg to the vector
 * 
 * \param reg The register to add
 */
static void add_to_vect(reg_id_t reg){
    bool add_reg = true;
    reg_id_t to_remove = DR_REG_NULL;

    /* If the register to add is a GPR */
    if(reg_is_gpr(reg)){
        /* Iterate through the current GPR vector */
        for(auto temp: gpr_reg){
            /* If the register is already in the vector, or if it overlaps
             * (e.g. gpr_reg contains RAX, and we want to add EAX), we don't
             * want to add reg to the vector
             */
            if(reg == temp || reg_overlap(reg, temp)){
                add_reg = false;
                break;
            }else if(reg_overlap(temp, reg)){
                to_remove = temp;
                break;
            }
        }
        /* If one of the register in the vector overlaps with the one
         * we want to add, we remove the former from the vector
         */
        if(to_remove != DR_REG_NULL){
            gpr_reg.erase(
                    std::remove(gpr_reg.begin(), gpr_reg.end(), to_remove),
                    gpr_reg.end());
        }

        /* If the register isn't already in the vector or overlaps with one
         * inside of it, add reg to the vector
         */
        if(add_reg){
            gpr_reg.push_back(reg);
        }

        /* If the register to add is a FP, we do the same as for GPR */
    }else if(reg_is_strictly_xmm(reg) || reg_is_strictly_ymm(reg)
             || reg_is_strictly_zmm(reg)){
        for(auto temp: float_reg){
            if(reg == temp || reg_overlap(reg, temp)){
                add_reg = false;
                break;
            }else if(reg_overlap(temp, reg)){
                to_remove = temp;
                break;
            }
        }
        if(to_remove != DR_REG_NULL){
            float_reg.erase(
                    std::remove(float_reg.begin(), float_reg.end(), to_remove),
                    float_reg.end());
        }
        if(add_reg){
            float_reg.push_back(reg);
        }
    }
}

/**
 * \brief Fill the two vectors according to the registers used by
 * an instruction
 * \details Checks all the sources and destinations of an instruction,
 * verify whether or not each is a register and call add_to_vect if so.
 * 
 * \param instr The instruction which operands are to check
 */
static void fill_reg_vect(instr_t *instr){
    opnd_t operand;
    reg_id_t reg1, reg2;
    int limit;

    limit = instr_num_srcs(instr);
    for(int i = 0; i < limit; ++i){
        reg1 = DR_REG_NULL;
        reg2 = DR_REG_NULL;
        operand = instr_get_src(instr, i);

        /* If the current operand is a register or a base + disp,
         * we get the associated register and call add_to_vect on them
         */
        if(opnd_is_reg(operand)){
            reg1 = opnd_get_reg(operand);
        }else if(opnd_is_base_disp(operand)){
            reg1 = opnd_get_base(operand);
            reg2 = opnd_get_index(operand);
        }
        add_to_vect(reg1);
        add_to_vect(reg2);
    }

    limit = instr_num_dsts(instr);
    for(int i = 0; i < limit; ++i){
        reg1 = DR_REG_NULL;
        reg2 = DR_REG_NULL;
        operand = instr_get_dst(instr, i);

        /* If the current operand is a register or a base + disp,
         * we get the associated register and call add_to_vect on them
         */
        if(opnd_is_reg(operand)){
            reg1 = opnd_get_reg(operand);
        }else if(opnd_is_base_disp(operand)){
            reg1 = opnd_get_base(operand);
            reg2 = opnd_get_index(operand);
        }
        add_to_vect(reg1);
        add_to_vect(reg2);
    }
}

/**
 * \brief Iterate through the instructions of a backend symbol to add the
 * registers used by each instruction to one of the two register vectors.
 * \details Recursive function which decodes the symbol at 
 * lib_data->start + offset as a list of instructions, iterate through it 
 * and call fill_reg_vect with each instruction. 
 * Also, follows each call in the symbol if the target app_pc
 * is not already in the app_pc vector, effectively instrumenting all the
 * functions used by the backend.
 * 
 * \param drcontext The current context
 * \param lib_data The module data corresponding to the backend library
 * \param offset The offset of a symbol from the start of the library
 * \param tabs The tabulations for the current function, used for display
 * purposes
 */
static void show_instr_of_symbols(void *drcontext, module_data_t *lib_data,
                                  size_t offset, int tabs){
    /* Decodes the current symbol as a block of instructions */
    instrlist_t *list_bb = decode_as_bb(drcontext, lib_data->start + offset);
    instr_t *instr = nullptr, *next = nullptr;
    app_pc apc = 0;

    for(instr = instrlist_first_app(list_bb); instr != NULL; instr = next){
        next = instr_get_next_app(instr);
        if(get_log_level() >= 3){
            print_tabs(tabs);
            dr_print_instr(drcontext, STDOUT, instr, "ENUM_SYMBOLS : ");
        }

        /* Fill the two vectors with the current instruction */
        fill_reg_vect(instr);

        /* If the instruction is a jump, it is also the last instruction of
         * of the block, so if we stop there, the end of the symbol won't be
         * instrumented. That means that have to skip the jump to continue 
         * instrumenting, i.e we call show_instr_of_symbols with a new offset
         * equal to : 
         * current instruction's app_pc - app_pc of the start of the library
         *                              + length of the current instruction
         * This sum will actually put the offset at the instruction following
         * the jump.
         */
        if(instr_is_ubr(instr) || instr_is_cbr(instr) ||
           instr_get_opcode(instr) == OP_jmp_ind ||
           instr_get_opcode(instr) == OP_jmp_far_ind){
            show_instr_of_symbols(drcontext, lib_data,
                                  instr_get_app_pc(instr)
                                  - lib_data->start
                                  + instr_length(drcontext, instr), tabs);
        }

        /* If the instruction is a call, we have to follow it to it's
         * destination so that we can instrument it too.
         * Therefore, we have to get the operand and get the app_pc associated.
         * If we already followed and went to the call target, that means we
         * already instrumented the destination, and it is necessary to do
         * so again. So to ensure that we don't check the same destination
         * twice throughout the analysis, we add the target app_pc to a vector
         * and check it when trying to follow a call.
         */
        if(instr_is_call(instr)){
            apc = opnd_get_pc(instr_get_src(instr, 0));
            if(std::find(app_pc_vect.begin(), app_pc_vect.end(), apc)
               == app_pc_vect.end()){
                /* If the target hasn't been checked already, 
                 * add it to the vector.
                 */
                app_pc_vect.push_back(apc);
                if(get_log_level() >= 3){
                    print_tabs(tabs);
                    dr_printf("Add the app_pc = %p to the vector\n", apc);
                }
                /* We have to follow the call, so we call show_instr_of_symbols
                 * with a new offset of :
                 * destination app_pc - app_pc of the start of the library
                 * This simply put us at the start of the call symbol.
                 */
                show_instr_of_symbols(drcontext, lib_data,
                                      apc - lib_data->start, tabs + 1);
            }else{
                if(get_log_level() >= 3){
                    print_tabs(tabs);
                    dr_printf("The app_pc = %p has already been checked\n",
                              apc);
                }
            }

            /* In the same way to have to call show_instr_of_symbols again
             * when there is a jump to instrument the rest of the symbol,
             * we also have to do it when a call is encountered, as it also
             * finish the instruction block
             */
            show_instr_of_symbols(drcontext, lib_data,
                                  instr_get_app_pc(instr)
                                  - lib_data->start
                                  + instr_length(drcontext, instr), tabs);
        }
    }
    /* When using decode_as_bb, we are to destroy the returned list */
    instrlist_clear_and_destroy(drcontext, list_bb);
}

/**
 * \brief Iterate through the function test_sse_src_order at the end of this 
 * file to check whether or not the implicit operands order problem of
 * DynamoRIO still exists or not.
 * \details Go through all the instructions of the test_sse_src_order
 * function until we find the divpd and vdivpd instructions. When found,
 * get the first operand and see if the implicit operands problem still exists.
 * The problem is the following : when an instruction has it's destination also
 * used as source, DynamoRIO considers the source part to be implicit and puts
 * it at the end of the sources of the instruction, effectively changing the
 * orders in which we have to consider each instruction. However, that
 * modification is not made when the destination is not a source too.
 * That is pretty disturbing in our case because SSE instructions have
 * the destination part also work as source, while AVX instructions explicitely
 * indicates all the sources and destination separately.
 * 
 * \param drcontext The current context
 * \param lib_data The module data corresponding to the backend library
 * \param offset The offset of a symbol from the start of the library
 */
static void analyse_symbol_test_sse_src(void *drcontext, module_data_t *lib_data, size_t offset){
    /* Decode the symbol as a list of instructions */
    instrlist_t *list_bb = decode_as_bb(drcontext, lib_data->start + offset);
    instr_t *instr = nullptr, *next_instr = nullptr;
    reg_id_t reg_src0_instr0 = DR_REG_NULL, reg_src0_instr1 = DR_REG_NULL;

    for(instr = instrlist_first_app(list_bb); instr != NULL; instr = next_instr){
        next_instr = instr_get_next_app(instr);
        /* If the instruction is a divpd or vdivpd, we get the first source */
        if(instr_get_opcode(instr) == OP_divpd){
            reg_src0_instr0 = opnd_get_reg(instr_get_src(instr, 0));
        }else if(instr_get_opcode(instr) == OP_vdivpd){
            reg_src0_instr1 = opnd_get_reg(instr_get_src(instr, 0));
        }
    }

    if(reg_src0_instr0 != DR_REG_NULL && reg_src0_instr1 != DR_REG_NULL){
        /* If the two sources we got are different, that means the problem
         * still exists, meaning we'll have to change the order when
         * modifying the instructions.
         */
        if(reg_src0_instr0 != reg_src0_instr1){
            if(get_log_level() > 2){
                /* Over the top warning */
                dr_fprintf(STDERR,
                           "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"
                           "!!!!!!!!!!!!! WARNING : SSE sources order is still incorrect in DynamoRIO, so we need to invert them !!!!!!!!!!!!!\n"
                           "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            }
            /* We will have to modify the order, so set the bool at true */
            set_need_sse_inverse(true);
        }
    }

    instrlist_clear_and_destroy(drcontext, list_bb);
}

/**
 * \brief Writes a vector to an output file.
 * \details Writes a vector of registers to the given output file,
 * in order to know which registers are used by the backend.
 * 
 * \param analyse_file The file in which to write the registers
 * \param vect The vectors of registers to write
 * \param vect_type The type of vector we're trying to write (either 
 * "float_reg" or "gpr_reg")
 */
static void write_vect(std::ofstream &analyse_file, std::vector<reg_id_t> vect,
                       const char *vect_type){
    analyse_file << vect_type << '\n';
    for(auto i = vect.begin(); i != vect.end(); ++i){
        /* Write the registers as ushort because that's the type of the enum
         * used by DynamoRIO for reg_id_t. There is no function to pass from
         * a char* to a reg_id_t, so we store each registers as their value in
         * the enum.
         */
        analyse_file << ((ushort)*i) << '\n';
    }
}

/**
 * \brief Write the two vectors of registers to the given file path.
 * \details Open the path given as an output file, and writes the vectors
 * of registers to it.
 * 
 * \param path The path of the file we want to write to, can be absolute or
 * relative.
 */
static void write_reg_to_file(const char *path){
    std::ofstream analyse_file;
    analyse_file.open(path);
    if(analyse_file.fail()){
        dr_fprintf(STDERR, "FAILED TO OPEN THE GIVEN FILE FOR WRITING : \"%s\"\n",
                   path);
    }else{
        /* Write the two vectors to the file, flush and close */
        write_vect(analyse_file, gpr_reg, "gpr_reg");
        write_vect(analyse_file, float_reg, "float_reg");
        analyse_file.flush();
        analyse_file.close();
    }
}

/**
 * \brief Read the content of an input file given by the path, and
 * populate the two vectors of registers.
 * \details Open the given path as an input file, reads each line and
 * populate the two vectors with each values.
 * 
 * \param path The path to the input file we read from.
 * \return True if we have to stop the execution of the program because of
 * a failure, or false if everything went smoothly.
 * \todo In the "read_reg_from_file" function, when we detect an number,
 * check if it is in the possible values for GPR and FP registers.
 */
static bool read_reg_from_file(const char *path){
    std::ifstream analyse_file;
    std::string buffer;
    int line_number = 0;
    bool float_vect = false;

    analyse_file.open(path);
    if(analyse_file.fail()){
        dr_fprintf(STDERR, "FAILED TO OPEN THE GIVEN FILE FOR READING : \"%s\"\n",
                   path);
        return true;
    }
    /* The file is supposed to be of the form :
     * "A
     * numbers
     * B
     * numbers"
     * With A being either "gpr_reg" or "float_reg" and B the other.
     * The numbers are the values which will be used to populate the vectors
     * by convertings them to reg_id_t.
     */
    while(std::getline(analyse_file, buffer)){
        if(get_log_level() > 2){
            std::cout << buffer << '\n';
        }
        /* If the line is "gpr_reg", the following numbers will populate
         * the gpr_reg vector. If it is "float_reg", they will populate
         * float_reg. If the line is a number, add it's cast to reg_id_t 
         * to the current vector selected. Else, the line is wrong, so we
         * close the file and quit.
         */
        if(buffer == "gpr_reg"){
            float_vect = false;
        }else if(buffer == "float_reg"){
            float_vect = true;
        }else if(is_number(buffer)){
            (float_vect ? float_reg : gpr_reg).push_back((reg_id_t)std::stoi(buffer));
        }else{
            dr_fprintf(STDERR, "FAILED TO CORRECTLY READ THE FILE : Problem on file line %d = \"%s\"\n",
                       line_number, buffer);
            analyse_file.close();
            return true;
        }
        ++line_number;
    }
    /* Everything went fine, so we close the file. Note that we assume the
     * registers in the vectors are those we have to save for the current
     * backend, so it is still possible that they're not the right registers.
     */
    analyse_file.close();
    return false;
}

/**
 * \brief Callback function that check if a symbol is one we're interested in
 * \details Callback function that will compare the given name of the symbol
 * to one we want to check. If it is, we will start checking all the
 * instructions in it, in order to populate the vectors.
 * 
 * \param name The name of the symbol
 * \param modoffs The offset of the sybol from the start of the library
 * \param data Eventual data passed from the function that does the callback
 * \return True if we want to continue going through the symbols
 */
bool enum_symbols_registers(const char *name, size_t modoffs, void *data){
    void *drcontext = nullptr;
    module_data_t *lib_data = nullptr;

    std::string str(name);
    /* We check whether the string contains "<>::apply" because that's the
     * name of the function we insert with dr_insert_call later. Therefore,
     * we consider it as our entry point to the backend, because it is the 
     * first function that shouldn't be called in the application.
     */
    if(str.find("<>::apply") != std::string::npos){
        drcontext = dr_get_current_drcontext();
        lib_data = dr_lookup_module_by_name("libpadloc.so");
        show_instr_of_symbols(drcontext, lib_data, modoffs, 0);
        /* As per DynamoRIO's API, we must free the module data */
        dr_free_module_data(lib_data);
    }
    return true;
}

/**
 * \brief Callback function that check whether or not the symbol is
 * test_sse_src_order and instrument it if so.
 * 
 * \param name The name of the symbol
 * \param modoffs The offset of the sybol from the start of the library
 * \param data Eventual data passed from the function that does the callback
 * \return True if we want to continue going through the symbols
 */
bool enum_symbols_sse(const char *name, size_t modoffs, void *data){
    void *drcontext = nullptr;
    module_data_t *lib_data = nullptr;

    std::string str(name);

    if(str.find("test_sse_src_order") != std::string::npos){
        drcontext = dr_get_current_drcontext();
        lib_data = dr_lookup_module_by_name("libpadloc.so");
        analyse_symbol_test_sse_src(drcontext, lib_data, modoffs);
        dr_free_module_data(lib_data);
        return false;
    }

    return true;
}

/**
 * \brief Get the current directory, assumed to be "dynamorio/build" and
 * appends the path to the library to it.
 * 
 * \param path A placeholder char*
 * \param length The length of the char*
 */
static void path_to_library(char *path, size_t length){
    module_data_t* mdata = dr_lookup_module_by_name("libpadloc.so");
    strncpy(path, mdata->full_path, length);
    dr_free_module_data(mdata);
}

/**
 * \brief Analyse the backend and put the content of the vectors in the
 * given file path/
 * \details Enumerate through all the symbols of the client, checking for
 * the entry to the backend function, then checking all the instructions to
 * add the registers to the vectors.
 * 
 * \param file The path of the output file
 */
static void AA_argument_detected(const char *file){
    char path[256];
    path_to_library(path, 256);
    if(drsym_enumerate_symbols(path, enum_symbols_registers,
                               NULL, DRSYM_DEFAULT_FLAGS) == DRSYM_SUCCESS){
        write_reg_to_file(file);
    }else{
        dr_fprintf(STDERR,
                   "ANALYSE FAILURE : Couldn't finish analysing the symbols of the library\n");
    }
}

bool analyse_argument_parser(std::string arg, int *i, int argc, const char *argv[]){
    if(arg == "--analyse_abort" || arg == "-aa"){
        *i += 1;
        if(*i < argc){
            /* The analyse and abort option was detected along with 
             * what may be a file, so we call AA_argument_detected 
             * and stop the execution.
             */
            AA_argument_detected(argv[*i]);
            return true;
        }else{
            dr_fprintf(STDERR,
                       "ANALYSE FAILURE : File not given for \"-aa\"\n");
            return true;
        }
    }else if(arg == "--analyse_file" || arg == "-af"){
        *i += 1;
        if(*i < argc){
            /* The analyse from file option was detected, so we call
             * read_reg_from_file with what may be a file, and return the
             * result. Also update the analyse mode so that we don't
             * analyse the backend on our side, overwriting what we got red
             * from the file.
             */
            set_analyse_mode(PLC_ANALYSE_NOT_NEEDED);
            return read_reg_from_file(argv[*i]);
        }else{
            dr_fprintf(STDERR,
                       "ANALYSE FAILURE : File not given for \"-af\"\n");
            return true;
        }
    }else if(arg == "--analyse_run" || arg == "-ar"){
        /* Do nothing, because the analysis will be done later. */
        return false;
    }else{
        /* If the argument is not one we know, increment the error counter */
        inc_error();
    }
    return false;
}

void analyse_mode_manager(){
    char path[256];
    path_to_library(path, 256);

    switch(get_analyse_mode()){
        case PLC_ANALYSE_NEEDED:
            /* Analyse the backend and update the vectors */
            if(drsym_enumerate_symbols(path, enum_symbols_registers,
                                       NULL, DRSYM_DEFAULT_FLAGS) != DRSYM_SUCCESS){
                DR_ASSERT_MSG(false,
                              "ANALYSE FAILURE : Couldn't finish analysing the backend\n");
            }
            break;
        default:
            break;
    }
#if defined(WINDOWS)
    if(get_log_level() >= 2)
    {
        dr_printf("WARNING : Assuming SSE instructions have their operands inverted, since we have no way to test this on Windows !\n");
    }
    set_need_sse_inverse(true);
#else
    /* Analyse the test_sse_src_order to update NEED_SSE_INVERSE accordingly */
    if(drsym_enumerate_symbols(path, enum_symbols_sse,
                               NULL, DRSYM_DEFAULT_FLAGS) != DRSYM_SUCCESS){
        dr_fprintf(STDERR,
                   "ANALYSE FAILURE : Couldn't finish analysing the symbols of the library\n");
    }
#endif
}

/**
 * \brief Function used to check if the implicit operands problem
 * was corrected or not.
 * \warning We assume the syntax used is ATT, since DynamoRIO can't compile if the one
 * used by the compiler is intel's.
 */
void test_sse_src_order(){
#if defined(WINDOWS)
    /* Inline assembly isn't available on MSVC in 64 bits mode, and
     * intrinsics can't help us in this.
     */
#elif defined(AARCH64)
    /*
     * TODO the assembly for AArch64
     */
#else
    __asm__ volatile(
    "\t.intel_syntax;\n"    /* We assume the syntax to be ATT */
    "\tdivpd %xmm0, %xmm1;\n"
    "\tvdivpd %xmm0, %xmm0, %xmm1;\n"
    "\t.att_syntax;\n"  /* Set the syntax back to ATT */
    );
#endif
}