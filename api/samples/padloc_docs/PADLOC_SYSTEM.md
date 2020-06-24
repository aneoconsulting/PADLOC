How does PADLOC work ?
============

At its core, **PADLOC** replaces the floating point instructions of an executable by their MCA counterpart. For that, it uses the DynamoRIO's API to decode the executable, insert new instructions and reencode the instructions to be executed.

___
# Instruction replacement {#instruction_replacement}

DynamoRIO allows us to get the different basic blocks of the program before they are executed. Upon receiving such basic block, we iterate over its instructions in order to find instrumentable instructions. We instrument only the floating point operations and not the rest.

Currently, the operations we instrument are :
- Addition (ADD)
- Substraction (SUB)
- Division (DIV)
- Multiplication (MUL)
- Fused Multiply Add (FMA)
- Fused Multiply Sub (FMS)
- Fused Negated Multiply Add (FNMA)
- Fused Negated Multiply Sub (FNMS)

We currently support the FMA3, SSE, AVX and AVX2 instruction sets.

When we encounter the first instrumentable instruction, we prepare the processor state to call a backend function. This backend function signature is defined by the Interflop interface and needs to replace the instruction it instruments. For ease of use, we call a function that only works with pointers as a middleware in order to call the backend accordingly. This is useful for packed instructions.

All the operations below are done by inserting instructions in place of the instrumented instruction.

## Identification of the instruction

To identify if the encountered instruction is instrumentable or not, we look at the opcode of that instruction. We compare it to a list we defined to get the OPERATION_CATEGORY of this instruction. The OPERATION_CATEGORY contains the type of instruction as bit flags and is used to simplify further treatment of the instruction. For opcodes that handle different operand sizes, we need to get the size of the operands. For this, we iterate over the operands of the instructions and find the biggest one.

## Registers Safekeeping

To not interfere with the behavior of the program outside of the MCA disturbance, we need to make sure that every register that isn't affected by the instrumented instruction stays unaffected, independently of the backend used. For that, we save all the general purpose registers (GPR) and the arithmetic flags in a buffer in memory. This buffer is a thread local storage (TLS) to prevent multi-thread corruption. We do the same for all the SIMD registers.

Note that saving the arithmetic flags is important as the backends have a tendency to alter them. The instructions we instrument don't normally affect the arithmetic flags and thus we need to make sure they stay untouched. It's not uncommon to see a comparison with a GPR followed by a floating point operations before the result of that comparison is used.

On x86-64 for example, we do the following :
- We save RCX to a spill slot ([dr_save_reg](http://dynamorio.org/docs/dr__ir__utils_8h.html#af294ac021c84f5ec47230ee7df0e6c02))
- We load the address of the TLS buffer for gpr in RCX ([drmgr_insert_read_tls_field](http://dynamorio.org/docs/group__drmgr.html#ga7c72a35608998e6e359a3a652a7f97f7))
- We move RAX to its saving location in the buffer (using RCX as a base address)
- We restore the spill slot into RAX and then save RAX to the location of RCX in the buffer (in order to save RCX)
- We execute the `lahf` instruction to load the arithmetic flags into RAX and save RAX to the save location of the arithmetic flags
- We iterate over all the other GPR in order to save them in their respective location
- We load the address of the TLS buffer for floating point registers in RCX
- We iterate over the SIMD registers and we save them all to their respective location

## Setting up the result TLS

In order for the middleware to have a place to put the result, we have a TLS that contains an adress. All the instructions we instrument have a SIMD register as destination. Thus the result TLS point towards the location in memory of the corresponding register.

Note that the address in the TLS isn't the actual address. It points to a 64-bits location containing the address of the result. This eliviates the need of a dr_insert_write_tls(), which is expensive.
Basically, the TLS contains `&(&result)` and we change `&result` according to the instruction.

## Preparation of the calling convention

We use the LEA instruction to setup the calling convention registers as addresses to the corresponding parameters. DynamoRIO uses the cdecl calling convention for Linux, and the Microsoft X64 calling convention on Windows.
When the instrumented instruction's operand is a SIMD register, we load the address of the saved register. As all x86-64 processors are Little Endian, the lower half of the SIMD registers are located at the start of the save for this register. For example, XMM1, the lower half of YMM1, is located at the same adress as YMM1, excepts that it's 128 bits long instead of 256 bits. 
When the operand is a memory adress, either a base+displacement or an absolute or relative address, we load that address directly.

In order to comply with the Microsoft X64 calling convention, and prevent register spilling in bad locations, we need to execute a `sub %rsp 32` assembly instruction to set the stack pointer to the correct location.

## Call insertion

We then insert the right call for the instrumentation. In order to limit branching to a maximum during execution, we use templates to insert the function corresponding to the exact specifications of the instrumented instruction based on the OPERATION_CATEGORY. 

## Register restoring

After the call is done, the GPR and SIMD may have been corrupted, thus we need to restore the save we have in memory. We restore the arithmetic flags and then we restore the SIMD and GPR. Since the SIMD registers that were affected by the instrumented instruction have their values already modified in memory, this restoring also serves as a way to push the result in the right register.

On x86-64 for example, we do :
- We load the address of the TLS buffer for floating point registers in RCX
- We iterate over the SIMD registers to restore them from the buffer
- We load the address of the TLS buffer for gpr in RCX
- We restore the arithmetic flags by moving them from memory into RAX then execute the `sahf` instruction
- We then restore RAX from the buffer
- We then iterate in reverse the gpr until RCX to restore their value from the buffer

## Contiguous instructions handling

When multiple instrumented instructions are one after the other, we can skip the restoring and resaving phases between them. That way, we save some unnecessary moves.

___
# Symbols handling system {#symbol_functionning}

PADLOC supports instrumenting limitation depending on the encountered symbol. For example, if you don't want to instrument the libm or a specific function, PADLOC allows you to do it (cf [Symbol handling](SYMBOL_HANDLING.md) for details on the usage)

## Module wide instrumentation
DynamoRIO defines a module as an executable file, or a library which contains instructions. When DynamoRIO loads one of the modules used by the program, an event is fired and we check if that module should be instrumented. If it shouldn't be instrumented, we won't see its basic blocks go through the basic block callback.

## Symbol wide instrumentation
Upon seeing a basic block, we need to know if we need to instrument it. For that, we check where lies the address of its first instruction. If the instruction is part of a symbol we instrument, we instrument the basic block, otherwise we don't instrument it.

## Symbol generation mechanism
To ease the making of the whitelist and blacklist, the client can generate a file containing the list of modules and symbols used by the program. For that, we replace the instrumentation routine by a symbol logging routine. When a basic block contains a instrumentable instruction, we register its symbol. For that, we use the drsym DynamoRIO extension to get the module and symbols associated with the address of the current basic block. When we obtain the names of the module and the symbol, we add them to the modules_vector. At the end, the modules_vector is written to a file in text format.

## Symbol lookup mechanism
When using the whitelist or the blacklist, we need to find which symbol or module a particular basic block corresponds to. Firstly, we parse the files to get a module_vector. The next step is to translate the module_vector into a lookup_vector. The module_vector only has the names of the symbols and modules, while the lookup_vector also has the adress ranges of them. Using the lookup_vector is then more efficient than using the module_vector because we don't have to keep the symbols table in memory and we just have to compare integers to ranges.
To translate the names into adresses, we again use drsym to find all the necessary informations.
For modules, their adresses are handled differently between Windows and Unix. In Windows, the modules are contiguous, meaning that there's no gap of adresses in it, while Unix can have different segments of module at different locations in memory. We have to take that into account when checking if the basic block is in a instrumented basic block.
For symbols, they are contiguous, so just checking if the address lies in the range of the symbol is sufficient.

___
# Limitations and improvements {#limitations}

## Performance
Currently, this technique is expensive because of its simplicity. A more complex analysis can help improve performance by limiting the number of added instructions to a minimum. Several attempts have been made but aren't yet successful and will require more time to complete : 

- An analysis of the registers corrupted by the backend could help in removing some unnecessary save and restores. A simple version of it which looks at the registers used (not the corrupted ones only) is already in the code but currently unused
- An intelligent save and restore mechanism to execute only the necessary moves could cut down the number of instructions further. An attempt is available on the "multiinstr" branch but it's slower than the simple version we have on the master branch while cutting down the number of instructions by half on the matmul test. Further investigation is required.
- Limit the memory movements by utilizing unused registers and reorganise the register usage.

## Support

Currently, we only support X86_64 processors on Linux and Windows. Support for other platforms would be possible, but DynamoRIO is limited to X86, X86_64, ARM and AArch64 at the time of writing.

Some ground work as been layed for AArch64 support, but the current technique isn't feasable because of the lack of a LEA instruction equivalent on AArch64.

We don't support FMA4 by choice and because of its lack of use in processors. However, support for it isn't difficult to implement for future development.

We currently don't support the AVX-512 instruction set due to its complexity. Some instructions have a set rounding mode we shouldn't modify. Some instructions have writing masks which complexifies the backend.

We currently don't handle every type of floating point instruction for the instruction sets we handle such as :
- Addition - Substraction (ADDSUB)
- Substraction - Addition (SUBADD)
- FMADDSUB
- FMSUBADD
- FNMADDSUB
- FNMSUBADD
- Horizontal add (HADD)
- Horizontal sub (HSUB)
- and others...

