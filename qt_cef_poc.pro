
QT += core gui widgets
TARGET = qt_cef_poc
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++14

# We want a console window for logs during debug
Debug:CONFIG += console

SOURCES += \
    cef.cc \
    main.cc \
    cef_widget.cc \
    main_window.cc

HEADERS  += \
    cef.h \
    cef_widget.h \
    main_window.h

Release:PROFILE = Release
Debug:PROFILE = Debug

INCLUDEPATH += $$(CEF_DIR)

win32 {
    LIBS += -L$$(CEF_DIR)/$$PROFILE -llibcef
    LIBS += -L$$(CEF_DIR)/libcef_dll_wrapper/$$PROFILE -llibcef_dll_wrapper
} else {
    LIBS += -L$$(CEF_DIR)/$$PROFILE -lcef
    LIBS += -L$$(CEF_DIR)/libcef_dll_wrapper/$$PROFILE -lcef_dll_wrapper
}
