# Keep the makefile compatible with SeaBreeze so this can be built with seabreeze

SEABREEZE = ../../SeaBreeze

APPS = heliospectraSTSdaemon
OBJS = $(addsuffix .o,$(APPS))
SUBDIRS=

all: deps objs $(APPS)


.PHONY:  help

help:
	@echo "Supported targets include:"
	@echo
	@echo "  make                   Build library, tests and samples (best with -j)"
	@echo "  make clean             Remove most build artifacts"
	@echo "  make help              This screen"
	

include $(SEABREEZE)/common.mk

$(APPS) : $(OBJS)
	@echo linking $@
	$(CC) -o $@  ${OBJS_C} -lseabreeze $(LFLAGS_APP)

