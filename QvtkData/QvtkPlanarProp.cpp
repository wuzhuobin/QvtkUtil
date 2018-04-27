#include "QvtkPlanarProp.h"

// me
#include "QvtkDataSet.h"
#include "QvtkScene.h"

// vtk
#include <vtkPlane.h>
#include <vtkProp3D.h>

// qt
#include <QDebug>
#include <QDomElement>

namespace Q {
namespace vtk {

const QString PlanarProp::ORIGIN_AND_NORMAL_PREFIX[3] = { "X", "Y", "Z" };

Q_VTK_DATA_CPP(PlanarProp);
PlanarProp::PlanarProp()
{
	//this->slicePlane = nullptr;

	QStandardItem* normal = createAttribute(K.PlanarNormal);
	this->planarNormal = createAttributesByColumns(
		QStringList() << ORIGIN_AND_NORMAL_PREFIX[0] << ORIGIN_AND_NORMAL_PREFIX[1] << ORIGIN_AND_NORMAL_PREFIX[2],
		QVariantList() << static_cast<double>(0) << static_cast<double>(0) << static_cast<double>(1),
		true,
		normal
	);
	insertSlotFunction(this->planarNormal[0], &PlanarProp::setPlanarNormal);
	insertSlotFunction(this->planarNormal[1], &PlanarProp::setPlanarNormal);
	insertSlotFunction(this->planarNormal[2], &PlanarProp::setPlanarNormal);

	QStandardItem* origin = createAttribute(K.PlanarOrigin);
	this->planarOrigin = createAttributesByColumns(
		QStringList() << ORIGIN_AND_NORMAL_PREFIX[0] << ORIGIN_AND_NORMAL_PREFIX[1] << ORIGIN_AND_NORMAL_PREFIX[2],
		QVariantList() << static_cast<double>(0) << static_cast<double>(0) << static_cast<double>(0),
		true,
		origin
	);

	insertSlotFunction(this->planarOrigin[0], &PlanarProp::setPlanarOrigin);
	insertSlotFunction(this->planarOrigin[1], &PlanarProp::setPlanarOrigin);
	insertSlotFunction(this->planarOrigin[2], &PlanarProp::setPlanarOrigin);

	this->planarOrientation = createAttribute(K.PlanarOrientation, static_cast<unsigned int>(2), true);
	insertSlotFunction(this->planarOrientation, &PlanarProp::setPlanarOrientation);

}

PlanarProp::~PlanarProp()
{

}

void PlanarProp::printSelf() const
{
	Prop::printSelf();
}

void PlanarProp::reset()
{
	Prop::reset();
	setPlanarNormal(0, 0, 1);

}

void PlanarProp::setPlanarNormal(double x, double y, double z)
{
	ENUM_ORIENTATION _orientation = static_cast<ENUM_ORIENTATION>(getPlanarOrientation());
	// for some reasons, the normal must follow the orientation
	// otherwise it will easily be endless loop
	double _normal[3] = { 0,0,0 };
	switch (_orientation)
	{
	case PlanarProp::ORIENTATION_YZ:
		_normal[0] = 1;
		break;
	case PlanarProp::ORIENTATION_XZ:
		_normal[1] = 1;
		break;
	case PlanarProp::ORIENTATION_XY:
		_normal[2] = 1;
		break;
	case PlanarProp::ORIENTATION_OBLIQUE:
		_normal[0] = x;
		_normal[1] = y;
		_normal[2] = z;
		break;
	default:
		break;
	}
	setAttributes(this->planarNormal, QVariantList() << _normal[0] << _normal[1] << _normal[2]);

}

void PlanarProp::getPlanarNormal(double normal[3])
{
	normal[0] = getAttributes(this->planarNormal)[0].toDouble();
	normal[1] = getAttributes(this->planarNormal)[1].toDouble();
	normal[2] = getAttributes(this->planarNormal)[2].toDouble();
}

void PlanarProp::setPlanarOrigin(double x, double y, double z)
{
	setAttributes(this->planarOrigin, QVariantList() << x << y << z );
}

void PlanarProp::getPlanarOrigin(double origin[3])
{
	origin[0] = getAttributes(this->planarOrigin)[0].toDouble();
	origin[1] = getAttributes(this->planarOrigin)[1].toDouble();
	origin[2] = getAttributes(this->planarOrigin)[2].toDouble();
}

void PlanarProp::setPlanarOrientation(unsigned int orientation)
{
	ENUM_ORIENTATION _orientation = static_cast<ENUM_ORIENTATION>(orientation);
	setAttribute(this->planarOrientation, _orientation);
	// for some reasons, the normal must follow the orientation
	// otherwise it will easily be endless loop
	double normal[3] = { 0,0,0 };
	switch (_orientation)
	{
	case PlanarProp::ORIENTATION_YZ:
		normal[0] = 1;
		setPlanarNormal(normal);
		break;
	case PlanarProp::ORIENTATION_XZ:
		normal[1] = 1;
		setPlanarNormal(normal); 
		break;
	case PlanarProp::ORIENTATION_XY:
		normal[2] = 1;
		setPlanarNormal(normal);
		break;
	case PlanarProp::ORIENTATION_OBLIQUE:
		break;
	default:
		break;
	}
}

int PlanarProp::getPlanarOrientation() const
{
	return getAttribute(this->planarOrientation).toInt();
}

void PlanarProp::propMatrixUpdate()
{
	double origin[3], normal[3];
	getPlanarOrigin(origin);
	getPlanarNormal(normal);

	setPlanarOrigin(origin);
	setPlanarNormal(normal);

	Prop::propMatrixUpdate();
}

void PlanarProp::setPlanarNormal(Data * self, QStandardItem * item)
{
	PlanarProp* _self = static_cast<PlanarProp*>(self);
	double normal[3];
	_self->getPlanarNormal(normal);
	_self->setPlanarNormal(normal);
}

void PlanarProp::setPlanarOrigin(Data * self, QStandardItem * item)
{
	PlanarProp* _self = static_cast<PlanarProp*>(self);
	double origin[3];
	_self->getPlanarOrigin(origin);
	_self->setPlanarOrigin(origin);
}

void PlanarProp::setPlanarOrientation(Data * self, QStandardItem * item)
{
	PlanarProp* _self = static_cast<PlanarProp*>(self);
	_self->setPlanarOrientation(getAttribute(item).toUInt());
}

}
}