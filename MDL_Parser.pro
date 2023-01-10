TEMPLATE = lib
CONFIG += staticlib
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += "include"
INCLUDEPATH += "../Utility/include"
INCLUDEPATH += "../Utility/include/Data_Structures"

SOURCES += \
	source/MDL_Reader.cpp \
	source/Type_Manager.cpp \
	source/Variable_Base.cpp

HEADERS += \
    include/MDL_Reader.h \
    include/Type_Manager.h \
    include/Variable_Base.h
