/*!
 * \file Test_MainWindow.cpp
 * \date 2017/07/14 16:45
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong, 
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#include "Test_MainWindow.h"

//me 
#include "Test_InteractorStyle.h"
#include "SCBViewer.h"
#include "SCBOrthogonalViewer.h"
#include "SCBPlanarViewer.h"

//vtk
#include <vtkActor.h>
#include <vtkCubeSource.h>
#include <vtkSmartPointer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderer.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkAxesActor.h>
#include <vtkTransform.h>
#include <vtkRenderWindowInteractor.h>

#include <QDockWidget>
#include <QLayout>
#include <QDebug>
#include <QSizePolicy>


MainWindow::MainWindow()
{
    this->m_ui = new Ui_MainWindow;
    this->m_ui->setupUi(this);

	this->m_ui->actionDisplay_Axis_Actor->setDisabled(true);
	this->m_ui->actionDescync_Upper_Left_Viewer->setDisabled(true);
    this->m_ui->actionDisplayCube->setDisabled(true);
    this->m_ui->actionTest_Switch_Interactor->setDisabled(true);

    
    connect(this->m_ui->actionPrepare_Viewers, SIGNAL(triggered()),
        this, SLOT(slotPrepareViewers()));

    connect(this->m_ui->actionDisplayCube, SIGNAL(triggered()),
        this, SLOT(slotDisplayCube()));

	connect(this->m_ui->actionDisplay_Axis_Actor, SIGNAL(triggered()),
		this, SLOT(slotDisplayAxisActor()));

    connect(this->m_ui->actionTest_Switch_Interactor, SIGNAL(triggered()),
        this, SLOT(slotSwitchInteractor()));
    
    connect(this->m_ui->actionDescync_Upper_Left_Viewer, SIGNAL(triggered()),
        this, SLOT(slotDesyncUpperLeftViewer()));
}

void MainWindow::slotPrepareViewers()
{
    // Create N viewers and stack it into the mainwindow
    QGridLayout *layout = new QGridLayout;
    this->m_ui->centralwidget->setLayout(layout);
    layout->setMargin(1);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);

    /* Place viewer UI into grid layout in the central widget */
    int col = 3;    // Each colume of grid holds three viewers
    int N = 6;      // Creates N viewers
    for (int i = 0; i < N; i++)
    {
		if (i < 3)
		{
			SCBOrthogonalViewer* viewer = new SCBOrthogonalViewer(this);
			//viewer->SetOrientation(SCBOrthogonalViewer::ORIENTATION(i));
			viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // this is important
			layout->addWidget(viewer, int(i / col), i % col);
		}
		else if (i < 6)
		{
			SCBViewer* viewer = new SCBViewer(this);
			viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // this is important
			layout->addWidget(viewer, int(i / col), i % col);
		}
		else
		{
			SCBPlanarViewer* viewer = new SCBPlanarViewer(this);
			viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // this is important
			layout->addWidget(viewer, int(i / col), i % col);
		}
    }

    this->m_ui->actionPrepare_Viewers->setDisabled(true);
    this->m_ui->actionDisplayCube->setDisabled(false);
	this->m_ui->actionDisplay_Axis_Actor->setDisabled(false);
}

void MainWindow::slotDisplayAxisActor()
{
       /* Add actor to all viewers */
    for (int i = 0; i < SCBOrthogonalViewer::GetAllViewers().size();i++)
    {
		SCBAbstractViewer* viewer = SCBOrthogonalViewer::GetAllViewers()[i];
        vtkSmartPointer<vtkPolyDataMapper> mapper
            = vtkSmartPointer<vtkPolyDataMapper>::New();
    
		
		vtkSmartPointer<vtkTransform> actorTrans
			= vtkSmartPointer<vtkTransform>::New();
		vtkSmartPointer<vtkAxesActor> actor 
			= vtkSmartPointer<vtkAxesActor>::New();
		actorTrans->Scale(5, 5, 5);
		//actorTrans->Translate(15,15,15);
		actor->SetUserTransform(actorTrans);

        viewer->GetRenderers()[0]->AddActor(actor);
        /* Different actors and different cameras */
        viewer->ResetCamera();
        viewer->GetRenderWindow()->Render();
    }

    this->m_ui->actionDisplay_Axis_Actor->setDisabled(true);
    this->m_ui->actionTest_Switch_Interactor->setDisabled(false);
}


void MainWindow::slotDisplayCube()
{
	/* Prepare actor source */
	vtkSmartPointer<vtkCubeSource> cubeSource
		= vtkSmartPointer<vtkCubeSource>::New();
	cubeSource->SetXLength(20);
	cubeSource->SetYLength(10);
	cubeSource->SetZLength(30);
	cubeSource->Update();

	/* Add actor to all viewers */
	for (int i = 0; i < SCBOrthogonalViewer::GetAllViewers().size(); i++)
	{
		SCBAbstractViewer* viewer = SCBOrthogonalViewer::GetAllViewers()[i];
		vtkSmartPointer<vtkPolyDataMapper> mapper
			= vtkSmartPointer<vtkPolyDataMapper>::New();
		vtkSmartPointer<vtkActor> actor
			= vtkSmartPointer<vtkActor>::New();

		mapper->SetInputConnection(cubeSource->GetOutputPort());
		mapper->Update();
		actor->SetMapper(mapper);
		viewer->GetRenderers()[0]->AddActor(actor);


		/* Different actors and different cameras */
		actor->GetProperty()->SetColor(0.1 * i, 0.5, 0.5);
		viewer->ResetCamera();
		viewer->GetRenderWindow()->Render();

	}

	this->m_ui->actionDisplayCube->setDisabled(true);
	this->m_ui->actionTest_Switch_Interactor->setDisabled(false);
}

void MainWindow::slotSwitchInteractor()
{
    /* Switch interactor of all viewers */
    for (int i = 0; i < SCBViewer::GetAllViewers().size(); i++)
    {
        SCBAbstractViewer* viewer = SCBViewer::GetAllViewers()[i];
        viewer->GetInteractor()->SetInteractorStyle(Test_InteractorStyle::New());
    }

    this->m_ui->actionTest_Switch_Interactor->setDisabled(true);
    this->m_ui->actionDescync_Upper_Left_Viewer->setDisabled(false);
}

void MainWindow::slotDesyncUpperLeftViewer()
{
    //SCBViewer::GetAllViewers()[0]-> SetCursorSync(false);
	SCBViewer::GetAllViewers()[0]->SetCursorDesyncFlag(true);
    this->m_ui->actionDescync_Upper_Left_Viewer->setDisabled(true);
}
