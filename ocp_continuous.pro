QT -= gui

CONFIG += c++14 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /usr/local/include/vtk-8.2

SOURCES += \
        ContinuousOCP.cpp \
        SimModels.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    SimModels.h \
    continuousOCP.h

LIBS += -L/usr/local/lib -lvtkCommonColor-8.2 -lvtkCommonCore-8.2 \
      -lvtkCommonComputationalGeometry-8.2 -lvtkCommonDataModel-8.2 -lvtkCommonExecutionModel-8.2 \
      -lvtkCommonMath-8.2 -lvtkCommonMisc-8.2 -lvtkCommonSystem-8.2 -lvtkCommonTransforms-8.2 -lvtkViewsCore-8.2 -lvtkInfovisCore-8.2 \
      -lvtkFiltersCore-8.2 -lvtkFiltersExtraction-8.2 -lvtkFiltersGeneral-8.2 -lvtkFiltersGeometry-8.2 -lvtkFiltersHybrid-8.2 \
      -lvtkFiltersSources-8.2 -lvtkFiltersStatistics-8.2 -lvtkImagingCore-8.2 -lvtkImagingColor-8.2 \
      -lvtkImagingFourier-8.2 -lvtkImagingMath-8.2 -lvtkImagingMorphological-8.2 -lvtkImagingStencil-8.2 \
      -lvtkIOImport-8.2 -lvtkIOCore-8.2 -lvtkIOExport-8.2 -lvtkIOGeometry-8.2 -lvtkIOImage-8.2 -lvtkIOLegacy-8.2 -lvtkIOPLY-8.2 -lvtkIOXML-8.2 \
      -lvtkInteractionStyle-8.2 -lvtkInteractionWidgets-8.2 -lvtkGUISupportQt-8.2 -lvtkRenderingCore-8.2 -lvtkRenderingOpenGL2-8.2 \
      -lvtkRenderingQt-8.2 -lvtkRenderingVolume-8.2 -lvtkRenderingVolumeOpenGL2-8.2 -lvtklibxml2-8.2 -lvtkmetaio-8.2 \
      -lvtkjsoncpp-8.2 -lvtksys-8.2 -lvtkzlib-8.2 -lvtkDICOMParser-8.2 -lvtkpng-8.2 -lvtktiff-8.2 -lvtkjpeg-8.2 -lvtkexpat-8.2 \
      -lvtkverdict-8.2 -lvtkNetCDF-8.2 -lvtksqlite-8.2 -lvtkexodusII-8.2 -lvtkfreetype-8.2
