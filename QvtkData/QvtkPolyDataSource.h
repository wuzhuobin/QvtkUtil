#ifndef __Qvtk_POLY_DATA_SOURCE_H__
#define __Qvtk_POLY_DATA_SOURCE_H__

// me
#include "QvtkAnnotationPolyData.h"

// vtk
class vtkSphereSource;
class vtkArrowSource;
class vtkConeSource;
class vtkCubeSource;
class vtkCylinderSource;
class vtkDiskSource;
class vtkLineSource;
class vtkRegularPolygonSource;
class vtkParametricEllipsoid;
class vtkPassArrays;

namespace Q {
namespace vtk {


class QVTKDATA_EXPORT PolyDataSource : public AnnotationPolyData
{
	Q_OBJECT;
	Q_VTK_DATA_H(
		PolyDataSource,
		Q_VTK_KEY(SourceType)
	);
public: 

	enum ENUM_SOURCE_TYPE
	{
		ARROW_SOURCE = 0,
		CONE_SOURCE = 1,
		CUBE_SOURCE = 2,
		CYLINDER_SOURCE = 3,
		DISK_SOURCE = 4,
		LINE_SOURCE = 5, 
		REGULAR_POLYGON_SOURCE = 6,
		SPHERE_SOURCE = 7
	};
	
	PolyDataSource();
	virtual ~PolyDataSource() override;
	virtual void printSelf() const override;

	virtual bool readData(QString rootDirectory = QString()) override;

	virtual unsigned int getSourceType() const;

	virtual vtkArrowSource* getArrorwSource() { return this->arrowSource; }
	virtual vtkConeSource* getConeSource() { return this->coneSource; }
	virtual vtkCubeSource* getCubeSource() { return this->cubeSource; }
	virtual vtkCylinderSource* getCylinderSource() { return this->cylinderSource; }
	virtual vtkDiskSource* getDiskSource() { return this->diskSource; }
	virtual vtkLineSource* getLineSource() { return this->lineSource; }
	virtual vtkRegularPolygonSource* getRegularPolygonSource() { return this->regularPolygonSource; }
	virtual vtkSphereSource* getSphereSource() { return this->sphereSource; }

public slots:

	virtual void reset() override;
	virtual void setSourceType(unsigned int i);
	void setSourceTypeToArrowSource() { this->setSourceType(PolyDataSource::ARROW_SOURCE); }
	void setSourceTypeToConeSource() { this->setSourceType(PolyDataSource::CONE_SOURCE); }
	void setSourceTypeToCubeSource() { this->setSourceType(PolyDataSource::CUBE_SOURCE); }
	void setSourceTypeToCylinderSource() { this->setSourceType(PolyDataSource::CYLINDER_SOURCE); }
	void setSourceTypeToDiskSource() { this->setSourceType(PolyDataSource::DISK_SOURCE); }
	void setSourceTypeToLineSource() { this->setSourceType(PolyDataSource::LINE_SOURCE); }
	void setSourceTypeToRegularPolygonSourceSource() { this->setSourceType(PolyDataSource::REGULAR_POLYGON_SOURCE); }
	void setSourceTypeToSphereSource() { this->setSourceType(PolyDataSource::SPHERE_SOURCE); }

protected:

	
	virtual Data* newInstance() const override { return new PolyDataSource; }

	static void setSourceType(Data* self, QStandardItem* item);

	vtkArrowSource* arrowSource;
	vtkConeSource* coneSource;
	vtkCubeSource* cubeSource;
	vtkCylinderSource* cylinderSource;
	vtkDiskSource* diskSource;
	vtkLineSource* lineSource;
	vtkRegularPolygonSource* regularPolygonSource;
	vtkSphereSource* sphereSource;
	vtkPassArrays* passArrays;

private:

	QStandardItem* sourceType;
};
}
}
#endif // !__Qvtk_POLY_DATA_SOURCE_H__
