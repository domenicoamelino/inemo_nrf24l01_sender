################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/iNEMO_Led.c \
../src/iNemo_SPI_Driver.c \
../src/main.c \
../src/syscall.c \
../src/tm_stm32f4_nrf24l01.c \
../src/tm_stm32f4_spi.c 

OBJS += \
./src/iNEMO_Led.o \
./src/iNemo_SPI_Driver.o \
./src/main.o \
./src/syscall.o \
./src/tm_stm32f4_nrf24l01.o \
./src/tm_stm32f4_spi.o 

C_DEPS += \
./src/iNEMO_Led.d \
./src/iNemo_SPI_Driver.d \
./src/main.d \
./src/syscall.d \
./src/tm_stm32f4_nrf24l01.d \
./src/tm_stm32f4_spi.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -DSTM32F10X_HD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -DDEBUG -I"../include" -I"../libs/cmsis/include" -I"../libs/StdPeriph/include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


