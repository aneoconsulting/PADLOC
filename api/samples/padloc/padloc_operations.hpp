#ifndef PADLOC_OPERATION_HEADER
#define PADLOC_OPERATION_HEADER

/**
 * \file padloc_operations.hpp
 * \brief Library Manipulation API Sample, part of the Padloc project.
 * 
 * \details This file is dedicated to the extraction of instructions features. \n
 *  We represent instructions features with a flag system defined in this file \n
 * 
 * \author Brasseur Dylan, Teaudors Mickaël, Valeri Yoann
 * \date 2019
 * \copyright Interflop 
 */

#include "dr_api.h"
#include "dr_ir_opcodes.h"

/*
 * The following is a list of flags caracterizing the operations we want to detect.
 * The flags allow us to detect the type of the instruction, the size, the operation, 
 * the order of operations if we detect a FMA/FMS, and the size of the SIMD instruction.
 * The other flags are masks to easily detect the operation, the type and the SIMD size.
*/

/**
 * \def PLC_OP_OTHER
 * \brief Flag for other operations, those that don't concern us
 */
#define PLC_OP_OTHER        0           // 0b00000000000000000		  

/**
 * \def PLC_OP_DOUBLE
 * \brief Flag for operations between doubles
 */
#define PLC_OP_DOUBLE       1           // 0b00000000000000001		  

/**
 * \def PLC_OP_FLOAT
 * \brief Flag for operations between floats
 */
#define PLC_OP_FLOAT        2           // 0b00000000000000010		  

/**
 * \def PLC_OP_PACKED
 * \brief Flag for operations between vectors
 */
#define PLC_OP_PACKED       4           // 0b00000000000000100

/**
 * \def PLC_OP_SCALAR
 * \brief Flag for operations between scalars
 */
#define PLC_OP_SCALAR       8           // 0b00000000000001000		  

/**
 * \def PLC_OP_ADD
 * \brief Flag for addition
 */
#define PLC_OP_ADD          16          // 0b00000000000010000		  

/**
 * \def PLC_OP_SUB
 * \brief Flag for substraction
 */
#define PLC_OP_SUB          32          // 0b00000000000100000		

/**
 * \def PLC_OP_MUL
 * \brief Flag for multiplication
 */
#define PLC_OP_MUL          64          // 0b00000000001000000		 

/**
 * \def PLC_OP_DIV
 * \brief Flag for division
 */
#define PLC_OP_DIV          128         // 0b00000000010000000		  

/**
 * \def PLC_OP_FMA
 * \brief Flag for fused Multiply Add : a * b + c
 */

#define PLC_OP_FMA          256        // 0b00000000100000000		  

/**
 * \def PLC_OP_FMS
 * \brief Flag for fused Multiply Sub : a * b - c
 */
#define PLC_OP_FMS          512        // 0b00000001000000000		  

/**
 * \def PLC_OP_132
 * \brief Flag for 132 order of FMA/FMS : a * c (+|-) b
 */
#define PLC_OP_132          1024       // 0b00000010000000000
/**
 * \def PLC_OP_213
 * \brief Flag for 213 order of FMA/FMS : b * a (+|-) c
 */
#define PLC_OP_213          2048       // 0b00000100000000000		

/**
 * \def PLC_OP_231
 * \brief Flag for 231 order of FMA/FMS : b * c (+|-) a
 */
#define PLC_OP_231          4096       // 0b00001000000000000		  

/**
 * \def PLC_OP_NEG
 * \brief Flag for negated FMA/FMS
 */
#define PLC_OP_NEG          8192       // 0b00010000000000000		 

/**
 * \def PLC_OP_128
 * \brief Flag for operations between vectors of length 128
 */
#define PLC_OP_128          16384      // 0b00100000000000000		  
/**
 * \def PLC_OP_256
 * \brief Flag for operations between vectors of length 256
 */
#define PLC_OP_256          32768      // 0b01000000000000000	

/**
 * \def PLC_OP_512
 * \brief Flag for operations between vectors of length 512
 */
#define PLC_OP_512          65536      // 0b10000000000000000		

/**
 * \def PLC_OP_MASK
 * \brief Mask to get the operation
 * \details Get information about the type of operation (add, sub ...) and the size of the operation (128 ...)
 */
#define PLC_OP_MASK         131056     // 0b11111111111110000		 

/**
 * \def PLC_OP_TYPE_MASK
 * \brief Mask to get the type of the operation.
 * \details Get information about the type of operation (add, sub ...) and NOT the size of the operation (128 ...)
 */
#define PLC_OP_TYPE_MASK    16368      // 0b00011111111110000

/**
 * \def PLC_SIMD_TYPE_MASK
 * \brief Mask to get the size of the operation.
 * \details Get information about the size of the operation (128 ...) and NOT the type of operation (add, sub ...) 
 */
#define PLC_SIMD_TYPE_MASK  114688     // 0b11100000000000000		  

/**
 * \def PLC_OP_FUSED
 * \brief Flag for fused operations, namely FMA and all the supported variations
 */
#define PLC_OP_FUSED        768        // 0b00000001100000000

/**
 * \def PLC_OP_SSE
 * \brief Flag for SSE instructions
 */
#define PLC_OP_SSE          131072     // 0b0100000000000000000

/**
 * \def PLC_OP_AVX
 * \brief Flag for AVX instructions
 */
#define PLC_OP_AVX          262144     // 0b1000000000000000000

/**
 * \def PLC_OP_UNSUPPORTED
 * \brief Flag for operations that are unsupported for the moment
 */
#define PLC_OP_UNSUPPORTED PLC_OP_OTHER

/**
 * \enum SIMD_CATEGORY
 * \brief This enum gives the length of operation we are modifying.
 */
enum SIMD_CATEGORY{
    PLC_SCALAR,
    PLC_128,
    PLC_256,
    PLC_512
};

/**
 * \enum INSTR_CATEGORY
 * \brief This enum gives the type of instruction we are modyfing.
 */
enum INSTR_CATEGORY{
    PLC_SSE,
    PLC_AVX,
};

/**
 * \enum OPERATION_CATEGORY
 * \brief This enum gathers all the possible operations we want to detect, as a combination of flags.
 */
enum OPERATION_CATEGORY{
    PLC_OTHER = PLC_OP_OTHER,
    PLC_UNSUPPORTED = PLC_OP_UNSUPPORTED,

    /* ****************** */
    /* ***** SCALAR ***** */
    /* ****************** */

    // Scalar Float SSE instructions
            PLC_ADDS_SSE = PLC_OP_FLOAT | PLC_OP_SCALAR | PLC_OP_ADD | PLC_OP_SSE,
    PLC_SUBS_SSE = PLC_OP_FLOAT | PLC_OP_SCALAR | PLC_OP_SUB | PLC_OP_SSE,
    PLC_MULS_SSE = PLC_OP_FLOAT | PLC_OP_SCALAR | PLC_OP_MUL | PLC_OP_SSE,
    PLC_DIVS_SSE = PLC_OP_FLOAT | PLC_OP_SCALAR | PLC_OP_DIV | PLC_OP_SSE,

    // Scalar Float AVX instructions
            PLC_ADDS_AVX = PLC_OP_FLOAT | PLC_OP_SCALAR | PLC_OP_ADD | PLC_OP_AVX,
    PLC_SUBS_AVX = PLC_OP_FLOAT | PLC_OP_SCALAR | PLC_OP_SUB | PLC_OP_AVX,
    PLC_MULS_AVX = PLC_OP_FLOAT | PLC_OP_SCALAR | PLC_OP_MUL | PLC_OP_AVX,
    PLC_DIVS_AVX = PLC_OP_FLOAT | PLC_OP_SCALAR | PLC_OP_DIV | PLC_OP_AVX,

    // Scalar Double SSE instructions
            PLC_ADDD_SSE = PLC_OP_DOUBLE | PLC_OP_SCALAR | PLC_OP_ADD | PLC_OP_SSE,
    PLC_SUBD_SSE = PLC_OP_DOUBLE | PLC_OP_SCALAR | PLC_OP_SUB | PLC_OP_SSE,
    PLC_MULD_SSE = PLC_OP_DOUBLE | PLC_OP_SCALAR | PLC_OP_MUL | PLC_OP_SSE,
    PLC_DIVD_SSE = PLC_OP_DOUBLE | PLC_OP_SCALAR | PLC_OP_DIV | PLC_OP_SSE,

    // Scalar Double AVX instructions
            PLC_ADDD_AVX = PLC_OP_DOUBLE | PLC_OP_SCALAR | PLC_OP_ADD | PLC_OP_AVX,
    PLC_SUBD_AVX = PLC_OP_DOUBLE | PLC_OP_SCALAR | PLC_OP_SUB | PLC_OP_AVX,
    PLC_MULD_AVX = PLC_OP_DOUBLE | PLC_OP_SCALAR | PLC_OP_MUL | PLC_OP_AVX,
    PLC_DIVD_AVX = PLC_OP_DOUBLE | PLC_OP_SCALAR | PLC_OP_DIV | PLC_OP_AVX,

    /* ****************** */
    /* ***** PACKED ***** */
    /* ****************** */

    // Packed float SSE
            PLC_PADDS_SSE = PLC_OP_FLOAT | PLC_OP_PACKED | PLC_OP_ADD | PLC_OP_SSE,
    PLC_PSUBS_SSE = PLC_OP_FLOAT | PLC_OP_PACKED | PLC_OP_SUB | PLC_OP_SSE,
    PLC_PMULS_SSE = PLC_OP_FLOAT | PLC_OP_PACKED | PLC_OP_MUL | PLC_OP_SSE,
    PLC_PDIVS_SSE = PLC_OP_FLOAT | PLC_OP_PACKED | PLC_OP_DIV | PLC_OP_SSE,

    //Packed double SSE
            PLC_PADDD_SSE = PLC_OP_DOUBLE | PLC_OP_PACKED | PLC_OP_ADD | PLC_OP_SSE,
    PLC_PSUBD_SSE = PLC_OP_DOUBLE | PLC_OP_PACKED | PLC_OP_SUB | PLC_OP_SSE,
    PLC_PMULD_SSE = PLC_OP_DOUBLE | PLC_OP_PACKED | PLC_OP_MUL | PLC_OP_SSE,
    PLC_PDIVD_SSE = PLC_OP_DOUBLE | PLC_OP_PACKED | PLC_OP_DIV | PLC_OP_SSE,

    // Packed float AVX
            PLC_PADDS_AVX = PLC_OP_FLOAT | PLC_OP_PACKED | PLC_OP_ADD | PLC_OP_AVX,
    PLC_PSUBS_AVX = PLC_OP_FLOAT | PLC_OP_PACKED | PLC_OP_SUB | PLC_OP_AVX,
    PLC_PMULS_AVX = PLC_OP_FLOAT | PLC_OP_PACKED | PLC_OP_MUL | PLC_OP_AVX,
    PLC_PDIVS_AVX = PLC_OP_FLOAT | PLC_OP_PACKED | PLC_OP_DIV | PLC_OP_AVX,

    //Packed double AVX
            PLC_PADDD_AVX = PLC_OP_DOUBLE | PLC_OP_PACKED | PLC_OP_ADD | PLC_OP_AVX,
    PLC_PSUBD_AVX = PLC_OP_DOUBLE | PLC_OP_PACKED | PLC_OP_SUB | PLC_OP_AVX,
    PLC_PMULD_AVX = PLC_OP_DOUBLE | PLC_OP_PACKED | PLC_OP_MUL | PLC_OP_AVX,
    PLC_PDIVD_AVX = PLC_OP_DOUBLE | PLC_OP_PACKED | PLC_OP_DIV | PLC_OP_AVX,

    /* ****************** */
    /* **** 128 size **** */
    /* ****************** */

    // Packed float 128 bits SSE
            PLC_PADDS_SSE_128 = PLC_PADDS_SSE | PLC_OP_128,
    PLC_PSUBS_SSE_128 = PLC_PSUBS_SSE | PLC_OP_128,
    PLC_PMULS_SSE_128 = PLC_PMULS_SSE | PLC_OP_128,
    PLC_PDIVS_SSE_128 = PLC_PDIVS_SSE | PLC_OP_128,

    // Packed float 128 bits AVX
            PLC_PADDS_AVX_128 = PLC_PADDS_AVX | PLC_OP_128,
    PLC_PSUBS_AVX_128 = PLC_PSUBS_AVX | PLC_OP_128,
    PLC_PMULS_AVX_128 = PLC_PMULS_AVX | PLC_OP_128,
    PLC_PDIVS_AVX_128 = PLC_PDIVS_AVX | PLC_OP_128,

    // Packed double 128 bits SSE
            PLC_PADDD_SSE_128 = PLC_PADDD_SSE | PLC_OP_128,
    PLC_PSUBD_SSE_128 = PLC_PSUBD_SSE | PLC_OP_128,
    PLC_PMULD_SSE_128 = PLC_PMULD_SSE | PLC_OP_128,
    PLC_PDIVD_SSE_128 = PLC_PDIVD_SSE | PLC_OP_128,

    // Packed double 128 bits AVX
            PLC_PADDD_AVX_128 = PLC_PADDD_AVX | PLC_OP_128,
    PLC_PSUBD_AVX_128 = PLC_PSUBD_AVX | PLC_OP_128,
    PLC_PMULD_AVX_128 = PLC_PMULD_AVX | PLC_OP_128,
    PLC_PDIVD_AVX_128 = PLC_PDIVD_AVX | PLC_OP_128,

    /* ****************** */
    /* **** 256 size **** */
    /* No SSE, AVX only */
    /* ****************** */

    // Packed float 256 bits
            PLC_PADDS_256 = PLC_PADDS_AVX | PLC_OP_256,
    PLC_PSUBS_256 = PLC_PSUBS_AVX | PLC_OP_256,
    PLC_PMULS_256 = PLC_PMULS_AVX | PLC_OP_256,
    PLC_PDIVS_256 = PLC_PDIVS_AVX | PLC_OP_256,

    //Packed double 256 bits
            PLC_PADDD_256 = PLC_PADDD_AVX | PLC_OP_256,
    PLC_PSUBD_256 = PLC_PSUBD_AVX | PLC_OP_256,
    PLC_PMULD_256 = PLC_PMULD_AVX | PLC_OP_256,
    PLC_PDIVD_256 = PLC_PDIVD_AVX | PLC_OP_256,

    /* ****************** */
    /* **** 512 size **** */
    /* No SSE, AVX only */
    /* ****************** */

    // Packed float 512 bits
            PLC_PADDS_512 = PLC_PADDS_AVX | PLC_OP_512,
    PLC_PSUBS_512 = PLC_PSUBS_AVX | PLC_OP_512,
    PLC_PMULS_512 = PLC_PMULS_AVX | PLC_OP_512,
    PLC_PDIVS_512 = PLC_PDIVS_AVX | PLC_OP_512,

    // Packed double 512 bits
            PLC_PADDD_512 = PLC_PADDD_AVX | PLC_OP_512,
    PLC_PSUBD_512 = PLC_PSUBD_AVX | PLC_OP_512,
    PLC_PMULD_512 = PLC_PMULD_AVX | PLC_OP_512,
    PLC_PDIVD_512 = PLC_PDIVD_AVX | PLC_OP_512,

    /* ****************** */
    /* ***** FUSED ***** */
    /* No SSE, AVX only */
    /* ****************** */

    //Scalar FMA and FMS Float
            PLC_A132SS = PLC_OP_FLOAT | PLC_OP_SCALAR | PLC_OP_FMA | PLC_OP_132 | PLC_OP_AVX,
    PLC_A213SS = PLC_OP_FLOAT | PLC_OP_SCALAR | PLC_OP_FMA | PLC_OP_213 | PLC_OP_AVX,
    PLC_A231SS = PLC_OP_FLOAT | PLC_OP_SCALAR | PLC_OP_FMA | PLC_OP_231 | PLC_OP_AVX,
    PLC_S132SS = PLC_OP_FLOAT | PLC_OP_SCALAR | PLC_OP_FMS | PLC_OP_132 | PLC_OP_AVX,
    PLC_S213SS = PLC_OP_FLOAT | PLC_OP_SCALAR | PLC_OP_FMS | PLC_OP_213 | PLC_OP_AVX,
    PLC_S231SS = PLC_OP_FLOAT | PLC_OP_SCALAR | PLC_OP_FMS | PLC_OP_231 | PLC_OP_AVX,

    //Scalar FMA and FMS Double
            PLC_A132SD = PLC_OP_DOUBLE | PLC_OP_SCALAR | PLC_OP_FMA | PLC_OP_132 | PLC_OP_AVX,
    PLC_A213SD = PLC_OP_DOUBLE | PLC_OP_SCALAR | PLC_OP_FMA | PLC_OP_213 | PLC_OP_AVX,
    PLC_A231SD = PLC_OP_DOUBLE | PLC_OP_SCALAR | PLC_OP_FMA | PLC_OP_231 | PLC_OP_AVX,
    PLC_S132SD = PLC_OP_DOUBLE | PLC_OP_SCALAR | PLC_OP_FMS | PLC_OP_132 | PLC_OP_AVX,
    PLC_S213SD = PLC_OP_DOUBLE | PLC_OP_SCALAR | PLC_OP_FMS | PLC_OP_213 | PLC_OP_AVX,
    PLC_S231SD = PLC_OP_DOUBLE | PLC_OP_SCALAR | PLC_OP_FMS | PLC_OP_231 | PLC_OP_AVX,

    //Packed FMA and FMS Float
            PLC_A132PS = PLC_OP_FLOAT | PLC_OP_PACKED | PLC_OP_FMA | PLC_OP_132 | PLC_OP_AVX,
    PLC_A213PS = PLC_OP_FLOAT | PLC_OP_PACKED | PLC_OP_FMA | PLC_OP_213 | PLC_OP_AVX,
    PLC_A231PS = PLC_OP_FLOAT | PLC_OP_PACKED | PLC_OP_FMA | PLC_OP_231 | PLC_OP_AVX,
    PLC_S132PS = PLC_OP_FLOAT | PLC_OP_PACKED | PLC_OP_FMS | PLC_OP_132 | PLC_OP_AVX,
    PLC_S213PS = PLC_OP_FLOAT | PLC_OP_PACKED | PLC_OP_FMS | PLC_OP_213 | PLC_OP_AVX,
    PLC_S231PS = PLC_OP_FLOAT | PLC_OP_PACKED | PLC_OP_FMS | PLC_OP_231 | PLC_OP_AVX,

    //Packed FMA and FMS Double
            PLC_A132PD = PLC_OP_DOUBLE | PLC_OP_PACKED | PLC_OP_FMA | PLC_OP_132 | PLC_OP_AVX,
    PLC_A213PD = PLC_OP_DOUBLE | PLC_OP_PACKED | PLC_OP_FMA | PLC_OP_213 | PLC_OP_AVX,
    PLC_A231PD = PLC_OP_DOUBLE | PLC_OP_PACKED | PLC_OP_FMA | PLC_OP_231 | PLC_OP_AVX,
    PLC_S132PD = PLC_OP_DOUBLE | PLC_OP_PACKED | PLC_OP_FMS | PLC_OP_132 | PLC_OP_AVX,
    PLC_S213PD = PLC_OP_DOUBLE | PLC_OP_PACKED | PLC_OP_FMS | PLC_OP_213 | PLC_OP_AVX,
    PLC_S231PD = PLC_OP_DOUBLE | PLC_OP_PACKED | PLC_OP_FMS | PLC_OP_231 | PLC_OP_AVX,

    //Negated Scalar FMA and FMS Float
            PLC_NA132SS = PLC_A132SS | PLC_OP_NEG,
    PLC_NA213SS = PLC_A213SS | PLC_OP_NEG,
    PLC_NA231SS = PLC_A231SS | PLC_OP_NEG,
    PLC_NS132SS = PLC_S132SS | PLC_OP_NEG,
    PLC_NS213SS = PLC_S213SS | PLC_OP_NEG,
    PLC_NS231SS = PLC_S231SS | PLC_OP_NEG,

    //Negated Scalar FMA and FMS Double
            PLC_NA132SD = PLC_A132SD | PLC_OP_NEG,
    PLC_NA213SD = PLC_A213SD | PLC_OP_NEG,
    PLC_NA231SD = PLC_A231SD | PLC_OP_NEG,
    PLC_NS132SD = PLC_S132SD | PLC_OP_NEG,
    PLC_NS213SD = PLC_S213SD | PLC_OP_NEG,
    PLC_NS231SD = PLC_S231SD | PLC_OP_NEG,

    //Negated Packed FMA and FMS Float
            PLC_NA132PS = PLC_A132PS | PLC_OP_NEG,
    PLC_NA213PS = PLC_A213PS | PLC_OP_NEG,
    PLC_NA231PS = PLC_A231PS | PLC_OP_NEG,
    PLC_NS132PS = PLC_S132PS | PLC_OP_NEG,
    PLC_NS213PS = PLC_S213PS | PLC_OP_NEG,
    PLC_NS231PS = PLC_S231PS | PLC_OP_NEG,

    //Negated Packed FMA and FMS Double
            PLC_NA132PD = PLC_A132PD | PLC_OP_NEG,
    PLC_NA213PD = PLC_A213PD | PLC_OP_NEG,
    PLC_NA231PD = PLC_A231PD | PLC_OP_NEG,
    PLC_NS132PD = PLC_S132PD | PLC_OP_NEG,
    PLC_NS213PD = PLC_S213PD | PLC_OP_NEG,
    PLC_NS231PD = PLC_S231PD | PLC_OP_NEG
};

/**
 * \brief Returns the operation category of \param instr
 */
enum OPERATION_CATEGORY plc_get_operation_category(instr_t *instr);

/**
 * \brief Returns true if the operation category has the double flag set
 */
inline bool plc_is_double(enum OPERATION_CATEGORY oc){
    return (oc & PLC_OP_DOUBLE) != 0;
}

/**
 * \brief Returns true if the operation category has the float flag set
 */
inline bool plc_is_float(enum OPERATION_CATEGORY oc){
    return (oc & PLC_OP_FLOAT) != 0;
}

/**
 * \brief Returns true if the operation category has the packed flag set
 */
inline bool plc_is_packed(enum OPERATION_CATEGORY oc){
    return (oc & PLC_OP_PACKED) != 0;
}

/**
 * \brief Returns true if the operation category has the scalar flag set
 */
inline bool plc_is_scalar(enum OPERATION_CATEGORY oc){
    return (oc & PLC_OP_SCALAR) != 0;
}

/**
 * \brief Returns true if the operation category has the add flag set
 */
inline bool plc_is_add(enum OPERATION_CATEGORY oc){
    return (oc & PLC_OP_ADD) != 0;
}

/**
 * \brief Returns true if the operation category has the sub flag set
 */
inline bool plc_is_sub(enum OPERATION_CATEGORY oc){
    return (oc & PLC_OP_SUB) != 0;
}

/**
 * \brief Returns true if the operation category has the mul flag set
 */
inline bool plc_is_mul(enum OPERATION_CATEGORY oc){
    return (oc & PLC_OP_MUL) != 0;
}

/**
 * \brief Returns true if the operation category has the div flag set
 */
inline bool plc_is_div(enum OPERATION_CATEGORY oc){
    return (oc & PLC_OP_DIV) != 0;
}

/**
 * \brief Returns true if the operation category has the fma flag set
 */
inline bool plc_is_fma(enum OPERATION_CATEGORY oc){
    return (oc & PLC_OP_FMA) != 0;
}

/**
 * \brief Returns true if the operation category has the fms flag set
 */
inline bool plc_is_fms(enum OPERATION_CATEGORY oc){
    return (oc & PLC_OP_FMS) != 0;
}

/**
 * \brief Returns true if the operation category has the order 132 flag set
 */
inline bool plc_is_fma132(enum OPERATION_CATEGORY oc){
    return (oc & PLC_OP_132) != 0;
}

/**
 * \brief Returns true if the operation category has the order 213 flag set
 */
inline bool plc_is_fma213(enum OPERATION_CATEGORY oc){
    return (oc & PLC_OP_213) != 0;
}

/**
 * \brief Returns true if the operation category has the order 231 flag set
 */
inline bool plc_is_fma231(enum OPERATION_CATEGORY oc){
    return (oc & PLC_OP_231) != 0;
}

/**
 * \brief Returns true if the operation category has the negated flag set
 */
inline bool plc_is_negated(enum OPERATION_CATEGORY oc){
    return (oc & PLC_OP_NEG) != 0;
}

/**
 * \brief Returns true if the operation category has the 128bits flag set
 */
inline bool plc_is_128(enum OPERATION_CATEGORY oc){
    return (oc & PLC_OP_128) != 0;
}

/**
 * \brief Returns true if the operation category has the 256bits flag set
 */
inline bool plc_is_256(enum OPERATION_CATEGORY oc){
    return (oc & PLC_OP_256) != 0;
}

/**
 * \brief Returns true if the operation category has the 512bits flag set
 */
inline bool plc_is_512(enum OPERATION_CATEGORY oc){
    return (oc & PLC_OP_512) != 0;
}

/**
 * \brief Returns true if the operation category has the fused flag set
 */
inline bool plc_is_fused(enum OPERATION_CATEGORY oc){
    return (oc & PLC_OP_FUSED) != 0;
}

/**
 * \brief Returns true if the operation category is instrumentable
 */
inline bool plc_is_instrumented(enum OPERATION_CATEGORY oc){
    return (oc != PLC_OP_OTHER && oc != PLC_OP_UNSUPPORTED);
}

#endif // PADLOC_OPERATION_HEADER
