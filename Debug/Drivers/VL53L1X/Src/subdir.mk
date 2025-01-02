################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/VL53L1X/Src/VL53L1X.c \
../Drivers/VL53L1X/Src/VL53L1X_api.c \
../Drivers/VL53L1X/Src/VL53L1X_calibration.c \
../Drivers/VL53L1X/Src/vl53l1_platform.c 

OBJS += \
./Drivers/VL53L1X/Src/VL53L1X.o \
./Drivers/VL53L1X/Src/VL53L1X_api.o \
./Drivers/VL53L1X/Src/VL53L1X_calibration.o \
./Drivers/VL53L1X/Src/vl53l1_platform.o 

C_DEPS += \
./Drivers/VL53L1X/Src/VL53L1X.d \
./Drivers/VL53L1X/Src/VL53L1X_api.d \
./Drivers/VL53L1X/Src/VL53L1X_calibration.d \
./Drivers/VL53L1X/Src/vl53l1_platform.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/VL53L1X/Src/%.o Drivers/VL53L1X/Src/%.su Drivers/VL53L1X/Src/%.cyclo: ../Drivers/VL53L1X/Src/%.c Drivers/VL53L1X/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F101xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"/home/rasmus/STM32CubeIDE/workspace_1.14.0/Test/Drivers/VL53L1X" -I"/home/rasmus/STM32CubeIDE/workspace_1.14.0/Test/Drivers/VL53L1X/Inc" -I"/home/rasmus/STM32CubeIDE/workspace_1.14.0/Test/Drivers/VL53L1X/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-VL53L1X-2f-Src

clean-Drivers-2f-VL53L1X-2f-Src:
	-$(RM) ./Drivers/VL53L1X/Src/VL53L1X.cyclo ./Drivers/VL53L1X/Src/VL53L1X.d ./Drivers/VL53L1X/Src/VL53L1X.o ./Drivers/VL53L1X/Src/VL53L1X.su ./Drivers/VL53L1X/Src/VL53L1X_api.cyclo ./Drivers/VL53L1X/Src/VL53L1X_api.d ./Drivers/VL53L1X/Src/VL53L1X_api.o ./Drivers/VL53L1X/Src/VL53L1X_api.su ./Drivers/VL53L1X/Src/VL53L1X_calibration.cyclo ./Drivers/VL53L1X/Src/VL53L1X_calibration.d ./Drivers/VL53L1X/Src/VL53L1X_calibration.o ./Drivers/VL53L1X/Src/VL53L1X_calibration.su ./Drivers/VL53L1X/Src/vl53l1_platform.cyclo ./Drivers/VL53L1X/Src/vl53l1_platform.d ./Drivers/VL53L1X/Src/vl53l1_platform.o ./Drivers/VL53L1X/Src/vl53l1_platform.su

.PHONY: clean-Drivers-2f-VL53L1X-2f-Src

