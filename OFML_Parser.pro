TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Bool_Variable.cpp \
        Float_Variable.cpp \
        Int_Variable.cpp \
        String_Variable.cpp \
        Type_Manager.cpp \
        Variable_Base.cpp \
        Variable_Manager.cpp \
        main.cpp

HEADERS += \
    Bool_Variable.h \
    Float_Variable.h \
    Int_Variable.h \
    String_Variable.h \
    Type_Manager.h \
    Variable_Base.h \
    Variable_Manager.h
