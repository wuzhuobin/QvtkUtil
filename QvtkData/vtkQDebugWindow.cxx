#include "vtkQDebugWindow.h"

//vtk 
#include <vtkObjectFactory.h>

// qt
#include <QDebug>
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
		this->String = string;
		this->String.remove(0, colon + 2);
		if (type == "ERROR") {
			qCritical() << this->String.toStdString().c_str();
		}
		else if (type == "Warning" || type == "Generic Warning") {
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

vtkQDebugWindow::vtkQDebugWindow()
{
}

vtkQDebugWindow::~vtkQDebugWindow()
{
}

void vtkQDebugWindow::initialize()
{
	this->String.clear();
}
