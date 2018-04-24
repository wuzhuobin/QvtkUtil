#ifndef __QVTK_PLANAR_VIEWER_H__
#define __QVTK_PLANAR_VIEWER_H__

// me
#include "QvtkOrthogonalViewer.h"

// vtk
class vtkTextActor;
class vtkAxisActor2D;

namespace Q {
namespace vtk {

class QVTKVIEWER_EXPORT PlanarViewer : public OrthogonalViewer
{
	Q_OBJECT;
public:
	explicit PlanarViewer(QWidget* parent = nullptr);
	virtual ~PlanarViewer() override;
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

}
}
#endif // !__QVTK_PLANAR_VIEWER_H__