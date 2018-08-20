/*
Author:		Wong, Matthew Lun
Date:		05, July 2017
Occupation:	Chinese University of Hong Kong,
			Department of Imaging and Inteventional Radiology,
			M.Phil Student

FileName: 	Test_Filter.cpp

Descriptions:
	This test demo the use of the example filter. Two main were written in this file for
    testing both vtk and itk wrapped SCBFilters.


Wong Matthew Lun
Copyright (C) 2017
*/

#include "ExampleFilter.h"

// vtk
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>
#include <vtkCubeSource.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkTriangleFilter.h>

// qt
#include <QApplication>
#include <QObject>
#include <QWidget>

#include "ExampleUpdater.h"

//int main(int argc, char *argv[]) {
//	QApplication app(argc, argv);
//
//
//	ExamplePolyDataFilter* filter = new ExamplePolyDataFilter;
//
//	 /*	Creates something to subdivide */
//	vtkCubeSource* source = vtkCubeSource::New();
//	source->SetXLength(10);
//	source->SetYLength(10);
//	source->SetYLength(10);
//	source->Update();
//
//	vtkTriangleFilter* tri = vtkTriangleFilter::New();
//	tri->SetInputConnection(source->GetOutputPort());
//	tri->Update();
//
//	/* Actually use the filter and show the UI */
//	filter->GetWidget()->show();
//	filter->SetInputConnection(tri->GetOutputPort());
//	filter->Update();
//
//	filter->Print(std::cout);
//	
//	/* Render the mini application */
//	vtkPolyDataMapper*			mapper = vtkPolyDataMapper::New();
//	vtkActor*					actor = vtkActor::New();
//	vtkRenderer*				ren = vtkRenderer::New();
//	vtkRenderWindow*			renwin = vtkRenderWindow::New();
//	vtkRenderWindowInteractor*	iren = vtkRenderWindowInteractor::New();
//
//	/* Add an updater which allows the renderer to automatically update if desired */
//	Updater* updater = new Updater;
//	updater->SetRenderWindow(renwin);
//	updater->ConnectModifyingObject(filter);
//
//	mapper->SetInputConnection(filter->GetOutputPort());
//	actor->SetMapper(mapper);
//	actor->GetProperty()->SetRepresentationToWireframe();
//
//	ren->AddActor(actor);
//	renwin->AddRenderer(ren);
//	renwin->SetInteractor(iren);
//	iren->Initialize();
//	ren->Render();
//	iren->Start();
//	return app.exec();
//}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* Test ITK Filter                                                      */
/************************************************************************/
//#include "Example_ITKFilter.h"
//#include <QFile>
//#include <vtkNIFTIImageReader.h>
//#include <vtkImageViewer.h>
//#include <vtkInteractorStyleImage.h>
//
//int main(int argc, char *argv[]) {
//    QApplication app(argc, argv);
//
//
//    /* Error checks and parse arguments */
//    if (argc < 2)
//    {
//        std::cout << "Usage: \n\n";
//        std::cout << "\t Test_Filter [filename] \n";
//    }
//    QString filename(argv[1]);
//    if (!QFile(filename).exists())
//    {
//        std::cerr << "File doesn't exist!" << endl;
//        return 1;
//    }
//    else if (filename.split('.').last() != "gz" &&
//        filename.split('.').last() != "nii")
//    {
//        std::cerr << "Must be nii file!" << endl;
//        return 1;
//    }
//
//    /* Read image if it exist */
//    vtkSmartPointer<vtkNIFTIImageReader> reader
//        = vtkSmartPointer<vtkNIFTIImageReader>::New();
//    reader->SetFileName(filename.toStdString().c_str());
//    reader->Update();
//
//    vtkImageData* im = reader->GetOutput();
//    int scalartype = im->GetScalarType();
//
//    /* Declare filter according to input image type */
//    SCBAbstractITKFilter* filter;
//    vtkAlgorithmOutput* filterport;
//    if (scalartype == VTK_FLOAT) 
//    {
//        typedef itk::Image<float, 3> ImageType;
//        typedef SCBExampleITKFilter<ImageType> FilterType;
//        filter = new FilterType();
//        ((FilterType*)filter)->SetInputData(im);
//        ((FilterType*)filter)->Update();
//        ((FilterType*)filter)->GetWidget()->show();
//        filterport = ((FilterType*)filter)->GetOutputPort();
//    }
//    else if (scalartype == VTK_DOUBLE)
//    {
//        typedef itk::Image<double, 3> ImageType;
//        typedef SCBExampleITKFilter<ImageType> FilterType;
//        filter = new FilterType();
//        ((FilterType*)filter)->SetInputData(im);
//        ((FilterType*)filter)->Update();
//        ((FilterType*)filter)->GetWidget()->show();
//        filterport = ((FilterType*)filter)->GetOutputPort();
//    }
//    else if (scalartype == VTK_SHORT)
//    {
//        typedef itk::Image<short, 3> ImageType;
//        typedef SCBExampleITKFilter<ImageType> FilterType;
//        filter = new FilterType();
//        ((FilterType*)filter)->SetInputData(im);
//        ((FilterType*)filter)->Update();
//        ((FilterType*)filter)->GetWidget()->show();
//        filterport = ((FilterType*)filter)->GetOutputPort();
//    }
//    else if (scalartype == VTK_INT)
//    {
//        typedef itk::Image<int, 3> ImageType;
//        typedef SCBExampleITKFilter<ImageType> FilterType;
//        filter = new FilterType();
//        ((FilterType*)filter)->SetInputData(im);
//        ((FilterType*)filter)->Update();
//        ((FilterType*)filter)->GetWidget()->show();
//        filterport = ((FilterType*)filter)->GetOutputPort();
//    }
//    else{
//        std::cerr << "No availabel scalar type!" << endl;
//        return app.exec();
//    }
//
//    /* Setup viewer */
//    vtkSmartPointer<vtkImageViewer> viewer 
//        = vtkSmartPointer<vtkImageViewer>::New();
//    viewer->SetInputConnection(filterport);
//    viewer->SetZSlice(50);
//
//    /* Connect filter to updater for realtime update */
//    Updater* updater = new Updater();
//    updater->SetRenderWindow(viewer->GetRenderWindow());
//    updater->ConnectModifyingObject(filter);
//
//    /* Render */
//    vtkRenderWindowInteractor*	iren = vtkRenderWindowInteractor::New();
//    viewer->SetupInteractor(iren);
//    iren->Initialize();
//    viewer->Render();
//    iren->Start();
//    return app.exec();
//}
//////////////////////////////////////////////////////////////////////////
#include "SCBITKImageToBinaryImage.h"
#include <QFile>
#include <vtkNIFTIImageReader.h>
#include <vtkImageViewer.h>
#include <vtkInteractorStyleImage.h>

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);


	/* Error checks and parse arguments */
	if (argc < 2)
	{
		std::cout << "Usage: \n\n";
		std::cout << "\t Test_Filter [filename] \n";
	}
	//QString filename(argv[1]);
	QString filename = "E:/TestProgram/git/SucabotProject/test.nii.gz";
	if (!QFile(filename).exists())
	{
		std::cerr << "File doesn't exist!" << endl;
		return 1;
	}
	else if (filename.split('.').last() != "gz" &&
		filename.split('.').last() != "nii")
	{
		std::cerr << "Must be nii file!" << endl;
		return 1;
	}

	/* Read image if it exist */
	vtkSmartPointer<vtkNIFTIImageReader> reader
		= vtkSmartPointer<vtkNIFTIImageReader>::New();
	reader->SetFileName(filename.toStdString().c_str());
	reader->Update();

	vtkImageData* im = reader->GetOutput();
	int scalartype = im->GetScalarType();

	/* Declare filter according to input image type */
	SCBAbstractITKFilter* filter;
	vtkAlgorithmOutput* filterport;
	if (scalartype == VTK_FLOAT)
	{
		typedef itk::Image<float, 3> ImageType;
		typedef SCBITKImageToBinaryImage<float> FilterType;
		filter = new FilterType;
		((FilterType*)filter)->SetInputData(im);
		((FilterType*)filter)->Update();
		((FilterType*)filter)->show();
		filterport = ((FilterType*)filter)->GetOutputPort();
	}
	else if (scalartype == VTK_DOUBLE)
	{
		typedef itk::Image<double, 3> ImageType;
		typedef SCBITKImageToBinaryImage<double> FilterType;
		filter = new FilterType;
		((FilterType*)filter)->SetInputData(im);
		((FilterType*)filter)->Update();
		((FilterType*)filter)->show();
		filterport = ((FilterType*)filter)->GetOutputPort();
	}
	else if (scalartype == VTK_SHORT)
	{
		typedef itk::Image<short, 3> ImageType;
		typedef SCBITKImageToBinaryImage<short> FilterType;
		filter = new FilterType;
		((FilterType*)filter)->SetInputData(im);
		((FilterType*)filter)->Update();
		((FilterType*)filter)->show();
		filterport = ((FilterType*)filter)->GetOutputPort();
	}
	else if (scalartype == VTK_INT)
	{
		typedef itk::Image<int, 3> ImageType;
		typedef SCBITKImageToBinaryImage<int> FilterType;
		filter = new FilterType;
		((FilterType*)filter)->SetInputData(im);
		((FilterType*)filter)->Update();
		((FilterType*)filter)->show();
		filterport = ((FilterType*)filter)->GetOutputPort();
	}
	else {
		std::cerr << "No availabel scalar type!" << endl;
		return app.exec();
	}

	/* Setup viewer */
	vtkSmartPointer<vtkImageViewer> viewer
		= vtkSmartPointer<vtkImageViewer>::New();
	viewer->SetInputConnection(filterport);
	viewer->SetZSlice(12);

	/* Connect filter to updater for realtime update */
	Updater* updater = new Updater();
	updater->SetRenderWindow(viewer->GetRenderWindow());
	updater->ConnectModifyingObject(filter);

	/* Render */
	vtkRenderWindowInteractor*	iren = vtkRenderWindowInteractor::New();
	viewer->SetupInteractor(iren);
	//vtkSmartPointer<vtkInteractorStyleImage> style =
	//	vtkSmartPointer<vtkInteractorStyleImage>::New(); //like paraview
	iren->Initialize();
	//iren->SetInteractorStyle(style);
	
	viewer->Render();
	iren->Start();
	return app.exec();
}