# invoke SourceDir generated makefile for empty.pem3
empty.pem3: .libraries,empty.pem3
.libraries,empty.pem3: package/cfg/empty_pem3.xdl
	$(MAKE) -f C:\Users\Tom\workspace_v7\I2C_port_for_bq27441/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\Tom\workspace_v7\I2C_port_for_bq27441/src/makefile.libs clean

