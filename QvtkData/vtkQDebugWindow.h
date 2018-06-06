#ifndef __VTK_QDEBUG_WINDOW_H__
#define __VTK_QDEBUG_WINDOW_H__
#pragma once;
// me
#include "qvtkdata_export.h"
// vtk
#include <vtkOutputWindow.h>
// qt
#include <QString>
class QMutex;
class QVTKDATA_EXPORT vtkQDebugWindow : public vtkOutputWindow
{
public:
	vtkTypeMacro(vtkQDebugWindow, vtkOutputWindow);
	static vtkQDebugWindow *New();
	virtual void PrintSelf(ostream &os, vtkIndent indent);

	virtual void DisplayText(const char* text) VTK_OVERRIDE;

	QString GetOutput() { return String; }

protected:
	vtkQDebugWindow();
	virtual ~vtkQDebugWindow() VTK_OVERRIDE;
	void initialize();
	QString String;
	QMutex *mutex;
};

#endif // !__VTK_QDEBUG_WINDOW_H__
