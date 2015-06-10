arm-none-eabi-as -mcpu=arm7tdmi arm7tdmi.S -o arm7tdmi.o
if errorlevel 1 goto ed
arm-none-eabi-objdump -d arm7tdmi.o > arm7tdmi.lst
arm-none-eabi-objdump -s arm7tdmi.o >> arm7tdmi.lst
del arm7tdmi.o
arm-none-eabi-as -mcpu=cortex-m0 cortex.S -o cortex.o
if errorlevel 1 goto ed
arm-none-eabi-objdump -d cortex.o > cortex.lst
arm-none-eabi-objdump -s cortex.o >> cortex.lst
del cortex.o
:ed
