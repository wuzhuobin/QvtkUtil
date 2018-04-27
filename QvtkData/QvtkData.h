/**
 * @file	Data\QvtkData.h.
 * @brief	Declares the QvtkUtil abstract data class.
 * @language	C++
 * @author	wuzhuobin jiejin2022@163.com
 * @date	2017/6/19
 * Qvtk for QvtkUtil
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *			This program is distributed in the hope that it will be useful, but	 *
 *			WITHOUT ANY WARRANTY; without even the implied warranty of			 * 
 *			MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.				 * 
 *			See the LICENSE for more detail.									 * 
 *			Copyright (c) WUZHUOBIN. All rights reserved.						 * 
 *			See COPYRIGHT for more detail.										 * 
 *			This software is distributed WITHOUT ANY WARRANTY; without even		 * 
 *			the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR	 * 
 *			PURPOSE.  See the above copyright notice for more information.		 *
 *			Internal usage only, without the permission of the author, please DO *
 *			NOT publish and distribute without the author's permission.  	     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#ifndef __QVTK_ABSTRACT_DATA_H__
#define __QVTK_ABSTRACT_DATA_H__

namespace Q {
namespace vtk {
	class SlotMapper;
	class Scene;
}
}
// me
#include "qvtkdata_export.h"
#include "QvtkDataMacro.h"

// qt
#include <QObject>
#include <QVariant>
#include <QVariantList>
#include <QList>
class QDomElement;
class QStandardItemModel;
class QStandardItem;
namespace Q {
namespace vtk {
/**
 * @class	Data
 * @brief	QvtkUtil AbstractData
 * @author	WUZHUOBIN
 * @date	2017/6/19
 * 
 * QvtkUtil AbstractData to Saving data in QvtkScene
 * Attributes of Data is in tree-like structue. 
 * @see QvtkScene
 * 
 * Invisible attribute: 
 *		UniqueName = "";
 *		Selectable = true;
 *		Selected = true;
 *
 * Visible attribute in #attributesModel (Tree Structure):
 *		ModelAttributes
 *		-Description = "";
 * 
 */

class QVTKDATA_EXPORT Data : public QObject
{
	Q_OBJECT;
	Q_PROPERTY(
		QString	UniqueName
		READ	getUniqueName
		WRITE	setUniqueName
		NOTIFY	uniqueNameChanged
	);
	Q_PROPERTY(
		bool	Selectable
		WRITE	setSelectable
		READ	getSelectable
	);
	Q_PROPERTY(
		bool	Selected
		READ	getSelected
		WRITE	setSelected
	);
	Q_PROPERTY(
		QString	Alias
		READ	getAlias
		WRITE	setAlias
		NOTIFY	aliasChanged
	);
	Q_PROPERTY(
		QString Description
		READ	getDescription
		WRITE	setDescription
		NOTIFY	descriptionChanged
	);
	
	Q_VTK_DATA_H(
		Data,
		Q_VTK_KEY(UniqueName)
		Q_VTK_KEY(Selectable)
		Q_VTK_KEY(Selected)
		Q_VTK_KEY(ModelAttributes)
		Q_VTK_KEY(Alias)
		Q_VTK_KEY(Description)
	);

public:
	static const int MAX_COLUMNS = 4; ///< Maximum column of the models
	/**
	 * @typedef	void(*slotFunction)(Data*, QStandardItem*);
	 * @brief	typdef for a slot function.
	 *
	 * This function can be inserted to the slotMappper, with defining #slotFunction and using
	 * #insertSlotFunction to insert to #slotMapper, the #slotFunction will be invoke when
	 * attributes are changed.
	 */
	typedef void(*slotFunction)(Data*, QStandardItem*);

	/**
	 * @fn	Data::Data();
	 * @brief	Constructor.
	 *
	 */
	Data();

	/**
	 * @fn	virtual Data::~Data() override;
	 * @brief	Destructor.
	 *
	 */
	virtual ~Data() override;

	/**
	 * @fn	virtual void printSelf() const;
	 * @brief	Using qDebug() to print self
	 * @const
	 *
	 */
	virtual void printSelf() const;

	/**
	 * @fn	virtual bool Data::isClass(QString className) const;
	 * @brief	Query if 'className' is this class.
	 * @const
	 * @param[in]	className	Name of the class.
	 * @return	True if class, false if not.
	 *
	 * Providing convenient method which is the same as QObject#inherits().
	 */

	virtual bool isClass(QString className) const { return inherits(className.toStdString().c_str()); }

	/**
	 * @fn	QString Data::getClassName() const;
	 * @brief	Gets class name.
	 * @const
	 * @return	The class name.
	 *
	 * Providing convenient method which is the same as QObject#metaObject()#className().
	 */
	QString getClassName() const { return this->metaObject()->className(); }

	/**
	 * @fn	virtual void readXML(const QDomElement& xml, QString directoryPath = QString());
	 * @brief	Reads an XML.
	 * @param[in]	xml	QDomElement to load current this data.
	 * @param[in]	directoryPath directoryPath for loading the vtkDataSet or data file
	 */

	virtual void readXML(const QDomElement& xml, QString directoryPath = QString());

	/**
	 * @fn	virtual void QvtkOAbstractData::writeXML(QStringList xml) = 0;
	 * @brief	Writes an XML.
	 * @const
	 * @param[in,out]	xml	QDomElement to write current this data.
	 * @param[in]	directoryPath directoryPath for write the vtkDataSet or data file.
	 */

	virtual void writeXML(QDomElement& xml, QString directoryPath = QString()) const;


	//virtual QString getTag() const = 0;

	/**
	 * @fn	virtual void QvtkOAbstractData::copy(QvtkOAbstractData* data);
	 * @brief	Copies the given data.
	 * @deprecated
	 * @todo	To think about how to complete this method.
	 * @param[in]	data If non-null, the data.
	 */
	virtual void copy(const Data* data) {}
	/**
	 * @fn	virtual QString Data::getUniqueName();
	 * @brief	return unique name of this data
	 * @const
	 * @return	#uniqueName.
	 */
	virtual QString getUniqueName() const { return this->uniqueName; }
	/**
	 * @fn	virtual QString getAlias() const;
	 * @brief	return the alias of this data.
	 * @const
	 * @return	the alias.
	 *
	 * Using the qt QObject#objectName() as the data's alias.
	 */
	virtual QString getAlias() const { return this->objectName(); }

	/**
	 * @fn	virtual bool Data::getAddedToScene() const;
	 * @brief	check whether the data has added to scene.
	 * @const
	 * @return	#addedToScene.
	 *
	 */
	virtual bool getAddedToScene() const { return this->addedToScene; }

	/**
	 * @fn	virtual QStandardItemModel* getModel() const;
	 * @brief	get Model of this data, model is for all visible attributes.
	 * @const
	 * @return	#attributesModel the model containing all visible attributes.
	 *
	 * QT's model/view/delegate design, to seperate the model for view.
	 */
	virtual QStandardItemModel* getModel() const;

	/**
	 * @fn	virtual bool getSelectable() const;
	 * @brief	get selectable.
	 * @const
	 * @return	#selectable
	 *
	 * If the data is selectable, the data should be visible or selectable in the scene' view.
	 * @see	QvtkScene
	 */
	virtual bool getSelectable() const { return this->selectable; }
	/**
	 * @fn	virtual bool getSelected() const.
	 * @brief	get selected.
	 * @const
	 * @return	#selected
	 *
	 * if true, the data is selected in the QvtkScene.
	 */
	virtual bool getSelected() const { return this->selected; }

	/**
	 * @fn	virtual QString getDescription() const;
	 * @brief	Get description.
	 * @const
	 * @return	data of #descriptoin.
	 *
	 * Get description of the data.
	 */
	virtual QString getDescription() const;


	public Q_SLOTS:
	/**
	 * @fn	virtual void setAlias(QString alias);
	 * @brief	Set alias
	 * @param[in]	set QObject#setObjectName.
	 *
	 * Convenient methods to set QvtkAbstract 's alias, using
	 * internal QObject#objectName() property as alias.
	 * By setting QvtkScene as object's parent to easily search.
	 */
	virtual void setAlias(QString alias) { this->setObjectName(alias); }
	/**
	 * @fn	virtual void QvtkOAbstractData::reset();
	 * @brief	Resets all attributes to default state.
	 */
	virtual void reset();
	/**
	 * @fn	virtual void setSelectable(bool selectable);
	 * @brief	Set selectable.
	 * @const
	 * @param[in]	selectable set #selectable.
	 *
	 * If the data is selectable, the data should be visible or selectable in the scene' view.
	 * @see	QvtkScene
	 */
	virtual void setSelectable(bool selectable) { this->selectable = selectable; }
	/**
	 * @fn	void selectableOn();
	 */
	void selectableOn() { setSelectable(true); }
	/**
	 * @fn	void selectableOff();
	 */
	void selectableOff() { setSelectable(false); }
	/**
	 * @fn	virtual void setSelected(bool selected);
	 * @brief	set selected.
	 * @param[in]	selected set #selected.
	 *
	 * when the data is selected, set selected.
	 */
	virtual void setSelected(bool selected) { this->selected = selected; }
	/**
	 * @fn	void selectedOn();
	 */
	void selectedOn() { setSelected(true); }
	/**
	 * @fn	void selectedOff();
	 */
	void selectedOff() { setSelected(false); }
	/**
	 * @fn virtual void setDescription(QString description);
	 * @brief	Set Description.
	 * @param[in]	descriptoin set #description 's data.
	 *
	 * Set description of the data.
	 */
	virtual void setDescription(QString description);

Q_SIGNALS:
	/**
	 * @fn 	void descriptionChanged(QString description);
	 * @brief	emit by #setDescription();
	 * @param[out]	description
	 */
	void descriptionChanged(QString description);

	/**
	 * @fn	void aliasChanged(QString alias);
	 * @brief	emit when alias changed.
	 * @param[out]	alias
	 */
	void aliasChanged(QString alias);

	void uniqueNameChanged(QString uniqueName);
protected:

	/**
	 * @class	QvtkScene
	 * @see QvtkScene
	 *
	 * Set QvtkScene as a friend class, QvtkScene is the only class can modify
	 * #uniqueName and #addedToScene.
	 */
	friend class Scene;

	/**
	 * @fn	virtual Data* Data::newInstance() = 0;
	 * @brief	Creates a new instance.
	 * @return	Null if it fails, else a pointer to a QvtkOAbstractData.
	 */
	virtual Data* newInstance() const = 0;

	/**
	 * @fn	virtual void updateUniqueReference(QString uniqueName);
	 * @brief	Update some unique attributes where replace oldName with newName.
	 * @param[in]	oldName The old unique name.
	 * @param[in]	newName The new unique name.
	 * @param[in, out]	xml The xml needs to be updated.
	 *
	 * Update some unique references. The unique references are something related to
	 * #uniqueName. Since the reference between different data are related to their
	 * unique names, so in case wrong reference in import/commit of QvtkScene, QvtkScene
	 * need to call this method to update their unique name reference with
	 * QvtkScene#generateUniqueName, which to make sure it is unique in scene and correctly
	 * refered.
	 */
	virtual void updateUniqueReferences(QString oldName, QString newName, QDomElement& xml) const;

	/**
	 * @fn	void insertSlotFunction(QStandardItem* item, slotFunction f);
	 * @brief	insert item(key) and slotFunction(value) to the slotMapper.
	 * @param[in]	item QStandardItem key to find the slotFunction.
	 * @param[in]	f value, slotFunction to be invoked when item is modified.
	 *
	 * Insert item for key and slotFunction to be invoked. The item is the visible key in the model.
	 * f is the slotFunction to be invoked when item is modified. Developer should insert items and its
	 * specified slotFunction.
	 */
	void insertSlotFunction(QStandardItem* item, slotFunction f);

	/**
	 * @fn	static QString translateAttributeKey(QString key);
	 * @brief
	 * @deprecated
	 * @static
	 */
	static QString translateAttributeKey(QString key);

	/**
	 * @fn	template<typename ValueType>
	 *		static void readArray(QString key, const QDomElement& xml, ValueType* values, int size);
	 * @brief	Template function for read a plain array.
	 * @static
	 * @tparam		ValueType the type of the plain array. Only the following types are now supported:
	 *				char, int, long, float, double
	 * @param[in]	key the key of the array.
	 * @param[in]	xml QDomElement to read the array's values.
	 * @param[out]	values read from QDomElement and write to values.
	 * @param[in]	size size of the plain array, the plain does not provide size information.
	 *
	 * Template function to read a plain array, the key of every value is key + '_'.
	 * This template function now only support the following types:
	 */
	template<typename ValueType>
	static void readArray(QString key, const QDomElement& xml, ValueType* values, int size);

	/**
	 * @fn	template<typename ValueType>
	 *		static void readArray(QString key, const QString* prefix, const QDomElement& xml, ValueType* values, int size);
	 * @brief	Template function for read a plain array.
	 * @static
	 * @tparam		ValueType the type of the plain array. Only the following types are now supported:
	 *				char, int, long, float, double
	 * @param[in]	key the key of the array.
	 * @param[in]	prefix the prefix key of every element of the array.
	 * @param[in]	xml QDomElement to read the array's values.
	 * @tparam[out]	values read from QDomElement and write to values.
	 * @param[in]	size size of the plain array, the plain does not provide size information.
	 *
	 * Template function to read a plain array, the key of every element is prefix. Their length should be same.
	 * This template function now only support the following types:
	 * char, int, long, float, double
	 */
	template<typename ValueType>
	static void readArray(QString key, const QString* prefix, const QDomElement& xml, ValueType* values, int size);
	/**
	 * @fn	template<typename ValueType>
	 *		static void writeArray(QString key, QDomElement& xml, const ValueType* values, int size);
	 * @brief	Template function for write a plain array.
	 * @tparam		ValueType the type of the plain array. Only the following types are now supported:
	 *				char, int, long, float, double
	 * @param[in]	key the key of the array.
	 * @param[out]	xml QDomElement to write the array's values.
	 * @tparam[in]	values read from values and write to QDomElement.
	 * @param[in]	size size of the plain array, the plain does not provide size information.
	 *
	 * Template function to write a plain array, the key of every value is key + '_'.
	 * This template function now only support the following types:
	 * char, int, long, float, double
	 */
	template<typename ValueType>
	static void writeArray(QString key, QDomElement& xml, const ValueType* values, int size);
	/**
	 * @fn	template<typename ValueType>
	 *		static void writeArray(QString key, QDomElement& xml, const ValueType* values, int size);
	 * @brief	Template function for write a plain array.
	 * @tparam		ValueType the type of the plain array. Only the following types are now supported:
	 *				char, int, long, float, double
	 * @param[in]	key the key of the array.
	 * @param[in]	prefix the prefix key of every element of the array.
	 * @param[out]	xml QDomElement to write the array's values.
	 * @tparam[in]	values read from values and write to QDomElement.
	 * @param[in]	size size of the plain array, the plain does not provide size information.
	 *
	 * Template function to write a plain array, the key of every element is prefix. Their length should be same.
	 * This template function now only support the following types:
	 * char, int, long, float, double
	 */
	template<typename ValueType>
	static void writeArray(QString key, const QString* prefix, QDomElement& xml, const ValueType* values, int size);
	/**
	 * @fn	virtual QStandardItem* createAttribute(QString key, QVariant value = QVariant(),
	 *		bool editable = false, QStandardItem* parent = nullptr);
	 * @brief	Create an attribute in QStandardModel.
	 * @param[in]	key key of the attribute to be created.
	 * @param[in]	value value of the attribute, default is QVariant, no value, which only a key will be display in
	 *				the view. QVariant can save the plain C++ type. Please make sure value's type using or
	 *				using static_cast() to cast it correctly.
	 * @param[in]	editable whether the attribute is editable. Default is false.
	 * @param[in]	parent	The parent of this attirbute. Default is nullptr, which is the top level attribute.
	 *				The parent should be an attribute without value.
	 *
	 * Create attribute of this data in #attributesModel.
	 */
	virtual QStandardItem* createAttribute(QString key, QVariant value = QVariant(),
		bool editable = false, QStandardItem* parent = nullptr);
	/**
	 * @fn	virtual QList<QStandardItem*> createAttributesByRows(QStringList keys,
	 *		QVariantList values, bool editable = false, QStandardItem* parent = nullptr);
	 * @brief	Create multiple attributes, an attribute per row.
	 * @param[in]	keys keys for attributes .
	 * @param[in]	values	values for attributes.
	 * @param[in]	editable Whether attributes are editable. Default is false.
	 * @param[in]	parent The parent of these attributes. Default is nullptr, which is the top level attribute.
	 *				The parent should be an attribute without value.
	 *
	 * Create attributes of this data in #attributesModel.
	 * The length of keys and values must be same.
	 */
	virtual QList<QStandardItem*> createAttributesByRows(QStringList keys,
		QVariantList values, bool editable = false, QStandardItem* parent = nullptr);
	/**
	 * @fn	virtual QList<QStandardItem*> createAttributesByColumns(QStringList keys,
	 *		QVariantList values, bool editable = false, QStandardItem* parent = nullptr);
	 * @brief	Create multiple attributes, an attribute per Columns.
	 * @param[in]	keys keys for attributes .
	 * @param[in]	values	values for attributes.
	 * @param[in]	editable Whether attributes are editable. Default is false.
	 * @param[in]	parent The parent of these attributes. Default is nullptr, which is the top level attribute.
	 *				The parent should be an attribute without value.
	 *
	 * Create attributes of this data in #attributesModel. The Columns is limited by #MAX_COLUMNS.
	 * If the length of attributes are bigger than #MAX_COLUMNS, the arrtibutes will become invisible.
	 * The length of keys and values must be same.
	 */
	virtual QList<QStandardItem*> createAttributesByColumns(QStringList keys,
		QVariantList values, bool editable = false, QStandardItem* parent = nullptr);
	/**
	 * @fn	static void setAttribute(QStandardItem* item, const QVariant value);
	 * @brief	static function to set item's value.
	 * @static
	 * @param[in]	item the item whose value to be set.
	 * @param[in]	value the value to be set.
	 */
	static void setAttribute(QStandardItem* item, const QVariant value);
	/**
	 * @fn	static QVariant getAttribute(const QStandardItem* item);
	 * @brief	static function to get item's value.
	 * @static
	 * @param[in]	item to be get.
	 * @return	the value of the item.
	 */
	static QVariant getAttribute(const QStandardItem* item);
	/**
	 * @fn static void setAttributes(QList<QStandardItem*> items, const QVariantList values);
	 * @brief	 static function to set multiple items ' values.
	 * @static
	 * @param[in]	items to be set.
	 * @param[in]	values to be set.
	 * function to set multiple attributes. The length of items and values must be same.
	 */
	static void setAttributes(QList<QStandardItem*> items, const QVariantList values);
	/**
	 * @fn	static QVariantList getAttributes(const QList<QStandardItem*> items);
	 * @brief	get multiple values of items.
	 * @static
	 * @param[in]	items to be set.
	 * @return	values of items.
	 * function to get multiple attributes. The length of values and itmes are the same.
	 */
	static QVariantList getAttributes(const QList<QStandardItem*> items);
	/**
	 * @fn	static void setDescription(Data* self, QStandardItem* item);
	 * @brief	slotFunction for #description callback.
	 * @static
	 * @param[in]	self to transmit this pointer.
	 * @param[in]	item to transmit caller item.
	 */
	static void setDescription(Data* self, QStandardItem* item);

private:
	/**
	 * @fn	static void readAttributes(const QDomElement& xml, QStandardItem* item);
	 * @brief	read attributes in #attributeModel recursively.
	 * @static
	 * @param[in]	xml QDomElement to read attributes.
	 * @param[out]	item write value to item from xml.
	 */
	static void readAttributes(const QDomElement& xml, QStandardItem* item);
	/**
	 * @fn	static void writeAttributes(QDomElement& xml, const QStandardItem* item);
	 * @brief	write attributes in #attributeModel recursively.
	 * @static
	 * @param[out]	xml QDomElement to write attributes.
	 * @param[in]	item write value form item to xml.
	 */
	static void writeAttributes(QDomElement& xml, const QStandardItem* item);

	QString uniqueName;   ///< Unique name of the data
	bool addedToScene;	///< whether the data is added to the current QvtkScene. 
	bool selectable;  ///< whether the data is visible and selectable in the QvtkScene.
	bool selected;	///< whether the data is selected in the QvtkScene. 
	QStandardItem* description; ///< Description of the data. 

	QStandardItemModel* attributesModel; ///< model containing visible attributes. 
	SlotMapper* slotMapper; ///< A mapper mapping from item to slotFunction. 

	private Q_SLOTS :
	void setUniqueName(QString uniqueName);
	/**
	 * @fn	void slotMapping(QStandardItem* item);
	 * @brief	slot function to connect #attributesModel#itemChanged().
	 */
	void slotMapping(QStandardItem* item);

};
}
}

#endif // !__QVTK_ABSTRACT_DATA_H__
