#include "QvtkPolyDataActor.h"

// me
#include "DataSet.h"
#include "QvtkPolyData.h"

// vtk
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkClipPolyData.h>
#include <vtkNew.h>
#include <vtkBox.h>
#include <vtkTransform.h>

// qt
#include <QDebug>
Q_VTK_DATACPP(QvtkPolyDataActor);
QvtkPolyDataActor::QvtkPolyDataActor()
{

	// install pipeline
	vtkNew<vtkBox> box;
	this->box = box.GetPointer();

	vtkNew<vtkClipPolyData> clipper;
	this->clipper = clipper.GetPointer();
	this->clipper->InsideOutOn();
	this->clipper->SetClipFunction(this->box);



	vtkNew<vtkPolyDataMapper> polydata;
	this->polyDataMapper = polydata.GetPointer();
	//this->polyDataMapper->ScalarVisibilityOff();
	this->polyDataMapper->SetInputConnection(this->clipper->GetOutputPort());

	vtkActor* actor = vtkActor::New();
	actor->SetMapper(this->polyDataMapper);
	this->setProp(actor);

}

QvtkPolyDataActor::~QvtkPolyDataActor()
{
	//qDebug() << this->getClassName();
	setRenderDataSet(nullptr);
}

void QvtkPolyDataActor::printSelf() const
{
	Prop::printSelf();
}

//bool QvtkPolyDataActor::isClass(QString className) const
//{
//	if(getClassName() != className){
//		return Prop::isClass(className);
//	}
//	return true;
//}

void QvtkPolyDataActor::reset()
{
	Prop::reset();
}

vtkActor* QvtkPolyDataActor::getActor() const
{
	return vtkActor::SafeDownCast(this->getProp());
}

void QvtkPolyDataActor::propMatrixUpdate()
{
	vtkNew<vtkTransform> transform;
	transform->SetMatrix(this->getProp()->GetMatrix());
	//this->box->SetTransform(transform.GetPointer());
	Prop::propMatrixUpdate();
}

void QvtkPolyDataActor::setDisplayRegion(const double region[6])
{
	Prop::setDisplayRegion(region);
	//this->box->SetBounds(region);
	if (this->getRenderDataSet()) {
		double _region[6];
		/*this->getRenderDataSet()->*/worldRegionToDataSetRegion(region, _region);
		this->box->SetBounds(_region);
	}
}

void QvtkPolyDataActor::setRenderDataSet(DataSet* data)
{
	if (this->getRenderDataSet() == data) {
		return;
	}

	// nullptr to remove connection
	if(this->getRenderDataSet()){

		this->clipper->SetInputConnection(nullptr);
		PolyData* polydata = qobject_cast<PolyData*>(this->getRenderDataSet());
		if (polydata) {
			disconnect(polydata, &PolyData::colorChanged,
				this, &QvtkPolyDataActor::setColor);
		}
	}
	Prop::setRenderDataSet(data);

	if(this->getRenderDataSet())
	{
		this->clipper->SetInputConnection(this->getRenderDataSet()->getOutputPort());
		PolyData* polydata = qobject_cast<PolyData*>(this->getRenderDataSet());
		if (!polydata) {
			qCritical() << "data is not PolyData";

		}
		else {
			double rgb[3];
			polydata->getColor(rgb);
			setColor(rgb);
			connect(polydata, &PolyData::colorChanged,
				this, &QvtkPolyDataActor::setColor);
		}
	}
}

void QvtkPolyDataActor::setOpacity(double opacity)
{
	if (getActor()) {
		getActor()->GetProperty()->SetOpacity(opacity);
	}
}

void QvtkPolyDataActor::setColor(const double rgb[3])
{
	if (getActor()) {
		getActor()->GetProperty()->SetColor(rgb[0], rgb[1], rgb[2]);
	}
}
