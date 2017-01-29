## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,em3 linker.cmd package/cfg/empty_pem3.oem3

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/empty_pem3.xdl
	$(SED) 's"^\"\(package/cfg/empty_pem3cfg.cmd\)\"$""\"C:/Users/Tom/workspace_v7/I2C_port_for_bq27441/.config/xconfig_empty/\1\""' package/cfg/empty_pem3.xdl > $@
	-$(SETDATE) -r:max package/cfg/empty_pem3.h compiler.opt compiler.opt.defs
