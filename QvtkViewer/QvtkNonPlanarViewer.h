#ifndef __Qvtk_NON_PLANAR_VIEWER_H__
#define __Qvtk_NON_PLANAR_VIEWER_H__

#include "QvtkOrthogonalViewer.h"

class ABSTRACTVIEWER_EXPORT QvtkNonPlanarViewer : public QvtkOrthogonalViewer
{
	Q_OBJECT;
public:
	explicit QvtkNonPlanarViewer(QWidget* parent = nullptr);

public slots:
	//virtual void SetOrientation(int orientation) override;
	//virtual void SetCursorPosition(double x, double y, double z) override;

protected:
	virtual void UpdateCameraViewPlaneNormal() override;
};

#endif // !__Qvtk_NON_PLANAR_VIEWER_H__
