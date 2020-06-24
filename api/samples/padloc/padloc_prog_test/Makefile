CC=g++
SOURCE=$(wildcard *.cpp) 
OUT=$(SOURCE:%.cpp=%.out)

all: $(OUT)

commutativity.out: commutativity.cpp
	$(CC) -o commutativity.out commutativity.cpp -O3

operand_order.out: operand_order.cpp
	$(CC) -o operand_order.out operand_order.cpp -O3 -masm=intel

matmul_blas.out: matmul_blas.cpp
	$(CC) -o matmul_blas.out matmul_blas.cpp -lopenblas -O3 -march=native

test_instructions_scalar.out: test_instructions_scalar_sse.out test_instructions_scalar_avx.out
	
test_instructions_scalar_sse.out: test_instructions.h test_instructions_scalar.cpp
	$(CC) -o test_instructions_scalar_sse.out test_instructions_scalar.cpp -msse -O3

test_instructions_scalar_avx.out: test_instructions.h test_instructions_scalar.cpp
	$(CC) -o test_instructions_scalar_avx.out test_instructions_scalar.cpp -mavx -O3

test_instructions_vect_sse.out: test_instructions.h test_instructions_vect_sse.cpp
	$(CC) -o test_instructions_vect_sse.out test_instructions_vect_sse.cpp -msse -O3

test_instructions_vect_avx.out: test_instructions.h test_instructions_vect_avx.cpp
	$(CC) -o test_instructions_vect_avx.out test_instructions_vect_avx.cpp -mavx -O3

test_instructions_fma.out: test_instructions.h test_instructions_fma.cpp
	$(CC) -o test_instructions_fma.out test_instructions_fma.cpp -mfma -O3

test_fma_order.out: test_fma_order.cpp
	$(CC) -o test_fma_order.out test_fma_order.cpp -masm=intel -O3

test_parallel.out: test_parallel.cpp
	$(CC) -o test_parallel.out test_parallel.cpp -fopenmp -mavx -O3


.PHONY: clean

clean:
	rm ./*.out