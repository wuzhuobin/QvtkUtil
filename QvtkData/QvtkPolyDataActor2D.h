#ifndef __Qvtk_ABSTRACT_ANNOTATION_PROP_H__
#define __Qvtk_ABSTRACT_ANNOTATION_PROP_H__

#include "QvtkPlanarProp.h"


#include <vtkSmartPointer.h>

class vtkActor;
class vtkBox;
class vtkClipPolyData;
class vtkPlane;
class vtkCutter;
class vtkStripperPolygon;
class vtkTriangleFilter;
class vtkPolyDataMapper;

namespace Q{
namespace vtk{


class QVTKDATA_EXPORT PolyDataActor2D : public PlanarProp
{
	Q_OBJECT;
	Q_VTK_DATA_H(PolyDataActor2D)
public:

	PolyDataActor2D();
	virtual ~PolyDataActor2D() override;
	virtual void printSelf() const override;

	virtual vtkActor* getActor() const;

	virtual void propMatrixUpdate() override;
public slots:

	virtual void reset() override;

	virtual void setPlanarNormal(double x, double y, double z) override;
	void setPlanarNormal(const double normal[3]) { this->setPlanarNormal(normal[0], normal[1], normal[2]); }
	virtual void setPlanarOrigin(double x, double y, double z) override;
	void setPlanarOrigin(const double origin[3]) { this->setPlanarOrigin(origin[0], origin[1], origin[2]); }
	virtual void setDisplayRegion(const double region[6]) override;

	virtual void setRenderDataSet(DataSet* data = nullptr) override;

	virtual void setOpacity(double opacity) override;

	virtual void setColor(const double rgb[3]);

protected:
	virtual Data* newInstance() const override { return new PolyDataActor2D; }

	vtkSmartPointer<vtkBox> box;

	vtkSmartPointer<vtkClipPolyData> clipper;

	vtkSmartPointer<vtkPlane> plane;
	vtkSmartPointer<vtkCutter> cutfilter;

	vtkSmartPointer<vtkStripperPolygon>        boundaryStrip;

	vtkSmartPointer<vtkTriangleFilter>  triFilter;

	vtkSmartPointer<vtkPolyDataMapper>  mapper;


};
}
}

#endif