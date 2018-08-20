#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "SCBSceneExample.h"
#include "SCBSceneConfiguration.h"

int main(int argc, char *argv[])
{
	//Q_INIT_RESOURCE(application);

	SCBSceneConfiguration();

	QApplication app(argc, argv);
	SCBSceneExample mainWin;
	mainWin.show();
	return app.exec();
}
