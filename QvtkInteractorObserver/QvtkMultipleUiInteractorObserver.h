#ifndef __Qvtk_MULTIPLE_UI_INTERACTOR_STYLE_H__
#define __Qvtk_MULTIPLE_UI_INTERACTOR_STYLE_H__

#include "QvtkAbstractInteractorObserver.h"

#define MULTIPLE_UI_INSTALL(STYLE_NAME) \
QvtkAbstractInteractorObserver::install(); \
this->widget = new QWidget; \
this->ui = new Ui:: STYLE_NAME; \
this->ui->setupUi(this->widget);

#define MULTIPLE_UI_UNINSTALL() \
QvtkAbstractInteractorObserver::uninstall(); \
delete this->ui; \
if (this->widget) \
{ \
	delete this->widget.data(); \
} \
this->widget = nullptr;

namespace Ui { class QvtkMultipleUiInteractorObserver; }

class ABSTRACTINTERACTOROBSERVER_EXPORT QvtkMultipleUiInteractorObserver: public QvtkAbstractInteractorObserver
{
	Q_OBJECT;
public:


	virtual void install() override;
	virtual void uninstall() override;

protected:
	explicit QvtkMultipleUiInteractorObserver();
	~QvtkMultipleUiInteractorObserver();

	Ui::QvtkMultipleUiInteractorObserver* ui;

};


#endif // !__Qvtk_MULTIPLE_UI_INTERACTOR_STYLE_H__
