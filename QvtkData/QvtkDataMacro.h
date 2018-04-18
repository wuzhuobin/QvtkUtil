#ifndef __Qvtk_ABSTRACT_DATA_MACRO_H__
#define __Qvtk_ABSTRACT_DATA_MACRO_H__


/**
 * Declaring attributes' keys
 */
#define Q_VTK_KEY(KEY) \
const QString KEY = #KEY;

/**
 * @fn	virtual QString getTag() const = 0;
 * @brief	Gets the tag.
 * @const
 * @return	The tag.
 */

/**
 * @fn	static QString getClassNameS(); 
 * @brief	static method return class name.
 * @static
 * @return	class name
 *
 * Providing convenient method the same as QOBject#staticMetaObject()#className().
 */

#define Q_VTK_DATAH(CLASS, ...) \
public: \
static QString getClassNameS(){return staticMetaObject.className();} \
static const struct CLASS##Keys \
{ ##__VA_ARGS__ } K;\

//#define Qvtk_NEW_INSTANCE(CLASS) 
#define Q_VTK_DATACPP(CLASS) \
const CLASS::CLASS##Keys CLASS::K; 



/* Define string for QVariant conversion, LIST NOT COMPLETE! */
#define Qvtk_todouble    toDouble
#define Qvtk_tofloat     toFloat
#define Qvtk_toint       toInt
#define Qvtk_tochar      toChar
#define Qvtk_tobool      toBool

/* This should be used at the begining of the class using the macros*/
#define Qvtk_DATASTRUCTURE(thisclass) \
    typedef thisclass ThisClass;

#ifdef _MSC_VER
/* For initializing non-array properties. Call in constructor */
#define Qvtk_DATA_CREATE_PROPERTIES_ATTRIBUTES_MACRO(propertytype, variablename, namestring, initvalue) \
	this->##variablename = createAttribute(namestring, static_cast<##propertytype>(initvalue), true); \
	insertSlotFunction(this->##variablename, &ThisClass::set##variablename)
	

/* For initializing array properties. Call in constructor, default to all being 0 */
#define Qvtk_DATA_CREATE_PROPERTIES_ARRAY_ATTRIBUTES_MACRO(propertytype, variablename, namestring, valuestringarray, len) \
	QStandardItem* standardItem##variablename = createAttribute(namestring); \
	QStringList keys##variablename; \
	QVariantList Values##variablename; \
    for (int i = 0; i < len; i++) \
	{ \
		keys##variablename << (valuestringarray[i]); \
		Values##variablename << static_cast<propertytype>(0); \
	} \
	this->##variablename = createAttributesByColumns(keys##variablename, Values##variablename, true, standardItem##variablename);
#else
#endif

/* Implement a protected updateByXXX() function for automatic update */
#ifdef _MSC_VER
#define Qvtk_DATA_DISPLAYED_PROPERTIES_STANDARD_SET_MACRO(propertytype, variablename) \
    void set##variablename(propertytype var) { \
        setAttribute(this->##variablename, var); \
        this->updateBy##variablename(); \
    } \
    \
    static void set##variablename(Data * self, QStandardItem * item) { \
        ThisClass* _self = static_cast<ThisClass*>(self); \
        _self->set##variablename(getAttribute(item).Qvtk_to##propertytype()); \
    }

#define Qvtk_DATA_HIDDEN_PROPERTIES_STANDARD_SET_MACRO(propertytype, variablename) \
    void set##variablename(propertytype var) { \
        setAttribute(this->##variablename, var); \
        this->updateBy##variablename(); \
    } 

#define Qvtk_DATA_PROPERTIES_STANDARD_GET_MACRO(propertytype, variablename) \
    propertytype get##variablename() const { \
        return getAttribute(this->##variablename).Qvtk_to##propertytype(); \
    }

#else
// #todo: write linux macro
#endif


#ifdef _MSC_VER
#define Qvtk_DATA_PROPERTIES_ARRAY_SET_MACRO(propertytype, variablename, len) \
    void set##variablename(const propertytype var[len]) {\
        QVariantList* l = new QVariantList(); \
        for(int i = 0; i < len; i++) { \
            *l << var[i]; \
        } \
        setAttributes(this->##variablename, *l); \
        this->updateBy##variablename(); \
    } \
    static void set##variablename(Data* self, QStandardItem* item) { \
        ThisClass* _self = static_cast<ThisClass*>(self); \
        propertytype var[3]; \
        _self->getHoverColor(var); \
        _self->setHoverColor(var); \
    };
#else
// #todo: write linux macro
#endif

#ifdef _MSC_VER
#define Qvtk_DATA_PROPERTIES_ARRAY_GET_MACRO(propertytype, variablename, len) \
    void get##variablename(propertytype outVar[len]) const { \
        QVariantList l = getAttributes(this->##variablename ); \
        for (int i = 0; i < len; i++) { \
            outVar[i] = l[i].Qvtk_to##propertytype (); \
        } \
    } \
	##propertytype* get##variablename() {\
		propertytype* outvar = new propertytype[len]; \
		get##variablename(outvar); \
		return outvar; \
	}
#else
// #todo: write linux macro
#endif

#ifdef _MSC_VER
#define Qvtk_DATA_PROPERTIES_3_ARRAY_MACRO(propertytype, variablename) \
    Qvtk_DATA_PROPERTIES_ARRAY_GET_MACRO(propertytype, variablename, 3); \
    Qvtk_DATA_PROPERTIES_ARRAY_SET_MACRO(propertytype, variablename, 3); \
     \
    virtual void set##variablename(propertytype v1, propertytype v2, propertytype v3) {\
        propertytype var[3]{v1, v2, v3}; \
        set##variablename(var); \
    } 
#else
#endif


#define Qvtk_PRINT_PROPERTY_VECTOR(displaystring, variablename, len) \
    QString msg##variablename = "\t"; \
    for (int i = 0; i < len; i++) { \
        msg##variablename += QString(getAttribute(this->##variablename [i]).toString()) + QString("\t"); \
    } \
    qDebug() << ##displaystring << ":" << msg##variablename;

#define Qvtk_QVARIANTLIST_3_TO_CSV_STRING(qvariantlist) \
	QString( \
		  ##qvariantlist[0].toString() + QString(",") \
		+ ##qvariantlist[1].toString() + QString(",") \
		+ ##qvariantlist[2].toString()) 

#define Qvtk_QVARIANTLIST_4_TO_CSV_STRING(qvariantlist) \
	QString( \
		  ##qvariantlist[0].toString() + QString(",") \
		+ ##qvariantlist[1].toString() + QString(",") \
		+ ##qvariantlist[2].toString() + QString(",") \
		+ ##qvariantlist[3].toString()) 

#define Qvtk_QVARIANTLIST_5_TO_CSV_STRING(qvariantlist) \
	QString( \
		  ##qvariantlist[0].toString() + QString(",") \
		+ ##qvariantlist[1].toString() + QString(",") \
		+ ##qvariantlist[2].toString() + QString(",") \
		+ ##qvariantlist[3].toString() + QString(",") \
		+ ##qvariantlist[4].toString()) 

#endif