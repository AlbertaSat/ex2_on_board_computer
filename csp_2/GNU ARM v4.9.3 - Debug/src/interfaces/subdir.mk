################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/interfaces/csp_if_i2c.c \
../src/interfaces/csp_if_lo.c 

OBJS += \
./src/interfaces/csp_if_i2c.o \
./src/interfaces/csp_if_lo.o 

C_DEPS += \
./src/interfaces/csp_if_i2c.d \
./src/interfaces/csp_if_lo.d 


# Each subdirectory must supply rules for building sources it contributes
src/interfaces/csp_if_i2c.o: ../src/interfaces/csp_if_i2c.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/interfaces/csp_if_i2c.d" -MT"src/interfaces/csp_if_i2c.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/interfaces/csp_if_lo.o: ../src/interfaces/csp_if_lo.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/interfaces/csp_if_lo.d" -MT"src/interfaces/csp_if_lo.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


