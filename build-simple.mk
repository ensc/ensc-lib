CC = gcc
TAR = tar
SED = sed
CMP = cmp
XZ = xz
PKG_CONFIG = pkg-config
XXD = xxd

BUILD_CC = $(CC)
BUILD_CFLAGS ?= $(CFLAGS)
BUILD_CPPFLAGS ?= $(CPPFLAGS)

MSGFMT = msgfmt
XGETTEXT = xgettext
MSGMERGE = msgmerge

TARFLAGS = --owner=root --group=root --mode=a+rX,go-w

INSTALL = install -p
MKDIR_P = install -d -m 0755
INSTALL_DATA = $(INSTALL) -m 0644
INSTALL_PROG = $(INSTALL) -m 0755

abs_top_builddir = $(abspath ./)

_createtarball = $(TAR) cf $1 $(TARFLAGS) $(if $2,-P --transform='s!^(($(abs_top_srcdir))|($(abs_top_builddir)/?))?!$2/!x') $3
_buildflags = $(foreach k,CPP $1 LD, $(AM_$2$kFLAGS) $($2$kFLAGS) $($kFLAGS_$@))

_pkg_get_cflags =	$(shell $(PKG_CONFIG) --cflags $1)
_pkg_get_libs =		$(shell $(PKG_CONFIG) --libs $1)

_pre_process = echo '$1' | $(CC) $(AM_CPPFLAGS) $(CPPFLAGS) $2 -E -
_find_symbol = $(shell $(call _pre_process,'$2',$3) | grep -q '\<$1\>' && S=HAVE_ || S=NO_; echo "-D$${S}$1=1")

prefix = /usr/local
bindir = $(prefix)/bin
sbindir = $(prefix)/sbin
libdir = $(prefix)/lib
libexecdir = $(prefix)/libexec
datadir = $(prefix)/share
localedir = $(datadir)/locale

localstatedir = $(prefix)/var

pkglibdir = $(libdir)/$(PACKAGE)
pkgdatadir = $(datadir)/$(PACKAGE)
pkglibexecdir = $(libexecdir)/$(PACKAGE)
pkgstatedir = $(localstatedir)/$(PACKAGE)

_target_types = PROGRAMS MODULES SCRIPTS

### default goal ###

all:

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
POTFILE ?=	$1/$$(PACKAGE).pot

_po_files = $$(addsuffix .po,$$(addprefix $1/,$$(LINGUAS)))
_mo_files = $$(_po_files:%.po=%.mo)
_full_mo_path = $$(DESTDIR)$$(localedir)/$$1/LC_MESSAGES/$$(PACKAGE).mo
_all_inst_mo  = $$(foreach l,$$(LINGUAS),$$(call _full_mo_path,$$l))

$$(_all_inst_mo):$$(call _full_mo_path,%):	$1/%.mo
	$$(MKDIR_P) $${@D}
	$$(INSTALL_DATA) $$< $$@

$$(PACKAGE)-$$(VERSION).tar:	$$(_po_files)

i18n:		$$(_mo_files)
install-i18n:	$$(_all_inst_mo)
install:	install-i18n
all:		i18n

clean-i18n:
	rm -f $$(_mo_files)
clean:	clean-i18n

$$(abspath $1):
	$$(MKDIR_P) $$@

%.mo:	%.po | $$(abspath $1)
	$$(MSGFMT) -c -o $$@ $$<

ifeq ($$(abspath $$(abs_top_srcdir)),$$(abspath $$(abs_top_builddir)))
%.po:	$$(POTFILE)
	$$(MSGMERGE) $$(AM_MSGMERGE_FLAGS) $$@ $$<
	@touch $$@

$$(POTFILE):	$3
	@rm -f $$@.tmp $$@.tmp1 $$@.tmp2
	$$(XGETTEXT) $$(AM_XGETTEXT_FLAGS) $$^ -o $$@.tmp
	$(SED) '1,/^$$$$/s/^"POT-Creation-Date: .*//' $$@.tmp > $$@.tmp1
	$(SED) '1,/^$$$$/s/^"POT-Creation-Date: .*//' $$@     > $$@.tmp2 || :
	$(CMP) -s $$@.tmp1 $$@.tmp2 || mv -f $$@.tmp $$@
	@rm -f $$@.tmp $$@.tmp1 $$@.tmp2
	@touch $$@
endif

.SECONDARY:	$$(POTFILE)

ifneq ($4,)
_build_mo_links = $$(addsuffix /LC_MESSAGES/$$(PACKAGE).mo,$$(addprefix $4/,$$(LINGUAS)))
i18n-buildenv:	$4/.stamp
$4/.stamp:	| $$(_build_mo_links)
	@touch $$@

$$(_build_mo_links):$4/%/LC_MESSAGES/$$(PACKAGE).mo:	$1/%.po
	@rm -rf $${@D}
	@$$(MKDIR_P) $${@D}
	ln -s $$(abspath $$(patsubst %.po,%.mo,$$<)) $$@
endif

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

$$(PACKAGE)-$$(VERSION).tar:	$2 $$(MAKEFILE_LIST)
	$(call _createtarball,$$@,$$(PACKAGE)-$$(VERSION),$$(sort $$(abspath $$^)))
endef
