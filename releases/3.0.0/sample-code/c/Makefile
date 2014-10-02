SEABREEZE = ../..

APPS = seabreeze-util $(basename $(wildcard demo-*.c)) 
OBJS = $(addsuffix .o,$(APPS))
UTIL = util.o

all: $(APPS) serial-sts-demo

include $(SEABREEZE)/common.mk

$(APPS) : $(OBJS) $(UTIL)
	@echo linking $@
	@$(CC) -o $@ $@.o $(UTIL) -lseabreeze $(LFLAGS_APP) -lpthread

serial-sts-demo : serial-sts-demo.o
	@echo linking $@
	@$(CC) -o $@ $@.o $(LFLAGS_APP) 
