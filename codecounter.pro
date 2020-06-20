QT += widgets
TARGET = CodeCounter
TEMPLATE = app
CONFIG += c++17 strict_c++ utf8_source warn_on
DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII
HEADERS += frmcountcode.h
SOURCES += main.cpp frmcountcode.cpp
FORMS += frmcountcode.ui
