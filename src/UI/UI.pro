QT += core widgets gui network
CONFIG += core widgets gui c++2a c++1z c++14

QMAKE_CXXFLAGS += -Werror 

# TODO Added because qt5 has a deprecated enum conversion and the
# warning is causing the compilation to fail. Remove whem qt5 is
# updated in arch
QMAKE_CXXFLAGS += -Wno-deprecated-enum-enum-conversion
