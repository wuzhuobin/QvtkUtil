/*!
 * \file ExampleFilter.cpp
 * \date 2017/07/06 10:24
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong, 
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief This class is a demo of how to implement an SCBFilter
 *
 * TODO: long description
 *
 * \note
*/

#include "ExampleFilter.h"

// me
#include "ui_ExampleFilter.h"

// vtk
#include <vtkLinearSubdivisionFilter.h>


ExamplePolyDataFilter::ExamplePolyDataFilter(QWidget* parent)
    : SCBAbstractPolyDataFilter(parent)
{
	/* Call this macro at the very beging of this constructor */
	SCB_FILTER_CONSTRUCT_MACRO(vtkLinearSubdivisionFilter);
}

ExamplePolyDataFilter::~ExamplePolyDataFilter()
{
	this->m_vtkfilter->Delete();
}


void ExamplePolyDataFilter::LoadUI()
{
		/* Setup UI */
		m_ui = new Ui_Form;
		m_ui->setupUi(this);
		m_ui->spinBoxSubdivisionNumber->setMinimum(1);
		m_ui->spinBoxSubdivisionNumber->setMaximum(5);
}

int ExamplePolyDataFilter::ConnectUI()
{
	connect(m_ui->spinBoxSubdivisionNumber, SIGNAL(valueChanged(int)), this, SLOT(slotNumberOfSubdivisionChange(int)));
	return 0;
}


vtkLinearSubdivisionFilter * ExamplePolyDataFilter::GetLinearSubdivisionFilter()
{
	return vtkLinearSubdivisionFilter::SafeDownCast(this->m_vtkfilter);
}

void ExamplePolyDataFilter::slotNumberOfSubdivisionChange(int division)
{
	if (GetLinearSubdivisionFilter())
	{
		GetLinearSubdivisionFilter()->SetNumberOfSubdivisions(division);
		
		/* Use this->Update() to emit a signal signalsModifiedFilter() 
		*  If you do not wnat renderer to Render(), don't use this function or 
		*  simply don't connect the signal to the updater
		*/
		this->Update();
	}
}

