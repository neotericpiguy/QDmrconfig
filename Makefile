BUILD_DIR=build

VERSION         = $(shell git describe --tags --abbrev=0)
VERSION         = RC
CXXFLAGS       +=-std=c++17
CFLAGS         ?= -g -O -Wall -Werror -fPIC -MMD
CFLAGS         += -DVERSION='"$(VERSION).$(GITCOUNT)"' \
                  $(shell pkg-config --cflags libusb-1.0) 
LDFLAGS        ?= -g -L$(BUILD_DIR)
LIBS            = $(shell pkg-config --libs --static libusb-1.0)
QT_LIBS         = -lQt5Core -lQt5Widgets -lQt5Gui

INCPATHS        += -Isrc/dmrconfig -Isrc/ -Isrc/UI -I/usr/include/qt/ -Isrc/common

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
MOC_CPPS=$(addprefix $(BUILD_DIR)/,$(MOC_SRCS:.hpp=.moc.cpp))
UI_OBJS+=$(addprefix $(BUILD_DIR)/,$(MOC_CPPS:.cpp=.o))

.SECONDARY: $(MOC_CPPS)
.PHONY: all clean

TARGET_GUI=QDmrconfig
TARGET_CLI=dmrconfig

TARGET_LIB=$(BUILD_DIR)/lib$(TARGET_CLI).a

all: $(TARGET_GUI) $(TARGET_CLI)
	@echo $^

$(TARGET_LIB): $(DMRCONFIG_OBJS)
	ar rcs $@ $^
	ranlib $@

$(TARGET_CLI): $(TARGET_LIB) $(MAIN_CLI_OBJ)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(INCPATHS) -o $@ $^ $(LIBS) $(TARGET_LIB)

$(TARGET_GUI): $(TARGET_LIB) $(MAIN_GUI_OBJ) $(COMMON_OBJS) $(UI_OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(INCPATHS) -o $@ $^ $(LIBS) $(QT_LIBS) $(TARGET_LIB)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p `dirname $@`
	$(CC) $(CFLAGS) $(INCPATHS) -c -o $@ $<

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p `dirname $@`
	$(CC) $(CXXFLAGS) $(CFLAGS) $(INCPATHS) -c -o $@ $<

$(BUILD_DIR)/%.moc.cpp: %.hpp
	moc $< -o $@

$(BUILD_DIR)/%.moc.o: %.moc.cpp
	@mkdir -p `dirname $@`
	$(CC) $(CFLAGS) $(INCPATHS) -c -o $@ $<

clean:
	-rm -rf $(BUILD_DIR) 

distclean: clean
	-rm -rf $(TARGET_GUI) $(TARGET_CLI)

-include $(DMRCONFIG_OBJS:%.o=%.d) 
-include $(MAIN_CLI_OBJ:%.o=%.d) 
-include $(MAIN_GUI_OBJ:%.o=%.d) 
-include $(COMMON_OBJS:%.o=%.d) 
-include $(UI_OBJS:%.o=%.d) 
-include $(MOC_OBJS:%.o=%.d) 
