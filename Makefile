BUILD_PATH=build

VERSION            = $(shell git describe --tags --abbrev=0)
HASH               = $(shell git rev-parse --short HEAD)
DMRCONFIG_VERSION  = $(shell git submodule status)
GITCOUNT           = $(shell git rev-list HEAD --count)

CFLAGS   ?= -g -O -Wall -Werror -fPIC -MMD -fcommon
CFLAGS   += -DVERSION='"$(VERSION).$(HASH)"'
CXXFLAGS += $(CFLAGS) -std=c++20 -Weffc++
LDFLAGS  ?= -g -L$(BUILD_PATH)

LIBS      = $(shell pkg-config --libs --static libusb-1.0 libmongoc-1.0)
QT_LIBS   = $(shell pkg-config --libs Qt5Widgets)
QT_CFLAGS = $(shell pkg-config --cflags Qt5Widgets)

INCPATHS         += -Isrc/dmrconfig -Isrc/ -Isrc/UI -Isrc/common -Isrc/common/BSONDoc
LIBMONGOCINCPATH += $(shell pkg-config --cflags libmongoc-1.0)
LIBUSBPATH       += $(shell pkg-config --cflags libusb-1.0)
LINCPATHS        += $(LIBMONGOCINCPATH) $(LIBUSBPATH)

DMRCONFIG_SRCS=$(shell find src/dmrconfig/ -iregex '.*\.c' -not -iregex '.*\(main\|windows\|macos\).*')
DMRCONFIG_OBJS=$(addprefix $(BUILD_PATH)/,$(DMRCONFIG_SRCS:.c=.o))

MAIN_CLI_SRC=$(shell find src/dmrconfig/ -iregex '.*main\.c')
MAIN_CLI_OBJ=$(addprefix $(BUILD_PATH)/,$(MAIN_CLI_SRC:.c=.o))

GUI_SRCS=src/main.cpp
GUI_SRCS+=$(shell find src/UI -iname '*.cpp')
GUI_HDRS+=$(shell find src/UI -iname '*.h*')
GUI_PRO+=$(shell find src/UI -iname '*.pro')

COMMON_SRCS=$(shell find src/common -iname '*.cpp')
COMMON_OBJS=$(addprefix $(BUILD_PATH)/,$(COMMON_SRCS:.cpp=.o))

TESTS_SRCS=$(shell find src/tests -iname '*.cpp')
TESTS_OBJS=$(addprefix $(BUILD_PATH)/,$(TESTS_SRCS:.cpp=.o))

TEST_SCRIPTS=$(shell find src/tests -iname '*Tests')

.PHONY: all clean

TARGET_GUI=QDmrconfig
TARGET_CLI=dmrconfig

TARGET_LIB=$(BUILD_PATH)/lib$(TARGET_CLI).a
COMMON_LIB=$(BUILD_PATH)/libcommon.a

all: $(TARGET_GUI) $(TARGET_CLI)
	@echo -e "\e[032m$^\e[0m"

$(TARGET_LIB): $(DMRCONFIG_OBJS)
	ar rcs $@ $^
	ranlib $@

$(COMMON_LIB): $(COMMON_OBJS)
	ar rcs $@ $^
	ranlib $@

$(TARGET_CLI): $(MAIN_CLI_OBJ) $(TARGET_LIB) 
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $(INCPATHS) $^ $(LIBS)

$(TARGET_GUI): $(TARGET_LIB) $(GUI_SRCS) $(GUI_HDRS) $(COMMON_LIB)
	@mkdir -p $(BUILD_PATH)/src/UI
	qmake \
		"DEFINES        += VERSION=\'\\\"$(VERSION).$(HASH)\\\"\'" \
		"SOURCES        += $(GUI_SRCS:%=../../../%)" \
		"HEADERS        += $(GUI_HDRS:%=../../../%)" \
		"HEADERS        += $(COMMON_SRCS:%.cpp=../../../%.hpp)" \
		"INCLUDEPATH    += $(INCPATHS:-I%=../../../%) $(LINCPATHS:-I%=%)" \
		"PRE_TARGETDEPS += ../../../$(COMMON_LIB) ../../../$(TARGET_LIB)" \
		"LIBS           += ../../../$(COMMON_LIB) ../../../$(TARGET_LIB) $(LIBS)" \
		"TARGET         = ../../../$(TARGET_GUI)" \
		$(GUI_PRO) -o $(BUILD_PATH)/src/UI/Makefile
	$(MAKE) -C $(BUILD_PATH)/src/UI

# Build dmrconfig
$(BUILD_PATH)/src/dmrconfig/%.o: src/dmrconfig/%.c
	@mkdir -p `dirname $@`
	$(CC) -o $@ -c $(CFLAGS) $(LIBUSBPATH) $<

# Build libcommon
$(BUILD_PATH)/src/common/%.o: src/common/%.cpp
	@mkdir -p `dirname $@`
	$(CC) -o $@ -c $(CXXFLAGS) $(LINCPATHS) $(LIBMONGOCINCPATH) $<

$(BUILD_PATH)/%.o: %.cpp
	@mkdir -p `dirname $@`
	$(CC) -o $@ -c $(CXXFLAGS) $(INCPATHS) $(LINCPATHS) $<

clean:
	-rm -rf $(BUILD_PATH) 

distclean: clean
	-rm -rf $(TARGET_GUI) $(TARGET_CLI)

repoclean: distclean
	git clean -ffd

$(BUILD_PATH)/tests: $(TESTS_OBJS) $(COMMON_LIB)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIBS)

$(BUILD_PATH)/run-tests: $(BUILD_PATH)/tests
	$(BUILD_PATH)/tests | tee $@

$(BUILD_PATH)/run-dmrconfig-tests: $(TARGET_CLI) $(TEST_SCRIPTS)
	./src/tests/btechTests examples/btech6x2.img.bak | tee $@

check: $(BUILD_PATH)/run-tests $(BUILD_PATH)/run-dmrconfig-tests $(TARGET_GUI)
	@echo -e "\e[32mAll Checks Passed\e[0m"

-include $(DMRCONFIG_OBJS:%.o=%.d) 
-include $(MAIN_CLI_OBJ:%.o=%.d) 
-include $(MAIN_GUI_OBJ:%.o=%.d) 
-include $(COMMON_OBJS:%.o=%.d) 
