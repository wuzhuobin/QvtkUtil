#ifndef __Qvtk_ANNOTATION_POLY_DATA_H__
#define __Qvtk_ANNOTATION_POLY_DATA_H__

#include "QvtkPolyData.h"

class ABSTRACTDATA_EXPORT QvtkAnnotationPolyData : public QvtkPolyData
{
	Q_OBJECT;
	Q_VTK_DATAH(
		QvtkAnnotationPolyData, 
		Q_VTK_KEY(AnnotationStatus)
		Q_VTK_KEY(HoverColor)
		Q_VTK_KEY(SelectedColor)
		)
public:

	enum ENUM_ANNOTATION_STATUS
	{
		NONE = 0,
		HOVER = 1,
		SELECTED = 2
	};

	QvtkAnnotationPolyData();
	virtual ~QvtkAnnotationPolyData() override;
	
	virtual void printSelf() const override;

	virtual unsigned int getAnnotationStatus() const;

	virtual void getHoverColor(double hoverColor[3]) const;

	virtual void getSelectedColor(double selectedColor[3]) const;

public slots:
	virtual void reset() override;

	virtual void setAnnotationStatusToNone() { setAnnotationStatus(NONE); }
	virtual void setAnnotationStatusToHover() { setAnnotationStatus(HOVER); }
	virtual void setAnnotationStatusToSelected() { setAnnotationStatus(SELECTED); }
	virtual void setAnnotationStatus(unsigned int i);

	virtual void setHoverColor(double r, double g, double b)
	{
		double hoverColor[3]{ r,g,b };
		setHoverColor(hoverColor);
	}
	virtual void setHoverColor(const double hoverColor[3]);

	virtual void setSelectedColor(double r, double g, double b)
	{
		double selectedColor[3]{ r,g,b };
		setSelectedColor(selectedColor);
	}
	virtual void setSelectedColor(const double selectedColor[3]);


signals:
	//void hoverColorChanged(const double rgb[3]);
	//void selectedColorChanged(const double rgb[3]);

protected:

	virtual Data* newInstance() { return new QvtkAnnotationPolyData; }

	static void setAnnotatoinStatus(Data* self, QStandardItem* item);
	static void setHoverColor(Data* self, QStandardItem* item);
	static void setSelectedColor(Data* self, QStandardItem* item);

private:
	QStandardItem* annotationStatus;
	QList<QStandardItem*> hoverColor;
	QList<QStandardItem*> selectedColor;

};


#endif // !__Qvtk_ANNOTATION_POLY_DATA_H__
