#include "QvtkMultipleUiInteractorObserver.h"

//me
#include "ui_QvtkMultipleUiInteractorObserver.h"

//qt
#include <qwidget.h>

void QvtkMultipleUiInteractorObserver::install()
{
	MULTIPLE_UI_INSTALL(QvtkMultipleUiInteractorObserver);
}

void QvtkMultipleUiInteractorObserver::uninstall()
{
	MULTIPLE_UI_UNINSTALL();
}

QvtkMultipleUiInteractorObserver::QvtkMultipleUiInteractorObserver()
{
	this->ui = nullptr;
}

QvtkMultipleUiInteractorObserver::~QvtkMultipleUiInteractorObserver()
{
	this->ui = nullptr;
}
