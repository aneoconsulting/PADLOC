/**
 * \file padloc_operations.cpp
 * \brief Library Manipulation API Sample, part of the Padloc project.
 * 
 * \details This file is dedicated to the extraction of instructions features. \n
 *  We represent instructions features with a flag system defined in the associated header file. \n
 *  In the rest of the program the flags are used in order to retrieve information about instructions.
 * 
 * \author Brasseur Dylan, Teaudors Mickaël, Valeri Yoann
 * \date 2019
 * \copyright Interflop 
 */

#include "padloc_operations.hpp"

/**
 * \def PREFIX_EVEX
 * \brief Mask to detect AVX 512 instructions
 */
#define PREFIX_EVEX 0x000100000

/**
 * \brief Get the flag corresponding to the size of the operands
 * \details This allows us to distinguish between instructions who have the same opcode
 * but different operand sizes
 * 
 * \param instr Instrumented instruction
 * \return [description]
 */
static unsigned int get_size_flag(instr_t *instr){
    int n = instr_num_srcs(instr);
    uint max_size = 0;
    for(int i = 0; i < n; i++){
        uint curr_size = opnd_size_in_bytes(opnd_get_size(instr_get_src(instr, i)));
        if(curr_size > max_size){
            max_size = curr_size;
        }
    }
    switch(max_size){
#if defined(X86)
        case 16:
            return PLC_OP_128;
        case 32:
            return PLC_OP_256;
        case 64:
            return PLC_OP_512;
        default:
            return 0;
#elif defined(AARCH64)
        case 4:
            return PLC_OP_SCALAR | PLC_OP_FLOAT;
        case 8:
            return PLC_OP_SCALAR | PLC_OP_DOUBLE;
        case 16:
            return PLC_OP_PACKED | PLC_OP_128;
        default:
            return 0;
#endif
    }
}

/**
 * \brief Returns the operation category of instr. The operation category specifies the features of the instruction
 * 
 * \param instr Analysed instruction
 * \return Operation category of instr
 * 
 * \warning If AVX_512 instruction is detected, it marks it as unsupported.
 * \todo Add support for AVX-512
 * \todo Add more instructions
 */
enum OPERATION_CATEGORY plc_get_operation_category(instr_t *instr){
#ifdef X86
    if(instr_get_prefix_flag(instr, PREFIX_EVEX))
    {
        return PLC_UNSUPPORTED;
    }
#endif
    switch(instr_get_opcode(instr)){
#ifdef X86
        // ####### SSE SCALAR #######
        case OP_addss: //SSE scalar float add
            return PLC_ADDS_SSE;

        case OP_addsd: //SSE scalar double add
            return PLC_ADDD_SSE;

        case OP_subsd: //SSE scalar double sub
            return PLC_SUBD_SSE;

        case OP_subss: //SSE scalar float sub
            return PLC_SUBS_SSE;

        case OP_mulsd: //SSE scalar double mul
            return PLC_MULD_SSE;

        case OP_mulss: //SSE scalar float mul
            return PLC_MULS_SSE;

        case OP_divsd: //SSE scalar double div
            return PLC_DIVD_SSE;

        case OP_divss: //SSE scalar float div
            return PLC_DIVS_SSE;

        // ####### AVX SCALAR #######
        case OP_vaddss: //AVX scalar float add
            return PLC_ADDS_AVX;

        case OP_vaddsd: //AVX scalar double add
            return PLC_ADDD_AVX;

        case OP_vsubsd: //AVX scalar double sub
            return PLC_SUBD_AVX;

        case OP_vsubss: //AVX scalar float sub
            return PLC_SUBS_AVX;

        case OP_vmulsd: //AVX scalar double mul
            return PLC_MULD_AVX;

        case OP_vmulss: //AVX scalar float mul
            return PLC_MULS_AVX;

        case OP_vdivsd: //AVX scalar double div
            return PLC_DIVD_AVX;

        case OP_vdivss: //AVX scalar float div
            return PLC_DIVS_AVX;

        // ###### SIMD ######
        // ###### SSE ######

        case OP_addps: //SSE packed float add
            return PLC_PADDS_SSE_128;

        case OP_addpd: //SSE packed double add
            return PLC_PADDD_SSE_128;

        case OP_subps: //SSE packed float sub
            return PLC_PSUBS_SSE_128;

        case OP_subpd: //SSE packet double sub
            return PLC_PSUBD_SSE_128;

        case OP_mulps: //SSE packed float mul
            return PLC_PMULS_SSE_128;

        case OP_mulpd: //SSE packet double mul
            return PLC_PMULD_SSE_128;

        case OP_divps: //SSE packed float div
            return PLC_PDIVS_SSE_128;

        case OP_divpd: //SSE packet double div
            return PLC_PDIVD_SSE_128;

        // ###### AVX ######

        case OP_vaddpd: //AVX packed double add
            return (OPERATION_CATEGORY)(PLC_PADDD_AVX | get_size_flag(instr));

        case OP_vaddps: //AVX packed float add
            return (OPERATION_CATEGORY)(PLC_PADDS_AVX | get_size_flag(instr));

        case OP_vsubps: //AVX packed float sub
            return (OPERATION_CATEGORY)(PLC_PSUBS_AVX | get_size_flag(instr));

        case OP_vsubpd: //AVX packet double sub
            return (OPERATION_CATEGORY)(PLC_PSUBD_AVX | get_size_flag(instr));

        case OP_vmulps: //AVX packed float mul
            return (OPERATION_CATEGORY)(PLC_PMULS_AVX | get_size_flag(instr));

        case OP_vmulpd: //AVX packet double mul
            return (OPERATION_CATEGORY)(PLC_PMULD_AVX | get_size_flag(instr));

        case OP_vdivps: //AVX packed float div
            return (OPERATION_CATEGORY)(PLC_PDIVS_AVX | get_size_flag(instr));

        case OP_vdivpd: //AVX packet double div
            return (OPERATION_CATEGORY)(PLC_PDIVD_AVX | get_size_flag(instr));


        // ###### FMA SCALAR ######

        case OP_vfmadd132ss:
            return (OPERATION_CATEGORY)(PLC_A132SS | get_size_flag(instr));

        case OP_vfmadd132sd:
            return (OPERATION_CATEGORY)(PLC_A132SD | get_size_flag(instr));

        case OP_vfmadd213ss:
            return (OPERATION_CATEGORY)(PLC_A213SS | get_size_flag(instr));

        case OP_vfmadd213sd:
            return (OPERATION_CATEGORY)(PLC_A213SD | get_size_flag(instr));

        case OP_vfmadd231ss:
            return (OPERATION_CATEGORY)(PLC_A231SS | get_size_flag(instr));

        case OP_vfmadd231sd:
            return (OPERATION_CATEGORY)(PLC_A231SD | get_size_flag(instr));

        // ###### FMS SCALAR ######

        case OP_vfmsub132ss:
            return (OPERATION_CATEGORY)(PLC_S132SS | get_size_flag(instr));

        case OP_vfmsub132sd:
            return (OPERATION_CATEGORY)(PLC_S132SD | get_size_flag(instr));

        case OP_vfmsub213ss:
            return (OPERATION_CATEGORY)(PLC_S213SS | get_size_flag(instr));

        case OP_vfmsub213sd:
            return (OPERATION_CATEGORY)(PLC_S213SD | get_size_flag(instr));

        case OP_vfmsub231ss:
            return (OPERATION_CATEGORY)(PLC_S231SS | get_size_flag(instr));

        case OP_vfmsub231sd:
            return (OPERATION_CATEGORY)(PLC_S231SD | get_size_flag(instr));

        // ###### FMA PACKED ######

        case OP_vfmadd132ps:
            return (OPERATION_CATEGORY)(PLC_A132PS | get_size_flag(instr));

        case OP_vfmadd132pd:
            return (OPERATION_CATEGORY)(PLC_A132PD | get_size_flag(instr));

        case OP_vfmadd213ps:
            return (OPERATION_CATEGORY)(PLC_A213PS | get_size_flag(instr));

        case OP_vfmadd213pd:
            return (OPERATION_CATEGORY)(PLC_A213PD | get_size_flag(instr));

        case OP_vfmadd231ps:
            return (OPERATION_CATEGORY)(PLC_A231PS | get_size_flag(instr));

        case OP_vfmadd231pd:
            return (OPERATION_CATEGORY)(PLC_A231PD | get_size_flag(instr));

        // ###### FMS PACKED ######

        case OP_vfmsub132ps:
            return (OPERATION_CATEGORY)(PLC_S132PS | get_size_flag(instr));

        case OP_vfmsub132pd:
            return (OPERATION_CATEGORY)(PLC_S132PD | get_size_flag(instr));

        case OP_vfmsub213ps:
            return (OPERATION_CATEGORY)(PLC_S213PS | get_size_flag(instr));

        case OP_vfmsub213pd:
            return (OPERATION_CATEGORY)(PLC_S213PD | get_size_flag(instr));

        case OP_vfmsub231ps:
            return (OPERATION_CATEGORY)(PLC_S231PS | get_size_flag(instr));

        case OP_vfmsub231pd:
            return (OPERATION_CATEGORY)(PLC_S231PD | get_size_flag(instr));

        // ###### Negated FMA SCALAR ######

        case OP_vfnmadd132ss:
            return (OPERATION_CATEGORY)(PLC_NA132SS | get_size_flag(instr));

        case OP_vfnmadd132sd:
            return (OPERATION_CATEGORY)(PLC_NA132SD | get_size_flag(instr));

        case OP_vfnmadd213ss:
            return (OPERATION_CATEGORY)(PLC_NA213SS | get_size_flag(instr));

        case OP_vfnmadd213sd:
            return (OPERATION_CATEGORY)(PLC_NA213SD | get_size_flag(instr));

        case OP_vfnmadd231ss:
            return (OPERATION_CATEGORY)(PLC_NA231SS | get_size_flag(instr));

        case OP_vfnmadd231sd:
            return (OPERATION_CATEGORY)(PLC_NA231SD | get_size_flag(instr));

        // ###### Negated FMS SCALAR ######

        case OP_vfnmsub132ss:
            return (OPERATION_CATEGORY)(PLC_NS132SS | get_size_flag(instr));

        case OP_vfnmsub132sd:
            return (OPERATION_CATEGORY)(PLC_NS132SD | get_size_flag(instr));

        case OP_vfnmsub213ss:
            return (OPERATION_CATEGORY)(PLC_NS213SS | get_size_flag(instr));

        case OP_vfnmsub213sd:
            return (OPERATION_CATEGORY)(PLC_NS213SD | get_size_flag(instr));

        case OP_vfnmsub231ss:
            return (OPERATION_CATEGORY)(PLC_NS231SS | get_size_flag(instr));

        case OP_vfnmsub231sd:
            return (OPERATION_CATEGORY)(PLC_NS231SD | get_size_flag(instr));

        // ###### Negated FMA PACKED ######

        case OP_vfnmadd132ps:
            return (OPERATION_CATEGORY)(PLC_NA132PS | get_size_flag(instr));

        case OP_vfnmadd132pd:
            return (OPERATION_CATEGORY)(PLC_NA132PD | get_size_flag(instr));

        case OP_vfnmadd213ps:
            return (OPERATION_CATEGORY)(PLC_NA213PS | get_size_flag(instr));

        case OP_vfnmadd213pd:
            return (OPERATION_CATEGORY)(PLC_NA213PD | get_size_flag(instr));

        case OP_vfnmadd231ps:
            return (OPERATION_CATEGORY)(PLC_NA231PS | get_size_flag(instr));

        case OP_vfnmadd231pd:
            return (OPERATION_CATEGORY)(PLC_NA231PD | get_size_flag(instr));

        // ###### Negated FMS PACKED ######

        case OP_vfnmsub132ps:
            return (OPERATION_CATEGORY)(PLC_NS132PS | get_size_flag(instr));

        case OP_vfnmsub132pd:
            return (OPERATION_CATEGORY)(PLC_NS132PD | get_size_flag(instr));

        case OP_vfnmsub213ps:
            return (OPERATION_CATEGORY)(PLC_NS213PS | get_size_flag(instr));

        case OP_vfnmsub213pd:
            return (OPERATION_CATEGORY)(PLC_NS213PD | get_size_flag(instr));

        case OP_vfnmsub231ps:
            return (OPERATION_CATEGORY)(PLC_NS231PS | get_size_flag(instr));

        case OP_vfnmsub231pd:
            return (OPERATION_CATEGORY)(PLC_NS231PD | get_size_flag(instr));

#elif defined(AARCH64)
        case OP_fadd:
        return (OPERATION_CATEGORY)(PLC_OP_ADD | get_size_flag(instr));

        case OP_fsub:
            return (OPERATION_CATEGORY)(PLC_OP_SUB | get_size_flag(instr));

        case OP_fmul:
            return (OPERATION_CATEGORY)(PLC_OP_MUL | get_size_flag(instr));

        case OP_fdiv:
            return (OPERATION_CATEGORY)(PLC_OP_DIV | get_size_flag(instr));

        case OP_fmadd:
            return (OPERATION_CATEGORY)(PLC_OP_FMA | get_size_flag(instr));

        case OP_fmsub:
            return (OPERATION_CATEGORY)(PLC_OP_FMS | get_size_flag(instr));
#endif

        default:
            return PLC_OTHER;
    }
}
