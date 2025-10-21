################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mini_Project4.c \
../gpio.c \
../icu.c \
../lcd.c \
../ultrasonic.c 

C_DEPS += \
./Mini_Project4.d \
./gpio.d \
./icu.d \
./lcd.d \
./ultrasonic.d 

OBJS += \
./Mini_Project4.o \
./gpio.o \
./icu.o \
./lcd.o \
./ultrasonic.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


