#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "SCBSceneExample.h"
#include "SCBSceneConfiguration.h"

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

	SCBSceneConfiguration();
	SCBSceneExample example;
    //if (!parser.positionalArguments().isEmpty())
    //    example.loadFile(parser.positionalArguments().first());
    example.show();
    return app.exec();
}