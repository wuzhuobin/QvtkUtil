/**
 * @file	Data\Scene.h.
 * @brief	Declares the Util scene class.
 * @language	C++
 * @author	wuzhuobin jiejin2022@163.com
 * @date	2017/6/19
 *  for Util
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

#ifndef __QVTK_SCENE_H__
#define __QVTK_SCENE_H__

// me
#include "qvtkdata_export.h"
namespace Q {
namespace vtk {

class Data;
class DataList;
class DataBase;
class DataNameCounts;
struct SceneConfiguration;
}
}

// qt
#include <QObject>
#include <QList>

/**
 * @class	Scene
 * @brief	Util Scene.
 * @author	WUZHUOBIN
 * @date	2017/6/19
 * 
 * This class is designed for doing the IoC(Inverse of Control). It can store
 * data which inherits Data.
 * Hope it can work.
 * It also provide ability to saving the scene the software environment. 
 */
namespace Q{
namespace vtk{
class QVTKDATA_EXPORT Scene final: public QObject
{
	Q_OBJECT;
public:
	/**
	 * Declaring some static const for Scene.
	 */
	static const QString DOM_NAME; ///< QDomDocument's name. "SceneXML"
	static const QString ROOT_DIRECTORY; ///< key for the attribute where Root directory of vtkDataSet to save. "RootDirectory"
	static const QString ATTRIBUTES; ///< top level key for all attributes

	/**
	 * @fn	static int getNumberOfScene();
	 * @brief	return the total number os scene in the program. 
	 * @static
	 * @return	the number of scene. 
	 */
	static int getNumberOfScene();
	/**
	 * @fn 	static void switchToNextScene();
	 * @brief	switch to next scene.
	 * @static
	 *
	 * Switch the current scene to the next scene. 
	 */
	static void switchToNextScene(); 

	/**
	 * @fn	static Scene*::getCurrentScene();
	 * @brief	Gets current scene.
	 * @static
	 * @return	the current scene.
	 */
	static Scene* getCurrentScene();
	
	/**
	 * @fn	void printSelf() const;
	 * @brief	Print out #Scene.
	 *
	 * */
	void printSelf() const;
	/**
	 * @fn	getURL() const;
	 * @const
	 * @brief	get #URL
	 * @return	#URL
	 */
	QString getURL() const { return this->URL; }
	/**
	 * @fn	setURL();
	 * @brief	set #URL
	 * @param	url
	 */
	void setURL(QString url){ this->URL = url; }
	/**
	 * @fn	void setSceneXMLString(QString xml);
	 * @brief	directly set#sceneXMLString.
	 * @param[in]	xml set to #sceneXMLString
	 *
	 */
	void setSceneXMLString(QString xml) { this->sceneXMLString = xml; }
	/**
	 * @fn	QString getSceneXMLString() const;
	 * @brief	get #sceneXMLString.
	 * @return	#sceneXMLString
	 */
	QString getSceneXMLString() const { return this->sceneXMLString; }
	/**
	 * @fn	void setLoadFromXMLString(bool flag);
	 * @brief	whether to load XML from file instead of #sceneXMLString. 
	 * @param[in]	flag	#loadFromXMLString
	 * 
	 * whether to load XML from file, if ture, the file of #URL will not be loaded. 
	 * Otherwise, the content of #loadFromXMLString will be replaced by 
	 * the file of the #URL. 
	 */
	void setLoadFromXMLString(bool flag) { this->loadFromXMLString = flag; }
	/**
	 * @fn	bool getLoadFromXMLString() const;
	 * @brief	Get the flag whether to load XML from file instead of #sceneXMLString. 
	 * @return	#loadFromXMLString.  
	 * 
	 * whether to load XML from file, if ture, the file of #URL will not be loaded. 
	 * Otherwise, the content of #loadFromXMLString will be replaced by 
	 * the file of the #URL. 
	 */
	bool getLoadFromXMLString() const { return this->loadFromXMLString; }
	/**
	 * @fn	void setSaveToXMLString(bool flag);
	 * @brief	Set the flag whether to save XML to file
	 * @param[in]	set #saveToXMLString.
	 * 
	 * if ture, the content of XML will only save to #sceneXMLString and it will not 
	 * output to the #URL. 
	 */
	void setSaveToXMLString(bool flag) { this->saveToXMLString = flag; }
	/**
	 * @fn	bool getSaveToXMLString() const;
	 * @brief	Get the flag whether to save xml to file
	 * @return	get #saveToXMLString. 
	 * 
	 * if ture, the content of XML will only save to #sceneXMLString and it will not
	 * output to the #URL.
	 */
	bool getSaveToXMLString() const { return this->saveToXMLString; }

	/**
	 * @fn	int import(bool clearFlag = true);
	 * @brief	Create new or add scene.
	 * Add to the scene from #URL or #sceneXMLString, if clearFlag is true, clear
	 * the scene first. If #loadFromXMLString is true, load from #sceneXMLString. 
	 * Else load from #URL location.
	 * @param	clearFlag	(Optional) True to call #clear().
	 * @return	if 1 import success, otherwise fail.
	 */
	
	int import(bool clearFlag = true);

	/**
	 * @fn	int commit(QString url = QString());
	 * @brief	Save the scene.
	 * Save the scene, if #saveToXMLString is true, save the #sceneXMLString.
	 * Else, save to the #URL location.
	 * @param	url if url is null, using #URL and #rootDirectory, otherwise writting 
	 *			at url and url's current directory
	 * @return	Non-zero on success.
	 * 			
	 */

	int commit(QString rootDirectory = ".");

	/**
	 * @fn	void clear();
	 * @brief	Clear the current scene.
	 */

	void clear();

	/**
	 * @fn	void resetData();
	 * @brief	Resets the data.
	 * call OAbstractData#reset() of all data in the current scene.
	 */

	void resetData();

	/**
	 * @fn	QStringList getAllData();
	 * @brief	Get unique names of all data
	 * @return	unique names of all data.
	 *
	 */
	QStringList getAllData();
	/**
	 * @fn	QStringList getAllSelectableData();
	 * @brief	Get all selected data's Data#uniqueName. 
	 * @return	Selected data's QStringList getAllSelectableData();
	 * 
	 * Get all data in the current scene whose #selected is set to true. 
	 * Return all their's unqiue name in a QStringList.
	 */
	QStringList getAllSelectableData();

	/**
	 * @fn Data* addData(Data* data, QString name = QString());
	 * @brief	add new data in the scene
	 * @param	data the data to be added to scene
	 * @param	the name which is used to be geneerate unique name.
	 * 
	 * The data to be added should be added with a unique name which is used to retrieve the
	 * data, #addData will call #generateUniqueName to generate a unique name to insure its
	 * unique.
	 */

	Data* addData(Data* data, QString name = QString());
	/**
	 * @fn	Data* addNewDataByClass(QString className, QString name = QString());
	 * @brief	Add new data by class name.
	 * @param[in]	className the class to be created and added. 
	 * @param[in]	name base name use to create unique name, default is class name. 
	 * @return	the created data. 
	 *
	 * Create a new data by class name and then added it to the scene by a base name, whose 
	 * defult is its class name. Then return the data's pointer.
	 */
	Data* addNewDataByClass(QString className, QString name = QString());
	/**
	 * @fn	template<typename Type>
	 *		Type* addNewDataByClass(QString name = QString());
	 * @brief	Add new data by class name.
	 * @see	#createDataByClassName()
	 * @tparam	Type class type to be created and added.
	 * @param[in]	name base name use to create unique name, default is class name. 
	 * @return	the created data. 
	 *
	 * Create a new data by class type and then added it to the scene by a base name, whose 
	 * defult is its class name. Then return the data's pointer.
	 */
	template<typename Type>
	Type* addNewDataByClass(QString name = QString());

	/**
	 * @fn	Data* addNewDataByTag(QString tag, QString name = QString());
	 * @brief	Add new data by tag. 
	 * @see	#createDataByClassName()
	 * @see	#addNewDataByClass()
	 * @param[in]	tag the data's tag.
	 * @param[in]	name the data's unqiue name. 
	 * @return	the data's pointer.
	 * 
	 * Create a new data by data's tag (THE DATA MSET BE REGISTER TO SCENE FIRST) then 
	 * addede it to the scene by a base name whose default is its class name. Return 
	 * it through pointer.
	 */
	Data* addNewDataByTag(QString tag, QString name = QString());

	/**
	 * @fn	void removeData(Data* data);
	 * @brief remove data in the scene. 
	 * @see	#addNewDataByTag()
	 * @param[in]	data data's pointer.
	 * 
	 */
	void removeData(Data* data);
	/**
	 * @fn	 void removeData(QString uniqueName);
	 * @brief	remove data in the scene by unique name. 
	 * @param[in]	uniqueName The data's unique name.
	 * 
	 */
	void removeData(QString uniqueName);

	/**
	 * @fn	Data* copyData();
	 * @brief
	 * @deprecated
	 */
	Data* copyData() { return NULL; };

	/**
	 * @fn	void registerData(Data* data, QString tag = QString());
	 * @brief	registerData to the current sceen. 
	 * @param[in]	data the data pointer to register. 
	 * @param[in]	Specifiy a tag for the data instead of using the its original tag.
	 *				Default is using its original tag.
	 *
	 * Tag should be unique in one scene. Every tag should be mapped to one class, while
	 * one class can be registered in the scene with different tag.
	 * e.g. tag Nail -> class Implant ; tag Drill -> class Implant 
	 */
	void registerData(Data* data, QString tag = QString());
	/**
	 * @fn	const QStringList getRegisteredTag() const;
	 * @brief	return the registered tags. 
	 * @const
	 * @return	return #regiseredTags.
	 */
	const QStringList getRegisteredTag() const { return this->registeredTags; }
	/**
	 * @fn	QStringList getRegisteredDataClassName() const; 
	 * @brief	return all class name of registered data.
	 * @const
	 * @return	all class name of registered data in #registeredTags.	
	 */
	QStringList getRegisteredDataClassName() const;
	/**
	 * @fn	void unregisterData(QString tag);
	 * @brief	
	 * @deprecated
	 */
	void unregisterData(QString tag);
	/**
	 * @fn	void unregisterData(int i);
	 * @brief	
	 * @deprecated
	 */
	void unregisterData(int i);
	/**
	 * @fn	Data* createDataByTag(QString tag);
	 * @brief	Create data which is registed in the scene with its registerd tag.
	 * @param[in]	tag the registered tag.
	 * @return	the create data's pointer.
	 * 
	 * Create data registed in the scene. If the data has not registerd in the scene, the 
	 * data cannot be created by this way, please by its own consturctor.
	 */
	Data* createDataByTag(QString tag);
	/**
	 * @fn	QStringList getAllDataByTag(QString tag);
	 * @brief	Get all data with suitable tag. 
	 * @param[in]	tag the registered tag.
	 * @return	A QStringLists contains all suitable data's unique name. 
	 * 
	 * Reture all data's tag which is the same as the input tag, and return a QStringList contains
	 * all their unique names.
	 */
	QStringList getAllDataByTag(QString tag);

	/**
	 * @fn	QString getClassNameByTag(QString tag);
	 * @brief	Given the tag and return the class name.
	 * @param[in]	tag tag name.
	 * @return	class name.
	 */
	QString getClassNameByTag(QString tag);

	/**
	 * @fn	Data* createDataByClassName(QString className);
	 * @brief	Create a data by given a class name and return its pointer. 
	 * @param[in]	className 
	 * @return	the pointer of the class. 
	 */
	Data* createDataByClassName(QString className);

	/**
	 * @fn	template<typename Type> 
	 *		Type* createDataByClassName();
	 * @brief	template version #createDataByClassName()
	 * @tparam	Type the Type which should inherit Data.
	 * @return	the pointer of the class. 
	 * 
	 * return the pointer of the class with casting. No need to do static_cast 
	 * manually. 
	 */
	template<typename Type> 
	Type* createDataByClassName();

	/**
	 * @fn	QStringList getAllDataByClassName(QString className);
	 * @brief	Get all data in the scene which its class is className
	 *			and return its unique name in a QStringList.
	 * @param[in[	className class name to be find in the scene. 
	 * @return	all unique names of the data.	
	 */
	QStringList getAllDataByClassName(QString className, bool exactly = false);
	
	/**
	 * @fn	QString getTagByClassName(QString className);
	 * @brief	Given class name and return the tag. 
	 * @param[in]	className 
	 * @return	the tag of the registerd class. 
	 * 
	 * Since one class can be registered more than one time with different tag, 
	 * this function only return the tag registerd in the first time. 
	 */
	QString getTagByClassName(QString className);

	/**
	 * @fn	Data* getDataByUniqueName(QString name);
	 * @brief	Get data pointer by unique name.
	 * @param[in]	name the unique name of the data needed to be retrieve
	 * @return	data pointer.
	 * 
	 * Since every data add to the sceene has a unique name, this is a useful function 
	 * to retrieve any data which is add to the scene. 
	 */
	Data* getDataByUniqueName(QString name);

	/**
	 * @fn	template<typename Type>
	 *		Type* getDataByUniqueName(QString name);
	 * @brief	Template version of #getDataByUniqueName() which can do casting. 
	 * @tparam	Type the type needed to be cast to.
	 * @param	name the unique name.
	 * @return	the casted data pointer.
	 * Since no information of the type can be got from unique name, this template version
	 * is just done qobject_cast. If woring Type is used, the pointer will be nullptr.
	 */
	template<typename Type>
	Type* getDataByUniqueName(QString name);

	/**
	 * @fn	Data* getDataByAlias(QString alias);
	 * @brief	Get Data By its alias. 
	 * @param[in]	alias 
	 * @return	The data pointer.
	 * 
	 * Get the data using its alias,  while alias is not unique. So it will return the first data whose alias is 
	 * matched. 
	 */
	Data* getDataByAlias(QString alias);
	
	/**
	 * @fn	template<typename Type>
	 *		Type* getDataByAlias(QString alias);
	 * @brief	Adding casting, the same as #getDataByAlias(). 
	 * @see	#getDataByAlias().
	 * @tparam	Type the type needed to be cast to. 
	 * @param[in]	alias the alias of the data. 
	 * @return	the casted data pointer. 
	 * 
	 * The same function as #getDataByAlias();
	 */
	template<typename Type>
	Type* getDataByAlias(QString alias);

	/**
	 * @fn	QStringList getAllDataByAlias(QString alias);	
	 * @brief	Get all data whose alias are matched. 
	 * @see	#getDataByAlias().
	 * @param[in]	alias
	 * @return	A list contains all data's uniqueName. 
	 * 
	 * Get all data using this alias. 
	 */
	QStringList getAllDataByAlias(QString alias);

	/**
	 * @fn	QString generateUniqueName(Data* data);
	 * @brief	generate an unique name by given data pointer. 
	 * @param[in]	data the data used in generating unique name. 
	 * @return	the generated unique name. 
	 * 
	 */
	QString generateUniqueName(Data* data);

	/**
	 * @fn	QString generateUniqueName(QString baseName);
	 * @brief	generate an unique name by given a base name. 
	 * @param[in]	baseName Base name to generate an unique name. 
	 * @return	the generated unique name. 
	 */
	QString generateUniqueName(QString baseName);
	/**
	 * @fn	QString xmlUniqueNameCheck(QString xml);
	 * @brief	Checking xml's unique name related attributes. 
	 * @param[in]	xml 
	 * 
	 * Checking the xml, updating the unique name related attributes, if it is repeated, update
	 * it with new value.
	 */
	QString xmlUniqueNameCheck(QString xml);

signals:

	void signalImported(); ///< signal emited when calling #import().
	void signalCleared(); ///< signal emited when calling #clear().
	void signalAddedData(QString uniqueName); ///< signal emited when calling #addData().
	void signalRemovedData(QString uniqueName); ///< signal emited when calling #removeData().
	


private:

	friend struct SceneConfiguration; ///< Registeration of the Scene and the whole software's configuration.
	/**
	 * @fn	explicit Scene(QObject* parent = nullptr);
	 * @brief	Constructor.
	 */

	explicit Scene();

	/**
	 * @fn	virtual ~Scene() override;
	 * @brief	Destructor.
	 * @override
	 */

	virtual ~Scene() override;

	/**
	 * @fn	int getUniqueNameIndex(QString baseName);
	 * @brief	Get the unique name index from #uniqueNames
	 * @param[in]	baseName The base name for finding the index. 
	 * @return	the times of the base name has already been used. 
	 * 
	 * To get the index of current base name have already been used. Then scene can 
	 * call #buildName() with the times of the base name been used by "baseName_time"
	 */
	int getUniqueNameIndex(QString baseName);
	/**
	 * @fn	QString buildName(QString baseName, int nameIndex);
	 * @brief	build the unique name by base name and the nameIndex. 
	 * @param[in]	baseName the base name to build unique name. 
	 * @param[in]	name index of the base name. 
	 * return	the unqiue name.
	 * 
	 * To build unqiue name with base name and name index to insure the unique name is really 
	 * an unique. The scene has saved the used base name and how many time it has used before in 
	 * #uniqueNames. With this information, it builds unique name by "baseName_time". 
	 * e.g.  "ImageA" = buildName("ImageA", 0) "ImageA_1" = buildName("ImageA", 2) 
	 */
	QString buildName(QString baseName, int nameIndex);

	QString URL;  ///< URL of the *.xml file

	DataNameCounts* uniqueNames;   ///< To contain registered uniqueName, int for the times of uniqueNames emerged

	QStringList registeredTags;   ///< The list for saving registered tags

	DataList* registeredData;  ///< Using index to fast access the registerdData

	DataBase* dataBase;   ///< the dataBase to access the containing data

	QString errorMessage; ///< Message describing the error

	QString sceneXMLString;   ///< The scene XML string

	bool loadFromXMLString;   ///< True to load from XML string

	bool saveToXMLString; ///< True to save to XML string

};


template<typename Type>
inline Type * Scene::addNewDataByClass(QString name)
{
	Data* data = addNewDataByClass(Type::getClassNameS(), name);
	return static_cast<Type*>(data);
}

template<typename Type>
inline Type * Scene::createDataByClassName()
{
	Data* data = createDataByClassName(Type::getClassNameS());
	return static_cast<Type*>(data);
}

template<typename Type>
inline Type* Scene::getDataByUniqueName(QString name)
{
	Data* data = getDataByUniqueName(name);
	return qobject_cast<Type*>(data);
}

template<typename Type>
inline Type* Scene::getDataByAlias(QString alias)
{
	return qobject_cast<Type*>(Scene::getDataByAlias(alias));
}

}
}
#endif // !__QVTK_SCENE_H__

