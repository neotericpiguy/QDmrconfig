BUILD_DIR=build

VERSION         = $(shell git describe --tags --abbrev=0)
VERSION         = RC
CFLAGS         ?= -g -O -Wall -Werror -fPIC -MMD
CFLAGS         += -DVERSION='"$(VERSION).$(GITCOUNT)"' \
                  $(shell pkg-config --cflags libusb-1.0)
LDFLAGS        ?= -g -L$(BUILD_DIR)
LIBS            = $(shell pkg-config --libs --static libusb-1.0)

CXXFLAGS       += $(CFLAGS) -std=c++17 -Weffc++ 

QT_LIBS         = $(shell pkg-config --libs Qt5Widgets)
QT_CFLAGS       = $(shell pkg-config --cflags Qt5Widgets)

INCPATHS       += -Isrc/dmrconfig -Isrc/ -Isrc/UI -Isrc/common

DMRCONFIG_SRCS=$(shell find src/dmrconfig/ -iregex '.*\.c' -not -iregex '.*\(main\|windows\|macos\).*')
DMRCONFIG_OBJS=$(addprefix $(BUILD_DIR)/,$(DMRCONFIG_SRCS:.c=.o))

MAIN_CLI_SRC=$(shell find src/dmrconfig/ -iregex '.*main\.c')
MAIN_CLI_OBJ=$(addprefix $(BUILD_DIR)/,$(MAIN_CLI_SRC:.c=.o))

MAIN_GUI_SRC=src/main.cpp
MAIN_GUI_OBJ=$(addprefix $(BUILD_DIR)/,$(MAIN_GUI_SRC:.cpp=.o))

COMMON_SRCS=$(shell find src/common -iname '*.cpp')
COMMON_OBJS=$(addprefix $(BUILD_DIR)/,$(COMMON_SRCS:.cpp=.o))

UI_SRCS=$(shell find src/UI -iname '*.cpp')
UI_OBJS=$(addprefix $(BUILD_DIR)/,$(UI_SRCS:.cpp=.o))

MOC_SRCS=$(shell find src/UI -iname '*.hpp')
MOC_CPPS=$(addprefix $(BUILD_DIR)/moc/,$(MOC_SRCS:.hpp=.moc.cpp))
UI_OBJS+=$(MOC_CPPS:.cpp=.o)

.SECONDARY: $(MOC_CPPS)
.PHONY: all clean

TARGET_GUI=QDmrconfig
TARGET_CLI=dmrconfig

TARGET_LIB=$(BUILD_DIR)/lib$(TARGET_CLI).a

all: $(TARGET_GUI) $(TARGET_CLI)
	@echo -e "\e[032m$^\e[0m"

$(TARGET_LIB): $(DMRCONFIG_OBJS)
	ar rcs $@ $^
	ranlib $@

$(TARGET_CLI): $(MAIN_CLI_OBJ) $(TARGET_LIB) 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCPATHS) -o $@ $^ $(LIBS)

$(TARGET_GUI): $(MAIN_GUI_OBJ) $(COMMON_OBJS) $(UI_OBJS) $(TARGET_LIB) 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCPATHS) -o $@ $^ $(LIBS) $(QT_LIBS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p `dirname $@`
	$(CC) -c $(CFLAGS) $(INCPATHS) -o $@ $<

$(BUILD_DIR)/moc/%.moc.cpp: %.hpp
	@mkdir -p `dirname $@`
	moc $< -o $@

%.moc.o: %.moc.cpp
	@mkdir -p `dirname $@`
	$(CC) -c $(CXXFLAGS) $(QT_CFLAGS) $(INCPATHS) -o $@ $<

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p `dirname $@`
	$(CC) -c $(CXXFLAGS) $(QT_CFLAGS) $(INCPATHS) -o $@ $<

clean:
	-rm -rf $(BUILD_DIR) 

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
