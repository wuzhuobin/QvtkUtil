#include "QvtkAbstractImageFilter.h"

// vtk
#include <vtkImageAlgorithm.h>

//qt 
#include <QDebug>

void QvtkAbstractImageFilter::SetInputData(vtkDataObject *data, int index /*= 0*/)
{
	if (!this->GetFilter())
		return ;

	if (index >= this->GetNumberOfInputs())
	{
		qCritical() << ERRMSG_ABSTRACT_FILTER_NUM_OF_IO;
		qCritical() << "Error code" << ERROR_INCORRECT_NUM_OF_INPUTS;
		throw std::logic_error(std::string() + ERRMSG_ABSTRACT_FILTER_NUM_OF_IO + "Error Code: " +
				std::to_string(ERROR_INCORRECT_NUM_OF_INPUTS));
	}



	return GetFilter()->SetInputData(data);
}


vtkImageData* QvtkAbstractImageFilter::GetOutput(int index /*= 0*/)
{
	if (!this->ErrorCheck())
		return nullptr;

	if (index >= this->GetNumberOfInputs())
	{
		qCritical() << ERRMSG_ABSTRACT_FILTER_NUM_OF_IO;
		qCritical() << "Error Code: " << ERROR_INCORRECT_NUM_OF_INPUTS;
		throw std::logic_error(std::string() + ERRMSG_ABSTRACT_FILTER_NUM_OF_IO + "Error Code:" +
				std::to_string(ERROR_INCORRECT_NUM_OF_INPUTS));
	}

	return GetFilter()->GetOutput(index);
}

QvtkAbstractImageFilter::QvtkAbstractImageFilter(QWidget* parent)
	:QvtkAbstractFilter(parent)
{
	
}

QvtkAbstractImageFilter::~QvtkAbstractImageFilter()
{
	// #TODO: Check existing input/output connection

	/* Delete if there are no dependencies to this filter */
}

vtkImageAlgorithm* QvtkAbstractImageFilter::GetFilter()
{
	return vtkImageAlgorithm::SafeDownCast(this->vtkfilter);
}

bool QvtkAbstractImageFilter::IsITKFilter()
{
    return false;
}
