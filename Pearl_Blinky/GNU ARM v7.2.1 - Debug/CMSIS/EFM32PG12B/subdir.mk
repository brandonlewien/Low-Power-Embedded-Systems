################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../CMSIS/EFM32PG12B/startup_gcc_efm32pg12b.s 

C_SRCS += \
../CMSIS/EFM32PG12B/system_efm32pg12b.c 

OBJS += \
./CMSIS/EFM32PG12B/startup_gcc_efm32pg12b.o \
./CMSIS/EFM32PG12B/system_efm32pg12b.o 

C_DEPS += \
./CMSIS/EFM32PG12B/system_efm32pg12b.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/EFM32PG12B/%.o: ../CMSIS/EFM32PG12B/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Assembler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -c -x assembler-with-cpp -I"C:/ECEN3360_Spring2019/Pearl_Blinky/hal-config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/halconfig/inc/hal-config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/common/halconfig" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/common/drivers" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/Device/SiliconLabs/EFM32PG12B/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/emlib/inc" '-DHAL_CONFIG=1' '-DEFM32PG12B500F1024GL125=1' -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CMSIS/EFM32PG12B/system_efm32pg12b.o: ../CMSIS/EFM32PG12B/system_efm32pg12b.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DHAL_CONFIG=1' '-DEFM32PG12B500F1024GL125=1' -I"C:/ECEN3360_Spring2019/Pearl_Blinky/hal-config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/halconfig/inc/hal-config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/common/halconfig" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//hardware/kit/common/drivers" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/Device/SiliconLabs/EFM32PG12B/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/emlib/inc" -O0 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"CMSIS/EFM32PG12B/system_efm32pg12b.d" -MT"CMSIS/EFM32PG12B/system_efm32pg12b.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


