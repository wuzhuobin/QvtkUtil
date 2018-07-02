//me
#include "QvtkSceneExample.h"
#include "ui_QvtkSceneExample.h"
#include "QvtkPlanarViewer.h"
#include "QvtkImage.h"
#include "QvtkImageSlice.h"
#include "QvtkImageLabel2.h"
//vtk
#include <vtkActor.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
//qt
#include <QDebug>
QvtkSceneExample::QvtkSceneExample(QWidget * parent)
	:QMainWindow(parent),
	m_ui(new Ui::QvtkSceneExample),
	m_viewer(new Q::vtk::PlanarViewer(this)),
	m_image(new Q::vtk::Image),
	m_label(new Q::vtk::ImageLabel2),
	m_imageSlice(new Q::vtk::ImageSlice),
	m_imageLabelSlice(new Q::vtk::ImageSlice)
{
	this->m_ui->setupUi(this);
	this->m_ui->verticalLayout->addWidget(this->m_viewer);
	this->m_viewer->GetCursorActor()->VisibilityOff();
	// Create a "grayscale" 16x16 image, 1-component pixels of type "unsigned short"
	vtkSmartPointer<vtkImageData> image =
		vtkSmartPointer<vtkImageData>::New();
	int imageExtent[6] = { 0, 15, 0, 15, 0, 0 };
	image->SetExtent(imageExtent);
	image->AllocateScalars(VTK_UNSIGNED_SHORT, 1);
	unsigned short scalarvalue = 0;
	for (int y = imageExtent[2]; y <= imageExtent[3]; y++)
	{
		for (int x = imageExtent[0]; x <= imageExtent[1]; x++)
		{
			unsigned short *pixel = static_cast<unsigned short*>(image->GetScalarPointer(x, y, 0));
			pixel[0] = scalarvalue;
			scalarvalue += 1;
		}
	}
	this->m_label->getImageData()->ShallowCopy(image);
	this->m_image->getImageData()->ShallowCopy(image);
	this->m_image->resetWindowLevel();
	this->m_label->resetWindowLevel();
	//this->m_label->setWindow(15 * 15);
	//this->m_label->setLevel(15 * 15 * 0.5);
}

QvtkSceneExample::~QvtkSceneExample()
{
	delete this->m_ui;
	delete this->m_image;
	delete this->m_label;
	delete this->m_imageSlice;
	delete this->m_imageLabelSlice;
}

void QvtkSceneExample::on_action_New2_triggered(bool checked)
{
	this->m_viewer->RemoveAllProp();
	this->m_imageLabelSlice->setRenderDataSet(nullptr);
	this->m_imageLabelSlice->setRenderDataSet(this->m_label);
	this->m_viewer->AddProp(this->m_imageLabelSlice);
	this->m_viewer->SetOrientationToAxial();
	this->m_viewer->resetCamera(1);
	this->m_viewer->resetCameraClippingRange(1);
	this->m_viewer->update();
}

void QvtkSceneExample::on_action_New_triggered(bool checked)
{
	this->m_viewer->RemoveAllProp();
	this->m_imageSlice->setRenderDataSet(nullptr);
	this->m_imageSlice->setRenderDataSet(this->m_image);
	this->m_viewer->AddProp(this->m_imageSlice);
	this->m_viewer->SetOrientationToAxial();
	this->m_viewer->resetCamera(0);
	this->m_viewer->resetCameraClippingRange(0);
	this->m_viewer->update();
}
