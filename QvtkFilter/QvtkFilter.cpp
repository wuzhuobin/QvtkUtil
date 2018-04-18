/*!
 * \file QvtkAbstractFilter.cpp
 * \date 2017/07/04 14:04
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong, 
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief Source code of the class AbstractFilter
 *
 * \note
*/
#include "QvtkAbstractFilter.h"

// vtk
#include <vtkAlgorithm.h>

// qt
#include <QString>
#include <QFile>
#include <QDebug>
#include <QWidget>
#include <QFuture>

int QvtkAbstractFilter::GetNumberOfInputs()
{
    return this->maxNumOfInput;
}


int QvtkAbstractFilter::GetNumberOfOutputs()
{
    return this->maxNumOfOutput;
}


vtkAlgorithmOutput * QvtkAbstractFilter::GetOutputPort(int port)
{
	if (this->GetFilter())
		return this->GetFilter()->GetOutputPort(port);
	else
		return nullptr; // should not reach this point
}

void QvtkAbstractFilter::SetInputConnection(vtkAlgorithmOutput* input, int port /*= 0*/)
{
	if (this->GetFilter())
	{
		this->GetFilter()->SetInputConnection(port, input);
	}
}

void QvtkAbstractFilter::PreConnectFilter(
	QvtkAbstractFilter* preFilter, int outputPort/*=0*/, int thisPort/*=0*/)
{
	if (this->GetFilter())
	{
		this->SetInputConnection(preFilter->GetOutputPort(outputPort), thisPort);
        this->prefilter = preFilter;
	}
}

void QvtkAbstractFilter::PostConnectFilter(
	QvtkAbstractFilter* postFilter, int postPort/*=0*/, int thisPort/*=0*/)
{
	if (this->GetFilter())
	{
		postFilter->PreConnectFilter(this, thisPort, postPort);
        this->postfilter = postFilter;
	}
}

void QvtkAbstractFilter::Print(std::ostream &stream)
{
	if (!this->ErrorCheck())
		this->vtkfilter->Print(stream);
}

QFuture<int*> QvtkAbstractFilter::ThreadedUpdate()
{
	qCritical() << "Not Implemented!";
	throw std::logic_error("Not Implemented!");
	return QFuture<int*>();
}

void QvtkAbstractFilter::Update()
{
	if (!this->ErrorCheck()) {
        //this->vtkfilter->SetGlobalWarningDisplay(0); 
        this->vtkfilter->SetDebug(false);
        this->vtkfilter->Update();
        this->vtkfilter->Modified();
		emit signalFilterModified();
	}
}


QvtkAbstractFilter::QvtkAbstractFilter(QWidget* parent)
	: QWidget(parent)
{
    this->maxNumOfInput = 1;
    this->maxNumOfOutput = 1;

	this->prefilter = nullptr;
	this->postfilter = nullptr;
}

QvtkAbstractFilter::~QvtkAbstractFilter()
{
}

QString QvtkAbstractFilter::GetFilterName()
{
    return filterNameString;
}

bool QvtkAbstractFilter::ErrorCheck()
{
	if (!this->vtkfilter){

		qCritical() << ERRMSG_ABSTRACT_FILTER_NULL_VTK_FILTER;
		qCritical() << "Error Code: " << ERROR_NULL_VTK_FILTER;
		throw std::logic_error(std::string() + ERRMSG_ABSTRACT_FILTER_NULL_VTK_FILTER +
				"Error Code: " + std::to_string(ERROR_NULL_VTK_FILTER));
		return true;
	}
	else
		return false;
}

vtkAlgorithm * QvtkAbstractFilter::GetFilter()
{
	return this->vtkfilter;
}
