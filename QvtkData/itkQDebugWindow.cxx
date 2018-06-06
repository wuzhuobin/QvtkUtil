// me 
#include "itkQDebugWindow.h"
// qt
#include <QDebug>
#include <QMutex>
void itk::QDebugWindow::DisplayText(const char * text)
{
	if (!text) {
		return;
	}
	QString string = text;
	int colon = string.indexOf(": ");
	if (colon != -1) {
		QString type = string.left(colon);
		QString _string = string;
		_string.remove(0, colon + 2);
		if (type == "itk::ERROR") {
			QMutexLocker(this->mutex);
			this->String = _string;
			qCritical() << _string.toStdString().c_str();
		}
		else if (type == "WARNING") {
			QMutexLocker(this->mutex);
			this->String = _string;
			qWarning() << this->String.toStdString().c_str();
		}
		else if (type == "Debug") {
			QMutexLocker(this->mutex);
			this->String = _string;
			qDebug() << this->String.toStdString().c_str();
		}
		else {
			goto fatal;
		}
		return;
	}
fatal:
	QMutexLocker(this->mutex);
	this->String = string;
	qFatal("%s", this->String.toStdString().c_str());
}

itk::QDebugWindow::QDebugWindow()
{
	this->mutex = new QMutex;
}

itk::QDebugWindow::~QDebugWindow()
{
	delete this->mutex;
}
