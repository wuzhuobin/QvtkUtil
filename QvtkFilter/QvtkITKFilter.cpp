// me
#include "QvtkAbstractITKFilter.h"

// vtk
#include <vtkPassThroughFilter.h>

QvtkAbstractITKFilter::QvtkAbstractITKFilter(QWidget* parent)
	:QvtkAbstractFilter(parent)

{
	this->vtkfilter = vtkPassThroughFilter::New();
}

QvtkAbstractITKFilter::~QvtkAbstractITKFilter()
{
	this->vtkfilter->Delete();
}

void QvtkAbstractITKFilter::PostConnectFilter(QvtkAbstractFilter* postFilter, int postPort /*= 0*/, int thisPort /*= 0*/)
{
    QString errmsg("Trying to call PostConnectFilter() of template filter. \
        This cannot be done because we cannot predict the output type of postFilter! \
        Use call PreConnectFilter() of postFilter instead! ");
    qCritical() << errmsg;
    std::logic_error(errmsg.toStdString());
}

vtkPassThroughFilter* QvtkAbstractITKFilter::GetFilter()
{
    return vtkPassThroughFilter::SafeDownCast(this->vtkfilter);
}

