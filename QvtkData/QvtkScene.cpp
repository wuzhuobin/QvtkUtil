#include "QvtkScene.h"

// me
#include "QvtkData.h"

// qt
#include <QHash>
#include <QList>
#include <QSharedPointer>
#include <QDir>
#include <QFileInfo>
#include <QDomDocument>
#include <QTextStream>
#include <QDebug>

namespace Q{
namespace vtk{
class DataList : public QList<QSharedPointer<Data>> {};
class DataBase : public QHash<QString, QSharedPointer<Data>> {};
class DataNameCounts : public QHash<QString, int> {};

const QString Scene::DOM_NAME = "SceneXML";
const QString Scene::ROOT_DIRECTORY = "RootDirectory";
const QString Scene::ATTRIBUTES = "Attributes";
QList<Scene*> selves;
QList<Scene*>::iterator current;

Scene::Scene()
	:QObject(nullptr)
{
	this->registeredData = new DataList;
	this->dataBase = new DataBase;
	this->uniqueNames = new DataNameCounts;
	this->loadFromXMLString = false;
	this->saveToXMLString = false;
	clear();

	selves.push_back(this);
	if (selves.size() == 1) {
		current = selves.begin();
	}
}

Scene::~Scene()
{
	if (this->uniqueNames)
	{
		delete this->uniqueNames;
	}
	if (this->dataBase)
	{
		delete this->dataBase;
	}
	if (this->registeredData)
	{
		delete this->registeredData;
	}
}

int Scene::getNumberOfScene()
{
	return selves.size();
}

void Scene::switchToNextScene()
{
	++current;
	if (current == selves.end())
	{
		current = selves.begin();
	}
}

Scene * Scene::getCurrentScene()
{
	return *current;
}

int Scene::import(bool clearFlag)
{
	if (clearFlag)
	{
		clear();
	}
	
	if ((this->URL.isEmpty() && !this->loadFromXMLString) ||
		(this->loadFromXMLString && this->sceneXMLString.isEmpty()))
	{
		//qDebug() << this->sceneXMLString;
		qCritical() <<"No URL specified";
		return 0;
	}
	
	if(!this->loadFromXMLString)
	{

		QFile file(this->URL);
		if (!file.open(QIODevice::ReadOnly))
		{
			qCritical() << QString("Cannot open file %1").arg(this->URL);
			return 0;
		}
		this->sceneXMLString = file.readAll();
		file.close();
	}

	// load don's content to #sceneXMLString.
	QDomDocument dom(DOM_NAME);
	// doing xml uniqueChecking
	//qDebug() << this->sceneXMLString;
	dom.setContent(this->xmlUniqueNameCheck(this->sceneXMLString));
	//qDebug() << dom.toString();

	QDomElement root = dom.documentElement();
	QString rootDirectory = root.attribute(ROOT_DIRECTORY);
	
	// if rootDirectory is empty, set it to program's current path. 
	if (rootDirectory.isEmpty()) {
		rootDirectory = ".";
	}
	// check whether the rootDirectory is relative or not.
	// if it is relative, convert it to absolute basing #URL.
	if (QDir::isRelativePath(rootDirectory)) {
		rootDirectory = QFileInfo(this->URL).absoluteDir().absoluteFilePath(rootDirectory);
		//rootDirectory = QFileInfo(QFileInfo(this->URL).absolutePath() +
		//		'/' + rootDirectory).canonicalFilePath() + '/';
	}

	QDomElement element = root.firstChildElement();
	while (!element.isNull()) {
		QString tag = element.tagName();
		QDomElement attributes = element.firstChildElement(ATTRIBUTES);
		Data* data = createDataByTag(tag);
		data->readXML(attributes, rootDirectory);
		addData(data, attributes.attribute(Data::K.UniqueName));
		element = element.nextSiblingElement();
	}
	emit signalImported();
	return 1;
}

int Scene::commit(QString rootDirectory)
{
	
	if ((this->URL.isEmpty())
		&& !this->saveToXMLString) {
		qCritical() << "URL is not set";
		return 0;
	}

	// if rootDirectory is empty, set it to program's current path. 
	if (rootDirectory.isEmpty()) {
		rootDirectory = ".";
	}

	QDomDocument dom(DOM_NAME);
	QDomElement root = dom.createElement(DOM_NAME);
	root.setAttribute(ROOT_DIRECTORY, rootDirectory);
	dom.appendChild(root);

	// check whether the rootDirectory is relative or not.
	// if it is relative, convert it to absolute basing #URL.
	if (QDir::isRelativePath(rootDirectory)) {
		rootDirectory = QFileInfo(this->URL).absoluteDir().absoluteFilePath(rootDirectory);
		//rootDirectory = QFileInfo(QFileInfo(this->URL).absolutePath() +
		//				'/' + rootDirectory).absoluteFilePath() + '/';
	}

	// if the folder does not exist, create one
	if (!QDir().mkpath(rootDirectory)) {
		qCritical() << "Cannot create path: " << rootDirectory;
	}

	DataBase::const_iterator cit = this->dataBase->cbegin();
	while (cit != this->dataBase->cend())
	{
		QDomElement element = dom.createElement(getTagByClassName((*cit)->getClassName()));
		root.appendChild(element);
		QDomElement attributes = dom.createElement(ATTRIBUTES);
		element.appendChild(attributes);
		attributes.setAttribute(Data::K.UniqueName, cit.value()->uniqueName);
		cit.value()->writeXML(attributes, rootDirectory);

		++cit;
	}

	this->sceneXMLString = dom.toString();
	if (!this->saveToXMLString)
	{
		QFile file(this->URL);
		if (!file.open(QFile::WriteOnly | QFile::Text))
		{
			qCritical() << QString("Cannot write file %1").arg(this->URL);
			return 0;
		}

		QTextStream(&file) << this->sceneXMLString;
		file.close();

	}
	return 1;
	
}

void Scene::clear()
{
	for (DataBase::const_iterator cit = this->dataBase->cbegin();
		cit != this->dataBase->cend(); ++cit) {
		emit signalRemovedData(cit.key());
	}
	this->dataBase->clear();
	this->uniqueNames->clear();
	emit signalCleared();
}

QString Scene::generateUniqueName(Data * data)
{
	if (!data)
	{
		qWarning() <<"Input is null";
		return generateUniqueName("NULL");
	}
	QString baseName = this->getTagByClassName(data->getClassName());
	return generateUniqueName(baseName);
}

QString Scene::generateUniqueName(QString baseName)
{
	int uniqueIDIndex = this->getUniqueNameIndex(baseName);
	// Register ID so it won't be available next time GetUniqueIDIndex is called
	(*this->uniqueNames)[baseName] = uniqueIDIndex;
	// Create the unique ID
	return this->buildName(baseName, uniqueIDIndex);
}

QString Scene::xmlUniqueNameCheck(QString xml)
{
	// load don's content to #sceneXMLString.
	QDomDocument dom(DOM_NAME);
	dom.setContent(xml);
	if (this->uniqueNames->size() > 0) {
		const QDomElement& root = dom.documentElement();
		QDomElement element1 = root.firstChildElement();
		while (!element1.isNull()) {
			// get the old name. 
			QString oldName = element1.firstChildElement(ATTRIBUTES).attribute(Data::K.UniqueName);
			// generate new unique name but not register it. Registering only happens in #addData.
			QString newName = this->buildName(oldName, this->getUniqueNameIndex(oldName));
			// loop over all element and call every element's Data#updateUniqueReferences();
			// for update xml. 
			QDomElement element2 = root.firstChildElement();
			while (!element2.isNull())
			{
				QString tag = element2.tagName();
				QDomElement xml = element2.firstChildElement(ATTRIBUTES);
				Data* data = createDataByTag(tag);
				data->updateUniqueReferences(oldName, newName, xml);

				delete data;
				element2 = element2.nextSiblingElement();
			}
			element1 = element1.nextSiblingElement();
		}
	}

	return dom.toString();
}

int Scene::getUniqueNameIndex(QString baseName)
{
	if (baseName.isEmpty())
	{
		qWarning() << "baseName is invalid";
	}
	int lastNameIndex = -1;
	DataNameCounts::const_iterator cit = this->uniqueNames->find(baseName);
	if (cit != this->uniqueNames->cend())
	{
		lastNameIndex = cit.value();
	}
	bool isUnique = false;
	int index = lastNameIndex;
	// keep looping until you find a name that isn't yet in the scene
	// TODO: this could be speeded up if it becomes a bottleneck
	while (!isUnique)
	{
		++index;
		QString candidateName = this->buildName(baseName, index);
		isUnique = !this->dataBase->contains(candidateName);
	}
	return index;
}

QString Scene::buildName(QString baseName, int nameIndex)
{
	QString name = baseName;
	if (nameIndex > 0)
	{
		// first name is "foo", then "foo_1", then "foo_2"
		name = name + "_" + QString::number(nameIndex);
	}
	return name;
}

void Scene::resetData()
{
	DataBase::const_iterator cit = this->dataBase->constBegin();
	while (cit != this->dataBase->constEnd())
	{
		cit.value()->reset();
		++cit;
	}
}

QStringList Scene::getAllData()
{
	return this->dataBase->keys();
}

QStringList Scene::getAllSelectableData()
{
	QStringList selectableData;
	
	DataBase::const_iterator cit = this->dataBase->constBegin();
	while (cit != this->dataBase->constEnd())
	{
		if (cit.value()->getSelectable())
		{
			selectableData << cit.key();
		}
		++cit;
	}

	return selectableData;
}

Data* Scene::addData(Data * data, QString name)
{
	if (!data)
	{
		qCritical() << "unable to add a null data to the scene";
		return nullptr;
	}
	if (data->addedToScene)
	{
		qWarning() << "data has been added";
		return nullptr;
	}

	data->addedToScene = true;
	// setParent to find children by alias
	//data->setParent(this);

	if (name.isEmpty()) 
	{
		data->setUniqueName(generateUniqueName(data));
	}
	else
	{
		data->setUniqueName(generateUniqueName(name));
	}
	// push to the database
	(*dataBase)[data->uniqueName] = QSharedPointer<Data>(data, &QObject::deleteLater);
	emit signalAddedData(data->uniqueName);
	return data;
}

Data * Scene::addNewDataByClass(QString className, QString name)
{
	Data* data = this->createDataByClassName(className);
	return addData(data, name);
}

Data * Scene::addNewDataByTag(QString tag, QString name)
{
	Data* data = this->createDataByTag(tag);
	return addData(data, name);
}

void Scene::removeData(Data * data)
{
	if (!data->addedToScene)
	{
		qWarning() << "data has alread been removed before or"
			"has not added";
		return;
	}

	// emit before it is really removed, otherwise the scene widget cannot
	// find its tag.
	emit signalRemovedData(data->uniqueName);
	data->addedToScene = false;
	//data->setParent(nullptr);
	this->dataBase->remove(data->uniqueName);
}

void Scene::removeData(QString uniqueName)
{
	if (!dataBase->contains(uniqueName))
	{
		qWarning() << "data has alread been removed before or"
			"has not added";
		return;
	}
	else
	{
		Data* data = dataBase->value(uniqueName).data();
		removeData(data);
	}
}

void Scene::registerData(Data * data, QString tag)
{
	if (!data)
	{
		qCritical() << "can't register a null";
		return;
	}

	if (tag.isEmpty())
	{
		tag = data->getClassName().remove(0, 8);
	}
	if (tag.isEmpty())
	{
		qCritical()	<< ": can't register a null tag name";
		return;
	}
	// Replace the previously registered node if any.
	// By doing so we make sure there is no more than 1 node matching a given
	// XML tag. It allows plugins to MRML to overide default behavior when
	// instantiating nodes via XML tags.
	for (int i = 0; i < this->registeredTags.size() && i < this->registeredData->size(); ++i)
	{
		if (this->registeredTags[i] == tag)
		{
			qWarning() << "Tag " << tag
				<< " has already been registered, unregistering previous node class "
				<< " to register the new class";
			// As the node was previously Registered to the scene, we need to
			// unregister it here. It should destruct the pointer as well (only 1
			// reference on the node).
			// Remove the outdated reference to the tag, it will then be added later
			// (after the for loop).
			// we could have replace the entry with the new node also.
			this->unregisterData(i);
			// we found a matching tag, there is maximum one in the list, no need to
			// search any further
			break;
		}
		if (this->registeredData->at(i)->getClassName() == data->getClassName()) {
			qWarning() << "Class" << data->getClassName() <<
				"has already been registered with tag " << tag <<
				". Unregistering previous node tag to register the new tag.";
			this->unregisterData(i);
			break;
		}
	}
	this->registeredData->push_back(QSharedPointer<Data>(data, &QObject::deleteLater));
	this->registeredTags.push_back(tag);
}

QStringList Scene::getRegisteredDataClassName() const
{
	QStringList className;

	for (int i = 0; i < this->registeredData->size(); i++)
	{
		className << this->registeredData->at(i)->getClassName();
	}

	return className;
}

void Scene::unregisterData(QString tag)
{
	int index = this->registeredTags.indexOf(tag);
	if (index < 0) {
		qWarning() << "Tag does not exist";
		return;
	}
	unregisterData(index);
}

void Scene::unregisterData(int i)
{
	if (i > this->registeredData->size() || i > this->registeredTags.size())
	{
		qWarning() << "Out of index";
	}

	this->registeredData->removeAt(i);
	this->registeredTags.removeAt(i);
}

Data * Scene::createDataByTag(QString tag)
{
	if (tag.isEmpty())
	{
		qCritical() << "tag is empty";
		return nullptr;
	}
	Data* data = NULL;
	int index = this->registeredTags.indexOf(tag);
	
	if (index < 0)
	{
		qCritical() << "tag is not registered";
		return nullptr;
	}

	data = this->registeredData->at(index)->newInstance();
	return data;

}

QStringList Scene::getAllDataByTag(QString tag)
{
	QStringList dataName;
	if (!this->registeredTags.contains(tag)) {
		qWarning() << "tag is not registered";
	}
	else
	{
		// find out a correct class
		int index = this->registeredTags.indexOf(tag);
		QString className = this->registeredData->at(index)->getClassName();

		dataName << getAllDataByClassName(className, true);
	}
	return dataName;
}

QString Scene::getClassNameByTag(QString tag)
{
	if (tag.isEmpty())
	{
		qCritical() << "tag is null";
		return QString();
	}

	int index = this->registeredTags.indexOf(tag);
	if (index < 0)
	{
		qCritical() << "tag is not registered";
		return QString();
	}
	
	return registeredData->at(index)->getClassName();
}

Data * Scene::createDataByClassName(QString className)
{
	QString tag = getTagByClassName(className);
	if (tag.isEmpty())
	{
		qWarning() << "tag is not registered";

		int id = QMetaType::type(className.toUtf8());
		if (id == QMetaType::UnknownType)
		{
			return nullptr;
		}
		return QVariant::fromValue(QMetaType::create(id)).value<Data*>();
	}

	return createDataByTag(tag);
}

QStringList Scene::getAllDataByClassName(QString className, bool exactly)
{
	QStringList dataNames;
	if (!className.isEmpty()) {

		DataBase::const_iterator cit = this->dataBase->constBegin();
		while (cit != this->dataBase->constEnd())
		{
			
			if (cit.value()->isClass(className))
			{
				if (!exactly || className == cit.value()->getClassName()) {
					dataNames << cit.key();
				}
			}
			++cit;
		}
	}

	return dataNames;
}

QString Scene::getTagByClassName(QString className)
{
	if (className.isEmpty())
	{
		qCritical() << "className is null";
		return QString();
	}

	for (int i = 0; i < this->registeredData->size(); i++)
	{
		Data* data = this->registeredData->at(i).data();
		if (data->getClassName() == className)
		{
			return registeredTags.at(i);
		}
	}
	qCritical() << "tag is not registered";
	return QString();
}

Data * Scene::getDataByUniqueName(QString name)
{
	return	this->dataBase->value(name).data();
}

Data * Scene::getDataByAlias(QString alias)
{
	for (DataBase::const_iterator cit = this->dataBase->cbegin();
		cit != this->dataBase->cend(); ++cit) {
		if (cit.value()->getAlias() == alias) {
			return cit.value().data();
		}
		
	}

	return nullptr;
}

QStringList Scene::getAllDataByAlias(QString alias)
{
	QStringList allData;
	for (DataBase::const_iterator cit = this->dataBase->cbegin();
		cit != this->dataBase->cend(); ++cit) {
		if (cit.value()->getAlias() == alias) {
			allData << cit.key();
		}
		
	}
	return allData;
}

void Scene::printSelf() const
{
	qDebug() << "ClassName" << '=' << this->metaObject()->className();
	qDebug() << "URL" << '=' << this->URL;
	qDebug() << "uniqueNames" << '=' << this->uniqueNames;
	qDebug() << "registeredTags" << '=' << this->registeredTags;
	qDebug() << "registeredData" << '=' << this->registeredData;
	qDebug() << "errorMessage" << '=' << this->errorMessage;
	qDebug() << "sceneXMLString" << '=' << this->sceneXMLString;
	qDebug() << "loadFromXMLString" << '=' << this->loadFromXMLString;
	qDebug() << "saveToXMLString" << '=' << this->saveToXMLString;

	qDebug() << "dataBase" << '=' << this->dataBase;
	qDebug() << '\n';

	for(DataBase::const_iterator cit = this->dataBase->cbegin();
			cit != this->dataBase->cend(); ++cit){
		qDebug() << cit.key();
		cit.value()->printSelf();
	}

}

}
}
