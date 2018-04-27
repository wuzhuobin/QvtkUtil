#ifndef __QVTK_DATA_SET_H__
#define __QVTK_DATA_SET_H__

// me
#include "QvtkData.h"
namespace Q {
namespace vtk{
	class Prop;
}
}

// vtk
class vtkMatrix4x4;
class vtkDataSet;
class vtkTrivialProducer;
class vtkTransform;
class vtkAlgorithmOutput;
class vtkTransform;

// qt
template <typename T>
class QList;
namespace Q{
namespace vtk{

class QVTKDATA_EXPORT DataSet: public Data
{
	Q_OBJECT;
	//Q_PROPERTY(
	//	QVariantList	Origin
	//	READ			getOrigin
	//	WRITE			setOrigin
	//	NOTIFY			originChanged
	//);
	Q_VTK_DATA_H(
		DataSet,
		Q_VTK_KEY(Origin)
		Q_VTK_KEY(Position)
		Q_VTK_KEY(Orientation)
		Q_VTK_KEY(Scale)
		Q_VTK_KEY(AdditionalMatrix)
		Q_VTK_KEY(UserMatrix)
		Q_VTK_KEY(Pickable)
		Q_VTK_KEY(Opacity)
		Q_VTK_KEY(RelativePath)
		Q_VTK_KEY(AbsolutePath)
		Q_VTK_KEY(ReferenceProps)
	);
public:
	/**
	 * Declaring static const attributes' keys
	 */
	static const QString ARRAY_PREFIX[3];
	static const QString MATRIX_PREFIX;
	static const QString PATH_SEPERATOR;
	static const QString REFERENCE_PROPS_PREFIX;

	DataSet();
	virtual ~DataSet() override;
	virtual void printSelf() const override;

	virtual void readXML(const QDomElement& xml, QString directoryPath = QString()) override;
	virtual void writeXML(QDomElement& xml, QString directoryPath = QString()) const override;

	virtual bool readData(QString rootDirectory) = 0;
	virtual bool writeData(QString rootDirectory) const = 0;

	virtual void readProp(const QDomElement& xml);
	virtual void writeProp(QDomElement& xml) const;

	virtual void dataSetCoordinateToWorldCoordinate(const double dataSet[3], double world[3]) const;
	virtual void worldCoordinateToDataSetCoordinate(const double world[3], double dataSet[3]) const;

	virtual void dataSetRegionToWorldRegion(const double dataSet[6], double world[6]) const;
	virtual void worldRegionToDataSetRegion(const double world[6], double dataSet[6]) const;

	virtual bool getPickable() const { return this->pickable; }
	virtual double getOpacity() const;

	virtual const double* getOrigin() const { return this->origin; }
	virtual void getOrigin(double origin[3]) const {
		origin[0] = this->origin[0];
		origin[1] = this->origin[1];
		origin[2] = this->origin[2];
	}

	virtual const double* getPosition() const { return this->position; }
	virtual void getPosition(double position[3]) const {
		position[0] = this->position[0];
		position[1] = this->position[1];
		position[2] = this->position[2];
	}

	virtual const double* getOrientation() const { return this->orientation; }
	virtual void getOrientation(double orientation[3]) const {
		orientation[0] = this->orientation[0];
		orientation[1] = this->orientation[1];
		orientation[2] = this->orientation[2];
	}

	virtual const double* getScale() const { return this->scale; }
	virtual void getScale(double scale[3]) const {
		scale[0] = this->scale[0];
		scale[1] = this->scale[1];
		scale[2] = this->scale[2];
	}
	/**
	 * @fn	virtual vtkMatrix4x4* getAdditionalMatrix() const;
	 * @brief	get #additionalMatrix.
	 * @deprecated
	 * @return	get #additionalMatrix.
	 */
	virtual vtkMatrix4x4* getAdditionalMatrix() const { return this->additionalMatrix; }
	
	virtual vtkMatrix4x4* getUserMatrix() const { return this->userMatrix; }
	/**
	 * @fn	virtual vtkTransform* getTransform() const;
	 * @brief	get #transform.
	 * @deprecated
	 * @return	get #transform.
	 */
	virtual void getTransform(vtkTransform* tranform) const;

	virtual QStringList getRelativePath() const { return this->relativePath; }

	virtual QStringList getAbsolutePath() const { return absolutePath; }

	virtual vtkDataSet* getDataSet() const;

	virtual vtkAlgorithmOutput* getOutputPort() const;

	virtual vtkAlgorithmOutput* getTransformOutputPort() const = 0;

	virtual void getWorldBounds(double bounds[6]) const;

	virtual void addReference(Prop* prop);
	
	virtual void removeReference(Prop* prop);
	
	virtual const QList<Prop*>* getReferenceProps() const { return this->referenceProps; }


public slots:


	/**
	 * @fn	virtual void reset() override;
	 * @brief	Resets all attributes to default state.
	 * @overrde
	 */
	virtual void reset() override;
	/**
	 * @brief	set/get method of #origin
	 * cannot be changed after setReferenceProps
	 */
	virtual void setOrigin(double x, double y, double z);
	virtual void setOrigin(const double origin[3]) { setOrigin(origin[0], origin[1], origin[2]); }

	/**
	 * @brief	set/get method of #position
	 * cannot be changed after setReferenceProps
	 */
	virtual void setPosition(double x, double y, double z);
	virtual void setPosition(const double position[3]) { setPosition(position[0], position[1], position[2]); }

	/**
	 * @brief	set/get method of #orientation
	 * cannot be changed after setReferenceProps
	 */
	virtual void setOrientation(double x, double y, double z);
	virtual void setOrientation(const double orientation[3]) { setOrientation(orientation[0], orientation[1], orientation[2]); }

	/**
	 * @brief	set/get method of #scale
	 * cannot be changed after setReferenceProps
	 */
	virtual void setScale(double x, double y, double z);
	virtual void setScale(const double scale[3]) { setScale(scale[0], scale[1], scale[2]); }


	virtual void setPickable(bool flag);

	virtual void setOpacity(double opacity);


	virtual void setRelativePath(const QStringList relativePath) {this->relativePath = relativePath;}

	virtual void setAbsolutePath(const QStringList absolutePath) { this->absolutePath = absolutePath;}

signals:
	void pickableChanged(bool pickable);
	void opacityChanged(double opacity);
	void originChanged(const double* xyz);
	void positionChanged(const double* xyz);
	void orientationChanged(const double* xyz);
	void scaleChanged(const double* xyz);

protected:

	virtual void updateUniqueReferences(QString newName, QString oldName, QDomElement& xml) const override;

	virtual void setAdditionalMatrix(vtkMatrix4x4* additionalMatrix);

	virtual void setUserMatrix(vtkMatrix4x4* userMatrix);

	static void writeMatrix(QString key, QDomElement& element, const vtkMatrix4x4* martrix);

	static void readMatrix(QString key, const QDomElement& element, vtkMatrix4x4* martrix);
	
	static void setOpacity(Data* self, QStandardItem* item);

	/**
	 * @fn	virtual void setTransform(vtkTransform* transform);
	 * @brief	set #transfrom
	 * @deprecated
	 * @param	transform set #transfrom
	 */
	//virtual void setTransform(vtkTransform* transform);

	virtual void setDataSet(vtkDataSet* data);


private:

	bool pickable;
	double origin[3];
	double position[3];
	double orientation[3];
	double scale[3];

	vtkMatrix4x4* additionalMatrix;
	vtkMatrix4x4* userMatrix;
	//vtkTransform* transform;

	QStandardItem* opacity;

	QStringList relativePath;
	QStringList absolutePath;

	vtkDataSet* data;
	vtkTrivialProducer* portProducer;

	QList<Prop*>* referenceProps;

private slots:

};
}
}

#endif // !__QVTK_DATA_SET_H__
