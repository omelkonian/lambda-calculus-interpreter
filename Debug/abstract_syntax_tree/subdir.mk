################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../abstract_syntax_tree/AST.cpp \
../abstract_syntax_tree/InternalNode.cpp \
../abstract_syntax_tree/Leaf.cpp \
../abstract_syntax_tree/Node.cpp \
../abstract_syntax_tree/VariablePool.cpp 

OBJS += \
./abstract_syntax_tree/AST.o \
./abstract_syntax_tree/InternalNode.o \
./abstract_syntax_tree/Leaf.o \
./abstract_syntax_tree/Node.o \
./abstract_syntax_tree/VariablePool.o 

CPP_DEPS += \
./abstract_syntax_tree/AST.d \
./abstract_syntax_tree/InternalNode.d \
./abstract_syntax_tree/Leaf.d \
./abstract_syntax_tree/Node.d \
./abstract_syntax_tree/VariablePool.d 


# Each subdirectory must supply rules for building sources it contributes
abstract_syntax_tree/%.o: ../abstract_syntax_tree/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


