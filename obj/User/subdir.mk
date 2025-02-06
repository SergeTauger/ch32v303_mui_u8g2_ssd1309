################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/buttons.c \
../User/ch32v30x_it.c \
../User/main.c \
../User/system_ch32v30x.c 

OBJS += \
./User/buttons.o \
./User/ch32v30x_it.o \
./User/main.o \
./User/system_ch32v30x.o 

C_DEPS += \
./User/buttons.d \
./User/ch32v30x_it.d \
./User/main.d \
./User/system_ch32v30x.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"/home/stauger/Projects/u8g2_mui_demo/ch32v303_mui_u8g2_ssd1309/Debug" -I"/home/stauger/Projects/u8g2_mui_demo/ch32v303_mui_u8g2_ssd1309/Core" -I"/home/stauger/Projects/u8g2_mui_demo/ch32v303_mui_u8g2_ssd1309/User" -I"/home/stauger/Projects/u8g2_mui_demo/ch32v303_mui_u8g2_ssd1309/Peripheral/inc" -I"/home/stauger/Projects/u8g2_mui_demo/ch32v303_mui_u8g2_ssd1309/User/GUI" -I"/home/stauger/Projects/u8g2_mui_demo/ch32v303_mui_u8g2_ssd1309/User/GUI/u8g2" -I"/home/stauger/Projects/u8g2_mui_demo/ch32v303_mui_u8g2_ssd1309/User/debounce" -std=gnu99 -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


