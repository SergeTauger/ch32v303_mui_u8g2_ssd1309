################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/debounce/subdir.mk
-include User/GUI/u8g2/subdir.mk
-include User/GUI/subdir.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Peripheral/src/subdir.mk
-include Debug/subdir.mk
-include Core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
Flowmeter_HMI.hex \

SECONDARY_LIST += \
Flowmeter_HMI.lst \

SECONDARY_SIZE += \
Flowmeter_HMI.siz \


# All Target
all: Flowmeter_HMI.elf secondary-outputs

# Tool invocations
Flowmeter_HMI.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU RISC-V Cross C Linker'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -T "/home/stauger/Projects/u8g2_mui_demo/ch32v303_mui_u8g2_ssd1309/Ld/Link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"Flowmeter_HMI.map" --specs=nano.specs --specs=nosys.specs -o "Flowmeter_HMI.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Flowmeter_HMI.hex: Flowmeter_HMI.elf
	@echo 'Invoking: GNU RISC-V Cross Create Flash Image'
	riscv-none-embed-objcopy -O ihex "Flowmeter_HMI.elf"  "Flowmeter_HMI.hex"
	@echo 'Finished building: $@'
	@echo ' '

Flowmeter_HMI.lst: Flowmeter_HMI.elf
	@echo 'Invoking: GNU RISC-V Cross Create Listing'
	riscv-none-embed-objdump --all-headers --demangle --disassemble "Flowmeter_HMI.elf" > "Flowmeter_HMI.lst"
	@echo 'Finished building: $@'
	@echo ' '

Flowmeter_HMI.siz: Flowmeter_HMI.elf
	@echo 'Invoking: GNU RISC-V Cross Print Size'
	riscv-none-embed-size --format=berkeley "Flowmeter_HMI.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) Flowmeter_HMI.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
