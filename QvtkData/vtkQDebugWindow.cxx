#include "vtkQDebugWindow.h"
//vtk 
#include <vtkObjectFactory.h>
// qt
#include <QDebug>
#include <QMutex>
vtkStandardNewMacro(vtkQDebugWindow);
void vtkQDebugWindow::PrintSelf(ostream & os, vtkIndent indent)
{
	Superclass::PrintSelf(os, indent);
}

void vtkQDebugWindow::DisplayText(const char * text)
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
		if (type == "ERROR") {
			QMutexLocker(this->mutex);
			this->String = _string;
			qCritical() << _string.toStdString().c_str();
		}
		else if (type == "Warning" || type == "Generic Warning") {
			QMutexLocker(this->mutex);
			this->String = _string;
			qWarning() << _string.toStdString().c_str();
		}
		else if (type == "Debug") {
			QMutexLocker(this->mutex);
			this->String = _string;
			qDebug() << _string.toStdString().c_str();
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

vtkQDebugWindow::vtkQDebugWindow()
{
	this->mutex = new QMutex;
}

vtkQDebugWindow::~vtkQDebugWindow()
{
	delete this->mutex;
}

void vtkQDebugWindow::initialize()
{
	this->String.clear();
}
