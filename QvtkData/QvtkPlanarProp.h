/*!
 * \file QvtkAbstractPlanarProp.h
 * \date 2017/08/10 14:09
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong, 
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief 
 *      
 * This class should be inherited by all the props that will react differently according to
 * current viewing directions.
 *
 * TODO: long description
 *
 * \note
*/

#ifndef __Qvtk_ABSTRACT_PLANAR_PROP_H__
#define __Qvtk_ABSTRACT_PLANAR_PROP_H__

#include "QvtkAbstractProp.h"

class ABSTRACTDATA_EXPORT QvtkAbstractPlanarProp: public QvtkAbstractProp
{
	Q_OBJECT;
	Q_VTK_DATAH(
		QvtkAbstractPlanarProp,
		Q_VTK_KEY(PlanarOrientation)
		Q_VTK_KEY(PlanarNormal)
		Q_VTK_KEY(PlanarOrigin)
	);
public:
	static const QString ORIGIN_AND_NORMAL_PREFIX[3];

	enum ENUM_ORIENTATION {
		ORIENTATION_YZ = 0,
		ORIENTATION_XZ = 1,
		ORIENTATION_XY = 2,
		ORIENTATION_OBLIQUE = 3
	};

    QvtkAbstractPlanarProp();
    virtual ~QvtkAbstractPlanarProp() override;
	virtual void printSelf() const override;

	virtual void getPlanarNormal(double normal[3]);

	virtual void getPlanarOrigin(double origin[3]);

	virtual int getPlanarOrientation() const;

	virtual void propMatrixUpdate() override;

public slots:

	virtual void reset() override;

	void setPlanarNormal(const double normal[3]) { setPlanarNormal(normal[0], normal[1], normal[2]); }
	virtual void setPlanarNormal(double x, double y, double z);

	void setPlanarOrigin(const double origin[3]) { setPlanarOrigin(origin[0], origin[1], origin[2]); }
	virtual void setPlanarOrigin(double x, double y, double z);

	virtual void setPlanarOrientation(unsigned int orientation);
	void setPlanarOrientationToYZ() { setPlanarOrientation(ORIENTATION_YZ); }
	void setPlanarOrientationToXZ() { setPlanarOrientation(ORIENTATION_XZ); }
	void setPlanarOrientationToXY() { setPlanarOrientation(ORIENTATION_XY); }
	void setPlanarOrientationToOblique() { setPlanarOrientation(ORIENTATION_OBLIQUE); }


protected:


	static void setPlanarNormal(Data* self, QStandardItem* item);
	static void setPlanarOrigin(Data* self, QStandardItem* item);
	static void setPlanarOrientation(Data* self, QStandardItem* item);

private:

	QList<QStandardItem*> planarNormal;

	QList<QStandardItem*> planarOrigin;

	QStandardItem* planarOrientation;

};


#endif // !__Qvtk_ABSTRACT_PLANAR_PROP_H__
