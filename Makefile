BUILD_PATH=build

VERSION         = $(shell git describe --tags --abbrev=0)
VERSION         = RC
CFLAGS         ?= -g -O -Wall -Werror -fPIC -MMD
CFLAGS         += -DVERSION='"$(VERSION).$(GITCOUNT)"' \
                  $(shell pkg-config --cflags libusb-1.0)
LDFLAGS        ?= -g -L$(BUILD_PATH)
LIBS            = $(shell pkg-config --libs --static libusb-1.0)

CXXFLAGS       += $(CFLAGS) -std=c++17 -Weffc++ 

QT_LIBS         = $(shell pkg-config --libs Qt5Widgets)
QT_CFLAGS       = $(shell pkg-config --cflags Qt5Widgets)

INCPATHS       += -Isrc/dmrconfig -Isrc/ -Isrc/UI -Isrc/common

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

UI_SRCS=$(shell find src/UI -iname '*.cpp')
UI_OBJS=$(addprefix $(BUILD_PATH)/,$(UI_SRCS:.cpp=.o))

MOC_SRCS=$(shell find src/UI -iname '*.hpp')
MOC_CPPS=$(addprefix $(BUILD_PATH)/moc/,$(MOC_SRCS:.hpp=.moc.cpp))
UI_OBJS+=$(MOC_CPPS:.cpp=.o)

.SECONDARY: $(MOC_CPPS)
.PHONY: all clean

TARGET_GUI=QDmrconfig
TARGET_CLI=dmrconfig

TARGET_LIB=$(BUILD_PATH)/lib$(TARGET_CLI).a

all: $(TARGET_GUI) $(TARGET_CLI)
	@echo -e "\e[032m$^\e[0m"

$(TARGET_LIB): $(DMRCONFIG_OBJS)
	ar rcs $@ $^
	ranlib $@

$(TARGET_CLI): $(MAIN_CLI_OBJ) $(TARGET_LIB) 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCPATHS) -o $@ $^ $(LIBS)

$(TARGET_GUI): $(TARGET_LIB) $(GUI_SRCS) $(GUI_HDRS)
	@mkdir -p $(BUILD_PATH)/src/UI
	qmake \
		"DEFINES      += VERSION=\'\\\"$(VERSION)\\\"\'" \
		"SOURCES      += $(GUI_SRCS:%=../../../%)" \
		"SOURCES      += $(COMMON_SRCS:%=../../../%)" \
		"HEADERS      += $(GUI_HDRS:%=../../../%)" \
		"INCLUDEPATH  += $(INCPATHS:-I%=../../../%)" \
		"LIBS         += $(TARGET_LIB:%=../../../%) $(LIBS)" \
		"TARGET        = ../../../$(TARGET_GUI)" \
		$(GUI_PRO) -o $(BUILD_PATH)/src/UI/Makefile
	$(MAKE) -C $(BUILD_PATH)/src/UI

$(BUILD_PATH)/%.o: %.c
	@mkdir -p `dirname $@`
	$(CC) -c $(CFLAGS) $(INCPATHS) -o $@ $<

$(BUILD_PATH)/moc/%.moc.cpp: %.hpp
	@mkdir -p `dirname $@`
	moc $< -o $@

%.moc.o: %.moc.cpp
	@mkdir -p `dirname $@`
	$(CC) -c $(CXXFLAGS) $(QT_CFLAGS) $(INCPATHS) -o $@ $<

$(BUILD_PATH)/%.o: %.cpp
	@mkdir -p `dirname $@`
	$(CC) -c $(CXXFLAGS) $(QT_CFLAGS) $(INCPATHS) -o $@ $<

clean:
	-rm -rf $(BUILD_PATH) 

distclean: clean
	-rm -rf $(TARGET_GUI) $(TARGET_CLI)

repoclean: distclean
	git clean -ffd

-include $(DMRCONFIG_OBJS:%.o=%.d) 
-include $(MAIN_CLI_OBJ:%.o=%.d) 
-include $(MAIN_GUI_OBJ:%.o=%.d) 
-include $(COMMON_OBJS:%.o=%.d) 
-include $(UI_OBJS:%.o=%.d) 
-include $(MOC_OBJS:%.o=%.d) 
