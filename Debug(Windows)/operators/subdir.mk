################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../operators/OperatorManager.cpp 

OBJS += \
./operators/OperatorManager.o 

CPP_DEPS += \
./operators/OperatorManager.d 


# Each subdirectory must supply rules for building sources it contributes
operators/%.o: ../operators/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"C:\cygwin\usr\include\readline" -O0 -g3 -Wall -c -fmessage-length=0 -lreadline -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


