#ifndef __Qvtk_POLY_DATA_H__
#define __Qvtk_POLY_DATA_H__

// me 
#include "QvtkDataSet.h"

//vtk
#include <vtkTransformPolyDataFilter.h>
class vtkPolyData;

namespace Q {
namespace vtk{

class QVTKDATA_EXPORT PolyData: public DataSet
{
	Q_OBJECT;
	Q_VTK_DATAH(
		PolyData, 
		Q_VTK_KEY(Color)
	)
public:
	static const QString COLOR_RGB[3];
	enum POLY_DATA_SUFFIX
	{
		UNKNOWN = 0,
		VTK = 1,
		VTP = 2,
		STL = 3,
		OBJ = 4
	};

	PolyData();
	virtual ~PolyData() override;
	virtual void printSelf() const override;

	virtual void readXML(const QDomElement& element, QString rootDirectory = QString()) override;
	virtual void writeXML(QDomElement& element, QString rootDirectory = QString()) const override;

	virtual bool readData(QString rootDirectory = QString()) override;
	virtual bool writeData(QString rootDirectory = QString()) const override;

	virtual void getColor(double color[3]) const;

	virtual vtkAlgorithmOutput* getTransformOutputPort() const;

	virtual vtkPolyData* getPolyData() const;

	virtual vtkPolyData* getTransformPolyData() const;
	static bool readDataSuffix(QString fileName, vtkPolyData* data, int suffix = -1);

	static bool readQRC(QString fileName, vtkPolyData * data, int suffix = -1);

	static bool writeDataSuffix(QString fileName, vtkPolyData* data, int suffix = -1);


public slots:

	virtual void reset() override;

	virtual void setColor(double r, double g, double b)
	{
		double color[3]{ r,g,b };
		setColor(color);
	}
	virtual void setColor(const double color[3]);


signals:
	
	void colorChanged(const double rgb[3]);

protected:
	
	virtual Data* newInstance() const override { return new PolyData; }

	static void setColor(Data* self, QStandardItem* item);

	static unsigned int suffixTranslate(QString suffix);

	vtkTransformPolyDataFilter* transformPolyDataFilter;

private:

	QList<QStandardItem*> color;


};

}
}

#endif // !__Qvtk_POLY_DATA_H__

