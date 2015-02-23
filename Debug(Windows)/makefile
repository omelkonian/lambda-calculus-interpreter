################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include tuples/subdir.mk
-include translator/subdir.mk
-include token/subdir.mk
-include tests/subdir.mk
-include system_commands/subdir.mk
-include scanner/subdir.mk
-include parser/subdir.mk
-include operators/subdir.mk
-include list/subdir.mk
-include linenoise/subdir.mk
-include evaluator/subdir.mk
-include error_handler/subdir.mk
-include church_numerals/subdir.mk
-include alias_manager/subdir.mk
-include abstract_syntax_tree/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Lambda\ Interpreter.exe

# Tool invocations
Lambda\ Interpreter.exe: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cygwin C++ Linker'
	g++ -L"C:\cygwin\lib" -o "Lambda Interpreter.exe" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(CPP_DEPS)$(C_DEPS) "Lambda Interpreter.exe"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
