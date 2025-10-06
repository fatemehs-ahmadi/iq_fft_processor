QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = task0
TEMPLATE = app

SOURCES += \
    chartmanager.cpp \
    main.cpp \
    mainwindow.cpp \
    kiss_fft.c \
    process.cpp

HEADERS += \
    chartmanager.h \
    fftw3.h \
    mainwindow.h \
    kiss_fft.h \
    process.h

FORMS += \
    mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


# Optional: Enable Welch processing
DEFINES += USE_PWELCH

# Include paths (if kissfft is in a subfolder)
INCLUDEPATH += $$PWD

# MSVC-specific settings (optional)
QMAKE_CXXFLAGS += /utf-8
