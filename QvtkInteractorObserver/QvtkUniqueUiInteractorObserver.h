#ifndef __QVTK_UNIQUE_UI_INTERACTOR_STYLE_H__
#define __QVTK_UNIQUE_UI_INTERACTOR_STYLE_H__

#include "QvtkInteractorObserver.h"

#define UNIQUE_UI_INSTALL(STYLE_NAME) \
InteractorObserver::install(); \
if (!this->getUniqueThis()) { \
	const QString uniqueName = "UNIQUE" + QString(this->metaObject()->className()); \
	observersDataBase.insert(uniqueName, this); \
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
	observersDataBase.remove(uniqueName); \
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
InteractorObserver::uninstall();

namespace Q {
namespace vtk {
namespace Ui {
	class UniqueUiInteractorObserver; 
}
}
}
namespace Q {
namespace vtk{
class QVTKINTERACTOROBSERVER_EXPORT UniqueUiInteractorObserver: public InteractorObserver
{
	Q_OBJECT;
public:
	Ui::UniqueUiInteractorObserver* getUi() { return this->ui; }
	// do not call #getCustomEnable() during #setCustomEnable();
	virtual void setCustomEnable(bool flag) override;
	virtual void install() override;
	virtual void uninstall() override;

	static InteractorObserver* getUniqueThis(QString className);
	static InteractorObserver* getUniqueThis(InteractorObserver* observer);
	InteractorObserver* getUniqueThis();
	template<typename Observer> 
	Observer* getUniqueThis() { return qobject_cast<Observer*>(this->getUniqueThis()); }

protected:


	explicit UniqueUiInteractorObserver();
	virtual ~UniqueUiInteractorObserver() override;
	virtual void uniqueInstall() = 0;
	virtual void uniqueUninstall() = 0;
	virtual void uniqueFunction() = 0;
	virtual void uniqueEnable() = 0;
	virtual void uniqueDisable() = 0;


	Ui::UniqueUiInteractorObserver* ui;
};

}
}

#endif // !__QVTK_UNIQUE_UI_INTERACTOR_STYLE_H__
