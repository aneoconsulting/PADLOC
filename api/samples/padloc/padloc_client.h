/**
 * \file padloc_client.h
 * \brief Library Manipulation API Sample, part of the Padloc project.
 * 
 * \author Brasseur Dylan, Teaudors Mickaël, Valeri Yoann
 * \date 2019
 * \copyright Interflop 
 */

#ifndef PADLOC_SYMBOL_H
#define PADLOC_SYMBOL_H

#include "dr_api.h"
#include "drmgr.h"
#include "backend/backend.hxx"
#include "padloc_operations.hpp"

#ifndef MAX_OPND_SIZE_BYTES
    /** 
     * \def MAX_OPND_SIZE_BYTES 
     * \brief operand maximum size
     * \details Value is 512 bits (AVX512 instructions) = 64 bytes 
     */
    #define MAX_OPND_SIZE_BYTES 64

#endif

/** 
 * \def PRINT_ERROR_MESSAGE
 * \brief Assert error message
 */
#define PRINT_ERROR_MESSAGE(message) dr_fprintf(STDERR, "%s\n", (message));

#ifdef WINDOWS

/** 
 * \def DR_REG_OP_A_ADDR
 * \brief Register to store the address of the first operand
 */
#define DR_REG_OP_A_ADDR DR_REG_XCX

/** 
 * \def DR_REG_OP_B_ADDR
 * \brief Register to store the address of the second operand
 */
#define DR_REG_OP_B_ADDR DR_REG_XDX

/** 
 * \def DR_REG_OP_C_ADDR
 * \brief Register to store the address of the third operand
 */
#define DR_REG_OP_C_ADDR DR_REG_R8

#elif defined(AARCH64)

/** 
 * \def DR_REG_OP_A_ADDR
 * \brief Register to store the address of the first operand
 */
#define DR_REG_OP_A_ADDR DR_REG_X0

/** 
 * \def DR_REG_OP_B_ADDR
 * \brief Register to store the address of the second operand
 */
#define DR_REG_OP_B_ADDR DR_REG_X1

/** 
 * \def DR_REG_OP_C_ADDR
 * \brief Register to store the address of the third operand
 */
#define DR_REG_OP_C_ADDR DR_REG_X2

#else

/** 
 * \def DR_REG_OP_A_ADDR
 * \brief Register to store the address of the first operand
 */
#define DR_REG_OP_A_ADDR DR_REG_XDI

/** 
 * \def DR_REG_OP_B_ADDR
 * \brief Register to store the address of the second operand
 */
#define DR_REG_OP_B_ADDR DR_REG_XSI

/** 
 * \def DR_REG_OP_C_ADDR
 * \brief Register to store the address of the third operand
 */
#define DR_REG_OP_C_ADDR DR_REG_XDX

#endif

#if defined(X86)

/**
 * \def DR_SCRATCH_REG
 * \brief Buffer register for intermediary storage 
 */
#define DR_SCRATCH_REG DR_REG_XCX

/**
 * \def AVX_SUPPORTED 
 * \brief True if the machine supports AVX instructions 
 * \warning Supported only on X86
 */
#define AVX_SUPPORTED (proc_has_feature(FEATURE_AVX))

/**
 * \def AVX_512_SUPPORTED 
 * \brief True if the machine supports AVX 512 instructions
 * \warning Supported only on X86 
 */
#define AVX_512_SUPPORTED (proc_has_feature(FEATURE_AVX512F))

#elif defined(AARCH64)

/**
 * \def DR_SCRATCH_REG
 * \brief Buffer register for intermediary storage 
 */
#define DR_SCRATCH_REG DR_REG_X5

#endif

/**
 * \def SPILL_SLOT_SCRATCH_REG
 * \brief Index of the spill slot to store the value of DR_SCRATCH_REG
 */
#define SPILL_SLOT_SCRATCH_REG SPILL_SLOT_1

/**
 * \def SRC
 * \brief Shortcut for instr_get_src
 */
#define SRC(instr, n) instr_get_src((instr), (n))

/**
 * \def DST
 * \brief Shortcut for instr_get_dst
 */
#define DST(instr, n) instr_get_dst((instr), (n))

#if defined(X86)

/**
 * \def MOVE_FLOATING_SCALAR
 * \brief Shortcut for INSTR_CREATE_movsd or INSTR_CREATE_movss depending on the precision.
 * \details Insert a specific MOVE instruction for one floating element.
 * \param is_double If true, then the instruction inserted is MOVSD, else it is MOVSS
 * \param drcontext DynamoRIO context
 * \param dest Destination operand (OPND type)
 * \param srcd Source operand (OPND type) for MOVSD
 * \param srcs  Source operand (OPND type) for MOVSS
 */
#define MOVE_FLOATING_SCALAR(is_double, drcontext, dest, srcd, srcs) (is_double) ? 	\
        INSTR_CREATE_movsd((drcontext), (dest), (srcd)) : 			  					\
        INSTR_CREATE_movss((drcontext), (dest), (srcs))

/**
 * \def MOVE_FLOATING_PACKED
 * \brief Shortcut for INSTR_CREATE_vmovupd or INSTR_CREATE_movupd depending on the instruction type (AVX or SSE).
 * \details Insert a specific MOVE instruction for an entire floating register.
 * \param is_avx If true, then the instruction inserted is VMOVUPD, else it is MOVUPD
 * \param drcontext DynamoRIO context
 * \param dest Destination operand (OPND type)
 * \param src Source operand (OPND type) 
 */
#define MOVE_FLOATING_PACKED(is_avx, drcontext, dest, src) (is_avx) ? 				\
        INSTR_CREATE_vmovupd((drcontext), (dest), (src)) : 								\
        INSTR_CREATE_movupd((drcontext), (dest), (src))
#endif

/* CREATE OPND */

/**
 * \def OP_REG
 * \brief Shortcut for opnd_create_reg
 */
#define OP_REG(reg_id) opnd_create_reg((reg_id))

/**
 * \def OP_INT
 * \brief Shortcut for opnd_create_immed_int
 */
#define OP_INT(val) opnd_create_immed_int((val), OPSZ_4)

/**
 * \def OP_INT64
 * \brief Shortcut for opnd_create_immed_int64
 */
#define OP_INT64(val) opnd_create_immed_int64((val), OPSZ_8)

#if defined(X86)

/**
 * \def OP_REL_ADDR
 * \brief Shortcut for opnd_create_rel_addr
 */
#define OP_REL_ADDR(addr) \
        opnd_create_rel_addr((addr), OPSZ_8)

/**
 * \def OP_BASE_DISP
 * \brief Shortcut for opnd_create_base_disp
 */
#define OP_BASE_DISP(base, disp, size) \
        opnd_create_base_disp((base), DR_REG_NULL, 0, (disp), (size))

#elif defined(AARCH64)

/**
 * \def OP_REL_ADDR
 * \brief Shortcut for opnd_create_rel_addr
 */
#define OP_REL_ADDR(addr) \
        opnd_create_rel_addr((addr), OPSZ_2)

/**
 * \def OP_BASE_DISP
 * \brief Shortcut for opnd_create_base_disp
 */
#define OP_BASE_DISP(base, disp, size) \
        opnd_create_base_disp((base), DR_REG_NULL, 0, (disp), (size))
#endif

/* TESTS OPND */

/**
 * \def IS_REG
 * \brief Shortcut for opnd_is_reg
 */
#define IS_REG(opnd) opnd_is_reg((opnd))

/**
 * \def OP_IS_BASE_DISP
 * \brief Shortcut for opnd_is_base_disp
 */
#define OP_IS_BASE_DISP(opnd) opnd_is_base_disp((opnd))

/**
 * \def OP_IS_REL_ADDR
 * \brief Shortcut for opnd_is_rel_addr
 */
#define OP_IS_REL_ADDR(opnd) opnd_is_rel_addr((opnd))

/**
 * \def OP_IS_ABS_ADDR
 * \brief Shortcut for opnd_is_abs_addr
 */
#define OP_IS_ABS_ADDR(opnd) opnd_is_abs_addr((opnd))

/**
 * \def OP_IS_ADDR
 * \brief True if the opnd is a relative or an absolute address
 */
#define OP_IS_ADDR(opnd) (OP_IS_REL_ADDR((opnd)) || OP_IS_ABS_ADDR((opnd)))

/* TESTS REG */

/**
 * \def IS_GPR
 * \brief Shortcut for reg_is_gpr
 */
#define IS_GPR(reg) reg_is_gpr((reg))

#if defined(X86)

/**
 * \def IS_XMM
 * \brief Shortcut for reg_is_strictly_xmm
 */
#define IS_XMM(reg) reg_is_strictly_xmm((reg))

/**
 * \def IS_YMM
 * \brief Shortcut for reg_is_strictly_ymm
 */
#define IS_YMM(reg) reg_is_strictly_ymm((reg))

/**
 * \def IS_ZMM
 * \brief Shortcut for reg_is_strictly_zmm
 */
#define IS_ZMM(reg) reg_is_strictly_zmm((reg))

#elif defined(AARCH64)

/**
 * \def IS_SIMD
 * \brief Shortcut for reg_is_simd
 */
#define IS_SIMD(reg) reg_is_simd((reg))

#endif

/* OPND GET VALUE */

/**
 * \def GET_REG
 * \brief Shortcut for opnd_get_reg
 */
#define GET_REG(opnd) opnd_get_reg((opnd)) /* if opnd is a register */

/**
 * \def GET_ADDR
 * \brief Shortcut for opnd_get_addr
 */
#define GET_ADDR(opnd) opnd_get_addr((opnd)) /*if opnd is an address */

/* TLS OPERATIONS */

/**
 * \def GET_TLS
 * \brief Shortcut for drmgr_get_tls_field
 */
#define GET_TLS(drcontext, tls) drmgr_get_tls_field((drcontext), (tls))

/**
 * \def SET_TLS
 * \brief Shortcut for drmgr_set_tls_field
 */
#define SET_TLS(drcontext, tls, value) drmgr_set_tls_field((drcontext), (tls), (void*)(value))

/**
 * \def INSERT_READ_TLS
 * \brief Shortcut for drmgr_insert_read_tls_field
 */
#define INSERT_READ_TLS(drcontext, tls, bb, instr, reg) \
        drmgr_insert_read_tls_field((drcontext), (tls), (bb), (instr), (reg))


/**
 * \def INSERT_WRITE_TLS
 * \brief Shortcut for drmgr_insert_write_tls_field
 */
#define INSERT_WRITE_TLS(drcontext, tls, bb, instr, reg, temp_reg) \
        drmgr_insert_write_tls_field((drcontext), (tls_stack), (bb), (instr), (buffer_reg), (temp_reg));

/* SIZES */

/**
 * \def REG_SIZE
 * \brief Get a register size in bytes
 */
#define REG_SIZE(reg) opnd_size_in_bytes(reg_get_size((reg)))

/**
 * \def OPSZ
 * \brief Shortcut for opnd_size_from_bytes. 
 * \details Get an OPSZ constant corresponding to a bytes size.
 */
#define OPSZ(bytes) opnd_size_from_bytes((bytes))

#if defined(X86)

/** 
 * \def NB_XMM_REG
 * \brief The number of XMM registers on X86 only
 */
#define NB_XMM_REG 16

/** 
 * \def NB_YMM_REG
 * \brief The number of YMM registers on X86 only
 */
#define NB_YMM_REG 16

/** 
 * \def NB_ZMM_REG
 * \brief The number of ZMM registers on X86 only
 */
#define NB_ZMM_REG 32

#elif defined(AARCH64)

/** 
 * \def NB_Q_REG
 * \brief The number of Q registers on AARCH64 only
 */
#define NB_Q_REG 0
#endif

/**
 * \brief Returns the index of the floating point registers tls
 */
int get_index_tls_float();

/**
 * \brief Returns the index of the gpr tls
 */
int get_index_tls_gpr();

/**
 * \brief Returns the index of the result tls
 */
int get_index_tls_result();

/**
 * \brief Sets the index of the gpr tls 
 * \param new_tls_value New value to set
 */
void set_index_tls_gpr(int new_tls_value);

/**
 * \brief Sets the index of the floating point registers tls
 * \param new_tls_value New value to set
 */
void set_index_tls_float(int new_tls_value);

/**
 * \brief Sets the index of the result tls
 * \param new_tls_value New value to set
 */
void set_index_tls_result(int new_tls_value);

/**
 * \brief Inserts newinstr in ilist prior to instr and set it as an application instruction
 
 * \param newinstr Instruction to insert
 * \param ilist List of Basic block instructions in which newinstr is insert
 * \param instr The instruction before which newinstr is inserted in the list
 */
void translate_insert(instr_t *newinstr, instrlist_t *ilist, instr_t *instr);


/**
 * \brief Insert the corresponding call into the application depending on the properties of the instruction overloaded.  
 * 
 * \param drcontext DynamoRIO's context
 * \param bb Current basic block
 * \param instr Instrumented instruction
 * \param oc Operation category of the instruction
 * \param is_double True if the baseline precision is double
 */
void insert_call(void *drcontext, instrlist_t *bb, instr_t *instr, OPERATION_CATEGORY oc, bool is_double);

/**
 * \brief Inserts prior to where meta-instructions to set the calling convention registers to the right adresses
 * \warning Assumes the GPR have been saved !
 * \param drcontext DynamoRIO's context
 * \param bb Current Basic Block
 * \param where instruction prior to whom we insert the meta-instructions 
 * \param instr Instrumented instruction
 * \param oc Operation category of the instrumented instruction
 */
void insert_set_operands(void *drcontext, instrlist_t *bb, instr_t *where, instr_t *instr, OPERATION_CATEGORY oc);

/**
 * \brief Inserts prior to \p where meta-instructions to restore the floating point registers (xmm-ymm-zmm)
 * \warning Assumes the gpr have been saved beforehand !
 * \param drcontext DynamoRIO context
 * \param bb Current basic bloc
 * \param where instruction prior to whom we insert the meta-instructions 
 */
void insert_restore_simd_registers(void *drcontext, instrlist_t *bb, instr_t *where);

/**
 * \brief Inserts prior to \p where meta-instructions to save the floating point registers (xmm-ymm-zmm)
 * \warning Assumes the gpr have been saved beforehand !
 * \param drcontext DynamoRIO context
 * \param bb Current basic bloc
 * \param where instruction prior to whom we insert the meta-instructions 
 */
void insert_save_simd_registers(void *drcontext, instrlist_t *bb, instr_t *where);

/**
 * \brief Prepares the address in the buffer of the tls register to point to the destination register in memory
 * \warning Assumes the gpr have been saved beforehand !
 * \param drcontext DynamoRIO's context
 * \param bb Current basic block
 * \param where instruction prior to whom we insert the meta-instructions 
 * \param destination SIMD registers that should have received the result
 */
void insert_set_destination_tls(void *drcontext, instrlist_t *bb, instr_t *where, reg_id_t destination);

/**
 * \brief Inserts prior to \param where meta-instructions to restore the arithmetic flags and the gpr registers
 * \param drcontext DynamoRIO context
 * \param bb Current basic bloc
 * \param where instruction prior to whom we insert the meta-instructions 
 */
void insert_restore_gpr_and_flags(void *drcontext, instrlist_t *bb, instr_t *where);

/**
 * \brief Inserts prior to \param where meta-instructions to save the arithmetic flags and the gpr registers
 * \param drcontext DynamoRIO context
 * \param bb Current basic bloc
 * \param where instruction prior to whom we insert the meta-instructions 
 */
void insert_save_gpr_and_flags(void *drcontext, instrlist_t *bb, instr_t *where);

/**
 * \brief Inserts prior to \p where meta-instructions to restore RSP from its saved value
 * 
 * \param drcontext DynamoRIO's context
 * \param bb Current Basic Block
 * \param where instruction prior to whom we insert the meta-instructions 
 */
void insert_restore_rsp(void *drcontext, instrlist_t *bb, instr_t *where);

#endif
