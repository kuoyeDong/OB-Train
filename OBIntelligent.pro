QT += quick serialport qml multimedia multimediawidgets sensors charts widgets core gui network webview webengine websockets webchannel
CONFIG += c++11 big_resources

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#include(deps/QmlVlc/QmlVlc.pri)
#include(deps/QmlFFMPEG/deployment.pri)

#INCLUDEPATH += deps

#INCLUDEPATH +=E:/vlc-3.0.6/sdk/include
#LIBS += E:/vlc-3.0.6/sdk/lib/libvlc.lib
#LIBS += E:/vlc-3.0.6/sdk/lib/libvlccore.lib

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
HEADERS += \
    serialtest.h \
    portmodel.h \
    websockettransport.h \
    jsapiobject.h \
    datafresh.h \
    jsapiobject.h \
    portmodel.h \
    serialtest.h \
    systemopen.h \
    websockettransport.h
    src/camera.hpp \
    src/imageprovider.h \
    src/imageshow.h \
    src/SaveFaceName.h \
    src/ZhSupport.h \
    src/tracking/external_lib.hpp \
    src/tracking/kcftracker.hpp \
    src/Detection/detection.h \
    src/Detection/Posture.h \
    src/Detection/TensorflowConfig.h \
    experiment_src/PersonExperiment/PersonExperiment.hpp \
    experiment_src/FaceExperiment/FaceExperiment.h \
    experiment_src/FaceExperiment/fastmtcnn.h

SOURCES += \
    serialtest.cpp \
    portmodel.cpp \
    jsapiobject.cpp \
    datafresh.cpp \
    jsapiobject.cpp \
    main.cpp \
    systemopen.cpp \
    src/camera.cpp \
    src/imageprovider.cpp \
    src/imageshow.cpp \
    src/SaveFaceName.cpp \
    src/ZhSupport.cpp \
    src/tracking/kcftracker.cpp \
    src/Detection/detection.cpp \
    src/Detection/Posture.cpp \
    experiment_src/PersonExperiment/ExperimentOne.cpp \
    experiment_src/PersonExperiment/PersonExperiment.cpp \
    experiment_src/FaceExperiment/FaceExperiment1.cpp \
    experiment_src/FaceExperiment/FaceExperiment.cpp \
    experiment_src/FaceExperiment/fastmtcnn.cpp \
    experiment_src/FaceExperiment/FaceExperiment2.cpp

RESOURCES += \
    sensor.qrc \
    qml.qrc \
    module.qrc \
    drone.qrc \
    visionvoice.qrc \
    machinelearning.qrc \
    robot.qrc


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


FORMS +=
