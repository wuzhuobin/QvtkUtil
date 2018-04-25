#include "QvtkMultipleUiInteractorObserver.h"

//me
#include "ui_QvtkMultipleUiInteractorObserver.h"

//qt
#include <qwidget.h>
namespace Q {
namespace vtk{
void MultipleUiInteractorObserver::install()
{
	MULTIPLE_UI_INSTALL(MultipleUiInteractorObserver);
}

void MultipleUiInteractorObserver::uninstall()
{
	MULTIPLE_UI_UNINSTALL();
}

MultipleUiInteractorObserver::MultipleUiInteractorObserver()
{
	this->ui = nullptr;
}

MultipleUiInteractorObserver::~MultipleUiInteractorObserver()
{
	this->ui = nullptr;
}
}
}
