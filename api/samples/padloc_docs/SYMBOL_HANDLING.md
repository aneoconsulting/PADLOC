Symbol handling
===
PADLOC allows the user to select which symbols and modules the client should instrument. It follows a system of whitelist and blacklist. See the [launch options](PADLOC_LAUNCH.md) for the command line options of symbol handling.

# Usage
The user can provide a whitelist or a blacklist to the client to use. When such a file is provided, the client will actively look for the symbol associated with the basic block and decide whether to instrument it or not.
The files used as a blacklist or a whitelist must be in plain text format and contain one symbol or module per line in the following format :
~~~~
module!symbol
~~~~
Comments are denoted by a `#`.
If the line doesn't contain a `!`, then the whole module is considered.

# Symbol list generation {#symbol_gen}
The user can generate the list of symbols into a file by using the `-g` option. Doing so will launch the program without instrumenting it. Instead, the client will collect the names of the modules and symbols it encountered during execution and write it to a given file. It will only register the symbols and modules that contain instructions we instrument. The obtained list can now be edited by commenting out lines.

# Whitelist and blacklist {#whitelist_blacklist}
When provided, the files for the whitelist and blacklist are parsed to extract the module and symbols names. If multiple files of the same category are specified, only the last one is taken into account. The way symbols are handled depends on which files are provided :

- **No whitelist or blacklist** : All modules and symbols will be instrumented
- **Whitelist only** : If only the whitelist is provided, then only the modules and/or symbols specified in the whitelist file will be instrumented. All the other modules and symbols won't be instrumented.
- **Blacklist only** : If only the blacklist is provided, then all but the modules and/or symbols specified in the blacklist will be instrumented.
- **Whitelist and blacklist** : When both are provided, only the modules and/or symbols in the whitelist that aren't in the blacklist are instrumented. All the other modules and symbols won't be instrumented

For example if the whitelist contains :
~~~~
Mod1
Mod2!Sym2
Mod2!Sym3
Mod3!Sym4
Mod4
~~~~
and the blacklist contains :
~~~~
Mod1!Sym1
Mod2!Sym2
Mod3
Mod4
Mod5
~~~~
We obtain these instrumentations :

|           | No file | Whitelist only | Blacklist only |  Whitelist & Blacklist |
|:---------:|:-------:|:--------------:|:--------------:|:----------------------:|
| Mod1!Sym0 |    ✅    |        ✅       |        ✅       |            ✅           |
| Mod1!Sym1 |    ✅    |        ✅       |        ❌       |            ❌           |
| Mod2!Sym2 |    ✅    |        ✅       |        ❌       |            ❌           |
| Mod2!Sym3 |    ✅    |        ✅       |        ✅       |            ✅           |
| Mod3!Sym4 |    ✅    |        ✅       |        ❌       |            ❌           |
| Mod3!Sym5 |    ✅    |        ❌       |        ❌       |            ❌           |
| Mod4!Sym6 |    ✅    |        ✅       |        ❌       |            ❌           |
| Mod5!Sym7 |    ✅    |        ❌       |        ❌       |            ❌           |
| Mod6!Sym8 |    ✅    |        ❌       |        ✅       |            ❌           |

# Internal functionning
See [Symbol handling system](@ref symbol_functionning)