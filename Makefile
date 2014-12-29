CUSTOMIZATIONS ?= assert license logging

FN_assert ?= assert
HDR_assert ?= <assert.h>

FN_com_err ?= com_err
HDR_com_err = <et/com_err.h>

PROJECT_LICENSE ?= \x20*\n\
 * This program is free software; you can redistribute it and/or modify\n\
 * it under the terms of the GNU General Public License as published by\n\
 * the Free Software Foundation; version 3 of the License.\n\
 *\n\
 * This program is distributed in the hope that it will be useful,\n\
 * but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\
 * GNU General Public License for more details.\n\
 *\n\
 * You should have received a copy of the GNU General Public License\n\
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.\n

export PROJECT_LICENSE

CUSTOMIZATION_assert = \
  -e 's!^\([[:space:]]*\)assert\([[:space:]]*(\)!\1${FN_assert}\2!' \
  -e 's!^\([[:space:]]*\#[[:space:]]*include[[:space:]]*\)<assert.h>!\1${HDR_assert}!' \

CUSTOMIZATION_license = \
  -e 's!@PROJECT_LICENSE@!printf "$${PROJECT_LICENSE}"!e'

CUSTOMIZATION_logging = \
  -e 's!^\([[:space:]]*\)com_err\([[:space:]]*(\)!\1${FN_com_err}\2!' \
 -e 's!^\([[:space:]]*\#[[:space:]]*include[[:space:]]*\)<et/com_err.h>!\1${HDR_com_err}!' \

COMPONENT_compiler = \
  compiler.h \
  compiler-lint.h \
  compiler-gcc.h \

COMPONENT_list = \
  $(COMPONENT_compiler) \
  list.h \

COMPONENT_io = \
  io.c \
  io.h \

COMPONENT_iniparser = \
  iniparser.c \
  iniparser.h \
  $(COMPONENT_i18n) \
  $(COMPONENT_xalloc) \
  $(COMPONENT_compiler)

COMPONENT_iniparser-pwdb = \
  iniparser-pwdb.c \
  $(COMPONENT_iniparser)

COMPONENT_i18n = \
  i18n.h

COMPONENT_http = \
  http-i18n.c \
  http-i18n.h \
  $(COMPONENT_xalloc) \

COMPONENT_logging = \
  logging.c \
  logging.h \
  $(COMPONENT_compiler) \

COMPONENT_ldap = \
  ldap-open.c \
  ldap.c \
  ldap.h \
  $(COMPONENT_mgmmem) \
  $(COMPONENT_compiler) \
  $(COMPONENT_xalloc)

COMPONENT_mgmmem = \
  mgmmem.c \
  mgmmem.h \
  $(COMPONENT_xalloc)

COMPONENT_build-simple = \
  build-simple.mk \

COMPONENT_safe_calloc = \
  safe_calloc.h

COMPONENT_xalloc = \
  xalloc.h

COMPONENT_strbuf = \
  strbuf.h \
  $(COMPONENT_xalloc)

COMPONENT_serialize = \
  serialize.h \
  $(COMPONENT_compiler) \
  $(COMPONENT_xalloc)

COMPONENT_unserialize = \
  unserialize.h \
  $(COMPONENT_strbuf)

COMPONENT_systemd = \
  sd-notify.h \
  sd-notify.c \
  sd-notifyf.c \
  $(COMPONENT_compiler)

-include $(D)/setup.mk

_SED_COMMAND = $(foreach c,$(CUSTOMIZATIONS),$(CUSTOMIZATION_$c) )

_COMPONENT = $(sort $(foreach c,$(COMPONENTS),$(COMPONENT_$c)))

all:
	sed ${_SED_COMMAND} $(_COMPONENT)

install:	$(addprefix $D/,$(_COMPONENT))

$D:
	install -d -m 0755 $@

$(addprefix $D/,$(_COMPONENT)):$D/%:	% Makefile | $D
	@test ! -e $@.patch
	@rm -f $@.tmp
	sed ${_SED_COMMAND} $< > $@.tmp
	test -e $@ && diff -u $@ $@.tmp > $@.patch || :
	test -s $@.patch || rm -f $@.patch
	@if cmp $@.tmp $@; then rm $@.tmp; else mv $@.tmp $@; fi
