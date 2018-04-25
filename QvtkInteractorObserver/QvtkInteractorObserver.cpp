#include "QvtkInteractorObserver.h"

//qt
#include <QWidget>
#include <QDebug>
namespace Q {
namespace vtk{
QMultiHash<QString, InteractorObserver*> InteractorObserver::observersDataBase;
void InteractorObserver::synchronalCall(InteractorObserver * observer, void(*functionSet)(InteractorObserver *it))
{
	synchronalCall(observer->metaObject()->className(), functionSet);
}

void InteractorObserver::install()
{
	installImp(this->metaObject());
}

void InteractorObserver::uninstall()
{
	uninstallImp(this->metaObject());
}

void InteractorObserver::synchronalCall(InteractorObserver * observer, std::function<void(InteractorObserver*it)> functionSet)
{
	synchronalCall(observer->metaObject()->className(), functionSet);
}

void InteractorObserver::synchronalCall(QString observerName, FunctionSet1 functionSet)
{
	QList<InteractorObserver*> list = observersDataBase.values(observerName);
	for (QList<InteractorObserver*>::const_iterator cit = list.cbegin();
		cit != list.cend(); ++cit) {
		if ((*cit)->getCustomEnable()) {
			functionSet(*cit);
		}
	}
}

void InteractorObserver::synchronalCall(QString observerName, FunctionSet2 functionSet)
{
	QList<InteractorObserver*> list = observersDataBase.values(observerName);
	for (QList<InteractorObserver*>::const_iterator cit = list.cbegin();
		cit != list.cend(); ++cit) {
		if ((*cit)->getCustomEnable()) {
			functionSet(*cit);
		}
	}
}

QWidget * InteractorObserver::getWidget()
{
	return this->widget.data();
}

InteractorObserver::InteractorObserver()
	:QObject(nullptr)
{
	this->widget = nullptr;
	this->customFlag = false;
}

InteractorObserver::~InteractorObserver()
{
	this->widget = nullptr;
}

void InteractorObserver::installImp(const QMetaObject * metaObject) 
{
	observersDataBase.insert(metaObject->className(), this);
	if (metaObject != &InteractorObserver::staticMetaObject) {
		this->installImp(metaObject->superClass());
	}
}

void InteractorObserver::uninstallImp(const QMetaObject * metaObject) 
{
	if (metaObject != &InteractorObserver::staticMetaObject) {
		this->uninstallImp(metaObject->superClass());
	}
	observersDataBase.remove(metaObject->className(), this);
}

}
}