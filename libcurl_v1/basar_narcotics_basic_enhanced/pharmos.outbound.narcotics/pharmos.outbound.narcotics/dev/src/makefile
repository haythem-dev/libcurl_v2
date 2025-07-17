MAKEFILE.GEN = Makefile

BASE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
EXT_DIR  := $(abspath $(BASE_DIR)../ext)
QT_DIR   := $(EXT_DIR)/qt5

all clean distclean install: $(MAKEFILE.GEN)
	$(MAKE) -f $(MAKEFILE.GEN) $@

$(MAKEFILE.GEN):
	#$(DEVLIB_PATH)/marathon/lib/qt5/5.2.0_2/bin/qmake -makefile -Wall -unix -o $(MAKEFILE.GEN);
	$(QT_DIR)/bin/qmake -makefile -Wall -unix -o $(MAKEFILE.GEN);
