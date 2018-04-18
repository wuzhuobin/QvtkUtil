// me
#include "vtkWidgetSet2.h"

// vtk
#include <vtkObjectFactory.h>
#include <vtkAbstractWidget.h>
vtkStandardNewMacro(vtkWidgetSet2);

void vtkWidgetSet2::AddWidget(vtkAbstractWidget * widget)
{
  for ( unsigned int i = 0; i < this->Widget.size(); i++)
  {
    if (this->Widget[i] == widget)
    {
      return;
    }
  }

  this->Widget.push_back(widget);
  widget->Register(this);
}

void vtkWidgetSet2::RemoveWidget(vtkAbstractWidget * widget)
{
	for (WidgetIteratorType it = this->Widget.begin();
		it != this->Widget.end(); ++it)
	{
		if (*it == widget)
		{
			this->Widget.erase(it);
			widget->UnRegister(this);
			break;
		}
	}
}
