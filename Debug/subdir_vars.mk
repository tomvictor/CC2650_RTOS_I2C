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
../Board.c \
../CC2650_LAUNCHXL.c \
../ccfg.c \
../empty.c 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_MISC_DIRS += \
./configPkg/ 

C_DEPS += \
./Board.d \
./CC2650_LAUNCHXL.d \
./ccfg.d \
./empty.d 

GEN_OPTS += \
./configPkg/compiler.opt 

OBJS += \
./Board.obj \
./CC2650_LAUNCHXL.obj \
./ccfg.obj \
./empty.obj 

GEN_MISC_DIRS__QUOTED += \
"configPkg\" 

OBJS__QUOTED += \
"Board.obj" \
"CC2650_LAUNCHXL.obj" \
"ccfg.obj" \
"empty.obj" 

C_DEPS__QUOTED += \
"Board.d" \
"CC2650_LAUNCHXL.d" \
"ccfg.d" \
"empty.d" 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" 

C_SRCS__QUOTED += \
"../Board.c" \
"../CC2650_LAUNCHXL.c" \
"../ccfg.c" \
"../empty.c" 


