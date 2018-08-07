/*
Author:		Wong, Matthew Lun
Date:		04, July 2017
Occupation:	Chinese University of Hong Kong,
			Department of Imaging and Inteventional Radiology,
			M.Phil Student

FileName: 	ExampleFilter.h

Descriptions:
	Example for SCBAbstracPolyDataFilter written to illustrate how to correctly write a 
	Filter

Wong Matthew Lun
Copyright (C) 2017
*/

#ifndef __SCB_EXAMPLE_FILTER_H__
#define __SCB_EXAMPLE_FILTER_H__

#include "SCBAbstractPolyDataFilter.h"


class vtkLinearSubdivisionFilter;
class Ui_Form;
namespace Ui { class Form; }


class ExamplePolyDataFilter final: public SCBAbstractPolyDataFilter
{
	Q_OBJECT
public:
	ExamplePolyDataFilter(QWidget* parent = nullptr);
	~ExamplePolyDataFilter();


	/**
	 * @brief LoadUI
	 *
	 * m_widget should be prepared after this LoadUI() function. 
	 * This function is a pure virtual function in base class.
	 *
	 * @return void
	 */

	virtual void LoadUI();

	/**
	 * @brief ConnectUI
	 *
	 * Signals from parameters UI should be connected to the corresponding 
	 * slots in this function
	 *
	 * @return int
	 */

	virtual int	ConnectUI();


public slots:
	void slotNumberOfSubdivisionChange(int);

protected:

	/**
	* @brief	Convenient method to get LinearSubdvisionFilter
	* Doing down cast of the #m_vtkFilter, and return it
	* @return	the down casted #m_vtkFilter
	*/
	virtual vtkLinearSubdivisionFilter* GetLinearSubdivisionFilter();

private:
	Ui_Form* m_ui;
};

#endif
