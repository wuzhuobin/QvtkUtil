#ifndef __VTK_INTERACTOR_STYLE_WINDOW_LEVEL_H__
#define __VTK_INTERACTOR_STYLE_WINDOW_LEVEL_H__

// me 
#include "abstractinteractorobserver_export.h"

// vtk
#include <vtkInteractorStyleTrackballCamera.h>

class ABSTRACTINTERACTOROBSERVER_EXPORT vtkInteractorStyleWindowLevel : public vtkInteractorStyleTrackballCamera
{
public:
	static const int VTKIS_WINDOW_LEVEL = 1024;

	vtkTypeMacro(vtkInteractorStyleWindowLevel, vtkInteractorStyleTrackballCamera);
	void PrintSelf(ostream& os, vtkIndent indent);

	virtual void OnMouseMove() override;
	virtual void OnLeftButtonDown() override;
	virtual void OnLeftButtonUp() override;
	virtual void OnChar() override;

	virtual void SetWindow(double window) = 0;
	virtual double GetWindow() = 0;
	virtual void SetLevel(double level) = 0;
	virtual double GetLevel() = 0;


	virtual double GetResetWindow() = 0;
	virtual double GetResetLevel() = 0;

protected:
	vtkInteractorStyleWindowLevel();
	virtual ~vtkInteractorStyleWindowLevel() override;

	/**
	* @override
	* almost the same as #vtkInteractorStyleImage, except for specifying vtkImageActor
	* by using #SetCurrentImageToNthImage() .
	*/
	virtual void StartWindowLevel();
	virtual void WindowLevel();
	virtual void EndWindowLevel();
	virtual void ResetWindowLevel();

	double InitialWindow;
	double InitialLevel;

	int WindowLevelStartPosition[2];
	int WindowLevelCurrentPosition[2];

private:
	vtkInteractorStyleWindowLevel(const vtkInteractorStyleTrackballCamera&) VTK_DELETE_FUNCTION;
	void operator=(const vtkInteractorStyleWindowLevel&) VTK_DELETE_FUNCTION;

};


#endif // !__VTK_INTERACTOR_STYLE_WINDOW_LEVEL_H__
