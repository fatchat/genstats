# Makefile for 

CC = g++
DEBUG = -g
INCLUDE = -I $(PWD)
PROFILE =

CFLAGS = -Wall -c $(DEBUG) $(INCLUDE) $(PROFILE)
LFLAGS = -Wall $(DEBUG) $(PROFILE) -L$(PWD)/lib

OTHERLIBS = 
MYLIBS = -lio

MYLIBDIR = lib
OBJDIR = objectfiles
BINDIR = bin

vpath %.cpp io:main
#vpath %.o $(OBJDIR)

# libs
IO_OBJS = cqinput.o

# targets
CQTEST_OBJS = cqtest.o
cqtestOBJS = $(patsubst %, $(OBJDIR)/%, $(CQTEST_OBJS))

T2B_OBJS = text2bin.o
text2binOBJS = $(patsubst %, $(OBJDIR)/%, $(T2B_OBJS))

TARGETS = cqtest \
	text2bin

libioOBJS = $(patsubst %, $(OBJDIR)/%, $(IO_OBJS))

LIBTARGETS = libio

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

