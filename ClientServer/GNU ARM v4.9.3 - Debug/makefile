################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include BSP/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: STK3700_freertos_demo.axf

# Tool invocations
STK3700_freertos_demo.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -T "STK3700_freertos_demo.ld" -L"E:\E drive\SiliconLabs\v4\CSPNetwork\freertos\GNU ARM v4.9.3 - Debug" -L"E:\E drive\SiliconLabs\v4\CSPNetwork\csp_2\GNU ARM v4.9.3 - Debug" -Xlinker --gc-sections -Xlinker -Map="STK3700_freertos_demo.map" --specs=nano.specs -o STK3700_freertos_demo.axf "./src/simple.o" "./emlib/em_assert.o" "./emlib/em_burtc.o" "./emlib/em_cmu.o" "./emlib/em_core.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_rmu.o" "./emlib/em_rtc.o" "./emlib/em_system.o" "./BSP/bsp_stk_leds.o" "./BSP/bsp_trace.o" "./src/low_power_tick_management.o" -lcsp_2 -lfreertos "./src/makePrintfWork.o" "./src/i2c_master_slave.o" "./src/sleep.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: STK3700_freertos_demo.hex'
	arm-none-eabi-objcopy -O ihex "STK3700_freertos_demo.axf" "STK3700_freertos_demo.hex"
	@echo ' '

	@echo 'Building bin file: STK3700_freertos_demo.bin'
	arm-none-eabi-objcopy -O binary "STK3700_freertos_demo.axf" "STK3700_freertos_demo.bin"
	@echo ' '

	@echo 'Building s37 file: STK3700_freertos_demo.s37'
	arm-none-eabi-objcopy -O srec "STK3700_freertos_demo.axf" "STK3700_freertos_demo.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "STK3700_freertos_demo.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) STK3700_freertos_demo.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
