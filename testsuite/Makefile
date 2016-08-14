TESTS = \
	ipaddress \
	serialize \
	iniparser \
	safe_calloc \
	mgmmem \
	sd-misc \
	strbuf \
	type-utils \
	unserialize \

STD =	gnu11

CFLAGS = \
	--std=$(STD) -Wall -W -pedantic -Werror \
	-Wno-pointer-arith -Wno-unused-parameter \
	-O2 -g3 \
	-DFORTIFY_SOURCE=2 -D_GNU_SOURCE

all:	$(addprefix .run-,${TESTS})

.run-%:	%
	$(abspath $*)

ipaddress:	LDLIBS=-lcom_err
ipaddress:	ipaddress.c ../ipaddress.c ../ipaddress.h

serialize:	serialize.c
unserialize:	unserialize.c

iniparser:	LDLIBS=-liniparser -lcom_err
iniparser:	iniparser.c ../iniparser.c ../iniparser-pwdb.c ../iniparser.h

safe_calloc:	safe_calloc.c ../safe_calloc.h
strbuf:		strbuf.c ../strbuf.h

mgmmem:		LDLIBS=-lcom_err
mgmmem:		mgmmem.c ../mgmmem.h ../mgmmem.c

sd-misc:	sd-misc.c

type-utils:	type-utils.c ../type-utils.h

$(TESTS):
	$(CC) $(CFLAGS) $(LDFLAGS) $(filter %.c,$^) -o $@ $(LDLIBS)

clean:
	rm -f ${TESTS}
