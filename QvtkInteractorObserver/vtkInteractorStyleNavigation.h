#ifndef __VTK_INTERACTOR_STYLE_NAVIGATION_H__
#define __VTK_INTERACTOR_STYLE_NAVIGATION_H__

// me
#include "abstractinteractorobserver_export.h"

// vtk
#include "vtkInteractorStyleTrackballCamera.h"
//class vtkTimerEventCallbackCommand;

class ABSTRACTINTERACTOROBSERVER_EXPORT vtkInteractorStyleNavigation : public vtkInteractorStyleTrackballCamera
{
public:
	static const int VTKIS_NAVIGATION = 1024;

	vtkTypeMacro(vtkInteractorStyleNavigation, vtkInteractorStyleTrackballCamera);
	void PrintSelf(ostream& os, vtkIndent indent);

	virtual void OnMouseMove() override;
	virtual void OnLeftButtonDown() override;
	virtual void OnLeftButtonUp() override;

	virtual void StartNavigation();
	virtual void EndNavigation();
	virtual void Navigation();

	virtual void Dolly() override;

	virtual void SetCursorPosition(const double index[3]);
	virtual void SetCursorPosition(double x, double y, double z) = 0;
	virtual void CentralizeCursorPosition() = 0;
	virtual void SetCameraScale(double scale);
protected:

	vtkInteractorStyleNavigation();
	virtual ~vtkInteractorStyleNavigation() VTK_OVERRIDE;


	virtual bool CalculateIndex(double index[3]);
	unsigned int NumberOfLeftClicks;
	int PreviousLeftPosition[2];
	static const unsigned int RESET_PIXEL_DISTANCE = 5;



};


#endif // !__VTK_INTERACTOR_STYLE_NAVIGATION_H__
