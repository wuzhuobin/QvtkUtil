#ifndef __Qvtk_ABSTRACT_ANNOTATION_PROP_H__
#define __Qvtk_ABSTRACT_ANNOTATION_PROP_H__

#include "QvtkAbstractPlanarProp.h"
//#include "DataMacro.h"


#include <vtkSmartPointer.h>

class vtkActor;
class vtkBox;
class vtkClipPolyData;
class vtkPlane;
class vtkCutter;
class vtkStripperPolygon;
class vtkTriangleFilter;
//class vtkTransformPolyDataFilter;
class vtkPolyDataMapper;
//class vtkLinesToPolysFilter;

class ABSTRACTDATA_EXPORT QvtkPolyDataActor2D : public QvtkAbstractPlanarProp
{
	Q_OBJECT;
	Q_VTK_DATAH(QvtkPolyDataActor2D)
public:

	QvtkPolyDataActor2D();
	virtual ~QvtkPolyDataActor2D() override;
	virtual void printSelf() const override;

	virtual vtkActor* getActor() const;

	virtual void propMatrixUpdate() override;
public slots:

	virtual void reset() override;

	virtual void setPlanarNormal(double x, double y, double z) override;

	virtual void setPlanarOrigin(double x, double y, double z) override;

	virtual void setDisplayRegion(const double region[6]) override;

	virtual void setRenderDataSet(DataSet* data = nullptr) override;

	virtual void setOpacity(double opacity) override;

	virtual void setColor(const double rgb[3]);

protected:
	virtual Data* newInstance() const override { return new QvtkPolyDataActor2D; }

	vtkSmartPointer<vtkBox> box;

	vtkSmartPointer<vtkClipPolyData> clipper;

	vtkSmartPointer<vtkPlane> plane;
	vtkSmartPointer<vtkCutter> cutfilter;

	vtkSmartPointer<vtkStripperPolygon>        boundaryStrip;

	vtkSmartPointer<vtkTriangleFilter>  triFilter;

	vtkSmartPointer<vtkPolyDataMapper>  mapper;


};


#endif