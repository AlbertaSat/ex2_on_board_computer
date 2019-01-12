################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../emlib/src/em_acmp.c \
../emlib/src/em_adc.c \
../emlib/src/em_aes.c \
../emlib/src/em_assert.c \
../emlib/src/em_burtc.c \
../emlib/src/em_can.c \
../emlib/src/em_cmu.c \
../emlib/src/em_core.c \
../emlib/src/em_cryotimer.c \
../emlib/src/em_crypto.c \
../emlib/src/em_csen.c \
../emlib/src/em_dac.c \
../emlib/src/em_dbg.c \
../emlib/src/em_dma.c \
../emlib/src/em_ebi.c \
../emlib/src/em_emu.c \
../emlib/src/em_gpcrc.c \
../emlib/src/em_gpio.c \
../emlib/src/em_i2c.c \
../emlib/src/em_idac.c \
../emlib/src/em_int.c \
../emlib/src/em_lcd.c \
../emlib/src/em_ldma.c \
../emlib/src/em_lesense.c \
../emlib/src/em_letimer.c \
../emlib/src/em_leuart.c \
../emlib/src/em_mpu.c \
../emlib/src/em_msc.c \
../emlib/src/em_opamp.c \
../emlib/src/em_pcnt.c \
../emlib/src/em_prs.c \
../emlib/src/em_qspi.c \
../emlib/src/em_rmu.c \
../emlib/src/em_rtc.c \
../emlib/src/em_rtcc.c \
../emlib/src/em_system.c \
../emlib/src/em_timer.c \
../emlib/src/em_usart.c \
../emlib/src/em_vcmp.c \
../emlib/src/em_vdac.c \
../emlib/src/em_wdog.c 

OBJS += \
./emlib/src/em_acmp.o \
./emlib/src/em_adc.o \
./emlib/src/em_aes.o \
./emlib/src/em_assert.o \
./emlib/src/em_burtc.o \
./emlib/src/em_can.o \
./emlib/src/em_cmu.o \
./emlib/src/em_core.o \
./emlib/src/em_cryotimer.o \
./emlib/src/em_crypto.o \
./emlib/src/em_csen.o \
./emlib/src/em_dac.o \
./emlib/src/em_dbg.o \
./emlib/src/em_dma.o \
./emlib/src/em_ebi.o \
./emlib/src/em_emu.o \
./emlib/src/em_gpcrc.o \
./emlib/src/em_gpio.o \
./emlib/src/em_i2c.o \
./emlib/src/em_idac.o \
./emlib/src/em_int.o \
./emlib/src/em_lcd.o \
./emlib/src/em_ldma.o \
./emlib/src/em_lesense.o \
./emlib/src/em_letimer.o \
./emlib/src/em_leuart.o \
./emlib/src/em_mpu.o \
./emlib/src/em_msc.o \
./emlib/src/em_opamp.o \
./emlib/src/em_pcnt.o \
./emlib/src/em_prs.o \
./emlib/src/em_qspi.o \
./emlib/src/em_rmu.o \
./emlib/src/em_rtc.o \
./emlib/src/em_rtcc.o \
./emlib/src/em_system.o \
./emlib/src/em_timer.o \
./emlib/src/em_usart.o \
./emlib/src/em_vcmp.o \
./emlib/src/em_vdac.o \
./emlib/src/em_wdog.o 

C_DEPS += \
./emlib/src/em_acmp.d \
./emlib/src/em_adc.d \
./emlib/src/em_aes.d \
./emlib/src/em_assert.d \
./emlib/src/em_burtc.d \
./emlib/src/em_can.d \
./emlib/src/em_cmu.d \
./emlib/src/em_core.d \
./emlib/src/em_cryotimer.d \
./emlib/src/em_crypto.d \
./emlib/src/em_csen.d \
./emlib/src/em_dac.d \
./emlib/src/em_dbg.d \
./emlib/src/em_dma.d \
./emlib/src/em_ebi.d \
./emlib/src/em_emu.d \
./emlib/src/em_gpcrc.d \
./emlib/src/em_gpio.d \
./emlib/src/em_i2c.d \
./emlib/src/em_idac.d \
./emlib/src/em_int.d \
./emlib/src/em_lcd.d \
./emlib/src/em_ldma.d \
./emlib/src/em_lesense.d \
./emlib/src/em_letimer.d \
./emlib/src/em_leuart.d \
./emlib/src/em_mpu.d \
./emlib/src/em_msc.d \
./emlib/src/em_opamp.d \
./emlib/src/em_pcnt.d \
./emlib/src/em_prs.d \
./emlib/src/em_qspi.d \
./emlib/src/em_rmu.d \
./emlib/src/em_rtc.d \
./emlib/src/em_rtcc.d \
./emlib/src/em_system.d \
./emlib/src/em_timer.d \
./emlib/src/em_usart.d \
./emlib/src/em_vcmp.d \
./emlib/src/em_vdac.d \
./emlib/src/em_wdog.d 


# Each subdirectory must supply rules for building sources it contributes
emlib/src/em_acmp.o: ../emlib/src/em_acmp.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_acmp.d" -MT"emlib/src/em_acmp.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_adc.o: ../emlib/src/em_adc.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_adc.d" -MT"emlib/src/em_adc.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_aes.o: ../emlib/src/em_aes.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_aes.d" -MT"emlib/src/em_aes.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_assert.o: ../emlib/src/em_assert.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_assert.d" -MT"emlib/src/em_assert.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_burtc.o: ../emlib/src/em_burtc.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_burtc.d" -MT"emlib/src/em_burtc.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_can.o: ../emlib/src/em_can.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_can.d" -MT"emlib/src/em_can.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_cmu.o: ../emlib/src/em_cmu.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_cmu.d" -MT"emlib/src/em_cmu.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_core.o: ../emlib/src/em_core.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_core.d" -MT"emlib/src/em_core.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_cryotimer.o: ../emlib/src/em_cryotimer.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_cryotimer.d" -MT"emlib/src/em_cryotimer.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_crypto.o: ../emlib/src/em_crypto.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_crypto.d" -MT"emlib/src/em_crypto.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_csen.o: ../emlib/src/em_csen.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_csen.d" -MT"emlib/src/em_csen.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_dac.o: ../emlib/src/em_dac.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_dac.d" -MT"emlib/src/em_dac.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_dbg.o: ../emlib/src/em_dbg.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_dbg.d" -MT"emlib/src/em_dbg.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_dma.o: ../emlib/src/em_dma.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_dma.d" -MT"emlib/src/em_dma.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_ebi.o: ../emlib/src/em_ebi.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_ebi.d" -MT"emlib/src/em_ebi.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_emu.o: ../emlib/src/em_emu.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_emu.d" -MT"emlib/src/em_emu.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_gpcrc.o: ../emlib/src/em_gpcrc.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_gpcrc.d" -MT"emlib/src/em_gpcrc.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_gpio.o: ../emlib/src/em_gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_gpio.d" -MT"emlib/src/em_gpio.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_i2c.o: ../emlib/src/em_i2c.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_i2c.d" -MT"emlib/src/em_i2c.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_idac.o: ../emlib/src/em_idac.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_idac.d" -MT"emlib/src/em_idac.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_int.o: ../emlib/src/em_int.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_int.d" -MT"emlib/src/em_int.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_lcd.o: ../emlib/src/em_lcd.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_lcd.d" -MT"emlib/src/em_lcd.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_ldma.o: ../emlib/src/em_ldma.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_ldma.d" -MT"emlib/src/em_ldma.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_lesense.o: ../emlib/src/em_lesense.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_lesense.d" -MT"emlib/src/em_lesense.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_letimer.o: ../emlib/src/em_letimer.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_letimer.d" -MT"emlib/src/em_letimer.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_leuart.o: ../emlib/src/em_leuart.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_leuart.d" -MT"emlib/src/em_leuart.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_mpu.o: ../emlib/src/em_mpu.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_mpu.d" -MT"emlib/src/em_mpu.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_msc.o: ../emlib/src/em_msc.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_msc.d" -MT"emlib/src/em_msc.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_opamp.o: ../emlib/src/em_opamp.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_opamp.d" -MT"emlib/src/em_opamp.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_pcnt.o: ../emlib/src/em_pcnt.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_pcnt.d" -MT"emlib/src/em_pcnt.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_prs.o: ../emlib/src/em_prs.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_prs.d" -MT"emlib/src/em_prs.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_qspi.o: ../emlib/src/em_qspi.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_qspi.d" -MT"emlib/src/em_qspi.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_rmu.o: ../emlib/src/em_rmu.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_rmu.d" -MT"emlib/src/em_rmu.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_rtc.o: ../emlib/src/em_rtc.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_rtc.d" -MT"emlib/src/em_rtc.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_rtcc.o: ../emlib/src/em_rtcc.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_rtcc.d" -MT"emlib/src/em_rtcc.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_system.o: ../emlib/src/em_system.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_system.d" -MT"emlib/src/em_system.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_timer.o: ../emlib/src/em_timer.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_timer.d" -MT"emlib/src/em_timer.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_usart.o: ../emlib/src/em_usart.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_usart.d" -MT"emlib/src/em_usart.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_vcmp.o: ../emlib/src/em_vcmp.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_vcmp.d" -MT"emlib/src/em_vcmp.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_vdac.o: ../emlib/src/em_vdac.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_vdac.d" -MT"emlib/src/em_vdac.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/src/em_wdog.o: ../emlib/src/em_wdog.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"emlib/src/em_wdog.d" -MT"emlib/src/em_wdog.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


