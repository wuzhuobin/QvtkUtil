#include "QvtkAbstractInteractorObserver.h"

//qt
#include <QWidget>
#include <QDebug>

QMultiHash<QString, QvtkAbstractInteractorObserver*> QvtkAbstractInteractorObserver::stylesDataBase;

void QvtkAbstractInteractorObserver::synchronalCall(QvtkAbstractInteractorObserver * style, void(*functionSet)(QvtkAbstractInteractorObserver *it))
{
	synchronalCall(style->metaObject()->className(), functionSet);
}

void QvtkAbstractInteractorObserver::install()
{
	installImp(this->metaObject());
}

void QvtkAbstractInteractorObserver::uninstall()
{
	uninstallImp(this->metaObject());
}

void QvtkAbstractInteractorObserver::synchronalCall(QvtkAbstractInteractorObserver * style, std::function<void(QvtkAbstractInteractorObserver*it)> functionSet)
{
	synchronalCall(style->metaObject()->className(), functionSet);
}

void QvtkAbstractInteractorObserver::synchronalCall(QString styleName, FunctionSet1 functionSet)
{
	QList<QvtkAbstractInteractorObserver*> list = stylesDataBase.values(styleName);
	for (QList<QvtkAbstractInteractorObserver*>::const_iterator cit = list.cbegin();
		cit != list.cend(); ++cit) {
		if ((*cit)->getCustomEnable()) {
			functionSet(*cit);
		}
	}
}

void QvtkAbstractInteractorObserver::synchronalCall(QString styleName, FunctionSet2 functionSet)
{
	QList<QvtkAbstractInteractorObserver*> list = stylesDataBase.values(styleName);
	for (QList<QvtkAbstractInteractorObserver*>::const_iterator cit = list.cbegin();
		cit != list.cend(); ++cit) {
		if ((*cit)->getCustomEnable()) {
			functionSet(*cit);
		}
	}
}

QWidget * QvtkAbstractInteractorObserver::getWidget()
{
	return this->widget.data();
}

QvtkAbstractInteractorObserver::QvtkAbstractInteractorObserver()
	:QObject(nullptr)
{
	this->widget = nullptr;
	this->customFlag = false;
}

QvtkAbstractInteractorObserver::~QvtkAbstractInteractorObserver()
{
	this->widget = nullptr;
}

void QvtkAbstractInteractorObserver::installImp(const QMetaObject * metaObject) 
{
	stylesDataBase.insert(metaObject->className(), this);
	if (metaObject != &QvtkAbstractInteractorObserver::staticMetaObject) {
		this->installImp(metaObject->superClass());
	}
}

void QvtkAbstractInteractorObserver::uninstallImp(const QMetaObject * metaObject) 
{
	if (metaObject != &QvtkAbstractInteractorObserver::staticMetaObject) {
		this->uninstallImp(metaObject->superClass());
	}
	stylesDataBase.remove(metaObject->className(), this);
}
