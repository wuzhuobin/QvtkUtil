// me
#include "QvtkProp.h"
#include "QvtkDataSet.h"
#include "QvtkScene.h"


// qt
#include <QDebug>
#include <QDomElement>

// vtk
#include <vtkProp3D.h>
#include <vtkTransform.h>
#include <vtkDataSet.h>
#include <vtkNew.h>
#include <vtkCommand.h>

// std
#include <sstream>
namespace Q {
namespace vtk{

Q_VTK_DATACPP(Prop)
class MatrixCallback final: public vtkCommand
{
public:
	static MatrixCallback* New() { return new MatrixCallback; }
	vtkTypeMacro(MatrixCallback, vtkCommand);
	virtual void Execute(vtkObject *caller, unsigned long eventId,
		void *callData) override {
		self->propMatrixUpdate();
	}
	Prop* self;

};

class PickedCallback final : public vtkCommand
{
public:
	static PickedCallback* New() { return new PickedCallback; }
	vtkTypeMacro(PickedCallback, vtkCommand);
	virtual void Execute(vtkObject* caller, unsigned long eventId,
		void* callData) override {
		emit self->picked();
	}
	Prop* self;
};

const QString Prop::DISPLAY_REGION_PREFIX[2] = { "Min", "Max" };

Prop::Prop()
{
	this->matrixCallback = MatrixCallback::New();
	this->matrixCallback->self = this;
	this->pickedCallback = PickedCallback::New();
	this->pickedCallback->self = this;
	this->cachedInverstedMatrix = vtkMatrix4x4::New();
	this->prop3D = nullptr;
	this->renderDataSet = nullptr;
	QStandardItem* regionX = createAttribute(K.DisplayRegionX);
	this->displayRegionX = createAttributesByColumns(
		QStringList() << DISPLAY_REGION_PREFIX[0] << DISPLAY_REGION_PREFIX[1],
		QVariantList() << static_cast<double>(0) << static_cast<double>(0),
		true,
		regionX
	);
	QStandardItem* regionY = createAttribute(K.DisplayRegionY);
	this->displayRegionY = createAttributesByColumns(
		QStringList() << DISPLAY_REGION_PREFIX[0] << DISPLAY_REGION_PREFIX[1],
		QVariantList() << static_cast<double>(0) << static_cast<double>(0),
		true,
		regionY
	);
	QStandardItem* regionZ = createAttribute(K.DisplayRegionZ);
	this->displayRegionZ = createAttributesByColumns(
		QStringList() << DISPLAY_REGION_PREFIX[0] << DISPLAY_REGION_PREFIX[1],
		QVariantList() << static_cast<double>(0) << static_cast<double>(0),
		true,
		regionZ
	);

	this->rendering = false;
	this->viewerID = -1;
	this->rendererID = -1;

	insertSlotFunction(this->displayRegionX[0], &Prop::setDisplayRegion);
	insertSlotFunction(this->displayRegionX[1], &Prop::setDisplayRegion);
	insertSlotFunction(this->displayRegionY[0], &Prop::setDisplayRegion);
	insertSlotFunction(this->displayRegionY[1], &Prop::setDisplayRegion);
	insertSlotFunction(this->displayRegionZ[0], &Prop::setDisplayRegion);
	insertSlotFunction(this->displayRegionZ[1], &Prop::setDisplayRegion);
}

Prop::~Prop()
{
	setRenderDataSet(nullptr);

	this->prop3D->Delete();

	this->cachedInverstedMatrix->Delete();

	this->pickedCallback->Delete();

	this->matrixCallback->Delete();

}

void Prop::setRenderDataSet(DataSet* data)
{
	// the same data has already been set.
	if(data == this->renderDataSet){
		return;
	}

	// remove the old one reference if it is not nullptr
	if(this->renderDataSet){
		this->renderDataSet->removeReference(this);
		// remove connection
		setUserMatrix(nullptr);
		// a checking for disable disconnect warning 
		if (this->renderDataSet) {
			disconnect(this->renderDataSet, &DataSet::opacityChanged,
				this, &Prop::setOpacity);
			disconnect(this->renderDataSet, &DataSet::orientationChanged,
				this, &Prop::setOrigin);
			disconnect(this->renderDataSet, &DataSet::positionChanged,
				this, &Prop::setPosition);
			disconnect(this->renderDataSet, &DataSet::orientationChanged,
				this, &Prop::setOrientation);
			disconnect(this->renderDataSet, &DataSet::scaleChanged,
				this, &Prop::setScale);
			//disconnect(this->renderDataSet, SIGNAL(originChanged(const double*)), this, SLOT(propMatrixUpdate()));
			//disconnect(this->renderDataSet, SIGNAL(positionChanged(const double*)), this, SLOT(propMatrixUpdate()));
			//disconnect(this->renderDataSet, SIGNAL(orientationChanged(const double*)), this, SLOT(propMatrixUpdate()));
			//disconnect(this->renderDataSet, SIGNAL(scaleChanged(const double*)), this, SLOT(propMatrixUpdate()));
		}
	}

	this->renderDataSet = data;
	// nullptr for removing, so do nothing
	if(this->renderDataSet){
		// normally prop3D is not a nullptr
		if (!this->prop3D) {
			qCritical() << "prop3D is still nullptr";
			return;
		}

		// renderData add reference
		this->renderDataSet->addReference(this);


		// the real transform of the prop3D is userTransform * additionalTransform
		//vtkTransform* userTransForm = vtkTransform::New();
		//userTransForm->Identity();
		//userTransForm->PostMultiply();
		//userTransForm->Concatenate(data->getUserMatrix());
		//userTransForm->Concatenate(data->getAdditionalMatrix());
		//userTransForm->Update();
		//this->prop3D->SetUserTransform(this->renderDataSet->getTransform());
		this->prop3D->SetOrigin(
			this->renderDataSet->getOrigin()[0],
			this->renderDataSet->getOrigin()[1],
			this->renderDataSet->getOrigin()[2]
		);
		connect(this->renderDataSet, &DataSet::originChanged,
			this, &Prop::setOrigin);
		//connect(this->renderDataSet, SIGNAL(originChanged(const double*)), this, SLOT(propMatrixUpdate()));
		this->prop3D->SetPosition(
			this->renderDataSet->getPosition()[0],
			this->renderDataSet->getPosition()[1],
			this->renderDataSet->getPosition()[2]
		);
		connect(this->renderDataSet, &DataSet::positionChanged,
			this, &Prop::setPosition);
		//connect(this->renderDataSet, SIGNAL(positionChanged(const double*)), this, SLOT(propMatrixUpdate()));

		this->prop3D->SetOrientation(
			this->renderDataSet->getOrientation()[0],
			this->renderDataSet->getOrientation()[1],
			this->renderDataSet->getOrientation()[2]
		);
		connect(this->renderDataSet, &DataSet::orientationChanged,
			this, &Prop::setOrientation);
		//connect(this->renderDataSet, SIGNAL(orientationChanged(const double*)), this, SLOT(propMatrixUpdate()));

		this->prop3D->SetScale(
			this->renderDataSet->getScale()[0],
			this->renderDataSet->getScale()[1],
			this->renderDataSet->getScale()[2]
		);
		connect(this->renderDataSet, &DataSet::scaleChanged,
			this, &Prop::setScale);
		//connect(this->renderDataSet, SIGNAL(scaleChanged(const double*)), this, SLOT(propMatrixUpdate()));

		setUserMatrix(this->renderDataSet->getUserMatrix());
		//this->prop3D->SetUserMatrix(this->renderDataSet->getUserMatrix());
		this->prop3D->SetPickable(this->renderDataSet->getPickable());
		setOpacity(this->renderDataSet->getOpacity());
		connect(this->renderDataSet, &DataSet::opacityChanged,
			this, &Prop::setOpacity);
		//userTransForm->Delete();

		// update the display region to largest region, if it is default value.
		// since the prop will apply the a userTransform, the transform should be apply.
		double region[6];
		getDisplayRegion(region);
		double _region[6] = { 0,0,0,0,0,0 };
		if (std::equal(region, region + 5, _region)) {
			// if _region is default value, using the whole bounds.
			this->renderDataSet->getWorldBounds(region);
		}
		setDisplayRegion(region);
		propMatrixUpdate();
	}

}

void Prop::setUserMatrix(vtkMatrix4x4 * matrix)
{
	if (getProp()->GetUserTransform()&& getProp()->GetUserMatrix() == matrix) {
		return;
	}
	if (getProp()->GetUserTransform()&& getProp()->GetUserMatrix()) {
		getProp()->GetUserMatrix()->RemoveObserver(this->matrixCallback);
	}
	getProp()->SetUserMatrix(matrix);
	if (getProp()->GetUserTransform()&& getProp()->GetUserMatrix()) {
		getProp()->GetUserMatrix()->AddObserver(vtkCommand::ModifiedEvent, this->matrixCallback);
		vtkMatrix4x4::Invert(getProp()->GetMatrix(), this->cachedInverstedMatrix);

	}
}

void Prop::setOrigin(const double origin[3])
{
	this->prop3D->SetOrigin(origin);
	this->propMatrixUpdate();
}

void Prop::setPosition(const double position[3])
{
	this->prop3D->SetPosition(position[0], position[1], position[2]);
	this->propMatrixUpdate();
}

void Prop::setOrientation(const double orientation[3])
{
	this->prop3D->SetOrientation(orientation[0], orientation[1], orientation[2]);
	this->propMatrixUpdate();
}

void Prop::setScale(const double scale[3])
{
	this->prop3D->SetScale(scale[0], scale[1], scale[2]);
	this->propMatrixUpdate();
}

void Prop::propMatrixUpdate()
{
	double world[6], dataSet[6];

	this->getDisplayRegion(world);

	for (int i = 0; i < 3; i++) {
		dataSet[i * 2] = this->cachedInverstedMatrix->GetElement(i, 0) * world[0] +
			this->cachedInverstedMatrix->GetElement(i, 1) * world[2] +
			this->cachedInverstedMatrix->GetElement(i, 2) * world[4] +
			this->cachedInverstedMatrix->GetElement(i, 3);
		dataSet[i * 2 + 1] = this->cachedInverstedMatrix->GetElement(i, 0) * world[1] +
			this->cachedInverstedMatrix->GetElement(i, 1) * world[3] +
			this->cachedInverstedMatrix->GetElement(i, 2) * world[5] +
			this->cachedInverstedMatrix->GetElement(i, 3);
	}
	vtkMatrix4x4::Invert(getProp()->GetMatrix(), this->cachedInverstedMatrix);
	this->dataSetRegionToWorldRegion(dataSet, world);
	this->setDisplayRegion(world);
}

void Prop::updateUniqueReferences(QString newName, QString oldName, QDomElement & xml) const
{
	Data::updateUniqueReferences(newName, oldName, xml);
	QString uniqueName = xml.attribute(K.RenderDataSet);
	if (uniqueName == oldName) {
		xml.setAttribute(K.RenderDataSet, newName);
	}
}

void Prop::setProp(vtkProp3D * prop)
{
	if (this->prop3D == prop)
	{
		return;
	}
	if (this->prop3D) {
		this->prop3D->RemoveObserver(this->pickedCallback);
		this->prop3D->Delete();
	}
	this->prop3D = prop;

	if (this->prop3D) {

		this->prop3D->AddObserver(vtkCommand::PickEvent, this->pickedCallback);
	}
}

void Prop::setDisplayRegion(Data * self, QStandardItem * item)
{
	Prop* _self = static_cast<Prop*>(self);
	double region[6];
	_self->getDisplayRegion(region);
	_self->setDisplayRegion(region);

}
vtkProp3D* Prop::getProp() const
{
	return this->prop3D;
}

void Prop::setDisplayRegion(const double region[6])
{
	//if (!this->renderDataSet) {
	//	qWarning() << "renderDataSet is nullptr.";
	//	qWarning() << "region will not changed.";
	//	return;
	//}
	setAttributes(this->displayRegionX, QVariantList() << region[0] << region[1]);
	setAttributes(this->displayRegionY, QVariantList() << region[2] << region[3]);
	setAttributes(this->displayRegionZ, QVariantList() << region[4] << region[5]);
}

void Prop::resetDisplayRegion()
{
	double bounds[6] = { 0, 0, 0, 0, 0, 0 };
	if (this->getRenderDataSet()) {
		this->getRenderDataSet()->getWorldBounds(bounds);
	}
	this->setDisplayRegion(bounds);
}

void Prop::printSelf() const
{
	Data::printSelf();
	std::stringstream ss;

	ss.clear();
	this->prop3D->Print(ss);
	qDebug() << "prop3D" << '=' << QString::fromStdString(ss.str());

	qDebug() << "rednerDataSet" << '=' << this->renderDataSet;

}

void Prop::readXML(const QDomElement& xml, QString directoryPath)
{
	Data::readXML(xml, directoryPath);

	setViewerID(xml.attribute(K.Viewer_ID).toLongLong());
	setRendererID(xml.attribute(K.Renderer_ID).toInt());

	readDataSet(xml);
}

void Prop::writeXML(QDomElement& xml, QString directoryPath) const
{
	Data::writeXML(xml, directoryPath);

	xml.setAttribute(K.Viewer_ID, getViewerID());
	xml.setAttribute(K.Renderer_ID, getLayer());

	writeDataSet(xml);
}

void Prop::reset()
{
	Data::reset();
	double region[6] = {0,0,0,0,0,0};
	setDisplayRegion(region);
}

void Prop::dataSetCoordinateToWorldCoordinate(const double dataSet[3], double world[3]) const
{
	vtkMatrix4x4* matrix = this->getProp()->GetMatrix();
	//double _dataSet[4] = { dataSet[0], dataSet[1], dataSet[2], 1 };
	for (int i = 0; i < 3; i++) {
		world[i] = matrix->GetElement(i, 0) * dataSet[0] +
			matrix->GetElement(i, 1) * dataSet[1] +
			matrix->GetElement(i, 2) * dataSet[2] +
			matrix->GetElement(i, 3);
	}
}

void Prop::worldCoordinateToDataSetCoordinate(const double world[3], double dataSet[3]) const
{
	vtkNew<vtkMatrix4x4> matrix;
	vtkMatrix4x4::Invert(this->getProp()->GetMatrix(), matrix.GetPointer());
	//vtkNew<vtkTransform> transform;
	//transform->SetMatrix(this->getProp()->GetMatrix());
	//transform->GetInverse(matrix.GetPointer());
	for (int i = 0; i < 3; i++) {
		dataSet[i] = matrix->GetElement(i, 0) * world[0] +
			matrix->GetElement(i, 1) * world[1] +
			matrix->GetElement(i, 2) * world[2] +
			matrix->GetElement(i, 3);
	}
}

void Prop::dataSetRegionToWorldRegion(const double dataSet[6], double world[6]) const
{
	vtkMatrix4x4* matrix = this->getProp()->GetMatrix();
	for (int i = 0; i < 3; i++) {
		world[i * 2] = matrix->GetElement(i, 0) * dataSet[0] +
			matrix->GetElement(i, 1) * dataSet[2] +
			matrix->GetElement(i, 2) * dataSet[4] +
			matrix->GetElement(i, 3);
		world[i * 2 + 1] = matrix->GetElement(i, 0) * dataSet[1] +
			matrix->GetElement(i, 1) * dataSet[3] +
			matrix->GetElement(i, 2) * dataSet[5] +
			matrix->GetElement(i, 3);
	}
}

void Prop::worldRegionToDataSetRegion(const double world[6], double dataSet[6]) const
{
	vtkNew<vtkMatrix4x4> matrix;
	//vtkNew<vtkTransform> transform;
	//transform->SetMatrix(this->getProp()->GetMatrix());
	vtkMatrix4x4::Invert(this->getProp()->GetMatrix(), matrix.GetPointer());
	//transform->GetInverse(matrix.GetPointer());
	for (int i = 0; i < 3; i++) {
		dataSet[i * 2] = matrix->GetElement(i, 0) * world[0] +
			matrix->GetElement(i, 1) * world[2] +
			matrix->GetElement(i, 2) * world[4] +
			matrix->GetElement(i, 3);
		dataSet[i * 2 + 1] = matrix->GetElement(i, 0) * world[1] +
			matrix->GetElement(i, 1) * world[3] +
			matrix->GetElement(i, 2) * world[5] +
			matrix->GetElement(i, 3);
	}
}



void Prop::getDisplayRegion(double region[6]) const
{
	const QVariantList& regionX = getAttributes(this->displayRegionX);
	const QVariantList& regionY = getAttributes(this->displayRegionY);
	const QVariantList& regionZ = getAttributes(this->displayRegionZ);

	region[0] = regionX[0].toDouble();
	region[1] = regionX[1].toDouble();
	region[2] = regionY[0].toDouble();
	region[3] = regionY[1].toDouble();
	region[4] = regionZ[0].toDouble();
	region[5] = regionZ[1].toDouble();

}

void Prop::readDataSet(const QDomElement& xml)
{
	QString uniqueName = xml.attribute(K.RenderDataSet);
	DataSet* dataSet = qobject_cast<DataSet*>(Scene::getCurrentScene()->getDataByUniqueName(uniqueName));
	if(!dataSet){
		//qDebug() << "Render data is not created yet.";
		//qDebug() << "Waiting redner data to set data";
		//qDebug() << uniqueName;
		return;
	}

	setRenderDataSet(dataSet);

}

void Prop::writeDataSet(QDomElement& xml) const
{
	if(!this->renderDataSet){
		qWarning() << "renderDataSet is nullptr";
		return;
	}
	xml.setAttribute(K.RenderDataSet, this->renderDataSet->getUniqueName());
}

}
}