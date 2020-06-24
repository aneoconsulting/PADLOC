#ifndef ANALYSE_BARRIER_HEADER
#define ANALYSE_BARRIER_HEADER

/**
 * \file analyse.hpp
 * \brief Backend analysis plugin header. Part of the PADLOC project.
 * 
 * \author Brasseur Dylan, Teaudors Mickaël, Valeri Yoann
 * \date 2019
 * \copyright Interflop
 */

#include "dr_api.h"
#include "drreg.h"

#include <vector>
#include <string>

/**
 * \brief Getter for the NEED_SSE_INVERSE boolean
 * \return NEED_SSE_INVERSE
 */
bool get_need_sse_inverse();

/**
 * \brief Setter for the NEED_SSE_INVERSE boolean
 * 
 * \param new_value The new value for NEED_SSE_INVERSE
 */
void set_need_sse_inverse(bool new_value);

/**
 * \brief Getter for the gpr_reg vector
 * \return gpr_reg
 */
std::vector<reg_id_t> get_gpr_reg();

/**
 * \brief Getter for the float_reg vector
 * \return float_reg
 */
std::vector<reg_id_t> get_float_reg();

/**
 * \brief Gather the gpr_reg and float_reg vectors into a single one,
 * containing all registers used by the backend
 * \return The combination of both vectors
 * \todo Actually use the vectors when we save the registers, instead
 * of saving everything.
 */
std::vector<reg_id_t> get_all_registers();

/**
 * \brief Prints the content of the gpr_reg and float_reg
 * register vectors independently, using print_vect for each.
 */
void print_register_vectors();

/**
 * \brief Prints the content of the gpr_reg and float_reg
 * register vectors at the same time, using print_vect for each.
 */
void print_register_vectors_single();

/**
 * \brief Parser for the command line
 * \details The options are : 
 *      - "analyse and abort", to analyse the backend, write the registers
 *      to a given file and abort
 *      - "analyse from file", to read an input file and parse it to populate
 *      the vectors
 *      - "analyse and run", to analyse the backend and run the program
 *      normaly. Default option.
 * 
 * \param arg The current argument as string
 * \param i The index of the current argument, given as pointer to be modified
 * if necessary when checking for an option with special parameters
 * \param argc The length of the command line
 * \param argv The list of arguments in the command line
 * \return True if the execution of the program must be stopped, else false
 */
bool analyse_argument_parser(std::string arg, int *i, int argc,
                             const char *argv[]);

/**
 * \brief Analyse the backend if needed, and update the NEED_SSE_INVERSE
 * boolean accordingly
 * \details Call drsym_enumerate_symbols to analyse the backend if needed,
 * updating the vectors of registers. Also call drsym_enumerate_symbols
 * with the SSE analyser, in order to update NEED_SSE_INVERSE.
 * \todo Add sanity checks to verify if the result of functions is good,
 * instead of assuming they are (e.g, control that "dr_lookup_module_by_name"
 * returns no error).
 */
void analyse_mode_manager();

#endif