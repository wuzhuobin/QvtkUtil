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

#include "Test_Pipeline_MainWindow.h"

//me 
#include <SCBViewer.h>
#include "Example_Pipeline.h"
#include "Example_ITKPipeline.h"
#include "Example_PDTransformFilter.h"
#include "Example_ConnectivityFilter.h"
#include "ExampleUpdater.h"

#include <QDockWidget>
#include <QLayout>
#include <QDebug>
#include <QSizePolicy>

#include <vtkLine.h>
#include <vtkTubeFilter.h>
#include <vtkCardinalSpline.h>
#include <vtkSplineFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCellArray.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkTransform.h>

// ITK pipeline test
#include <vtkImageData.h>
#include <vtkImageViewer2.h>
#include <vtkNIFTIImageReader.h>

#include "ui_Test_Pipeline_MainWindow.h"


#define PAUSE_MACRO std::cout << "Press any key to continue..."; std::cin.get();

MainWindow::MainWindow()
{
    this->m_ui = new Ui_MainWindow;
    this->m_ui->setupUi(this);

    /* Set layout to central widget for holding out SCBViewer */
    QGridLayout* layout = new QGridLayout();
    this->m_ui->centralwidget->setLayout(layout);
    this->m_viewer = new SCBViewer();
    this->m_viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // this is important
    this->m_viewer->GetInteractor()->SetInteractorStyle(vtkInteractorStyleTrackballCamera::New());
    layout->addWidget(this->m_viewer);

    /* Prepare the input for the pipeline */
    this->PrepareInput();

    /* Prepare and connect the pipeline */
    //this->SetUpITKPipeline();
	this->SetUpPipeline();
}

MainWindow::~MainWindow()
{
    if (this->m_input)
        this->m_input->Delete();

    if (this->m_viewer)
        delete this->m_viewer;

    if (this->m_ui)
        delete this->m_ui;
}

void MainWindow::PrepareInput()
{

    vtkSmartPointer<vtkPolyData> sourcePD = vtkSmartPointer<vtkPolyData>::New();

    vtkSmartPointer<vtkPoints> pts = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
    pts->InsertNextPoint(0., 1., 0.);
    pts->InsertNextPoint(2., 1., 0.);
    pts->InsertNextPoint(-1., -2., 5.);
    pts->InsertNextPoint(2., 4., 0.);
    pts->InsertNextPoint(5., 1., 3.);
    pts->InsertNextPoint(5., 10., 0.);

    line->GetPointIds()->InsertNextId(0);
    line->GetPointIds()->InsertNextId(1);
    line->GetPointIds()->InsertNextId(2);
    line->GetPointIds()->InsertNextId(3);
    line->GetPointIds()->InsertNextId(4);
    line->GetPointIds()->InsertNextId(5);

    cells->InsertNextCell(line);

    sourcePD->SetPoints(pts);
    sourcePD->SetLines(cells);

    vtkSmartPointer<vtkCardinalSpline> spline = vtkSmartPointer<vtkCardinalSpline>::New();
    spline->SetLeftConstraint(2);
    spline->SetLeftValue(0);
    spline->SetRightConstraint(2);
    spline->SetRightValue(0);


    vtkSmartPointer<vtkSplineFilter> splinefilter = vtkSmartPointer<vtkSplineFilter>::New();
    splinefilter->SetSpline(spline);
    splinefilter->SetInputData(sourcePD);
    splinefilter->SetSubdivide(50);
    splinefilter->Update();

    vtkSmartPointer<vtkTubeFilter> filter = vtkSmartPointer<vtkTubeFilter>::New();
    filter->SetInputConnection(splinefilter->GetOutputPort());
    filter->SetRadius(1);
    filter->SetCapping(true);
    filter->SetNumberOfSides(30);
    filter->Update();

    this->m_input = vtkPolyData::New();
    this->m_input->DeepCopy(filter->GetOutput());
}

void MainWindow::SetUpPipeline()
{
    /* Setup UI as dock widget */
    ExamplePipeline* pipeline = new ExamplePipeline();

    /* Setup pipeline input and output */
    static_cast<ExamplePDTransfromFilter*>(pipeline->GetComponentFilter(0))->SetInputData(this->m_input);
    pipeline->UpdatePipeline();

    /* Create actor for display */
    vtkSmartPointer<vtkPolyDataMapper> mapper 
        = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> actor 
        = vtkSmartPointer<vtkActor>::New();
    mapper->SetInputConnection((pipeline->GetComponentFilter(2)->GetOutputPort()));
    actor->SetMapper(mapper);

    /* GetRenderers will be depricated after AddProp is prepared! */
    QDockWidget* dock = new QDockWidget(this);
    dock->setWidget(pipeline);
    this->addDockWidget(Qt::RightDockWidgetArea, dock);
    this->m_viewer->GetRenderers()[0]->AddActor(actor);
    this->m_viewer->ResetCamera();
    this->m_viewer->ResetCameraClippingRange();

    /* Connect a simple updater for updating the render window realtime */
    Updater* up = new Updater;
    up->SetRenderWindow(this->m_viewer->GetRenderWindow());
    up->ConnectModifyingObject(pipeline->GetComponentFilter(0));
    up->ConnectModifyingObject(pipeline->GetComponentFilter(1));
    up->ConnectModifyingObject(pipeline->GetComponentFilter(2));
}

void MainWindow::SetUpITKPipeline()
{
	SCBExampleITKPipeline* pipeline = new SCBExampleITKPipeline;

	vtkSmartPointer<vtkNIFTIImageReader> reader
		= vtkSmartPointer<vtkNIFTIImageReader>::New();

	reader->SetFileName("E:/TestProgram/git/SucabotProject/test1.nii.gz");
	reader->Update();

	reader->GetOutput()->Print(cout);

	if (!reader->GetOutput())
	{
		cerr << "No such file!";
		PAUSE_MACRO;
	}

	pipeline->SetInputData(reader->GetOutput());
	pipeline->UpdatePipeline();

	/* GetRenderers will be depricated after AddProp is prepared! */
	QDockWidget* dock = new QDockWidget(this);
	dock->setWidget(pipeline);
	this->addDockWidget(Qt::RightDockWidgetArea, dock);

	vtkSmartPointer<vtkImageViewer2> viewer
		= vtkSmartPointer<vtkImageViewer2>::New();
	viewer->SetInputConnection(pipeline->GetComponentFilter(1)->GetOutputPort());
	viewer->SetRenderWindow(this->m_viewer->GetRenderWindow());
	viewer->SetupInteractor(this->m_viewer->GetInteractor());
	viewer->Render();

	Updater* up = new Updater;
	up->SetRenderWindow(this->m_viewer->GetRenderWindow());
	up->ConnectModifyingObject(pipeline->GetComponentFilter(1));
}
