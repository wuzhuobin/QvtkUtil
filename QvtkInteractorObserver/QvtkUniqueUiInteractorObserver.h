#ifndef __Qvtk_UNIQUE_UI_INTERACTOR_STYLE_H__
#define __Qvtk_UNIQUE_UI_INTERACTOR_STYLE_H__

#include "QvtkAbstractInteractorObserver.h"

#define UNIQUE_UI_INSTALL(STYLE_NAME) \
QvtkAbstractInteractorObserver::install(); \
if (!this->getUniqueThis()) { \
	const QString uniqueName = "UNIQUE" + QString(this->metaObject()->className()); \
	stylesDataBase.insert(uniqueName, this); \
	this->widget = new QWidget; \
	this->ui = new Ui::STYLE_NAME; \
	this->getUi()->setupUi(this->widget); \
	this->uniqueInstall(); \
} \
else { \
	this->widget = getUniqueThis()->getWidget(); \
	this->ui = static_cast<STYLE_NAME*>(getUniqueThis())->getUi(); \
} 



#define UNIQUE_UI_UNINSTALL() \
if (getUniqueThis() == this) { \
	const QString uniqueName = "UNIQUE" + QString(this->metaObject()->className()); \
	stylesDataBase.remove(uniqueName); \
	this->uniqueUninstall(); \
	if (this->widget) { \
		delete this->getWidget(); \
		this->widget = nullptr; \
	} \
	delete this->getUi(); \
	this->ui = nullptr; \
} \
else { \
	this->widget = nullptr; \
	this->ui = nullptr; \
} \
QvtkAbstractInteractorObserver::uninstall();

namespace Ui { class QvtkUniqueUiInteractorObserver; }


class ABSTRACTINTERACTOROBSERVER_EXPORT QvtkUniqueUiInteractorObserver: public QvtkAbstractInteractorObserver
{
	Q_OBJECT;
public:
	Ui::QvtkUniqueUiInteractorObserver* getUi() { return this->ui; }
	// do not call #getCustomEnable() during #setCustomEnable();
	virtual void setCustomEnable(bool flag) override;
	virtual void install() override;
	virtual void uninstall() override;

	static QvtkAbstractInteractorObserver* getUniqueThis(QString className);
	static QvtkAbstractInteractorObserver* getUniqueThis(QvtkAbstractInteractorObserver* observer);
	QvtkAbstractInteractorObserver* getUniqueThis();
	template<typename Observer> 
	Observer* getUniqueThis() { return qobject_cast<Observer*>(this->getUniqueThis()); }

protected:


	explicit QvtkUniqueUiInteractorObserver();
	virtual ~QvtkUniqueUiInteractorObserver() override;
	virtual void uniqueInstall() = 0;
	virtual void uniqueUninstall() = 0;
	virtual void uniqueFunction() = 0;
	virtual void uniqueEnable() = 0;
	virtual void uniqueDisable() = 0;


	Ui::QvtkUniqueUiInteractorObserver* ui;
};


#endif // !__Qvtk_UNIQUE_UI_INTERACTOR_STYLE_H__
