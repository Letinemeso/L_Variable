TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
	Base_Types/Bool_Variable.cpp \
	Base_Types/Float_Variable.cpp \
	Base_Types/Int_Variable.cpp \
	Base_Types/String_Variable.cpp \
        Type_Manager.cpp \
        Variable_Base.cpp \
        Variable_Manager.cpp \
        main.cpp

HEADERS += \
    Base_Types/Bool_Variable.h \
    Base_Types/Float_Variable.h \
    Base_Types/Int_Variable.h \
    Base_Types/String_Variable.h \
    Type_Manager.h \
    Variable_Base.h \
    Variable_Manager.h
