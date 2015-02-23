################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../error_handler/AutoCorrector.cpp \
../error_handler/error_handling.cpp 

OBJS += \
./error_handler/AutoCorrector.o \
./error_handler/error_handling.o 

CPP_DEPS += \
./error_handler/AutoCorrector.d \
./error_handler/error_handling.d 


# Each subdirectory must supply rules for building sources it contributes
error_handler/%.o: ../error_handler/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


