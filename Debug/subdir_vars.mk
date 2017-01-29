################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../empty.cfg 

CMD_SRCS += \
../CC2650_LAUNCHXL.cmd 

C_SRCS += \
../ccfg.c \
../main.c 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_MISC_DIRS += \
./configPkg/ 

C_DEPS += \
./ccfg.d \
./main.d 

GEN_OPTS += \
./configPkg/compiler.opt 

OBJS += \
./ccfg.obj \
./main.obj 

GEN_MISC_DIRS__QUOTED += \
"configPkg\" 

OBJS__QUOTED += \
"ccfg.obj" \
"main.obj" 

C_DEPS__QUOTED += \
"ccfg.d" \
"main.d" 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" 

C_SRCS__QUOTED += \
"../ccfg.c" \
"../main.c" 


