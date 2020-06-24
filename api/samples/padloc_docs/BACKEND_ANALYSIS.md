Backend analysis
===

PADLOC allows users to easily switch between backends. These backends are files that have functions performing all the basic operations (add, sub, mul and div) and FMA/FMS. These functions are responsible for new behaviour of the operations.

However, of course, these functions use registers (GPR and SIMD), and since we use DynamoRIO, simply calling these functions would modify the values in potentially already used registers, we must ensure the stability of the program.

To that end, PADLOC provides a plugin which checks all the functions used by the backend and gives back all the GPR and SIMD registers used. That way, we can only save those registers, and make sure the program still is stable, even after insertion of functions.

The way the plugin works is that it creates 2 vectors of registers, one for GPR and one for SIMD registers, and fills them with the registers used by each functions of the backend.

To perform this, the client uses DynamoRIO's API to open the client as a library so that we can go through all the symbols. Afterwards, we search for the functions called "apply", which are the functions inserted by our client. There are two symbols called "apply", one for simple operations, and one for FMA/FMS.

When a function with this name is detected, we get it as a list of instructions and iterate all over it, and for each instructions, we check if GPR or SIMD registers are used. If so, we add them to the vectors of registers. Of course, we make sure to iterate through all calls inside each function at most once, and to skip all jumps, as we want to iterate through everything in each and every function.

Unfortunately, we didn't have the time to completely use it in our current version, but the plugin works fine.