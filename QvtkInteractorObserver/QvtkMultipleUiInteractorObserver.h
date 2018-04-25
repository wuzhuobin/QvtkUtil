#ifndef __QVTK_MULTIPLE_UI_INTERACTOR_STYLE_H__
#define __QVTK_MULTIPLE_UI_INTERACTOR_STYLE_H__

#include "QvtkInteractorObserver.h"

#define MULTIPLE_UI_INSTALL(STYLE_NAME) \
InteractorObserver::install(); \
this->widget = new QWidget; \
this->ui = new Ui:: STYLE_NAME; \
this->ui->setupUi(this->widget);

#define MULTIPLE_UI_UNINSTALL() \
InteractorObserver::uninstall(); \
delete this->ui; \
if (this->widget) \
{ \
	delete this->widget.data(); \
} \
this->widget = nullptr;
namespace Q {
namespace vtk {
namespace Ui { class MultipleUiInteractorObserver; }
}
}
namespace Q {
namespace vtk{
class QVTKINTERACTOROBSERVER_EXPORT MultipleUiInteractorObserver: public InteractorObserver
{
	Q_OBJECT;
public:


	virtual void install() override;
	virtual void uninstall() override;

protected:
	explicit MultipleUiInteractorObserver();
	~MultipleUiInteractorObserver();

	Ui::MultipleUiInteractorObserver* ui;

};

}
}

#endif // !__QVTK_MULTIPLE_UI_INTERACTOR_STYLE_H__
