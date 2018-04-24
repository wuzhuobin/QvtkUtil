#ifndef __QVTK_NON_PLANAR_VIEWER_H__
#define __QVTK_NON_PLANAR_VIEWER_H__

#include "QvtkOrthogonalViewer.h"
namespace Q {
namespace vtk{
class QVTKVIEWER_EXPORT NonPlanarViewer : public OrthogonalViewer
{
	Q_OBJECT;
public:
	explicit NonPlanarViewer(QWidget* parent = nullptr);

public slots:
	//virtual void SetOrientation(int orientation) override;
	//virtual void SetCursorPosition(double x, double y, double z) override;

protected:
	virtual void UpdateCameraViewPlaneNormal() override;
};
}
}

#endif // !__QVTK_NON_PLANAR_VIEWER_H__
