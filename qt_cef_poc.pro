
QT += core gui widgets
TARGET = qt_cef_poc
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++14

# We want a console window for logs during debug
debug:CONFIG += console

SOURCES += \
    cef.cc \
    main.cc \
    cef_widget.cc \
    main_window.cc \
    cef_handler.cc

HEADERS  += \
    cef.h \
    cef_widget.h \
    main_window.h \
    cef_handler.h

release:PROFILE = Release
debug:PROFILE = Debug

INCLUDEPATH += $$(CEF_DIR)

win32 {
    SOURCES += \
        cef_win.cc \
        cef_widget_win.cc

    LIBS += -luser32
    LIBS += -L$$(CEF_DIR)/$$PROFILE -llibcef
    LIBS += -L$$(CEF_DIR)/libcef_dll_wrapper/$$PROFILE -llibcef_dll_wrapper

    # Chromium reads the manifest, needs a specific one
    # See http://magpcss.org/ceforum/viewtopic.php?f=6&t=14721
    CONFIG -= embed_manifest_exe
    RC_FILE = qt_cef_poc.rc
}

unix {
    SOURCES += \
        cef_linux.cc \
        cef_widget_linux.cc
    LIBS += -L$$(CEF_DIR)/$$PROFILE -lcef
    LIBS += -L$$(CEF_DIR)/libcef_dll_wrapper -lcef_dll_wrapper_$$PROFILE
}
