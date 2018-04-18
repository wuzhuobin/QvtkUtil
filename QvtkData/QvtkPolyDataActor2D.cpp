// me
#include "QvtkPolyDataActor2D.h"
#include "QvtkScene.h"
#include "Data.h"
#include "QvtkPolyData.h"

// vtk
#include <vtkBox.h>
#include <vtkClipPolyData.h>
#include <vtkPlane.h>
#include <vtkCutter.h>
#include <vtkStripper.h>
#include <vtkTriangleFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkInformationVector.h>
#include <vtkInformation.h>
#include <vtkTransform.h>
#include <vtkNew.h>
#include <vtkMatrix4x4.h>

#include <vtkPassThroughFilter.h>

#include <QDomElement>
#include <QDebug>

Q_VTK_DATACPP(QvtkPolyDataActor2D);
class vtkStripperPolygon : public vtkStripper 
{
public :
	static vtkStripperPolygon* New() { return new vtkStripperPolygon; }
	vtkTypeMacro(vtkStripperPolygon, vtkStripper);

protected:
	// Usual data generation method
	virtual int RequestData(
		vtkInformation *request,
		vtkInformationVector **inputVector,
		vtkInformationVector *outputVector) VTK_OVERRIDE {
		int returnValue = Superclass::RequestData(request, inputVector, outputVector);

		// get the info objects
		vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
		vtkInformation *outInfo = outputVector->GetInformationObject(0);

		vtkPolyData *output = vtkPolyData::SafeDownCast(
			outInfo->Get(vtkDataObject::DATA_OBJECT()));

		output->SetPolys(output->GetLines());
		return returnValue;
	}



};

QvtkPolyDataActor2D::QvtkPolyDataActor2D()
{
	this->box = vtkSmartPointer<vtkBox>::New();
	this->clipper = vtkSmartPointer<vtkClipPolyData>::New();
	this->clipper->SetClipFunction(this->box);
	this->clipper->InsideOutOn();


    //this->transFilter     = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	this->plane = vtkSmartPointer<vtkPlane>::New();
    this->cutfilter       = vtkSmartPointer<vtkCutter>::New();
	this->cutfilter->SetCutFunction(this->plane);
	this->cutfilter->SetInputConnection(this->clipper->GetOutputPort());


    this->boundaryStrip   = vtkSmartPointer<vtkStripperPolygon>::New();
	this->boundaryStrip->SetInputConnection(this->cutfilter->GetOutputPort());

    this->triFilter       = vtkSmartPointer<vtkTriangleFilter>::New();
	this->triFilter->SetInputConnection(this->boundaryStrip->GetOutputPort());
	

	this->mapper          = vtkSmartPointer<vtkPolyDataMapper>::New();
	this->mapper->SetInputConnection(triFilter->GetOutputPort());
	//this->mapper->ScalarVisibilityOff();
	//this->mapper->SetScalarMaterialModeToAmbient();
	//this->mapper->SetScalarMaterialModeToAmbientAndDiffuse();
	//this->mapper->SetScalarMaterialModeToDefault();
	//this->mapper->SetScalarVisibility(false);


	vtkActor* actor = vtkActor::New();
	actor->SetMapper(this->mapper);
	this->setProp(actor);
	//Qvtk_DATA_CREATE_PROPERTIES_ARRAY_ATTRIBUTES_MACRO(double, HoverColor, HOVER_COLOR, HOVER_COLOR_RGB, 3);
	//Qvtk_DATA_CREATE_PROPERTIES_ARRAY_ATTRIBUTES_MACRO(double, SelectedColor, SELECTED_COLOR, SELECTED_COLOR_RGB, 3);
	//Qvtk_DATA_CREATE_PROPERTIES_ARRAY_ATTRIBUTES_MACRO(double, IdleColor, IDLE_COLOR, IDLE_COLOR_RGB, 3);
	//Qvtk_DATA_CREATE_PROPERTIES_ATTRIBUTES_MACRO(double, Opacity, OPACITY, 1);

 //   this->PropState = this->createAttribute(PROP_STATE_STRING, IDLE, false);

 //   this->setIdleColor(0.8, 0.4, 0.4);
 //   this->setHoverColor(0.4, 0.8, 0.8);
 //   this->setSelectedColor(1, 0, 0);
}

QvtkPolyDataActor2D::~QvtkPolyDataActor2D()
{
	setRenderDataSet(nullptr);
}

void QvtkPolyDataActor2D::printSelf() const
{
    QvtkAbstractPlanarProp::printSelf();

 //   Qvtk_PRINT_PROPERTY_VECTOR("Hover Color",	HoverColor, 3);
	//Qvtk_PRINT_PROPERTY_VECTOR("Idle Color",		IdleColor, 3);
	//Qvtk_PRINT_PROPERTY_VECTOR("Selected Color", SelectedColor, 3);

	//qDebug() << "Opacity:\t" << this->Opacity;

 //   qDebug() << "Pickable:\t" << this->pickable;
}



//void QvtkPolyDataActor2D::setBasePolyData(QvtkPolyData* inPD)
//{
//	this->basePDUniqueName = inPD->getUniqueName();
//
//    this->setRenderDataSet(inPD);
//
//    this->initializeProp();
//}

//QvtkPolyData* QvtkPolyDataActor2D::getBasePolyData()
//{
//    QvtkPolyData *castedptr = dynamic_cast<QvtkPolyData *>(this->getRenderDataSet());
//    if (NULL != castedptr)
//    {
//        return (QvtkPolyData*)this->getRenderDataSet();
//    }
//    else
//    {
//        qCritical() << "Data set cannot be casted to polydata!";
//        return nullptr;
////    }
//}
//
//void QvtkPolyDataActor2D::readXML(const QDomElement& xml, QString directoryPath /*= QString()*/)
//{
//    QvtkAbstractPlanarProp::readXML(xml, directoryPath);
//
//	setPickable(static_cast<bool>(xml.attribute(PICKABLE).toInt()));
//	setOpacity(xml.attribute(OPACITY).toDouble());
//	setHoverColor(csvStringToValues<double>(xml.attribute(HOVER_COLOR)));
//	setHoverColor(csvStringToValues<double>(xml.attribute(SELECTED_COLOR)));
//	setHoverColor(csvStringToValues<double>(xml.attribute(IDLE_COLOR)));
//
//	/* Load polydata */
//	QvtkPolyData* basePD = (QvtkPolyData*)QvtkScene::getCurrentScene()
//		->getDataByUniqueName(xml.attribute(BASE_QvtkPOLYDATA_UNIQUE_NAME));
//	if (!basePD)
//	{
//		qWarning() << "Cannot load base polydata from current scene!";
//	}
//	else
//	{
//		this->setRenderDataSet(basePD);
//	}
//}
//
//void QvtkPolyDataActor2D::writeXML(QDomElement& xml, QString directoryPath /*= QString()*/) const
//{
//    QvtkAbstractPlanarProp::writeXML(xml, directoryPath);
//
//	xml.setAttribute(BASE_QvtkPOLYDATA_UNIQUE_NAME, ((QvtkPolyData*)this->getRenderDataSet())->getUniqueName());
//    xml.setAttribute(PICKABLE, getPickable());
//	xml.setAttribute(OPACITY, getOpacity());
//	xml.setAttribute(HOVER_COLOR,	 Qvtk_QVARIANTLIST_3_TO_CSV_STRING(getAttributes(HoverColor)));
//	xml.setAttribute(SELECTED_COLOR, Qvtk_QVARIANTLIST_3_TO_CSV_STRING(getAttributes(SelectedColor)));
//	xml.setAttribute(HOVER_COLOR,	 Qvtk_QVARIANTLIST_3_TO_CSV_STRING(getAttributes(IdleColor)));
//
//}

void QvtkPolyDataActor2D::reset()
{
    QvtkAbstractPlanarProp::reset();
    //
    //this->setIdleColor(0.8, 0.4, 0.4);
    //this->setHoverColor(0.4, 0.8, 0.8);
}

void QvtkPolyDataActor2D::setPlanarNormal(double x, double y, double z)
{
	QvtkAbstractPlanarProp::setPlanarNormal(x, y, z);
	// normal does not need translate
	double _normal[4] = { x, y, z, 1 };

	//vtkNew<vtkMatrix4x4> rotation;
	//vtkMatrix4x4::Invert(getProp()->GetMatrix(), (rotation.GetPointer()));
	////rotation->DeepCopy(getProp()->GetMatrix());
	//rotation->SetElement(0, 3, 0);
	//rotation->SetElement(1, 3, 0);
	//rotation->SetElement(2, 3, 0);

	//rotation->MultiplyPoint(_normal, _normal);



	this->plane->SetNormal(_normal);
	//if (this->getRenderDataSet()) {
	//	double _normal[3];
	//	qDebug() << "world: ";
	//	qDebug() << normal[0];
	//	qDebug() << normal[1];
	//	qDebug() << normal[2];
	//	/*this->getRenderDataSet()->*/worldCoordinateToDataSetCoordinate(normal, _normal);
	//	this->plane->SetNormal(_normal);

	//	qDebug() << "dataset: ";
	//	qDebug() << -normal[0];
	//	qDebug() << -normal[1];
	//	qDebug() << -normal[2];
	//}
}

void QvtkPolyDataActor2D::setPlanarOrigin(double x, double y, double z)
{
	QvtkAbstractPlanarProp::setPlanarOrigin(x, y, z);
	double _origin[4] = { x, y, z, 1 };

	// origin does not need rotate
	//vtkNew<vtkMatrix4x4> translation;

	//translation->SetElement(0, 3, -getProp()->GetMatrix()->GetElement(0, 3));
	//translation->SetElement(1, 3, -getProp()->GetMatrix()->GetElement(1, 3));
	//translation->SetElement(2, 3, -getProp()->GetMatrix()->GetElement(2, 3));

	//translation->MultiplyPoint(_origin, _origin);

	this->plane->SetOrigin(_origin);

	//if (this->getRenderDataSet()) {
	//	double _origin[3];
	//	/*this->getRenderDataSet()->*/worldCoordinateToDataSetCoordinate(origin, _origin);
	//	this->plane->SetOrigin(_origin);
	//}
	//this->clipper->GetOutput()->Print(cout);
	//this->cutfilter->GetOutput()->Print(cout);
	//this->boundaryStrip->GetOutput()->Print(cout);
	//this->boundaryStrip->GetOutput()->Print(cout);
}

void QvtkPolyDataActor2D::setDisplayRegion(const double region[6])
{
	QvtkAbstractPlanarProp::setDisplayRegion(region);
	//this->box->set
	//this->box->SetBounds(region);
	if (this->getRenderDataSet()) {
		double _region[6];
		/*this->getRenderDataSet()->*/worldRegionToDataSetRegion(region, _region);
		this->box->SetBounds(_region);

	}
}

vtkActor * QvtkPolyDataActor2D::getActor() const
{
	return vtkActor::SafeDownCast(this->getProp());
}

void QvtkPolyDataActor2D::propMatrixUpdate()
{
	vtkNew<vtkTransform> transform;
	transform->SetMatrix(this->getProp()->GetMatrix());
	//vtkNew<vtkTransform> transform2;
	//transform->Inverse();
	this->plane->SetTransform(transform.GetPointer());
	//this->box->SetTransform(transform.GetPointer());
	QvtkAbstractPlanarProp::propMatrixUpdate();
}

void QvtkPolyDataActor2D::setRenderDataSet(DataSet * data)
{

	if (this->getRenderDataSet() == data) {
		return;
	}

	// nullptr to remove connection
	if (this->getRenderDataSet()) {
		//this->plane->SetTransform(static_cast<vtkTransform*>(nullptr));
		this->clipper->SetInputConnection(nullptr);
		QvtkPolyData* polydata = qobject_cast<QvtkPolyData*>(this->getRenderDataSet());
		if (polydata) {
			disconnect(polydata, &QvtkPolyData::colorChanged,
				this, &QvtkPolyDataActor2D::setColor);
		}
	}
	QvtkAbstractPlanarProp::setRenderDataSet(data);
	if(this->getRenderDataSet())
	{
		//this->plane->SetTransform(this->getProp()->GetUserTransform());
		this->clipper->SetInputConnection(data->getOutputPort());
		QvtkPolyData* polydata = qobject_cast<QvtkPolyData*>(data);
		if (!polydata) {
			qCritical() << "data is not QvtkPolyData";

		}
		else {
			double rgb[3];
			polydata->getColor(rgb);
			setColor(rgb);
			connect(polydata, &QvtkPolyData::colorChanged,
				this, &QvtkPolyDataActor2D::setColor);
		}
	}
}

void QvtkPolyDataActor2D::setOpacity(double opacity)
{
	if (getActor()) {
		getActor()->GetProperty()->SetOpacity(opacity);
	}
}

void QvtkPolyDataActor2D::setColor(const double rgb[3])
{
	if (getActor()) {
		getActor()->GetProperty()->SetColor(rgb[0], rgb[1], rgb[2]);
	}
}
