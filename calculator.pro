TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp\
        expression.cpp\
        value.cpp\
        verylong.cpp\
        identity.cpp

HEADERS += \
        expression.h\
        value.h\
        verylong.h\
        identity.h

#unix {
#    YACCSOURCES = parser.yy
#    LEXSOURCES = scanner.ll
#}
