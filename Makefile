BUILD_PATH=build

VERSION            = $(shell git describe --tags --abbrev=0)
HASH               = $(shell git rev-parse --short HEAD)
DMRCONFIG_VERSION  = $(shell git submodule status)
GITCOUNT           = $(shell git rev-list HEAD --count)

PKG_CONFIG     ?= pkg-config

CFLAGS   ?= -g -O -Wall -Werror -fPIC -MMD -fcommon
CFLAGS   += -DVERSION='"$(VERSION).$(HASH)"'
CXXFLAGS += $(CFLAGS) -Weffc++
LDFLAGS  ?= -L$(BUILD_PATH)

LIBS      = $(shell pkg-config --libs --static libusb-1.0 libmongoc-1.0)
QT_LIBS   = $(shell pkg-config --libs Qt5Widgets)
QT_CFLAGS = $(shell pkg-config --cflags Qt5Widgets)

DMRCONFIG_PATH=src/dmrconfig
DMRCONFIG_SRCS=$(shell find $(DMRCONFIG_PATH) -iregex '.*\.c' -not -iregex '.*\(main\|windows\|macos\).*')
DMRCONFIG_OBJS=$(addprefix $(BUILD_PATH)/,$(DMRCONFIG_SRCS:.c=.o))
DMRCONFIG_MAIN_SRC=src/dmrconfig/main.c
DMRCONFIG_MAIN_OBJ=$(addprefix $(BUILD_PATH)/,$(DMRCONFIG_MAIN_SRC:.c=.o))

GUI_PATH=src/UI
GUI_SRCS=src/main.cpp
GUI_SRCS+=$(wildcard $(GUI_PATH)/*.cpp)
GUI_HDRS+=$(wildcard $(GUI_PATH)/*.hpp)
GUI_PRO+=$(wildcard $(GUI_PATH)/*.pro)

COMMON_PATH=src/common
COMMON_INCPATHS=$(addprefix -I,$(shell find $(COMMON_PATH) -type d))
COMMON_SRCS=$(wildcard $(COMMON_PATH)/*/*.cpp $(COMMON_PATH)/*.cpp)
COMMON_HDRS=$(wildcard $(COMMON_OBJS)/*/*.hpp $(COMMON_OBJS)/*.hpp)
COMMON_OBJS=$(addprefix $(BUILD_PATH)/,$(COMMON_SRCS:.cpp=.o))

TESTS_PATH=src/tests
TESTS_INCPATHS=$(addprefix -I,$(shell find $(TESTS_PATH) -type d))
TESTS_SRCS=$(wildcard $(TESTS_PATH)/*/*.cpp $(TESTS_PATH)/*.cpp)
TESTS_HDRS=$(wildcard $(TESTS_PATH)/*/*.h* $(TESTS_PATH)/*.h*)
TESTS_OBJS=$(addprefix $(BUILD_PATH)/,$(TESTS_SRCS:.cpp=.o))

TEST_SCRIPTS=$(wildcard $(TESTS_PATH)/*Tests)

LIBMONGOCINCPATH += $(shell pkg-config --cflags libmongoc-1.0)
LIBUSBPATH       += $(shell pkg-config --cflags libusb-1.0)
LINCPATHS        += $(LIBMONGOCINCPATH) $(LIBUSBPATH)

.PHONY: all clean

#
# Actual Build targets
# 
TARGET_GUI=QDmrconfig
TARGET_CLI=dmrconfig

TARGET_LIB=$(BUILD_PATH)/lib$(TARGET_CLI).a
COMMON_LIB=$(BUILD_PATH)/libcommon.a

#
# Make sure pkg-config is installed.
#
ifeq ($(shell $(PKG_CONFIG) --version),)
    $(error Fatal error: pkg-config is not installed)
endif

all: $(TARGET_GUI) $(TARGET_CLI)
	@echo -e "\e[032m$^\e[0m"

$(TARGET_LIB): $(DMRCONFIG_OBJS)
	ar rcs $@ $^
	ranlib $@

$(COMMON_LIB): $(COMMON_OBJS)
	ar rcs $@ $^
	ranlib $@

$(TARGET_CLI): $(DMRCONFIG_MAIN_OBJ) $(TARGET_LIB) 
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $^ $(LIBS)

$(TARGET_GUI): $(TARGET_LIB) $(GUI_SRCS) $(GUI_HDRS) $(COMMON_LIB)
	@mkdir -p $(BUILD_PATH)/src/UI
	qmake \
		"DEFINES        += VERSION=\'\\\"$(VERSION).$(HASH)\\\"\'" \
		"SOURCES        += $(GUI_SRCS:%=../../../%)" \
		"HEADERS        += $(GUI_HDRS:%=../../../%)" \
		"HEADERS        += $(COMMON_SRCS:%.cpp=../../../%.hpp)" \
		"INCLUDEPATH    += $(COMMON_INCPATHS:-I%=../../../%) $(LINCPATHS:-I%=%)" \
		"PRE_TARGETDEPS += ../../../$(COMMON_LIB) ../../../$(TARGET_LIB)" \
		"LIBS           += ../../../$(COMMON_LIB) ../../../$(TARGET_LIB) $(LIBS)" \
		"TARGET         = ../../../$(TARGET_GUI)" \
		$(GUI_PRO) -o $(BUILD_PATH)/src/UI/Makefile
	$(MAKE) -C $(BUILD_PATH)/src/UI

# Build dmrconfig
$(BUILD_PATH)/$(DMRCONFIG_PATH)/%.o: $(DMRCONFIG_PATH)/%.c
	@mkdir -p `dirname $@`
	$(CC) -o $@ -c $(CFLAGS) $(LIBUSBPATH) $<

# Build libcommon
$(BUILD_PATH)/$(COMMON_PATH)/%.o: $(COMMON_PATH)/%.cpp
	@mkdir -p `dirname $@`
	$(CC) -o $@ -c $(CXXFLAGS) $(LINCPATHS) $(LIBMONGOCINCPATH) $<

$(BUILD_PATH)/$(TESTS_PATH)/%.o: $(TESTS_PATH)/%.cpp
	@mkdir -p `dirname $@`
	$(CC) -o $@ -c $(CXXFLAGS) $(TESTS_INCPATHS) $(COMMON_INCPATHS) $(LINCPATHS) $<

clean:
	-rm -rf $(BUILD_PATH) 

distclean: clean
	-rm -rf $(TARGET_GUI) $(TARGET_CLI)

repoclean:
	git clean -ffd

$(BUILD_PATH)/tests: $(TESTS_OBJS) $(COMMON_LIB)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIBS)

$(BUILD_PATH)/run-tests: $(BUILD_PATH)/tests
	$(BUILD_PATH)/tests
	@touch $@

$(BUILD_PATH)/run-dmrconfig-tests: $(TARGET_CLI) $(TEST_SCRIPTS)
	./src/tests/btechTests examples/btech6x2.img.bak
	@touch $@

#Not required but good to have
$(BUILD_PATH)/style-check: $(GUI_SRCS) $(GUI_HDRS) $(TESTS_SRCS) $(TESTS_HDRS) $(COMMON_SRCS) $(COMMON_HDRS)
	clang-format -i $^ || true
	@touch $@

check: $(BUILD_PATH)/run-tests $(BUILD_PATH)/run-dmrconfig-tests $(TARGET_GUI)
	@echo -e "\e[32mAll Checks Passed\e[0m"

-include $(DMRCONFIG_OBJS:%.o=%.d) 
-include $(DMRCONFIG_MAIN_OBJ:%.o=%.d) 
-include $(MAIN_GUI_OBJ:%.o=%.d) 
-include $(COMMON_OBJS:%.o=%.d) 
