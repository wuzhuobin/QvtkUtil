/**
 * @file	E:\ccode\QvtkUtilProject\Project\code\interactorstyle\QvtkAbstractInteractorObserver.h.
 *
 * @brief	Declares the Qvtk abstract interactor style class.
 */

#ifndef __Qvtk_ABSTRACT_INTERACTOR_STYLE_H__
#define __Qvtk_ABSTRACT_INTERACTOR_STYLE_H__

#define SYNCHRONAL_CALL(CLASSNAME, FUNCTIONS) {\
QList<QvtkAbstractInteractorObserver*> list = stylesDataBase.values(#CLASSNAME); \
foreach(QvtkAbstractInteractorObserver* _style, list) \
{ \
	CLASSNAME* style = qobject_cast<CLASSNAME*>(_style); \
	if(style && style->getCustomEnable()){ \
		FUNCTIONS; \
	} \
}}

//me
#include "abstractinteractorobserver_export.h"

// qt
#include <QObject>
#include <QPointer>
#include <QHash>

// std
#include <functional>
class QWidget;

class ABSTRACTINTERACTOROBSERVER_EXPORT QvtkAbstractInteractorObserver: public QObject
{
	Q_OBJECT;
public:

	/**
	 * @fn	virtual void QvtkAbstractInteractorObserver::setCustomEnable(bool flag)
	 *
	 * @brief	Enable custom function.
	 * When developer need enable or disable custom function for this InteractorStyle,
	 * should wrap under this method. But call the 
	 * QvtkAbstractInteractorObserver#setCustomEnable() anyway to set the flag correctly.
	 * @param	flag	True to flag.
	 */

	virtual void setCustomEnable(bool flag) { this->customFlag = flag; }

	/**
	 * @fn	virtual bool QvtkAbstractInteractorObserver::getCustomEnable()
	 *
	 * @brief	Gets custom enable.
	 * @return	True if it succeeds, false if it fails.
	 */

	virtual bool getCustomEnable() { return this->customFlag; }
	void customEnableOn() { this->setCustomEnable(true); }
	void customEnableOff() { this->setCustomEnable(false); }

	/**
	 * @fn	virtual void QvtkAbstractInteractorObserver::install();
	 *
	 * @brief	Installs QWidget and UI.
	 */

	virtual void install();

	/**
	 * @fn	virtual void QvtkAbstractInteractorObserver::uninstall();
	 *
	 * @brief	Uninstalls QWidget and UI.
	 */

	virtual void uninstall();

	/**
	 * @fn	void QvtkAbstractInteractorObserver::synchronalCall(QvtkAbstractInteractorObserver* style, void(*functionSet)(QvtkAbstractInteractorObserver* it));
	 *
	 * @brief	Synchronously call all member function which belongs to the same class
	 * This function provides the probability to call all member functions which are public 
	 * this is for when InteractorStyle controlling multiple viewers, it need to do some 
	 * asynchronouskly work.
	 * @param [in,out]	style	   	If non-null, the style.
	 * @param [in,out]	functionSet	If non-null, set the function belongs to.
	 */
	typedef void(*FunctionSet1)(QvtkAbstractInteractorObserver* it);

	static void synchronalCall(QvtkAbstractInteractorObserver* style, 
		FunctionSet1 functionSet);

	static void synchronalCall(QString styleName, 
		FunctionSet1 functionSet);

	typedef std::function<void(QvtkAbstractInteractorObserver*)> FunctionSet2;

	static void synchronalCall(QvtkAbstractInteractorObserver* style,
		FunctionSet2 functionSet);

	static void synchronalCall(QString styleName,
		FunctionSet2 functionSet);

	template<typename Style>
	using FunctionSet3 = std::function<void(Style*)>;
	
	template<typename Style>
	static void synchronalCall(FunctionSet3<Style> functionSet);

	virtual QWidget* getWidget();

protected:
	explicit QvtkAbstractInteractorObserver();
	virtual ~QvtkAbstractInteractorObserver();

	void installImp(const QMetaObject* metaObject);

	void uninstallImp(const QMetaObject* metaObject);

	QPointer<QWidget> widget; ///< The widget pointer to save the ui.

	bool customFlag;  ///< True to custom flag
	/**
	 * The database makes accessing all InteractorStyles possible. 
	 */
	static QMultiHash<QString, QvtkAbstractInteractorObserver*> stylesDataBase;	///< The styles data base

};

template<typename Style>
inline void QvtkAbstractInteractorObserver::synchronalCall(FunctionSet3<Style> functionSet)
{
	
	QList<QvtkAbstractInteractorObserver*> list = stylesDataBase.values(Style::staticMetaObject.className());
	for (QList<QvtkAbstractInteractorObserver*>::const_iterator cit = list.cbegin();
		cit != list.cend(); ++cit) {
		if ((*cit)->getCustomEnable()) {
			Style* style = static_cast<Style*>(*cit);
			functionSet(style);
		}
	}
}
#endif // !__Qvtk_ABSTRACT_INTERACTOR_STYLE_H__

