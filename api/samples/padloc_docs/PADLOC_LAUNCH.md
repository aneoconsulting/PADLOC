Launch options
===

On this page, you will find the command line options you can give to the PADLOC client. To see the command line options of DynamoRIO, go to [this page](http://dynamorio.org/docs/using.html#sec_options).

# Usage
To set the command line options of the PADLOC client, you need to type them after the name of the library, but before the `--`. For example, to activate the debug mode and use whitelist.txt as a whitelist :
~~~~sh
bin64/drrun -c libpadloc.so -d -w whitelist.txt -- path/to/executable
~~~~

# Options

## Debug options 

- **-l** *&lt;integer&gt;* | **--loglevel** *&lt;integer&gt;* : (Default level : 0) Sets the loglevel to the given integer, with 1 being debug mode, 2 being warning mode and 3 being error mode
- **-d** | **--debug** : Sets the client to debug mode (equivalent to <b>-l 1</b>)

## Symbol handling options

See [Symbol Handling](SYMBOL_HANDLING.md) for details

- **-n** | **--no-lookup** : (Default) Disables the lookup for the symbols, instruments every floating operation
- **-w** *&lt;filename&gt;* | **--whitelist** *&lt;filename&gt;* : Instruments only the symbols present in the given file
- **-b** *&lt;filename&gt;* | **--blacklist** *&lt;filename&gt;* : Disable the instrumentation for the symbols present in the given file
- **-g** *&lt;filename&gt;* | **--generate** *&lt;filename&gt;* : Generates the list of symbols in the given program and writes it to the given file, doesn't instrument anything

## Backend analysis options
- **-ar** | **--analyse_run** : (Default) Analyse the backend normally and run the program afterwards
- **-aa** *&lt;filename&gt;* | **--analyse_abort** *&lt;filename&gt;* : Analyse the registers used by the backend, dump them into the given file, and stop execution
- **-af** *&lt;filename&gt;* | **--analyse_file** *&lt;filename&gt;* : Read the values in the given file, and use those values to save particular registers

## Miscellaneous

- **-h** | **--help** : Displays the help of the client, stops the program