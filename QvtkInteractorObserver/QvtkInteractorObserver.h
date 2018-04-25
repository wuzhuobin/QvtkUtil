/**
 * @file	E:\ccode\QvtkUtilProject\Project\code\interactorobserver\InteractorObserver.h.
 *
 * @brief	Declares the Qvtk abstract interactor observer class.
 */

#ifndef __QVTK_INTERACTOR_STYLE_H__
#define __QVTK_INTERACTOR_STYLE_H__

#define SYNCHRONAL_CALL(CLASSNAME, FUNCTIONS) {\
QList<InteractorObserver*> list = observersDataBase.values(#CLASSNAME); \
foreach(InteractorObserver* _observer, list) \
{ \
	CLASSNAME* observer = qobject_cast<CLASSNAME*>(_observer); \
	if(observer && observer->getCustomEnable()){ \
		FUNCTIONS; \
	} \
}}

//me
#include "qvtkinteractorobserver_export.h"

// qt
#include <QObject>
#include <QPointer>
#include <QHash>

// std
#include <functional>
class QWidget;
namespace Q {
namespace vtk{
class QVTKINTERACTOROBSERVER_EXPORT InteractorObserver: public QObject
{
	Q_OBJECT;
public:

	/**
	 * @fn	virtual void InteractorObserver::setCustomEnable(bool flag)
	 *
	 * @brief	Enable custom function.
	 * When developer need enable or disable custom function for this InteractorStyle,
	 * should wrap under this method. But call the 
	 * InteractorObserver#setCustomEnable() anyway to set the flag correctly.
	 * @param	flag	True to flag.
	 */

	virtual void setCustomEnable(bool flag) { this->customFlag = flag; }

	/**
	 * @fn	virtual bool InteractorObserver::getCustomEnable()
	 *
	 * @brief	Gets custom enable.
	 * @return	True if it succeeds, false if it fails.
	 */

	virtual bool getCustomEnable() { return this->customFlag; }
	void customEnableOn() { this->setCustomEnable(true); }
	void customEnableOff() { this->setCustomEnable(false); }

	/**
	 * @fn	virtual void InteractorObserver::install();
	 *
	 * @brief	Installs QWidget and UI.
	 */

	virtual void install();

	/**
	 * @fn	virtual void InteractorObserver::uninstall();
	 *
	 * @brief	Uninstalls QWidget and UI.
	 */

	virtual void uninstall();

	/**
	 * @fn	void InteractorObserver::synchronalCall(InteractorObserver* observer, void(*functionSet)(InteractorObserver* it));
	 *
	 * @brief	Synchronously call all member function which belongs to the same class
	 * This function provides the probability to call all member functions which are public 
	 * this is for when InteractorStyle controlling multiple viewers, it need to do some 
	 * asynchronouskly work.
	 * @param [in,out]	observer	   	If non-null, the observer.
	 * @param [in,out]	functionSet	If non-null, set the function belongs to.
	 */
	typedef void(*FunctionSet1)(InteractorObserver* it);

	static void synchronalCall(InteractorObserver* observer, 
		FunctionSet1 functionSet);

	static void synchronalCall(QString observerName, 
		FunctionSet1 functionSet);

	typedef std::function<void(InteractorObserver*)> FunctionSet2;

	static void synchronalCall(InteractorObserver* observer,
		FunctionSet2 functionSet);

	static void synchronalCall(QString observerName,
		FunctionSet2 functionSet);

	template<typename Observer>
	using FunctionSet3 = std::function<void(Observer*)>;
	
	template<typename Observer>
	static void synchronalCall(FunctionSet3<Observer> functionSet);

	virtual QWidget* getWidget();

protected:
	explicit InteractorObserver();
	virtual ~InteractorObserver();

	void installImp(const QMetaObject* metaObject);

	void uninstallImp(const QMetaObject* metaObject);

	QPointer<QWidget> widget; ///< The widget pointer to save the ui.

	bool customFlag;  ///< True to custom flag
	/**
	 * The database makes accessing all InteractorStyles possible. 
	 */
	static QMultiHash<QString, InteractorObserver*> observersDataBase;	///< The observers data base

};

template<typename Observer>
inline void InteractorObserver::synchronalCall(FunctionSet3<Observer> functionSet)
{
	
	QList<InteractorObserver*> list = observersDataBase.values(Observer::staticMetaObject.className());
	for (QList<InteractorObserver*>::const_iterator cit = list.cbegin();
		cit != list.cend(); ++cit) {
		if ((*cit)->getCustomEnable()) {
			Observer* observer = static_cast<Observer*>(*cit);
			functionSet(observer);
		}
	}
}
}
}
#endif // !__QVTK_INTERACTOR_STYLE_H__

