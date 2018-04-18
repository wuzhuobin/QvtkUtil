#include "QvtkAbstractPlanarProp.h"

// me
#include "DataSet.h"
#include "QvtkScene.h"

// vtk
#include <vtkPlane.h>
#include <vtkProp3D.h>

// qt
#include <QDebug>
#include <QDomElement>

const QString QvtkAbstractPlanarProp::ORIGIN_AND_NORMAL_PREFIX[3] = { "X", "Y", "Z" };

Q_VTK_DATACPP(QvtkAbstractPlanarProp);
QvtkAbstractPlanarProp::QvtkAbstractPlanarProp()
{
	//this->slicePlane = nullptr;

	QStandardItem* normal = createAttribute(K.PlanarNormal);
	this->planarNormal = createAttributesByColumns(
		QStringList() << ORIGIN_AND_NORMAL_PREFIX[0] << ORIGIN_AND_NORMAL_PREFIX[1] << ORIGIN_AND_NORMAL_PREFIX[2],
		QVariantList() << static_cast<double>(0) << static_cast<double>(0) << static_cast<double>(1),
		true,
		normal
	);
	insertSlotFunction(this->planarNormal[0], &QvtkAbstractPlanarProp::setPlanarNormal);
	insertSlotFunction(this->planarNormal[1], &QvtkAbstractPlanarProp::setPlanarNormal);
	insertSlotFunction(this->planarNormal[2], &QvtkAbstractPlanarProp::setPlanarNormal);

	QStandardItem* origin = createAttribute(K.PlanarOrigin);
	this->planarOrigin = createAttributesByColumns(
		QStringList() << ORIGIN_AND_NORMAL_PREFIX[0] << ORIGIN_AND_NORMAL_PREFIX[1] << ORIGIN_AND_NORMAL_PREFIX[2],
		QVariantList() << static_cast<double>(0) << static_cast<double>(0) << static_cast<double>(0),
		true,
		origin
	);

	insertSlotFunction(this->planarOrigin[0], &QvtkAbstractPlanarProp::setPlanarOrigin);
	insertSlotFunction(this->planarOrigin[1], &QvtkAbstractPlanarProp::setPlanarOrigin);
	insertSlotFunction(this->planarOrigin[2], &QvtkAbstractPlanarProp::setPlanarOrigin);

	this->planarOrientation = createAttribute(K.PlanarOrientation, static_cast<unsigned int>(2), true);
	insertSlotFunction(this->planarOrientation, &QvtkAbstractPlanarProp::setPlanarOrientation);

}

QvtkAbstractPlanarProp::~QvtkAbstractPlanarProp()
{

}

void QvtkAbstractPlanarProp::printSelf() const
{
	QvtkAbstractProp::printSelf();
}

void QvtkAbstractPlanarProp::reset()
{
	QvtkAbstractProp::reset();
	setPlanarNormal(0, 0, 1);

}

void QvtkAbstractPlanarProp::setPlanarNormal(double x, double y, double z)
{
	ENUM_ORIENTATION _orientation = static_cast<ENUM_ORIENTATION>(getPlanarOrientation());
	// for some reasons, the normal must follow the orientation
	// otherwise it will easily be endless loop
	double _normal[3] = { 0,0,0 };
	switch (_orientation)
	{
	case QvtkAbstractPlanarProp::ORIENTATION_YZ:
		_normal[0] = 1;
		break;
	case QvtkAbstractPlanarProp::ORIENTATION_XZ:
		_normal[1] = 1;
		break;
	case QvtkAbstractPlanarProp::ORIENTATION_XY:
		_normal[2] = 1;
		break;
	case QvtkAbstractPlanarProp::ORIENTATION_OBLIQUE:
		_normal[0] = x;
		_normal[1] = y;
		_normal[2] = z;
		break;
	default:
		break;
	}
	setAttributes(this->planarNormal, QVariantList() << _normal[0] << _normal[1] << _normal[2]);

}

void QvtkAbstractPlanarProp::getPlanarNormal(double normal[3])
{
	normal[0] = getAttributes(this->planarNormal)[0].toDouble();
	normal[1] = getAttributes(this->planarNormal)[1].toDouble();
	normal[2] = getAttributes(this->planarNormal)[2].toDouble();
}

void QvtkAbstractPlanarProp::setPlanarOrigin(double x, double y, double z)
{
	setAttributes(this->planarOrigin, QVariantList() << x << y << z );
}

void QvtkAbstractPlanarProp::getPlanarOrigin(double origin[3])
{
	origin[0] = getAttributes(this->planarOrigin)[0].toDouble();
	origin[1] = getAttributes(this->planarOrigin)[1].toDouble();
	origin[2] = getAttributes(this->planarOrigin)[2].toDouble();
}

void QvtkAbstractPlanarProp::setPlanarOrientation(unsigned int orientation)
{
	ENUM_ORIENTATION _orientation = static_cast<ENUM_ORIENTATION>(orientation);
	setAttribute(this->planarOrientation, _orientation);
	// for some reasons, the normal must follow the orientation
	// otherwise it will easily be endless loop
	double normal[3] = { 0,0,0 };
	switch (_orientation)
	{
	case QvtkAbstractPlanarProp::ORIENTATION_YZ:
		normal[0] = 1;
		setPlanarNormal(normal);
		break;
	case QvtkAbstractPlanarProp::ORIENTATION_XZ:
		normal[1] = 1;
		setPlanarNormal(normal); 
		break;
	case QvtkAbstractPlanarProp::ORIENTATION_XY:
		normal[2] = 1;
		setPlanarNormal(normal);
		break;
	case QvtkAbstractPlanarProp::ORIENTATION_OBLIQUE:
		break;
	default:
		break;
	}
}

int QvtkAbstractPlanarProp::getPlanarOrientation() const
{
	return getAttribute(this->planarOrientation).toInt();
}

void QvtkAbstractPlanarProp::propMatrixUpdate()
{
	double origin[3], normal[3];
	getPlanarOrigin(origin);
	getPlanarNormal(normal);

	setPlanarOrigin(origin);
	setPlanarNormal(normal);

	QvtkAbstractProp::propMatrixUpdate();
}

void QvtkAbstractPlanarProp::setPlanarNormal(Data * self, QStandardItem * item)
{
	QvtkAbstractPlanarProp* _self = static_cast<QvtkAbstractPlanarProp*>(self);
	double normal[3];
	_self->getPlanarNormal(normal);
	_self->setPlanarNormal(normal);
}

void QvtkAbstractPlanarProp::setPlanarOrigin(Data * self, QStandardItem * item)
{
	QvtkAbstractPlanarProp* _self = static_cast<QvtkAbstractPlanarProp*>(self);
	double origin[3];
	_self->getPlanarOrigin(origin);
	_self->setPlanarOrigin(origin);
}

void QvtkAbstractPlanarProp::setPlanarOrientation(Data * self, QStandardItem * item)
{
	QvtkAbstractPlanarProp* _self = static_cast<QvtkAbstractPlanarProp*>(self);
	_self->setPlanarOrientation(getAttribute(item).toUInt());
}
