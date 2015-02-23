################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../token/Token.cpp \
../token/TokenValue.cpp 

OBJS += \
./token/Token.o \
./token/TokenValue.o 

CPP_DEPS += \
./token/Token.d \
./token/TokenValue.d 


# Each subdirectory must supply rules for building sources it contributes
token/%.o: ../token/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"C:\cygwin\usr\include\readline" -O0 -g3 -Wall -c -fmessage-length=0 -lreadline -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


