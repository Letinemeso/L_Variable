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
	source/Variable_Manager.cpp
#	source/Base_Types/Array_Variable.cpp \
#	source/Base_Types/Bool_Variable.cpp \
#	source/Base_Types/Float_Variable.cpp \
#	source/Base_Types/Int_Variable.cpp \
#	source/Base_Types/String_Variable.cpp \
#	source/Base_Types/Variable_Base.cpp \

HEADERS += \
    include/MDL_Reader.h \
    include/Type_Manager.h \
    include/Variable_Manager.h
#    include/Base_Types/Array_Variable.h \
#    include/Base_Types/Bool_Variable.h \
#    include/Base_Types/Float_Variable.h \
#    include/Base_Types/Int_Variable.h \
#    include/Base_Types/String_Variable.h \
#    include/Base_Types/Variable_Base.h \
