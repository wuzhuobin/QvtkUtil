#include "QvtkViewer.h"
#include "ui_QvtkViewer.h"

#include <QDebug>

//qt
#include <QVTKInteractor.h>

//vtk
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>

QvtkViewer::QvtkViewer(QWidget * parent)
	:QvtkAbstractViewer(parent)
{
	this->ui = new Ui::QvtkViewer;
	this->ui->setupUi(this);

    /* This macro is called the first thing */
    Qvtk_INIT_VIEWER_MACRO();

    //////////////////////////////////////////////////////////////////////////
    /// Optional below, implemented according to requirements 
	//this->GetRenderWindow()->SetNumberOfLayers(2);
 //   vtkRenderer* ren = this->AddRenderer();
 //   ren->SetLayer(1);
}

QvtkViewer::~QvtkViewer()
{
	delete this->ui;
}

vtkRenderWindow * QvtkViewer::GetRenderWindow()
{
	return GetGenericOpenGLRenderWindow();
}

vtkGenericOpenGLRenderWindow *QvtkViewer::GetGenericOpenGLRenderWindow()
{
	return this->ui->qvtkWidget->GetRenderWindow();
}

vtkRenderWindowInteractor* QvtkViewer::GetInteractor()
{
	return GetQVTKInteractor();
}

QVTKInteractor *QvtkViewer::GetQVTKInteractor()
{
	return this->ui->qvtkWidget->GetInteractor();
}

QVTKWidget2* QvtkViewer::GetQVTKWidget()
{
	return this->ui->qvtkWidget;
}
