// me
#include "QvtkDataSet.h"
#include "QvtkProp.h"
#include "QvtkScene.h"

// qt
#include <QList>
#include <qdom.h>
#include <qdebug.h>

// vtk
#include <vtkMatrix4x4.h>
#include <vtkDataSet.h>
#include <vtkTrivialProducer.h>
#include <vtkTransform.h>
#include <vtkNew.h>
#include <vtkProp3D.h>

// std
#include <sstream>

namespace Q{
namespace vtk{


const QString DataSet::ARRAY_PREFIX[3]{"X", "Y", "Z"};
const QString DataSet::MATRIX_PREFIX("Matrix_");
const QString DataSet::PATH_SEPERATOR(";");
const QString DataSet::REFERENCE_PROPS_PREFIX("ReferenceProps_");

Q_VTK_DATA_CPP(DataSet);
DataSet::DataSet():
	origin{0, 0, 0},
	position{0, 0, 0}, 
	orientation{0, 0, 0}, 
	scale{1, 1, 1}
{

	this->additionalMatrix = nullptr;
	vtkMatrix4x4* additionalMatrix = vtkMatrix4x4::New();
	additionalMatrix->Identity();
	this->setAdditionalMatrix(additionalMatrix);
	additionalMatrix->Delete();


	this->userMatrix = nullptr;
	vtkMatrix4x4* userMatrix = vtkMatrix4x4::New();
	userMatrix->Identity();
	this->setUserMatrix(userMatrix);
	userMatrix->Delete();

	//this->transform = nullptr;
	//vtkTransform* transform = vtkTransform::New();
	//this->setTransform(transform);
	//transform->Delete();

	this->pickable = true;

	this->opacity = createAttribute(K.Opacity, static_cast<double>(1), true);
	this->insertSlotFunction(this->opacity, &DataSet::setOpacity);

	this->data = nullptr;
	this->portProducer = vtkTrivialProducer::New();

	this->referenceProps = new QList<Prop*>;
}

DataSet::~DataSet()
{
	for(QList<Prop*>::const_iterator cit = this->referenceProps->cbegin();
			cit != this->referenceProps->cend(); ++cit){
		removeReference(*cit);
	}

	if(this->portProducer){
		this->portProducer->Delete();
	}
	if(this->referenceProps){
		delete this->referenceProps;
	}
}

void DataSet::setOpacity(double opacity)
{
	setAttribute(this->opacity, opacity);
	emit  this->opacityChanged(opacity);
}

double DataSet::getOpacity() const
{
	return getAttribute(this->opacity).toDouble();
}

void DataSet::dataSetCoordinateToWorldCoordinate(const double dataSet[3], double world[3]) const
{
	double dataSet1[4]{ dataSet[0], dataSet[1], dataSet[2], 1 };
	double world1[4];
	vtkTransform* transform = vtkTransform::New();
	this->getTransform(transform);
	transform->MultiplyPoint(dataSet1, world1);
	transform->Delete();
	memcpy(world, world1, 3 * sizeof(const double));
}

void DataSet::worldCoordinateToDataSetCoordinate(const double world[3], double dataSet[3]) const
{

	double world1[4]{ world[0], world[1], world[2], 1 };
	double dataSet1[4];
	vtkTransform* transform = vtkTransform::New();
	this->getTransform(transform);
	transform->Inverse();
	transform->MultiplyPoint(world1, dataSet1);
	transform->Delete();
	memcpy(dataSet, dataSet1, 3 * sizeof(const double));
}

void DataSet::dataSetRegionToWorldRegion(const double dataSet[6], double world[6]) const
{
	double dataSet1[4] = { dataSet[0], dataSet[2], dataSet[4], 1 };
	double dataSet2[4] = { dataSet[1], dataSet[3], dataSet[5], 1 };
	double world1[4];
	double world2[4];
	vtkTransform* transform = vtkTransform::New();
	this->getTransform(transform);
	transform->MultiplyPoint(dataSet1, world1);
	transform->MultiplyPoint(dataSet2, world2);
	transform->Delete();

	world[0] = world1[0];
	world[1] = world2[0];
	world[2] = world1[1];
	world[3] = world2[1];
	world[4] = world1[2];
	world[5] = world2[2];

}

void DataSet::worldRegionToDataSetRegion(const double world[6], double dataSet[6]) const
{
	double world1[4] = { world[0], world[2], world[4], 1 };
	double world2[4] = { world[1], world[3], world[5], 1 };
	double dataSet1[4];
	double dataSet2[4];

	vtkTransform* transform = vtkTransform::New();
	this->getTransform(transform);
	transform->Inverse();
	transform->MultiplyPoint(world1, dataSet1);
	transform->MultiplyPoint(world2, dataSet2);
	transform->Delete();

	dataSet[0] = dataSet1[0];
	dataSet[1] = dataSet2[0];
	dataSet[2] = dataSet1[1];
	dataSet[3] = dataSet2[1];
	dataSet[4] = dataSet1[2];
	dataSet[5] = dataSet2[2];
}


void DataSet::readXML(const QDomElement & xml, QString directoryPath)
{
	Data::readXML(xml);
	setRelativePath(xml.attribute(K.RelativePath).split(PATH_SEPERATOR));
	setAbsolutePath(xml.attribute(K.AbsolutePath).split(PATH_SEPERATOR));
	
	readData(directoryPath);

	readArray(K.Origin, ARRAY_PREFIX, xml, this->origin, 3);
	readArray(K.Position, ARRAY_PREFIX, xml, this->position, 3);
	readArray(K.Orientation, ARRAY_PREFIX, xml, this->orientation, 3);
	readArray(K.Scale, ARRAY_PREFIX, xml, this->scale, 3);

	vtkMatrix4x4* additionalMatrix = vtkMatrix4x4::New();
	readMatrix(K.AdditionalMatrix, xml, additionalMatrix);
	setAdditionalMatrix(additionalMatrix);
	additionalMatrix->Delete();


	vtkMatrix4x4* userMatrix = vtkMatrix4x4::New();
	readMatrix(K.UserMatrix, xml, userMatrix);
	setUserMatrix(userMatrix);
	userMatrix->Delete();

	setPickable(static_cast<bool>(xml.attribute(K.Pickable).toInt()));


	readProp(xml);
}

void DataSet::writeXML(QDomElement & xml, QString directoryPath) const
{
	Data::writeXML(xml, directoryPath);
	xml.setAttribute(K.RelativePath, getRelativePath().join(PATH_SEPERATOR));
	xml.setAttribute(K.AbsolutePath, getAbsolutePath().join(PATH_SEPERATOR));

	writeData(directoryPath);
	
	//QDomDocument dom = xml.ownerDocument();
	writeArray(K.Origin, ARRAY_PREFIX, xml, getOrigin(), 3);
	writeArray(K.Position, ARRAY_PREFIX, xml, getPosition(), 3);
	writeArray(K.Orientation, ARRAY_PREFIX, xml, getOrientation(), 3);
	writeArray(K.Scale, ARRAY_PREFIX, xml, getScale(), 3);
	
	writeMatrix(K.AdditionalMatrix, xml, getAdditionalMatrix());
	
	writeMatrix(K.UserMatrix, xml, getUserMatrix());

	xml.setAttribute(K.Pickable, getPickable());

	writeProp(xml);
}

//bool DataSet::isClass(QString className) const
//{
//	if(getClassName() != className){
//		return Data::isClass(className);
//	}
//	return true;
//}

void DataSet::readProp(const QDomElement& xml)
{
	QDomElement element = xml.firstChildElement(K.ReferenceProps);


	int index = 0;
	QString uniqueName = element.attribute(REFERENCE_PROPS_PREFIX + QString::number(index));
	while(!uniqueName.isEmpty()){
		Prop* prop = qobject_cast<Prop*>(Scene::getCurrentScene()->getDataByUniqueName(uniqueName));
		uniqueName = element.attribute(REFERENCE_PROPS_PREFIX + QString::number(++index));
		if(!prop){
			//qDebug() << "Reference prop is not created yet.";
			//qDebug() << "Waiting prop to set data";
			//qDebug() << uniqueName;
			continue;
		}
		addReference(prop);
	}

}

void DataSet::writeProp(QDomElement& xml) const
{
	QDomDocument dom = xml.ownerDocument();
	QDomElement element = dom.createElement(K.ReferenceProps);
	xml.appendChild(element);

	QList<Prop*>::const_iterator cit = this->referenceProps->cbegin();
	int index = 0;
	while(cit != this->referenceProps->cend()){
		element.setAttribute(REFERENCE_PROPS_PREFIX + QString::number(index), (*cit)->getUniqueName());

		++cit;
		++index;
	}
}

void DataSet::setPosition(double x, double y, double z)
{
	this->position[0] = x;
	this->position[1] = y;
	this->position[2] = z;

	//foreach(Prop* prop, *referenceProps)
	//{
	//	prop->getProp()->SetPosition(
	//		this->position[0],
	//		this->position[1],
	//		this->position[2]
	//	);
	//}
	emit  this->positionChanged(this->position);
	emit  this->positionChanged(this->getPosition());
}

void DataSet::setOrigin(double x, double y, double z)
{
	this->origin[0] = x;
	this->origin[1] = y;
	this->origin[2] = z;
	//foreach(Prop* prop, *referenceProps)
	//{
	//	prop->getProp()->SetOrigin(
	//		this->origin[0],
	//		this->origin[1],
	//		this->origin[2]
	//	);
	//}
	emit  this->originChanged(this->origin);
	emit  this->originChanged(this->getOrigin());
}


void DataSet::setScale(double x, double y, double z)
{
	this->scale[0] = x;
	this->scale[1] = y;
	this->scale[2] = z;
	//foreach(Prop* prop, *referenceProps)
	//{
	//	prop->getProp()->SetScale(
	//		this->scale[0],
	//		this->scale[1],
	//		this->scale[2]
	//	);
	//}
	emit  this->scaleChanged(this->scale);
	emit this->scaleChanged(this->getScale());
}

void DataSet::setOrientation(double x, double y, double z)
{
	this->orientation[0] = x;
	this->orientation[1] = y;
	this->orientation[2] = z;

	//foreach(Prop* prop, *referenceProps)
	//{
	//	prop->getProp()->setOrientation(
	//		this->orientation[0],
	//		this->orientation[1],
	//		this->orientation[2]
	//	);
	//}
	emit  this->orientationChanged(this->orientation);
	emit this->orientationChanged(this->getOrientation());
}

void DataSet::setDataSet(vtkDataSet * data)
{

//	if(this->data)
//	{
//		this->data->Delete();
//	}
	this->data = data;
	this->portProducer->SetOutput(data);
}

void DataSet::getTransform(vtkTransform* transform) const
{
	if (transform == nullptr) {
		qCritical() << "input transform is a nullptr.";
	}

	transform->Identity();
	transform->PostMultiply();
	transform->Translate(
		-this->origin[0],
		-this->origin[1],
		-this->origin[2]);
	transform->Scale(this->scale);
	transform->RotateY(this->orientation[1]);
	transform->RotateX(this->orientation[0]);
	transform->RotateZ(this->orientation[2]);

	transform->Translate(this->position);
	transform->Translate(this->origin);

	transform->Concatenate(this->getUserMatrix());
	//transform->Concatenate(this->getAdditionalMatrix());
}

vtkDataSet * DataSet::getDataSet() const
{
	return this->data;
}

vtkAlgorithmOutput * DataSet::getOutputPort() const
{
	return portProducer->GetOutputPort();
}


void DataSet::getWorldBounds(double bounds[6]) const
{
	dataSetRegionToWorldRegion(this->getDataSet()->GetBounds(), bounds);
}

void DataSet::writeMatrix(QString key, QDomElement & element, const vtkMatrix4x4 * martrix)
{
	QDomDocument dom = element.ownerDocument();
	QDomElement matrix = dom.createElement(key);
	element.appendChild(matrix);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			matrix.setAttribute(MATRIX_PREFIX + QString::number(i) + QString::number(j), martrix->GetElement(i, j));
		}
	}
}

void DataSet::readMatrix(QString key, const QDomElement & element, vtkMatrix4x4 * martrix)
{
	QDomElement matrixElement = element.firstChildElement(key);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			double value = matrixElement.attribute(MATRIX_PREFIX + QString::number(i) + QString::number(j)).toDouble();
			martrix->SetElement(i, j, value);
		}
	}
}

void DataSet::setOpacity(Data * self, QStandardItem * item)
{
	DataSet* _self = static_cast<DataSet*>(self);
	_self->setOpacity(getAttribute(item).toDouble());
}

void DataSet::updateUniqueReferences(QString newName, QString oldName, QDomElement & xml) const
{
	Data::updateUniqueReferences(newName, oldName, xml);

	QDomElement element = xml.firstChildElement(K.ReferenceProps);
	int index = 0;
	QString uniqueName = element.attribute(REFERENCE_PROPS_PREFIX + QString::number(index));
	while (!uniqueName.isEmpty()) {
		if (uniqueName == oldName) {
			element.setAttribute(REFERENCE_PROPS_PREFIX + QString::number(index), newName);
		}
		uniqueName = element.attribute(REFERENCE_PROPS_PREFIX + QString::number(++index));
	}
}

void DataSet::setAdditionalMatrix(vtkMatrix4x4* additionalMatrix)
{
	if(this->additionalMatrix == additionalMatrix){
		return;
	}
	if(this->additionalMatrix)
	{
		this->additionalMatrix->UnRegister(this->portProducer);
	}
	this->additionalMatrix = additionalMatrix;
	if (this->additionalMatrix)
	{
		this->additionalMatrix->Register(this->portProducer);
	}
}

void DataSet::setUserMatrix(vtkMatrix4x4* userMatrix)
{
	if (this->userMatrix == userMatrix) {
		return;
	}
	if(this->userMatrix)
	{
		this->userMatrix->UnRegister(this->portProducer);
	}
	this->userMatrix = userMatrix;
	if (this->userMatrix) 
	{
		this->userMatrix->Register(this->portProducer);

	}
}

void DataSet::setPickable(bool flag)
{
	this->pickable = flag;
	emit  this->pickableChanged(flag);
	foreach(Prop* prop, *referenceProps)
	{
		prop->getProp()->SetPickable(this->pickable);
	}
}

void DataSet::printSelf() const
{
	Data::printSelf();

	std::stringstream ss;

	qDebug() << "additionalMatrix";
	ss.clear();
	this->additionalMatrix->Print(ss);
	qDebug() << QString::fromStdString(ss.str());

	qDebug() << "userlMatrix";
	ss.clear();
	this->userMatrix->Print(ss);
	qDebug() << QString::fromStdString(ss.str());

	qDebug() << "pickable" << '=' << this->pickable;

	qDebug() << "relativePath" << '=' << this->relativePath;
	qDebug() << this->absolutePath << '=' << this->absolutePath;


	qDebug() << "data";
	ss.clear();
	this->data->Print(ss);
	qDebug() << QString::fromStdString(ss.str());

	qDebug() << "portProducer";
	ss.clear();
	this->portProducer->Print(ss);
	qDebug() << QString::fromStdString(ss.str());


}

void DataSet::addReference(Prop* prop)
{
	this->addReference(prop, true);
}

void vtk::DataSet::addReference(Prop * prop, bool propSetRenderDataset = true)
{
	if (!prop) {
		qCritical() << "input is a nullptr";
		return;
	}

	// repeatedly add reference and do nothing.
	if (this->referenceProps->contains(prop)) {
		return;
	}

	this->referenceProps->append(prop);

	if (propSetRenderDataset)
		prop->setRenderDataSet(this);
}

void DataSet::removeReference(Prop* prop)
{
	this->removeReference(prop,true);
}

void vtk::DataSet::removeReference(Prop * prop, bool propSetRenderDataset = true)
{
	if (!prop) {
		qCritical() << "input is a nullptr";
		return;
	}
	//qDebug() << prop->getClassName();
	// repeated remove reference and do nothing.
	if (!this->referenceProps->contains(prop)) {
		return;
	}

	this->referenceProps->removeOne(prop);
	if (propSetRenderDataset)
		prop->setRenderDataSet(nullptr);
}

void DataSet::reset()
{
	Data::reset();
	this->setOrigin(0, 0, 0);
	this->setPosition(0, 0, 0);
	this->setPosition(0, 0, 0);
	this->setOrientation(0, 0, 0);
	this->setScale(1, 1, 1);

	this->userMatrix->Identity();
	this->additionalMatrix->Identity();

	this->setOpacity(1);

}
}
}
