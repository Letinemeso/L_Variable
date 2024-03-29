TEMPLATE = lib
CONFIG += staticlib
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += "include"
INCLUDEPATH += "../L_Utility/include"
INCLUDEPATH += "../L_Utility/include/Data_Structures"

SOURCES += \
	source/MDL_Reader.cpp \
	source/Object_Constructor.cpp \
	source/Type_Manager.cpp \
	source/Variable_Base.cpp \
	source/Builder_Stub.cpp \

HEADERS += \
    include/MDL_Reader.h \
	include/Object_Constructor.h \
    include/Type_Manager.h \
    include/Variable_Base.h \
    include/Builder_Stub.h \
