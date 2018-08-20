/*!
* \file Test_Pipeline_MainWindow.cpp
* \date 2017/07/19 17:33
*
* \author 		Wong, Matthew Lun
* Occupation	Chinese University of Hong Kong,
* 				Department of Imaging and Interventional Radiology,
* 				M.Phil Student
* Contact: 	fromosia@gmail.com
*
* \brief   Simple mainwindow written to demonstrate the use of SCBPipeline.
*
* \note
*/
#ifndef __SCB_TEST_PIPELINE_MAINWINDOW_H__
#define __SCB_TEST_PIPELINE_MAINWINDOW_H__


//qt
#include <QObject>
#include <QMainWindow>

#include "Example_Pipeline.h"

class vtkPolyData;
class SCBViewer;
class Ui_MainWindow;
namespace Ui { class MainWindow; }

/// Maniwindow for test
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();;
 
    Ui_MainWindow* m_ui;

protected:
    /**
     * void PrepareInput
     *
     * @brief 
     *
     * Prepare the initial data for inputting to the pipeline
     *
     * @return void
     */
    void PrepareInput();

    /**
     * void SetUpPipeline
     *
     * @brief 
     *
     * Create and connect the pipeline. Display widget as dock widget
     *
     * @return void
     */
    void SetUpPipeline();

	/**
	 * void SetUpITKPipeline
	 *
	 * @brief 
	 *
	 * Create and connect pipeline of ITK filters
	 *
	 * @return void
	 */
	void SetUpITKPipeline();

    ExamplePipeline* m_pipeline;
    vtkPolyData* m_input;

    SCBViewer* m_viewer;
};

#endif //__SCB_TEST_MAINWINDOW_H__