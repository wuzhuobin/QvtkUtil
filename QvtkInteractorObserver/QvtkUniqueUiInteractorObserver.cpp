#include "QvtkUniqueUiInteractorObserver.h"

//me
#include "ui_QvtkUniqueUiInteractorObserver.h"


//qt
#include <QWidget>
#include <QDebug>

QvtkUniqueUiInteractorObserver::QvtkUniqueUiInteractorObserver()
{
	this->ui = nullptr;
}

void QvtkUniqueUiInteractorObserver::install()
{
	//UNIQUE_UI_INSTALL(QvtkUniqueUiInteractorObserver);
	QvtkAbstractInteractorObserver::install();
	if (!this->getUniqueThis()) {
		const QString uniqueName = "UNIQUE" + QString(this->metaObject()->className());
		stylesDataBase.insert(uniqueName, this);
		this->widget = new QWidget;
		this->ui = new Ui::QvtkUniqueUiInteractorObserver;
		this->getUi()->setupUi(this->widget);
		this->uniqueInstall();
	}
	else {
		this->widget = getUniqueThis()->getWidget();
		this->ui = static_cast<QvtkUniqueUiInteractorObserver*>(getUniqueThis())->getUi();
	}
}

QvtkUniqueUiInteractorObserver::~QvtkUniqueUiInteractorObserver()
{
	this->ui = nullptr;
}

void QvtkUniqueUiInteractorObserver::uninstall()
{
	if (getUniqueThis() == this) {
		const QString uniqueName = "UNIQUE" + QString(this->metaObject()->className());
		stylesDataBase.remove(uniqueName);
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
	QvtkAbstractInteractorObserver::uninstall();
	//UNIQUE_UI_UNINSTALL();
}

QvtkAbstractInteractorObserver * QvtkUniqueUiInteractorObserver::getUniqueThis(QString className)
{
	const QString uniqueName = "UNIQUE" + QString(className);
	if (!stylesDataBase.contains(uniqueName)) {
		return nullptr;
	}
	return (stylesDataBase.value(uniqueName));
}

QvtkAbstractInteractorObserver * QvtkUniqueUiInteractorObserver::getUniqueThis(QvtkAbstractInteractorObserver * observer)
{
	// push_front, so it is the last
	//return stylesDataBase.values(this->metaObject()->className()).last();
	return QvtkUniqueUiInteractorObserver::getUniqueThis(observer->metaObject()->className());
	//const QString uniqueName = "UNIQUE" + QString(observer->metaObject()->className());
	//if (!stylesDataBase.contains(uniqueName)) {
	//	return nullptr;
	//}
	//return (stylesDataBase.value(uniqueName));
}

QvtkAbstractInteractorObserver * QvtkUniqueUiInteractorObserver::getUniqueThis()
{
	// push_front, so it is the last
	//return stylesDataBase.values(this->metaObject()->className()).last();
	return getUniqueThis(this);
	//const QString uniqueName = "UNIQUE" + QString(this->metaObject()->className());
	//if (!stylesDataBase.contains(uniqueName)) {
	//	return nullptr;
	//}
	//return (stylesDataBase.value(uniqueName));
}

void QvtkUniqueUiInteractorObserver::setCustomEnable(bool flag)
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
	QvtkAbstractInteractorObserver::setCustomEnable(flag);
}
