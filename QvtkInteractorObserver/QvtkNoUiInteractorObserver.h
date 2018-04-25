#ifndef __QVTK_NO_UI_INTERACTOR_STYLE_H__
#define __QVTK_NO_UI_INTERACTOR_STYLE_H__

#include "QvtkInteractorObserver.h"
namespace Q {
namespace vtk{
class QVTKINTERACTOROBSERVER_EXPORT NoUiInteractorObserver: public InteractorObserver
{
	Q_OBJECT;
};

}
}
#endif // !__QVTK_NO_UI_INTERACTOR_STYLE_H__
