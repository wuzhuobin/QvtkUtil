// me
#include "QvtkAbstractPolyDataFilter.h"

// vtk
#include <vtkPolyDataAlgorithm.h>

// qt
#include <QDebug>

void QvtkAbstractPolyDataFilter::SetInputData(vtkDataObject *data, int index)
{
	if (this->ErrorCheck())
		return;

    if (index >= this->GetNumberOfInputs())
    {
        qCritical() << ERRMSG_ABSTRACT_FILTER_NUM_OF_IO;
		qCritical() << "Error Code: " << ERROR_INCORRECT_NUM_OF_INPUTS;
		throw std::logic_error(std::string() + ERRMSG_ABSTRACT_FILTER_NUM_OF_IO +
				"Error Code: " + std::to_string(ERROR_INCORRECT_NUM_OF_INPUTS));
	}



	GetFilter()->SetInputData(index, data);
}

vtkPolyData* QvtkAbstractPolyDataFilter::GetOutput(int index/*=0*/)
{
	if (this->ErrorCheck())
		return nullptr;

	if (index >= this->GetNumberOfOutputs())
    {
		qCritical() << ERRMSG_ABSTRACT_FILTER_NUM_OF_IO;
		qCritical() << "Error Code: " << ERROR_INCORRECT_NUM_OF_OUTPUTS;
		throw std::logic_error(std::string() + ERRMSG_ABSTRACT_FILTER_NUM_OF_IO + "Error Code:" +
				std::to_string(ERROR_INCORRECT_NUM_OF_OUTPUTS));

	}

	return GetFilter()->GetOutput(index);
}

QvtkAbstractPolyDataFilter::QvtkAbstractPolyDataFilter(QWidget* parent)
	:QvtkAbstractFilter(parent)
{

}

QvtkAbstractPolyDataFilter::~QvtkAbstractPolyDataFilter()
{

}

vtkPolyDataAlgorithm * QvtkAbstractPolyDataFilter::GetFilter()
{
	return vtkPolyDataAlgorithm::SafeDownCast(this->vtkfilter);
}
