#ifndef __Qvtk_POLY_DATA_ACTOR_H__
#define __Qvtk_POLY_DATA_ACTOR_H__

// me
#include "Prop.h"

class vtkActor;
class vtkPolyDataMapper;
class vtkClipPolyData;
class vtkBox;

class QVTKDATA_EXPORT QvtkPolyDataActor: public Prop
{
	Q_OBJECT;
	Q_VTK_DATAH(QvtkPolyDataActor)
public:

	QvtkPolyDataActor();
	virtual ~QvtkPolyDataActor() override;
	virtual void printSelf() const override;
	virtual vtkActor* getActor() const;
	virtual void propMatrixUpdate() override;
public slots:

	virtual void reset() override;

	virtual void setDisplayRegion(const double region[6]) override;

	virtual void setRenderDataSet(DataSet* data) override;
	virtual void setOpacity(double opacity) override;
	virtual void setColor(const double rgb[3]);
protected:
	virtual Data* newInstance() const override { return new QvtkPolyDataActor; }

	vtkBox* box;
	vtkClipPolyData* clipper;
	vtkPolyDataMapper* polyDataMapper;

};


#endif // !__Qvtk_POLY_DATA_ACTOR_H__
