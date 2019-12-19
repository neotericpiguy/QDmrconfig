BUILD_DIR=build

VERSION         = $(shell git describe --tags --abbrev=0)
VERSION         = TEST
CXXFLAGS       +=-std=c++17
CFLAGS         ?= -g -O -Wall -Werror -fPIC -MMD
CFLAGS         += -DVERSION='"$(VERSION).$(GITCOUNT)"' \
                  $(shell pkg-config --cflags libusb-1.0) 
LDFLAGS        ?= -g
LIBS            = $(shell pkg-config --libs --static libusb-1.0)
LIBS            += -lQt5Core -lQt5Widgets -lQt5Gui

INCPATHS        += -Isrc/dmrconfig -Isrc/ -Isrc/UI -I/usr/include/qt/ -Isrc/common

DMRCONFIG_SRC=util.o radio.o dfu-libusb.o uv380.o md380.o rd5r.o \
                  gd77.o hid.o serial.o d868uv.o dm1801.o hid-libusb.o
DMRCONFIG_SRCS=$(addprefix src/dmrconfig/,$(DMRCONFIG_SRC:.c=.o))
DMRCONFIG_OBJS=$(addprefix $(BUILD_DIR)/,$(DMRCONFIG_SRCS:.c=.o))

SRCS=src/main.cpp $(shell find src/common -iname '*.cpp')
OBJS=$(addprefix $(BUILD_DIR)/,$(SRCS:.cpp=.o))

UI_SRCS=$(shell find src/UI -iname '*.cpp')
UI_OBJS=$(addprefix $(BUILD_DIR)/,$(UI_SRCS:.cpp=.o))

MOC_SRCS=$(shell find src/UI -iname '*.hpp')
MOC_CPPS=$(addprefix $(BUILD_DIR)/,$(MOC_SRCS:.hpp=.moc.cpp))
MOC_OBJS=$(addprefix $(BUILD_DIR)/,$(MOC_CPPS:.cpp=.o))

TARGET=QDmrconfig

#QT_MESSAGE_PATTERN
#QT_MESSAGE_PATTERN="[%{type}] %{appname} (%{file}:%{line}) - %{message}" ./QDmrconfig

all: $(TARGET)

$(TARGET): $(OBJS) $(UI_OBJS) $(DMRCONFIG_OBJS) $(MOC_OBJS)
	@echo $(MOC_OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(INCPATHS) -o $@ $^ $(LIBS)

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
	-rm -rf $(TARGET)

-include $(ALL_CPP_FILES:%.cpp=%.d)
-include $(OBJS:%.o=%.d) 
-include $(UI_OBJS:%.o=%.d) 
-include $(DMRCONFIG_OBJS:%.o=%.d) 
-include $(MOC_OBJS:%.o=%.d)
