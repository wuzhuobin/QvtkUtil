// me 
#include "itkQDebugWindow.h"

// qt
#include <QDebug>
void itk::QDebugWindow::DisplayText(const char * text)
{
	if (!text) {
		return;
	}
	QString string = text;
	int colon = string.indexOf(": ");
	if (colon != -1) {
		QString type = string.left(colon);
		this->String = string;
		this->String.remove(0, colon + 2);
		if (type == "itk::ERROR") {
			qCritical() << this->String.toStdString().c_str();
		}
		else if (type == "WARNING") {
			qWarning() << this->String.toStdString().c_str();
		}
		else if (type == "Debug") {
			qDebug() << this->String.toStdString().c_str();
		}
		else {
			goto fatal;
		}
		return;
	}
fatal:
	this->String = string;
	qFatal("%s", this->String.toStdString().c_str());
}

itk::QDebugWindow::QDebugWindow()
{
}

itk::QDebugWindow::~QDebugWindow()
{
}
