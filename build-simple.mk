CC = gcc
TAR = tar
XZ = xz
PKG_CONFIG = pkg-config
MSGFMT = msgfmt
XXD = xxd

TARFLAGS = --owner=root --group=root --mode=a+rX,go-w

INSTALL = install -p
MKDIR_P = install -d -m 0755
INSTALL_DATA = $(INSTALL) -m 0644
INSTALL_PROG = $(INSTALL) -m 0755

abs_top_builddir = $(abspath ./)

_createtarball = $(TAR) cf $1 $(TARFLAGS) $(if $2,-P --transform='s!^(($(abs_top_srcdir))|($(abs_top_builddir)/?))?!$2/!x') $3
_buildflags = $(foreach k,CPP $1 LD, $(AM_$kFLAGS) $($kFLAGS) $($kFLAGS_$@))

_pkg_get_cflags =	$(shell $(PKG_CONFIG) --cflags $1)
_pkg_get_libs =		$(shell $(PKG_CONFIG) --libs $1)

prefix = /usr/local
bindir = $(prefix)/bin
sbindir = $(prefix)/sbin
libdir = $(prefix)/lib
libexecdir = $(prefix)/libexec
datadir = $(prefix)/share
localedir = $(datadir)/locale

pkglibdir = $(libdir)/$(PACKAGE)
pkgdatadir = $(datadir)/$(PACKAGE)
pkglibexecdir = $(libexecdir)/$(PACKAGE)

_target_types = PROGRAMS MODULES SCRIPTS

#################

define _installrule
_targets :=	$(foreach t,${_target_types},$($1_$t) )
_dstdir :=	$$(DESTDIR)$$(${1}dir)/
_abstargets :=	$$(addprefix $${_dstdir},$$(_targets))

all:		${_targets}
install:	install-$1
install-$1:	$$(_abstargets)

$${_dstdir}:
	mkdir -p $$@

$$(addprefix $${_dstdir},$${$1_PROGRAMS}):	_install_method=$${INSTALL_PROG}
$$(addprefix $${_dstdir},$${$1_MODULES}):	_install_method=$${INSTALL_PROG}

$$(_abstargets):$$(DESTDIR)$$(${1}dir)/%:	% | $${_dstdir}
	$${_install_method} $$< $$@

endef

################

define _i18nrule
LINGUAS ?=	$2

_po_files = $$(addsuffix .po,$$(addprefix $1/,$$(LINGUAS)))
_mo_files = $$(_po_files:%.po=%.mo)
_full_mo_path = $$(DESTDIR)$$(localedir)/$$1/LC_MESSAGES/$$(PACKAGE).mo
_all_inst_mo  = $$(foreach l,$$(LINGUAS),$$(call _full_mo_path,$$l))

$$(_all_inst_mo):$$(call _full_mo_path,%):	$1/%.mo
	$$(MKDIR_P) $${@D}
	$$(INSTALL_DATA) $$< $$@

i18n:		$$(_mo_files)
install-i18n:	$$(_all_inst_mo)
install:	install-i18n
all:		i18n

clean-i18n:
	rm -f $$(_mo_files)
clean:	clean-i18n

%.mo:	%.po
	$$(MSGFMT) -c -o $$@ $$<
endef

################

define _compressrule
%.xz:	%
	@rm -f $$@
	$$(XZ) -c $$< > $$@
endef

################

define _distrule
$(eval $(call _compressrule))

dist:	$(addprefix $$(PACKAGE)-$$(VERSION).tar,$1)

$$(PACKAGE)-$$(VERSION).tar:	$2 $$(MAKEFILE_LIST) $(_po_files)
	$(call _createtarball,$$@,$$(PACKAGE)-$$(VERSION),$$(sort $$(abspath $$^)))
endef
