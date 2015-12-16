./akc ../examples/tests.ak out.asm
nasm -felf64 out.asm
ld out.o -o out
./out
