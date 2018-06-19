#ifndef __QVTK_PROP_H__
#define __QVTK_PROP_H__
#pragma once
// me
#include "QvtkData.h"
namespace Q {
namespace vtk{
class DataSet;
class MatrixCallback;
class PickedCallback;
}
}
// vtk
class vtkProp3D;
class vtkMatrix4x4;
namespace Q {
namespace vtk {
class QVTKDATA_EXPORT Prop: public Data
{
	Q_OBJECT;
	Q_VTK_DATA_H(
		Prop, 
		Q_VTK_KEY(DisplayRegionX)
		Q_VTK_KEY(DisplayRegionY)
		Q_VTK_KEY(DisplayRegionZ)
		Q_VTK_KEY(RenderDataSet)
		Q_VTK_KEY(Viewer_ID)
		Q_VTK_KEY(Renderer_ID)
	);
public:
	static const QString DISPLAY_REGION_PREFIX[2];
	Prop();
	virtual ~Prop() override;
	virtual void printSelf() const override;
	virtual void readXML(const QDomElement& xml, QString directoryPath = QString()) override;
	virtual void writeXML(QDomElement& xml, QString directoryPath = QString()) const override;
	virtual void readDataSet(const QDomElement& xml);
	virtual void writeDataSet(QDomElement& xml) const;
	virtual void dataSetCoordinateToWorldCoordinate(const double dataSet[3], double world[3]) const;
	virtual void worldCoordinateToDataSetCoordinate(const double world[3], double dataSet[3]) const;
	virtual void dataSetRegionToWorldRegion(const double dataSet[6], double world[6]) const;
	virtual void worldRegionToDataSetRegion(const double world[6], double dataSet[6]) const;
	virtual void getDisplayRegion(double region[6]) const;
	virtual bool getRendering() const { return this->rendering; }
	virtual long long getViewerID() const { return this->viewerID; }
	virtual int getLayer() const { return this->rendererID; }
	virtual vtkProp3D* getProp() const;
	DataSet* getRenderDataSet() { return this->renderDataSet; }
public Q_SLOTS:
	virtual void reset() override;
	virtual void setDisplayRegion(const double region[6]);
	virtual void resetDisplayRegion();
	virtual void setRendering(bool flag) { this->rendering = flag; }
	void RenderingOn() { setRendering(true); }
	void RenderingOff() { setRendering(false); }
	virtual void setViewerID(long long id) { this->viewerID = id; }
	virtual void setRendererID(int id) { this->rendererID = id; }
	virtual void setRenderDataSet(DataSet* data = nullptr);
	virtual void setUserMatrix(vtkMatrix4x4 * matrix);
	virtual void setOpacity(double opacit) = 0;
	virtual void setOrigin(const double origin[3]);
	virtual void setPosition(const double position[3]);
	virtual void setOrientation(const double orientation[3]);
	virtual void setScale(const double scale[3]);
	virtual void propMatrixUpdate();
signals:
	void picked();
protected:
	virtual void updateUniqueReferences(QString newName, QString oldName, QDomElement& xml) const override;
	virtual void setProp(vtkProp3D* prop);
	static void setDisplayRegion(Data* self, QStandardItem* item);
private:
	vtkProp3D* prop3D;
	vtkMatrix4x4* cachedInverstedMatrix;
	QList<QStandardItem*> displayRegionX;
	QList<QStandardItem*> displayRegionY;
	QList<QStandardItem*> displayRegionZ;
	bool rendering;
	long long viewerID;
	int rendererID; ///< here is just the id of renderer
	DataSet* renderDataSet;
	MatrixCallback* matrixCallback;
	PickedCallback* pickedCallback;
};

}
}

#endif // !__QVTK_PROP_H__
