################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ch32v30x_it.c \
../User/main.c \
../User/system_ch32v30x.c 

OBJS += \
./User/ch32v30x_it.o \
./User/main.o \
./User/system_ch32v30x.o 

C_DEPS += \
./User/ch32v30x_it.d \
./User/main.d \
./User/system_ch32v30x.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"/home/stauger/mrs_community-workspace/Flowmeter_HMI/Debug" -I"/home/stauger/mrs_community-workspace/Flowmeter_HMI/Core" -I"/home/stauger/mrs_community-workspace/Flowmeter_HMI/User" -I"/home/stauger/mrs_community-workspace/Flowmeter_HMI/Peripheral/inc" -I"/home/stauger/mrs_community-workspace/Flowmeter_HMI/User/GUI" -I"/home/stauger/mrs_community-workspace/Flowmeter_HMI/User/GUI/u8g2" -std=gnu99 -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


