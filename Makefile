# Makefile for 

CC = g++
DEBUG = -g
INCLUDE = -I $(PWD)
PROFILE =

CFLAGS = -Wall -c $(DEBUG) $(INCLUDE) $(PROFILE)
LFLAGS = -Wall $(DEBUG) $(PROFILE) -L$(PWD)/lib

OTHERLIBS = -lz
MYLIBS = -lio -lmath -lutil

MYLIBDIR = lib
OBJDIR = objectfiles
BINDIR = bin

vpath %.cpp io:main:math:util
#vpath %.o $(OBJDIR)

# libs
IO_OBJS = cqinput.o zlib_infdef.o
libioOBJS = $(patsubst %, $(OBJDIR)/%, $(IO_OBJS))

MATH_OBJS = linalg.o buckets.o entropy.o
libmathOBJS = $(patsubst %, $(OBJDIR)/%, $(MATH_OBJS))

UTIL_OBJS = get_opt.o
libutilOBJS = $(patsubst %, $(OBJDIR)/%, $(UTIL_OBJS))

LIBTARGETS = libio libmath libutil

# targets
CQTEST_OBJS = cqtest.o
cqtestOBJS = $(patsubst %, $(OBJDIR)/%, $(CQTEST_OBJS))

T2B_OBJS = text2bin.o
text2binOBJS = $(patsubst %, $(OBJDIR)/%, $(T2B_OBJS))

LINALG_OBJS = linalgtest.o
linalgOBJS = $(patsubst %, $(OBJDIR)/%, $(LINALG_OBJS))

ZLIBTEST_OBJS = zlibtest.o
zlibtestOBJS = $(patsubst %, $(OBJDIR)/%, $(ZLIBTEST_OBJS))

HIST_OBJS = histograms.o
histogramsOBJS = $(patsubst %, $(OBJDIR)/%, $(HIST_OBJS))

ENT_TEST_OBJS = ent_test.o
ent_testOBJS = $(patsubst %, $(OBJDIR)/%, $(ENT_TEST_OBJS))

TARGETS = cqtest \
	text2bin \
	linalg \
	zlibtest \
	histograms \
	ent_test \

.PHONY: clean all libs
all : libs $(TARGETS)
libs: $(LIBTARGETS)
clean :
	rm -f \
	$(foreach target,$(TARGETS),$(BINDIR)/$(target)) \
	$(OBJDIR)/*.o \
	$(foreach lib,$(LIBTARGETS),$(MYLIBDIR)/$(lib).so) \
	$(foreach lib,$(LIBTARGETS),$(MYLIBDIR)/$(lib).a)

define get_object_files
$(1): $$($(1)OBJS)
endef

$(foreach prog,$(TARGETS),$(eval $(call get_object_files,$(prog))))
$(TARGETS):
	$(CC) $(LFLAGS) $^ $(OTHERLIBS) $(MYLIBS) -o $(BINDIR)/$@

$(foreach lib,$(LIBTARGETS),$(eval $(call get_object_files,$(lib))))
$(LIBTARGETS):
	libtool -static -o $(MYLIBDIR)/$@.a $^

$(OBJDIR)/%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

help :
	@echo $(TARGETS)
	@echo all libs clean

