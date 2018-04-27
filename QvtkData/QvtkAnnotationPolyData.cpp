#include "QvtkAnnotationPolyData.h"

// qt
#include <QDebug>
namespace Q{
namespace vtk{
Q_VTK_DATA_CPP(AnnotationPolyData);
AnnotationPolyData::AnnotationPolyData()
{
	QStandardItem* hoverItem = createAttribute(K.HoverColor);
	QStandardItem* selectedItem = createAttribute(K.SelectedColor);

	QStringList hoverKeys;
	QVariantList hoverValues;
	QStringList selectedKeys;
	QVariantList selectedValues;

	for (int i = 0; i < 3; i++)
	{
		hoverKeys << (COLOR_RGB[i]);
		hoverValues << static_cast<double>(0);
		selectedKeys << (COLOR_RGB[i]);
		selectedValues << static_cast<double>(0);
	}

	this->hoverColor = createAttributesByColumns(hoverKeys, hoverValues, true, hoverItem);
	this->selectedColor = createAttributesByColumns(selectedKeys, selectedValues, true, selectedItem);


	for (int i = 0; i< 3; i++)
	{
		insertSlotFunction(this->hoverColor[i], &AnnotationPolyData::setHoverColor);
		insertSlotFunction(this->selectedColor[i], &AnnotationPolyData::setSelectedColor);
	}

	this->annotationStatus = createAttribute(K.AnnotationStatus, static_cast<unsigned int>(0), true);
	insertSlotFunction(this->annotationStatus, &AnnotationPolyData::setAnnotatoinStatus);
}

AnnotationPolyData::~AnnotationPolyData()
{
}

void AnnotationPolyData::printSelf() const
{
	PolyData::printSelf();
	for (int i = 0; i < this->hoverColor.size(); ++i) {
		qDebug() << "hoverColor" << i << '=' << getAttribute(this->hoverColor[i]);
	}

	for (int i = 0; i < this->selectedColor.size(); ++i) {
		qDebug() << "selectedColor" << i << '=' << getAttribute(this->selectedColor[i]);
	}
}

void AnnotationPolyData::reset()
{
	PolyData::reset();
	setHoverColor(0, 0, 0);
	setSelectedColor(0, 0, 0);
}

void AnnotationPolyData::setHoverColor(const double hoverColor[3])
{
	setAttributes(this->hoverColor, QVariantList() << hoverColor[0]
		<< hoverColor[1] << hoverColor[2]);
	//emit hoverColorChanged(hoverColor);
}

unsigned int AnnotationPolyData::getAnnotationStatus() const
{
	return getAttribute(this->annotationStatus).toUInt();
}

void AnnotationPolyData::getHoverColor(double hoverColor[3]) const
{
	QVariantList list = getAttributes(this->hoverColor);
	for (int i = 0; i < 3; ++i)
	{
		hoverColor[i] = list[i].toDouble();
	}
}

void AnnotationPolyData::setSelectedColor(const double selectedColor[3])
{
	setAttributes(this->selectedColor, QVariantList() << selectedColor[0] <<
		selectedColor[1] << selectedColor[2]);
	//emit selectedColorChanged(selectedColor);
}

void AnnotationPolyData::getSelectedColor(double selectedColor[3]) const
{
	QVariantList list = getAttributes(this->selectedColor);
	for (int i = 0; i < 3; ++i)
	{
		selectedColor[i] = list[i].toDouble();
	}
}

void AnnotationPolyData::setAnnotationStatus(unsigned int i)
{
	setAttribute(this->annotationStatus, i);
	
	ENUM_ANNOTATION_STATUS _annotationStatus = static_cast<ENUM_ANNOTATION_STATUS>(i);
	double color[3];
	switch (_annotationStatus)
	{
	case AnnotationPolyData::HOVER:
		this->getHoverColor(color);
		break;
	case AnnotationPolyData::SELECTED:
		this->getSelectedColor(color);
		break;
	case AnnotationPolyData::NONE:
		this->getColor(color);
	default:
		break;
	}

	emit colorChanged(color);
}

void AnnotationPolyData::setAnnotatoinStatus(Data * self, QStandardItem * item)
{
	AnnotationPolyData* _self = static_cast<AnnotationPolyData*>(self);
	_self->setAnnotationStatus(getAttribute(item).toUInt());
}

void AnnotationPolyData::setHoverColor(Data * self, QStandardItem * item)
{
	AnnotationPolyData* _self = static_cast<AnnotationPolyData*>(self);
	double color[3];
	_self->getHoverColor(color);
	_self->setHoverColor(color);
}

void AnnotationPolyData::setSelectedColor(Data * self, QStandardItem * item)
{
	AnnotationPolyData* _self = static_cast<AnnotationPolyData*>(self);
	double color[3];
	_self->getSelectedColor(color);
	_self->setSelectedColor(color);
}

}
}