#ifndef __Qvtk_PLANAR_VIEWER_H__
#define __Qvtk_PLANAR_VIEWER_H__

// me
#include "QvtkOrthogonalViewer.h"

// vtk
class vtkTextActor;
class vtkAxisActor2D;


class ABSTRACTVIEWER_EXPORT QvtkPlanarViewer : public OrthogonalViewer
{
	Q_OBJECT;
public:
	explicit QvtkPlanarViewer(QWidget* parent = nullptr);
	virtual ~QvtkPlanarViewer() override;
	vtkRenderer* GetAnnotationRenderer() { return this->renderers[1]; }
public slots:
	virtual void IncrementSlice(bool sign);
	virtual void SetOrientation(int orientation) override;
	//virtual void SetCursorPosition(double x, double y, double z) override;
	virtual void UpdateAxes(bool flag);
	virtual void UpdateCursorPosition(double x, double y, double z) override;

protected:
	// fix to itksnap
	virtual void UpdateCameraViewPlaneNormal() override;

	vtkTextActor* orientationActor[4];
	vtkAxisActor2D* verticalAxis;
	vtkAxisActor2D* horizontalAxis;


};

#endif // !__Qvtk_PLANAR_VIEWER_H__
