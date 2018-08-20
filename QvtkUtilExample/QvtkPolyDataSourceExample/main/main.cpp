// qt
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
// me
#include "QvtkScene.h"
#include "QvtkPolyDataSource.h"
#include "QvtkPolyDataActor.h"
#include "QvtkSceneExample.h"
//namespace Q {
//	namespace vtk {
//		struct SceneConfiguration {
//			SceneConfiguration() {
//				Scene *scene = new Scene;
//				scene->registerData(new PolyDataSource);
//				scene->registerData(new PolyDataActor);
//			}
//		};
//	}
//}
int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(application);
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationName("Application Example");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);
    //if (!parser.positionalArguments().isEmpty())
    //    example.loadFile(parser.positionalArguments().first());
	QvtkSceneExample example;
    example.show();
    return app.exec();
}