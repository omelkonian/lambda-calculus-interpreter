################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../translator/Translator.cpp 

OBJS += \
./translator/Translator.o 

CPP_DEPS += \
./translator/Translator.d 


# Each subdirectory must supply rules for building sources it contributes
translator/%.o: ../translator/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"C:\cygwin\usr\include\readline" -O0 -g3 -Wall -c -fmessage-length=0 -lreadline -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


