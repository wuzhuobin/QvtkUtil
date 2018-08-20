/*!
 * \file Example_ConnectivityFilter.h
 * \date 2017/07/19 16:30
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong, 
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief   An Example implementation of polydata filter used to demonstrate both
 *          how to use SCBAbstractFilter class and also SCBPipeline.
 *
 *
 * \note
*/

#ifndef __SCB_EXAMPLE_CONNECTIVITY_FILTER_H__
#define __SCB_EXAMPLE_CONNECTIVITY_FILTER_H__
#include "SCBAbstractPolyDataFilter.h"

#include <vtkPolyDataConnectivityFilter.h>

class vtkConnectivityFilter;
class Ui_ConnectivityFilterUI;
namespace Ui { class ConnectivityFilterUI; }


class ExampleConnectivityFilter : public SCBAbstractPolyDataFilter
{
    Q_OBJECT
public:
    ExampleConnectivityFilter(QWidget* parent = nullptr);
    ~ExampleConnectivityFilter();


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
    void slotFilterSettingsUpdate();

protected:

    /**
    * @brief	Convenient method to get LinearSubdvisionFilter
    * Doing down cast of the #m_vtkFilter, and return it
    * @return	the down casted #m_vtkFilter
    */
    virtual vtkPolyDataConnectivityFilter* GetFilter();

private:
    Ui_ConnectivityFilterUI* m_ui;
};

#endif
