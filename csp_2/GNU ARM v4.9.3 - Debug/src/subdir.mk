################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/csp_bridge.c \
../src/csp_buffer.c \
../src/csp_conn.c \
../src/csp_crc32.c \
../src/csp_debug.c \
../src/csp_dedup.c \
../src/csp_endian.c \
../src/csp_iflist.c \
../src/csp_io.c \
../src/csp_port.c \
../src/csp_promisc.c \
../src/csp_qfifo.c \
../src/csp_route.c \
../src/csp_service_handler.c \
../src/csp_services.c \
../src/csp_sfp.c 

OBJS += \
./src/csp_bridge.o \
./src/csp_buffer.o \
./src/csp_conn.o \
./src/csp_crc32.o \
./src/csp_debug.o \
./src/csp_dedup.o \
./src/csp_endian.o \
./src/csp_iflist.o \
./src/csp_io.o \
./src/csp_port.o \
./src/csp_promisc.o \
./src/csp_qfifo.o \
./src/csp_route.o \
./src/csp_service_handler.o \
./src/csp_services.o \
./src/csp_sfp.o 

C_DEPS += \
./src/csp_bridge.d \
./src/csp_buffer.d \
./src/csp_conn.d \
./src/csp_crc32.d \
./src/csp_debug.d \
./src/csp_dedup.d \
./src/csp_endian.d \
./src/csp_iflist.d \
./src/csp_io.d \
./src/csp_port.d \
./src/csp_promisc.d \
./src/csp_qfifo.d \
./src/csp_route.d \
./src/csp_service_handler.d \
./src/csp_services.d \
./src/csp_sfp.d 


# Each subdirectory must supply rules for building sources it contributes
src/csp_bridge.o: ../src/csp_bridge.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/csp_bridge.d" -MT"src/csp_bridge.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/csp_buffer.o: ../src/csp_buffer.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/csp_buffer.d" -MT"src/csp_buffer.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/csp_conn.o: ../src/csp_conn.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/csp_conn.d" -MT"src/csp_conn.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/csp_crc32.o: ../src/csp_crc32.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/csp_crc32.d" -MT"src/csp_crc32.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/csp_debug.o: ../src/csp_debug.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/csp_debug.d" -MT"src/csp_debug.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/csp_dedup.o: ../src/csp_dedup.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/csp_dedup.d" -MT"src/csp_dedup.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/csp_endian.o: ../src/csp_endian.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/csp_endian.d" -MT"src/csp_endian.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/csp_iflist.o: ../src/csp_iflist.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/csp_iflist.d" -MT"src/csp_iflist.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/csp_io.o: ../src/csp_io.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/csp_io.d" -MT"src/csp_io.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/csp_port.o: ../src/csp_port.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/csp_port.d" -MT"src/csp_port.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/csp_promisc.o: ../src/csp_promisc.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/csp_promisc.d" -MT"src/csp_promisc.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/csp_qfifo.o: ../src/csp_qfifo.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/csp_qfifo.d" -MT"src/csp_qfifo.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/csp_route.o: ../src/csp_route.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/csp_route.d" -MT"src/csp_route.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/csp_service_handler.o: ../src/csp_service_handler.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/csp_service_handler.d" -MT"src/csp_service_handler.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/csp_services.o: ../src/csp_services.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/csp_services.d" -MT"src/csp_services.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/csp_sfp.o: ../src/csp_sfp.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32GG/Include" -I"E:/SiliconLabs/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/EFM32GG_STK3700/config" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\include\csp" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\include" -I"E:\SiliconLabs\v4\CSPNetwork\freertos\src\efm32gg" -I"E:\SiliconLabs\v4\CSPNetwork\csp_2\emlib\inc" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/csp_sfp.d" -MT"src/csp_sfp.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


