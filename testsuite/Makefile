TESTS = \
	ipaddress \
	serialize \
	iniparser

CFLAGS = \
	--std=gnu99 -Wall -W -pedantic -Werror \
	-Wno-pointer-arith -Wno-unused-parameter \
	-O2 -g3 \
	-DFORTIFY_SOURCE=2 -D_GNU_SOURCE

all:	$(addprefix .run-,${TESTS})

.run-%:	%
	$(abspath $*)

ipaddress:	LDLIBS=-lcom_err
ipaddress:	ipaddress.c ../ipaddress.c ../ipaddress.h

serialize:	serialize.c

iniparser:	LDLIBS=-liniparser -lcom_err
iniparser:	iniparser.c ../iniparser.c ../iniparser-pwdb.c ../iniparser.h
