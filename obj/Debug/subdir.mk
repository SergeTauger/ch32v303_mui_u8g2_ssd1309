################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Debug/debug.c 

OBJS += \
./Debug/debug.o 

C_DEPS += \
./Debug/debug.d 


# Each subdirectory must supply rules for building sources it contributes
Debug/%.o: ../Debug/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"/home/stauger/Projects/u8g2_mui_demo/ch32v303_mui_u8g2_ssd1309/Debug" -I"/home/stauger/Projects/u8g2_mui_demo/ch32v303_mui_u8g2_ssd1309/Core" -I"/home/stauger/Projects/u8g2_mui_demo/ch32v303_mui_u8g2_ssd1309/User" -I"/home/stauger/Projects/u8g2_mui_demo/ch32v303_mui_u8g2_ssd1309/Peripheral/inc" -I"/home/stauger/Projects/u8g2_mui_demo/ch32v303_mui_u8g2_ssd1309/User/GUI" -I"/home/stauger/Projects/u8g2_mui_demo/ch32v303_mui_u8g2_ssd1309/User/GUI/u8g2" -I"/home/stauger/Projects/u8g2_mui_demo/ch32v303_mui_u8g2_ssd1309/User/debounce" -std=gnu99 -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


