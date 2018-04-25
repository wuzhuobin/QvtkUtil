#include "QvtkUniqueUiInteractorObserver.h"

//me
#include "ui_QvtkUniqueUiInteractorObserver.h"


//qt
#include <QWidget>
#include <QDebug>
namespace Q {
namespace vtk{
UniqueUiInteractorObserver::UniqueUiInteractorObserver()
{
	this->ui = nullptr;
}

void UniqueUiInteractorObserver::install()
{
	//UNIQUE_UI_INSTALL(UniqueUiInteractorObserver);
	InteractorObserver::install();
	if (!this->getUniqueThis()) {
		const QString uniqueName = "UNIQUE" + QString(this->metaObject()->className());
		observersDataBase.insert(uniqueName, this);
		this->widget = new QWidget;
		this->ui = new Ui::UniqueUiInteractorObserver;
		this->getUi()->setupUi(this->widget);
		this->uniqueInstall();
	}
	else {
		this->widget = getUniqueThis()->getWidget();
		this->ui = static_cast<UniqueUiInteractorObserver*>(getUniqueThis())->getUi();
	}
}

UniqueUiInteractorObserver::~UniqueUiInteractorObserver()
{
	this->ui = nullptr;
}

void UniqueUiInteractorObserver::uninstall()
{
	if (getUniqueThis() == this) {
		const QString uniqueName = "UNIQUE" + QString(this->metaObject()->className());
		observersDataBase.remove(uniqueName);
		this->uniqueUninstall();
		if (this->widget) {
			delete this->getWidget();
			this->widget = nullptr;
		}
		delete this->getUi();
		this->ui = nullptr;
	}
	else {
		this->widget = nullptr;
		this->ui = nullptr;
	}
	InteractorObserver::uninstall();
	//UNIQUE_UI_UNINSTALL();
}

InteractorObserver * UniqueUiInteractorObserver::getUniqueThis(QString className)
{
	const QString uniqueName = "UNIQUE" + QString(className);
	if (!observersDataBase.contains(uniqueName)) {
		return nullptr;
	}
	return (observersDataBase.value(uniqueName));
}

InteractorObserver * UniqueUiInteractorObserver::getUniqueThis(InteractorObserver * observer)
{
	// push_front, so it is the last
	//return observersDataBase.values(this->metaObject()->className()).last();
	return UniqueUiInteractorObserver::getUniqueThis(observer->metaObject()->className());
	//const QString uniqueName = "UNIQUE" + QString(observer->metaObject()->className());
	//if (!observersDataBase.contains(uniqueName)) {
	//	return nullptr;
	//}
	//return (observersDataBase.value(uniqueName));
}

InteractorObserver * UniqueUiInteractorObserver::getUniqueThis()
{
	// push_front, so it is the last
	//return observersDataBase.values(this->metaObject()->className()).last();
	return getUniqueThis(this);
	//const QString uniqueName = "UNIQUE" + QString(this->metaObject()->className());
	//if (!observersDataBase.contains(uniqueName)) {
	//	return nullptr;
	//}
	//return (observersDataBase.value(uniqueName));
}

void UniqueUiInteractorObserver::setCustomEnable(bool flag)
{
	// turn on for once
	if (flag && flag != getUniqueThis()->getCustomEnable()) 
	{
		uniqueEnable();
	}

	// turn off for once
	if (!flag && flag != getUniqueThis()->getCustomEnable())
	{
		uniqueDisable();
	}

	// call anyway for once 
	if (flag != getUniqueThis()->getCustomEnable())
	{
		uniqueFunction();
	}
	InteractorObserver::setCustomEnable(flag);
}

}
}