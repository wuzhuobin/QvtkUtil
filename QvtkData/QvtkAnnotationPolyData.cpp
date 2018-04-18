#include "QvtkAnnotationPolyData.h"

// qt
#include <QDebug>

Q_VTK_DATACPP(QvtkAnnotationPolyData);
QvtkAnnotationPolyData::QvtkAnnotationPolyData()
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
		insertSlotFunction(this->hoverColor[i], &QvtkAnnotationPolyData::setHoverColor);
		insertSlotFunction(this->selectedColor[i], &QvtkAnnotationPolyData::setSelectedColor);
	}

	this->annotationStatus = createAttribute(K.AnnotationStatus, static_cast<unsigned int>(0), true);
	insertSlotFunction(this->annotationStatus, &QvtkAnnotationPolyData::setAnnotatoinStatus);
}

QvtkAnnotationPolyData::~QvtkAnnotationPolyData()
{
}

void QvtkAnnotationPolyData::printSelf() const
{
	QvtkPolyData::printSelf();
	for (int i = 0; i < this->hoverColor.size(); ++i) {
		qDebug() << "hoverColor" << i << '=' << getAttribute(this->hoverColor[i]);
	}

	for (int i = 0; i < this->selectedColor.size(); ++i) {
		qDebug() << "selectedColor" << i << '=' << getAttribute(this->selectedColor[i]);
	}
}

void QvtkAnnotationPolyData::reset()
{
	QvtkPolyData::reset();
	setHoverColor(0, 0, 0);
	setSelectedColor(0, 0, 0);
}

void QvtkAnnotationPolyData::setHoverColor(const double hoverColor[3])
{
	setAttributes(this->hoverColor, QVariantList() << hoverColor[0]
		<< hoverColor[1] << hoverColor[2]);
	//emit hoverColorChanged(hoverColor);
}

unsigned int QvtkAnnotationPolyData::getAnnotationStatus() const
{
	return getAttribute(this->annotationStatus).toUInt();
}

void QvtkAnnotationPolyData::getHoverColor(double hoverColor[3]) const
{
	QVariantList list = getAttributes(this->hoverColor);
	for (int i = 0; i < 3; ++i)
	{
		hoverColor[i] = list[i].toDouble();
	}
}

void QvtkAnnotationPolyData::setSelectedColor(const double selectedColor[3])
{
	setAttributes(this->selectedColor, QVariantList() << selectedColor[0] <<
		selectedColor[1] << selectedColor[2]);
	//emit selectedColorChanged(selectedColor);
}

void QvtkAnnotationPolyData::getSelectedColor(double selectedColor[3]) const
{
	QVariantList list = getAttributes(this->selectedColor);
	for (int i = 0; i < 3; ++i)
	{
		selectedColor[i] = list[i].toDouble();
	}
}

void QvtkAnnotationPolyData::setAnnotationStatus(unsigned int i)
{
	setAttribute(this->annotationStatus, i);
	
	ENUM_ANNOTATION_STATUS _annotationStatus = static_cast<ENUM_ANNOTATION_STATUS>(i);
	double color[3];
	switch (_annotationStatus)
	{
	case QvtkAnnotationPolyData::HOVER:
		this->getHoverColor(color);
		break;
	case QvtkAnnotationPolyData::SELECTED:
		this->getSelectedColor(color);
		break;
	case QvtkAnnotationPolyData::NONE:
		this->getColor(color);
	default:
		break;
	}

	emit colorChanged(color);
}

void QvtkAnnotationPolyData::setAnnotatoinStatus(Data * self, QStandardItem * item)
{
	QvtkAnnotationPolyData* _self = static_cast<QvtkAnnotationPolyData*>(self);
	_self->setAnnotationStatus(getAttribute(item).toUInt());
}

void QvtkAnnotationPolyData::setHoverColor(Data * self, QStandardItem * item)
{
	QvtkAnnotationPolyData* _self = static_cast<QvtkAnnotationPolyData*>(self);
	double color[3];
	_self->getHoverColor(color);
	_self->setHoverColor(color);
}

void QvtkAnnotationPolyData::setSelectedColor(Data * self, QStandardItem * item)
{
	QvtkAnnotationPolyData* _self = static_cast<QvtkAnnotationPolyData*>(self);
	double color[3];
	_self->getSelectedColor(color);
	_self->setSelectedColor(color);
}
