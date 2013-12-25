# -------------------------------------------------
# QT Project File
# -------------------------------------------------
TARGET = analysis

TEMPLATE = app

SOURCES += qchartviewer.cpp \
    analysis.cpp

HEADERS += qchartviewer.h \
    analysis.h
    
RESOURCES += analysis.qrc

INCLUDEPATH += ../ChartDirector/include

DEFINES += CHARTDIR_HIDE_OBSOLETE _CRT_SECURE_NO_WARNINGS

win32:LIBS += ../ChartDirector/lib/chartdir51.lib
win32:QMAKE_POST_LINK = copy /Y ..\\ChartDirector\\lib\\chartdir51.dll $(DESTDIR)

macx:LIBS += -L../ChartDirector/lib -lchartdir
macx:QMAKE_POST_LINK += mkdir -p \"`dirname $(TARGET)`/../Frameworks\";
macx:QMAKE_POST_LINK += cp ../ChartDirector/lib/libchartdir.5.dylib \"`dirname $(TARGET)`/../Frameworks\";
macx:QMAKE_POST_LINK += install_name_tool -change libchartdir.5.dylib \
     \@loader_path/../Frameworks/libchartdir.5.dylib \"$(TARGET)\";

unix:!macx:LIBS += -L../ChartDirector/lib -lchartdir
unix:!macx:QMAKE_RPATHDIR += ../ChartDirector/lib
