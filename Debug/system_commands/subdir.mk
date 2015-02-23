################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../system_commands/SystemCommandManager.cpp 

OBJS += \
./system_commands/SystemCommandManager.o 

CPP_DEPS += \
./system_commands/SystemCommandManager.d 


# Each subdirectory must supply rules for building sources it contributes
system_commands/%.o: ../system_commands/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


