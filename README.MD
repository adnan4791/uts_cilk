how to compile
clang -fopencilk -lm -O3 bots_main.c bots_common.c brg_sha.c uts.c -o utscilk

How to run

$./utscilk -f test.input

Opencilk code is in uts.c
