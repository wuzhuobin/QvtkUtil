/*!
 * \file Example_PDTransformFilter.h
 * \date 2017/07/19 17:31
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
#ifndef __SCB_PDTRANSFORM_FILTER_H__
#define __SCB_PDTRANSFORM_FILTER_H__

#include "SCBAbstractPolyDataFilter.h"


#include <vtkTransformPolyDataFilter.h>

class QDoubleSpinBox;
class vtkTransform;
class vtkTransformPolyDataFilter;
class Ui_PDTransformFilterUI;
namespace Ui { class PDTransformFilterUI; }

class ExamplePDTransfromFilter : public SCBAbstractPolyDataFilter
{
    Q_OBJECT
public:
    ExamplePDTransfromFilter(QWidget* parent = nullptr);
    ~ExamplePDTransfromFilter();


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
    /**
     * void slotFilterSettingsUpdate
     *
     * @brief 
     *
     * Call when spinboxes are updated. 
     *
     * @return void
     */
    void slotFilterSettingsUpdate();
    
    /**
     * void slotSliderRotationsHandle
     *
     * @brief 
     *
     * Call when sliders are updated
     *
     * @return void
     */
    void slotSliderRotationsHandle();

protected:

    /**
    * @brief	Convenient method to get LinearSubdvisionFilter
    * Doing down cast of the #m_vtkFilter, and return it
    * @return	the down casted #m_vtkFilter
    */
    virtual vtkTransformPolyDataFilter* GetFilter();

    vtkTransform* m_transform;

    /**
     * void MatrixUI2VTK
     *
     * @brief 
     *
     * Change vtkTransform according to UI.
     *
     * @return void
     */
    void MatrixUI2VTK();


    /**
     * void MatrixVTK2UI
     *
     * @brief 
     *
     * Change UI according to vtkTransform
     *
     * @return void
     */
    void MatrixVTK2UI();

private:
    Ui_PDTransformFilterUI* m_ui;

    QList<QDoubleSpinBox*> m_matrixSpinBoxList;
};

#endif
