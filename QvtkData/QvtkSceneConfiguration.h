#ifndef __QvtkO_SCENE_CONFIGURATION_H__
#define __QvtkO_SCENE_CONFIGURATION_H__

// me
#include "QvtkScene.h"
#include "QvtkPolyData.h"
#include "Image.h"
#include "QvtkVolume.h"
#include "QvtkPolyDataActor.h"
#include "ImageSlice.h"
#include "QvtkPolyDataActor2D.h"
#include "QvtkAnnotationPolyData.h"
#include "QvtkPolyDataSource.h"
#include "itkQDebugWindow.h"
#include "vtkQDebugWindow.h"

//qt
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDateTime>



/**
 * @struct	QvtkSceneConfiguration
 *
 * @brief	A Qvtko scene configuration.
 * Please do not include this to any file, it use a internal linkage to do the configuration.
 * @author	wuzhuobin
 * @date	2017/6/21
 */

static const QString LOGGING_DATE_TIME = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
struct QvtkSceneConfiguration
{
	QvtkSceneConfiguration()
	{
		if (!QDir("LOG").exists()) {
			QDir(".").mkdir("LOG");
		}
		qInstallMessageHandler([](QtMsgType type, const QMessageLogContext &context, const QString &msg) {
			QByteArray localMsg = msg.toLocal8Bit();
			QString strMsg = QDateTime::currentDateTime().toString("yyyy/MM/dd    hh:mm:ss:zzz    ");
			switch (type) {
			case QtDebugMsg:
				strMsg += "Debug: \n";
				break;
			case QtInfoMsg:
				strMsg += "Info: \n";
				break;
			case QtWarningMsg:
				strMsg += "Warning: \n";
				break;
			case QtCriticalMsg:
				strMsg += "Critical: \n";
				break;
			case QtFatalMsg:
				strMsg += "Fatal: \n";
				break;
			}

			QString strMessage = strMsg + QString("\n%1 (%2:%3, %4)\n")
				.arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
			cerr << strMessage.toStdString() << "\n";
			QFile file("LOG/LOG_" + LOGGING_DATE_TIME + ".txt");
			file.open(QIODevice::ReadWrite | QIODevice::Append);
			QTextStream stream(&file);
			stream << strMessage << "\r\n";
			file.flush();
			file.close();
		});

		vtkOutputWindow::SetInstance(vtkQDebugWindow::New());
		itk::OutputWindow::SetInstance(itk::QDebugWindow::New());


		QvtkScene* scene = new QvtkScene();

		scene->registerData(new QvtkAnnotationPolyData);
		
		scene->registerData(new Image);

		scene->registerData(new ImageSlice);

		scene->registerData(new PolyData);

		scene->registerData(new QvtkPolyDataActor);

		scene->registerData(new QvtkPolyDataActor2D);

		scene->registerData(new QvtkPolyDataSource);

		scene->registerData(new QvtkVolume);

	}
};

#endif // !__QvtkO_SCENE_CONFIGURATION_H__
