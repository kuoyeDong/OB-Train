#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqmlengine.h>
#include <qqmlcontext.h>
#include <qqml.h>
#include <QStringList>
#include <qtwebengineglobal.h>
#include <QtWebView>
#include <QWebSocketServer>
#include <QCoreApplication>


#include "serialtest.h"
#include "portmodel.h"
#include "systemopen.h"
#include "datafresh.h"
#include "websockettransport.h"
#include "jsapiobject.h"
#include "src/imageshow.h"
#include "src/saveFaceName.h"
#include "openfile.h"

#include "experiment_src/PersonExperiment/PersonExperiment.hpp"
#include "experiment_src/FaceExperiment/FaceExperiment.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//test
    QApplication  app(argc, argv);

	ShowImage CodeImage_Face;
	ShowImage CodeImage_Tracking;
	ShowImage CodeImage_Posture;

	PersonExperiment ExpImage;
	FaceExperiment ExpFaceImage;

    QtWebEngine::initialize();

    qmlRegisterType<SerialTest>("RegisterMyType", 1, 0, "MyClassType");

    qmlRegisterType<systemOpen>("RegisterSystemOpen", 1, 0, "MySystemOpen");

    qmlRegisterType<dataFresh>("RegisterDataFresh", 1, 0, "MyDataFresh");

    qmlRegisterType<WebSocketTransport>("io.decovar.WebSocketTransport", 1, 0, "WebSocketTransport");

    qmlRegisterType<jsApiObject>("RegisterJsApiObject", 1, 0, "MyJsApiObject");
	
	qmlRegisterType<Openfile>("liam.Openfile", 1, 0, "Openfile");

    qmlRegisterType<SaveFaceName>("RegisterFaceName", 1, 0, "MySaveFaceName");

    QQmlApplicationEngine engine;

    portModel combo;
    QQmlContext *ownContext = engine.rootContext();
    ownContext->setContextProperty("myModel", QVariant::fromValue(combo.comboList()));

    QQmlContext *classContext = engine.rootContext();
    classContext->setContextProperty("comboModel", &combo);

	engine.rootContext()->setContextProperty("CodeImage_Face", &CodeImage_Face);
	engine.addImageProvider(QLatin1String("CodeImg_Face"), CodeImage_Face.m_pImgProvider);

	engine.rootContext()->setContextProperty("CodeImage_Tracking", &CodeImage_Tracking);
	engine.addImageProvider(QLatin1String("CodeImg_Tracking"), CodeImage_Tracking.m_pImgProvider);

	engine.rootContext()->setContextProperty("CodeImage_Posture", &CodeImage_Posture);
	engine.addImageProvider(QLatin1String("CodeImg_Posture"), CodeImage_Posture.m_pImgProvider);

	engine.rootContext()->setContextProperty("ExpImage", &ExpImage);
	engine.addImageProvider(QLatin1String("ExpImg"), ExpImage.m_pImgProvider);

	engine.rootContext()->setContextProperty("ExpFaceImage", &ExpFaceImage);
	engine.addImageProvider(QLatin1String("ExpFaceImg"), ExpFaceImage.m_pImgProvider);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;


    return app.exec();
}
