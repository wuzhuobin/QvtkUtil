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

class ABSTRACTDATA_EXPORT QvtkPolyDataSource : public QvtkAnnotationPolyData
{
	Q_OBJECT;
	Q_VTK_DATAH(
		QvtkPolyDataSource,
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
	
	QvtkPolyDataSource();
	virtual ~QvtkPolyDataSource() override;
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
	void setSourceTypeToArrowSource() { this->setSourceType(QvtkPolyDataSource::ARROW_SOURCE); }
	void setSourceTypeToConeSource() { this->setSourceType(QvtkPolyDataSource::CONE_SOURCE); }
	void setSourceTypeToCubeSource() { this->setSourceType(QvtkPolyDataSource::CUBE_SOURCE); }
	void setSourceTypeToCylinderSource() { this->setSourceType(QvtkPolyDataSource::CYLINDER_SOURCE); }
	void setSourceTypeToDiskSource() { this->setSourceType(QvtkPolyDataSource::DISK_SOURCE); }
	void setSourceTypeToLineSource() { this->setSourceType(QvtkPolyDataSource::LINE_SOURCE); }
	void setSourceTypeToRegularPolygonSourceSource() { this->setSourceType(QvtkPolyDataSource::REGULAR_POLYGON_SOURCE); }
	void setSourceTypeToSphereSource() { this->setSourceType(QvtkPolyDataSource::SPHERE_SOURCE); }

protected:

	
	virtual Data* newInstance() const override { return new QvtkPolyDataSource; }

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

#endif // !__Qvtk_POLY_DATA_SOURCE_H__
