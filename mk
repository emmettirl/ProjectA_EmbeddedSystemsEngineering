echo 1. create font.o file
arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm font font.o
#nm -S -t d font.o

echo 2. create panda1.o file
arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm logo logo.o
#arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm panda2 panda2.o

echo 3. compile-link
arm-none-eabi-as -mcpu=arm926ej-s -g ts.s -o ts.o
arm-none-eabi-gcc -c -mcpu=arm926ej-s -g t.c -o t.o
arm-none-eabi-ld -T t.ld ts.o t.o -o t.elf
arm-none-eabi-objcopy -O binary t.elf t.bin


echo ready to go?
read dummy

qemu-system-arm -M versatilepb -m 128M -kernel t.bin -serial mon:stdio



 





