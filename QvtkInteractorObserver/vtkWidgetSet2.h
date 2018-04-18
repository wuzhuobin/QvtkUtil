#ifndef __VTK_WIDGET_SET2_H__
#define __VTK_WIDGET_SET2_H__

// me
#include "abstractinteractorobserver_export.h"

// vtk 
#include <vtkWidgetSet.h>

/** 
 * @class	vtkWidgetSet2
 * @brief	Override 2 methods, which make Qvtk woring in somehow.
 *
 * Override vtkWidgetSet#AddWidget and vtkWidgetSet#RemoveWidget, which has a function that was 
 * only applicable in vtkParallelopipedWidget, the static_cast to vtkParallelopipedWidget
 * should be removed when vtkWidgetSet.
 */

class ABSTRACTINTERACTOROBSERVER_EXPORT vtkWidgetSet2 : public vtkWidgetSet
{
public:
	static vtkWidgetSet2* New();
	vtkTypeMacro(vtkWidgetSet2, vtkWidgetSet);

	void AddWidget(vtkAbstractWidget* widget);

	void RemoveWidget(vtkAbstractWidget* widget);


};




#endif // !__VTK_WIDGET_SET2_H__
