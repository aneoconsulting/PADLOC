PADLOC Client
===

<b>P</b>roficient <b>A</b>rithmetic <b>D</b>isturber for F<b>LO</b>ating Point <b>C</b>omputation, aka **PADLOC**, is a client for [DynamoRIO](http://dynamorio.org) that replaces floating point operations in binaries by their [Monte Carlo Arithmetic](http://web.cs.ucla.edu/~stott/mca/) counterpart.

**PADLOC** is part of the Interflop Initiative, which is a joint project between the Université de Perpignan Via Domitia (UPVD), the Laboratoire d'Informatique Paris 6 (LIP6), the Université de Versailles Saint-Quentin-en-Yvelines (UVSQ), Électricité de France (EDF), Intel, CEA LIST and ANEO. The collaboration’s purpose is to come up with a performant platform to analyse program’s behaviour facing FP operations.

**Monte Carlo Arithmetic** (MCA) works by randomizing the rounding mode of floating point operations. The IEEE 754 standard defines 4 rouding modes : Towards \f$+\infty \f$ (Upward), Towards \f$-\infty\f$ (Downward), Towards \f$0\f$ and Nearest. By default, all floating point operations are performed using the Nearest rounding mode. MCA allows to randomly switch between Upward and Downward, which can provide data for a statistical analysis of the results of a program.

With **PADLOC**, we provide a simple way to apply MCA to programs that were already compiled. The overhead varries a lot depending on the program and more optimizations for the future. Symbol handling allows the user to switch on and off the instrumentation depending on the file (executable, library...) and the associated symbols.

We currently support the instrumentation of scalar and packed SSE and AVX instructions on Linux and Windows (experimental). Support for AVX-512 and AArch64 is partially done, but isn't functionnal yet. 

___
# Quick install {#quick_install}

<h3>Linux</h3>

~~~~sh
git clone https://github.com/micky1608/dynamorio.git
cd dynamorio
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j
~~~~

<h3> Windows</h3>

~~~~sh
git clone https://github.com/micky1608/dynamorio.git
cd dynamorio
mkdir build
cd build
cmake -G"Visual Studio 2012 Win64" ..
cmake --build . --config RelWithDebInfo
~~~~

___
# Quick launch {#quick_launch}

<h3>Linux</h3>

~~~~sh
cd path/to/build/
bin64/drrun -c libpadloc.so -- path/to/executable
~~~~

<h3> Windows</h3>

~~~~sh
cd path\to\build\
bin64\drrun.exe -c padloc.dll -- path/to/executable
~~~~