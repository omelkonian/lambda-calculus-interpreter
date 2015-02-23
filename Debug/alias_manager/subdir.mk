################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../alias_manager/AliasManager.cpp \
../alias_manager/Consultor.cpp 

OBJS += \
./alias_manager/AliasManager.o \
./alias_manager/Consultor.o 

CPP_DEPS += \
./alias_manager/AliasManager.d \
./alias_manager/Consultor.d 


# Each subdirectory must supply rules for building sources it contributes
alias_manager/%.o: ../alias_manager/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


