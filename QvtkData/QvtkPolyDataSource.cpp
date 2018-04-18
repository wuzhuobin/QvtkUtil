#include "QvtkPolyDataSource.h"

// vtk
#include <vtkSphereSource.h>
#include <vtkArrowSource.h>
#include <vtkConeSource.h>
#include <vtkCubeSource.h>
#include <vtkCylinderSource.h>
#include <vtkDiskSource.h>
#include <vtkLineSource.h>
#include <vtkRegularPolygonSource.h>
#include <vtkPassArrays.h>
//#include <vtkNew.h>
#include <vtkPointData.h>

// qt
#include <QDebug>
Q_VTK_DATACPP(QvtkPolyDataSource);
QvtkPolyDataSource::QvtkPolyDataSource()
{
	this->sphereSource = vtkSphereSource::New();
	this->arrowSource = vtkArrowSource::New();
	this->coneSource = vtkConeSource::New();
	this->cubeSource = vtkCubeSource::New();
	this->cylinderSource = vtkCylinderSource::New();
	this->diskSource = vtkDiskSource::New();
	this->lineSource = vtkLineSource::New();
	this->regularPolygonSource = vtkRegularPolygonSource::New();
	this->passArrays = vtkPassArrays::New();
	this->passArrays->UseFieldTypesOn();
	this->passArrays->AddFieldType(vtkDataObject::POINT);
	//this->passArrays->AddFieldType(vtkDataObject::CELL);
	//this->passArrays->AddFieldType(vtkDataObject::FIELD);

	this->sourceType = createAttribute(K.SourceType, static_cast<unsigned int>(0), true);
	insertSlotFunction(this->sourceType, &QvtkPolyDataSource::setSourceType);
}

QvtkPolyDataSource::~QvtkPolyDataSource()
{
	this->sphereSource->Delete();
	this->arrowSource->Delete();
	this->coneSource->Delete();
	this->cubeSource->Delete();
	this->cylinderSource->Delete();
	this->diskSource->Delete();
	this->lineSource->Delete();
	this->regularPolygonSource->Delete();
	this->passArrays->Delete();
}

void QvtkPolyDataSource::printSelf() const
{
	QvtkAnnotationPolyData::printSelf();
}

bool QvtkPolyDataSource::readData(QString rootDirectory)
{
	// if no root directory, read from polydata sources.
	if (rootDirectory.isEmpty()) {
		vtkPolyDataAlgorithm* source = nullptr;
		ENUM_SOURCE_TYPE _sourceType = static_cast<ENUM_SOURCE_TYPE>(getAttribute(this->sourceType).toUInt());
		switch (_sourceType)
		{
		case QvtkPolyDataSource::ARROW_SOURCE:
			source = this->arrowSource;
			break;
		case QvtkPolyDataSource::CONE_SOURCE:
			source = this->coneSource;
			break;
		case QvtkPolyDataSource::CUBE_SOURCE:
			source = this->cubeSource;
			break;
		case QvtkPolyDataSource::CYLINDER_SOURCE:
			source = this->cylinderSource;
			break;
		case QvtkPolyDataSource::DISK_SOURCE:
			source = this->diskSource;
			break;
		case QvtkPolyDataSource::LINE_SOURCE:
			source = this->lineSource;
			break;
		case QvtkPolyDataSource::REGULAR_POLYGON_SOURCE:
			source = this->regularPolygonSource;
			break;
		case QvtkPolyDataSource::SPHERE_SOURCE:
			source = this->sphereSource;
			break;
		default:
			break;
		}
		this->passArrays->SetInputConnection(source->GetOutputPort());
		this->passArrays->Update();
		this->getPolyData()->ShallowCopy(this->passArrays->GetOutput());
		return true;
	}
	else {
		return QvtkAnnotationPolyData::readData(rootDirectory);
	}
}

unsigned int QvtkPolyDataSource::getSourceType() const
{
	return getAttribute(this->sourceType).toUInt();
}

void QvtkPolyDataSource::reset()
{
	QvtkAnnotationPolyData::reset();
	setSourceType(0);
}

void QvtkPolyDataSource::setSourceType(unsigned int i)
{
	setAttribute(this->sourceType, i);
	readData();
}

void QvtkPolyDataSource::setSourceType(Data * self, QStandardItem * item)
{
	QvtkPolyDataSource* _self = static_cast<QvtkPolyDataSource*>(self);
	_self->setSourceType(getAttribute(item).toUInt());

}
