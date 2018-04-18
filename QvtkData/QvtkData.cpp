#include "QvtkData.h"

//qt
#include <QDomDocument>
#include <QDebug>
#include <QStandardItemModel>
#include <QHash>

namespace Q{
namespace vtk{

Q_VTK_DATACPP(Data);
class SlotMapper: public QHash<QStandardItem*, Data::slotFunction>{};
QHash<QString, QString> attributeDictionary;
Data::Data()
	:uniqueName(QString())
{
	this->selectable = true;
	this->selected = false;
	this->addedToScene = false;

	this->attributesModel = new QStandardItemModel(0, MAX_COLUMNS*2, this);
	this->attributesModel->invisibleRootItem()->setData(K.ModelAttributes, Qt::DisplayRole);
	
	this->slotMapper = new SlotMapper;

	insertSlotFunction(this->description, &Data::setDescription);
	
	this->description = createAttribute(K.Description, 
		static_cast<QString>(""), true);

	connect(this->attributesModel, &QStandardItemModel::itemChanged, 
		this, &Data::slotMapping);
	connect(this, &Data::objectNameChanged,
		this, &Data::aliasChanged);
}

Data::~Data()
{
	//qDebug() << getUniqueName();
	delete this->slotMapper;
}

void Data::printSelf() const
{
	qDebug() << "ClassName" << '=' << getClassName();
	qDebug() << "uniqueName" << '=' << this->uniqueName;
	qDebug() << "addedToScene" << '=' << this->addedToScene;
	qDebug() << "selectable" << '=' << this->selectable;
	qDebug() << "selected" << '=' << this->selected;
	qDebug() << "description" << '=' << getAttribute(this->description);
}

void Data::readXML(const QDomElement & xml, QString directoryPath)
{
	readAttributes(xml, this->attributesModel->invisibleRootItem());
	setSelectable(static_cast<bool>(xml.attribute(K.Selectable).toInt()));
	setSelected(static_cast<bool>(xml.attribute(K.Selected).toInt()));
	setAlias(xml.attribute(K.Alias));
}

void Data::writeXML(QDomElement & xml, QString directoryPath) const
{
	writeAttributes(xml, this->attributesModel->invisibleRootItem());
	xml.setAttribute(K.Selectable, getSelectable());
	xml.setAttribute(K.Selected, getSelected());
	xml.setAttribute(K.Alias, getAlias());
}

void Data::reset()
{
	setDescription("");
}

QStandardItemModel * Data::getModel() const
{
	return this->attributesModel;
}

void Data::setDescription(QString description)
{
	setAttribute(this->description, description);
	emit descriptionChanged(description);
}

void Data::updateUniqueReferences(QString newName, QString oldName, QDomElement& xml) const
{
	if (xml.attribute(K.UniqueName) == oldName) {
			xml.setAttribute(K.UniqueName, newName);
	}
}

void Data::setDescription(Data * self, QStandardItem * item)
{
	self->setDescription(item->data(Qt::DisplayRole).toString());
}

void Data::insertSlotFunction(QStandardItem * item, slotFunction f)
{
	this->slotMapper->insert(item, f);
}

QString Data::translateAttributeKey(QString key)
{
	QStringList list = key.split("::");
	QString translated = *(++list.begin());
	return translated;
}

template<typename ValueType>
void Data::readArray(QString key, const QDomElement & xml, ValueType * values, int size)
{
	QDomElement arrayElement = xml.firstChildElement(key);
	for (int i = 0; i < size; ++i)
	{
		values[i] = qvariant_cast<ValueType>(QVariant(arrayElement.attribute(key + '_')));
	}
}

template<typename ValueType>
void Data::readArray(QString key, const QString * prefix, const QDomElement & xml, ValueType * values, int size)
{
	QDomElement arrayElement = xml.firstChildElement(key);
	for (int i = 0; i < size; ++i)
	{
		values[i] = qvariant_cast<ValueType>(QVariant(arrayElement.attribute(prefix[i])));
	}
}

template<typename ValueType>
void Data::writeArray(QString key, const QString * prefix, QDomElement & xml, const ValueType * values, int size)
{
	QDomDocument dom = xml.ownerDocument();
	QDomElement arrayElement = dom.createElement(key);
	xml.appendChild(arrayElement);

	for (int i = 0; i < size; ++i)
	{
//		The lower code is ambiguous in g++.
//		arrayElement.setAttribute(prefix[i], static_cast<ValueType>(values[i]));
		if(std::is_same<int, ValueType>::value){
			arrayElement.setAttribute(prefix[i], static_cast<int>(values[i]));
		}
		else if(std::is_same<double, ValueType>::value){
			arrayElement.setAttribute(prefix[i], static_cast<double>(values[i]));
		}
		else{
			arrayElement.setAttribute(prefix[i], QString::number(values[i]));
		}

	}

}

template<typename ValueType>
void Data::writeArray(QString key, QDomElement & xml, const ValueType * values, int size)
{

	QDomDocument dom = xml.ownerDocument();
	QDomElement arrayElement = dom.createElement(key);
	xml.appendChild(arrayElement);

	for (int i = 0; i < size; ++i)
	{

//		The lower code is ambiguous in g++
//		arrayElement.setAttribute(key + '_', static_cast<ValueType>(values[i]));

		if(std::is_same<int, ValueType>::value){
			arrayElement.setAttribute(key + '_', static_cast<int>(values[i]));
		}
		else if(std::is_same<double, ValueType>::value){
			arrayElement.setAttribute(key + '_' , static_cast<double>(values[i]));
		}
		else{
			arrayElement.setAttribute(key + '_', QString::number(values[i]));
		}
	}

}

/**
 * Only the following method can be used for the template function #readArray() and #writeArray().
 */
template void Data::readArray(QString key, const QDomElement& xml, char* values, int size);
template void Data::readArray(QString key, const QDomElement& xml, int* values, int size);
template void Data::readArray(QString key, const QDomElement& xml, long* values, int size);
template void Data::readArray(QString key, const QDomElement& xml, float* values, int size);
template void Data::readArray(QString key, const QDomElement& xml, double* values, int size);

template void Data::readArray(QString key, const QString* prefix, const QDomElement& xml, char* values, int size);
template void Data::readArray(QString key, const QString* prefix, const QDomElement& xml, int* values, int size);
template void Data::readArray(QString key, const QString* prefix, const QDomElement& xml, long* values, int size);
template void Data::readArray(QString key, const QString* prefix, const QDomElement& xml, float* values, int size);
template void Data::readArray(QString key, const QString* prefix, const QDomElement& xml, double* values, int size);

template void Data::writeArray(QString key, QDomElement& xml, const char* values, int size);
template void Data::writeArray(QString key, QDomElement& xml, const int* values, int size);
template void Data::writeArray(QString key, QDomElement& xml, const long* values, int size);
template void Data::writeArray(QString key, QDomElement& xml, const float* values, int size);
template void Data::writeArray(QString key, QDomElement& xml, const double* values, int size);

template void Data::writeArray(QString key, const QString* prefix, QDomElement& xml, const char* values, int size);
template void Data::writeArray(QString key, const QString* prefix, QDomElement& xml, const int* values, int size);
template void Data::writeArray(QString key, const QString* prefix, QDomElement& xml, const long* values, int size);
template void Data::writeArray(QString key, const QString* prefix, QDomElement& xml, const float* values, int size);
template void Data::writeArray(QString key, const QString* prefix, QDomElement& xml, const double* values, int size);

QString Data::getDescription() const
{
	return getAttribute(this->description).toString();
}

QStandardItem * Data::createAttribute(QString key, QVariant value, bool editable, QStandardItem * parent)
{
	if (parent == nullptr) {
		parent = this->attributesModel->invisibleRootItem();
	}

	QStandardItem* keyItem = new QStandardItem(key);
	keyItem->setEditable(false);
	QStandardItem* valueItem = new QStandardItem;
	valueItem->setEditable(editable);
	QList<QStandardItem*> list;
	list << keyItem << valueItem;
	for (int i = 2; i < MAX_COLUMNS*2; i++)
	{
		QStandardItem* nullItem = new QStandardItem;
		nullItem->setEditable(false);
		list << nullItem;
	}
	parent->appendRow(list);
	
	if (!value.isNull()) {

		valueItem->setData(value, Qt::DisplayRole);
		return valueItem;
	}
	return keyItem;
}

QList<QStandardItem*> Data::createAttributesByRows(QStringList keys, QVariantList values, bool editable, QStandardItem * parent)
{

	QList<QStandardItem*> list;
	if (keys.count() != values.count())
	{
		qCritical() << "The number of keys and values are not paired up. ";
	}
	else
	{
		for (int i = 0; i < keys.count(); i++)
		{
			list << createAttribute(keys[i], values[i], editable, parent);
		}
	}
	return list;

}

QList<QStandardItem*> Data::createAttributesByColumns(QStringList keys, QVariantList values, bool editable, QStandardItem * parent)
{
	if (parent == nullptr) {
		parent = this->attributesModel->invisibleRootItem();
	}

	QList<QStandardItem*> list;
	if (keys.count() != values.count())
	{
		qCritical() << "The number of keys and values are not paired up. ";
	}
	else
	{
		QList<QStandardItem*> _list;

		for (int i = 0; i < keys.count() ; i++)
		{
			if (i > MAX_COLUMNS) {
				qWarning() << "The number of attributes are over the MAX_COLUMNX" <<
					MAX_COLUMNS << ", and will not be display";
			}
			QStandardItem* keyItem = new QStandardItem(keys[i]);
			keyItem->setEditable(false);
			QStandardItem* valueItem = new QStandardItem;

			if (!values[i].isNull()) {
				valueItem->setData(values[i], Qt::DisplayRole);
			}

			valueItem->setEditable(editable);
			_list << keyItem << valueItem;
			list << valueItem;
		}
		parent->appendRow(_list);

	}
	return list;
}

void Data::setAttribute(QStandardItem * item, QVariant value)
{
	disconnect(item->model(), &QStandardItemModel::itemChanged,
		static_cast<Data*>(item->model()->parent()), &Data::slotMapping);
	item->setData(value, Qt::DisplayRole);
	connect(item->model(), &QStandardItemModel::itemChanged,
		static_cast<Data*>(item->model()->parent()), &Data::slotMapping);

}

QVariant Data::getAttribute(const QStandardItem * item)
{
	return item->data(Qt::DisplayRole);
}

void Data::setAttributes(QList<QStandardItem*> items, QVariantList values)
{
	if (items.count() != values.count())
	{
		qCritical() << "the numbers of items and values are not paired up. ";
		return;
	}

	for (int i = 0; i < items.count(); i++)
	{
		if (i == 0) {
			disconnect(items[i]->model(), &QStandardItemModel::itemChanged,
				static_cast<Data*>(items[i]->model()->parent()), &Data::slotMapping);
		}
		setAttribute(items[i], values[i]);
		if (i == items.size() - 1) {
			connect(items[i]->model(), &QStandardItemModel::itemChanged,
				static_cast<Data*>(items[i]->model()->parent()), &Data::slotMapping);

		}
	}
}

QVariantList Data::getAttributes(QList<QStandardItem*> items)
{
	QVariantList values;
	for (int i = 0; i < items.count(); i++)
	{
		values << getAttribute(items[i]);
	}
	return values;
}

void Data::readAttributes(const QDomElement & xml, QStandardItem * item)
{
	if (item->hasChildren())
	{
		QDomElement element = xml.firstChildElement(getAttribute(item).toString());
		
		if (item != item->model()->invisibleRootItem())
		{
			for (int i = 0; i < item->rowCount(); i++)
			{
				// only even columns have values.
				for (int j = 0; j < item->columnCount() / 2; j++)
				{
					QStandardItem* child = item->child(i, j * 2);
					readAttributes(element, child);
				}
			}
		}
		else {
			for (int i = 0; i < item->rowCount(); i++)
			{
				readAttributes(element, item->child(i));
			}
		}
	}
	else {
		QStandardItem* parent = item->parent();
		// if parent is null the item is a top level, so it use root item instead;
		if (parent == nullptr) {
			parent = item->model()->invisibleRootItem();
		}
		int row = item->row();
		int column = item->column();
		// only even column item has value, 
		// odd column item has no value.  
		if (row != -1 && column % 2 == 0)
		{
			QVariant::Type type = getAttribute(parent->child(row, column + 1)).type();
			QVariant value = QVariant(xml.attribute(item->text()));
			value.convert(type);
			setAttribute(parent->child(row, column + 1), value);
		}
	}
}

void Data::writeAttributes(QDomElement & xml, const QStandardItem * item)
{
	if (item->hasChildren())
	{
		QDomDocument dom = xml.ownerDocument();

		QDomElement newElement = dom.createElement(item->text());
		xml.appendChild(newElement);
		// if item is not a invisibleRootItem, count its column as well.
		if (item != item->model()->invisibleRootItem())
		{
			for (int i = 0; i < item->rowCount(); i++)
			{
				// only even columns have values.
				for (int j = 0; j < item->columnCount() / 2; j++)
				{
					QStandardItem* child = item->child(i, j * 2);
					writeAttributes(newElement, child);
				}
			}
		}
		// if item is a invisibleRootItem, only count row.
		else
		{
			for (int i = 0; i < item->rowCount(); i++)
			{
				writeAttributes(newElement, item->child(i));
			}
		}

	}
	// if item have no children, output its value
	else
	{
		QStandardItem* parent = item->parent();
		// if parent is null the item is a top level, so it use root item instead;
		if (parent == nullptr) {
			parent = item->model()->invisibleRootItem();
		}
		int row = item->row();
		int column = item->column();
		// only even column item has value, 
		// odd column item has no value.  
		if (row != -1 && column % 2 == 0)
		{
			QString value = getAttribute(parent->child(row, column + 1)).toString();
			xml.setAttribute(item->text(), value);
		}
	}
}

void Data::slotMapping(QStandardItem * item)
{

	if (getAttribute(item).isValid()) {
		slotFunction f = slotMapper->value(item);
		if (f != nullptr) {
			(*f)(this, item);
		}
	}

}

void Data::setUniqueName(QString uniqueName)
{
	this->uniqueName = uniqueName;
	emit uniqueNameChanged(this->uniqueName);
}
}
}
