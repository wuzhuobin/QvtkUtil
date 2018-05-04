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
#include <vtkPointData.h>

// qt
#include <QDebug>
#include <QCoreApplication>
namespace Q {
namespace vtk {

Q_VTK_DATA_CPP(PolyDataSource);
PolyDataSource::PolyDataSource()
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
	insertSlotFunction(this->sourceType, &PolyDataSource::setSourceType);
}

PolyDataSource::~PolyDataSource()
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

void PolyDataSource::printSelf() const
{
	AnnotationPolyData::printSelf();
}

bool PolyDataSource::readData(QString rootDirectory)
{
	// if it has root directory, read from polydata sources.
	if (rootDirectory.isEmpty()) {
		return AnnotationPolyData::readData(rootDirectory);
	}
	else {
		vtkPolyDataAlgorithm* source = nullptr;
		ENUM_SOURCE_TYPE _sourceType = static_cast<ENUM_SOURCE_TYPE>(getAttribute(this->sourceType).toUInt());
		switch (_sourceType)
		{
		case PolyDataSource::ARROW_SOURCE:
			source = this->arrowSource;
			break;
		case PolyDataSource::CONE_SOURCE:
			source = this->coneSource;
			break;
		case PolyDataSource::CUBE_SOURCE:
			source = this->cubeSource;
			break;
		case PolyDataSource::CYLINDER_SOURCE:
			source = this->cylinderSource;
			break;
		case PolyDataSource::DISK_SOURCE:
			source = this->diskSource;
			break;
		case PolyDataSource::LINE_SOURCE:
			source = this->lineSource;
			break;
		case PolyDataSource::REGULAR_POLYGON_SOURCE:
			source = this->regularPolygonSource;
			break;
		case PolyDataSource::SPHERE_SOURCE:
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
}

unsigned int PolyDataSource::getSourceType() const
{
	return getAttribute(this->sourceType).toUInt();
}

void PolyDataSource::reset()
{
	AnnotationPolyData::reset();
	setSourceType(0);
}

void PolyDataSource::setSourceType(unsigned int i)
{
	setAttribute(this->sourceType, i);
	readData("...");
}

void PolyDataSource::setSourceType(Data * self, QStandardItem * item)
{
	PolyDataSource* _self = static_cast<PolyDataSource*>(self);
	_self->setSourceType(getAttribute(item).toUInt());

}
}
}
