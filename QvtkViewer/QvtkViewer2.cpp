#include "QvtkViewer2.h"
#include "ui_QvtkViewer2.h"

#include <QDebug>

//qt
#include <QVTKInteractor.h>

//vtk
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>

namespace Q {
namespace vtk{

Viewer2::Viewer2(QWidget * parent)
	:Viewer(parent)
{
	this->ui = new Ui::Viewer2;
	this->ui->setupUi(this);

    /* This macro is called the first thing */
    //QVTK_INIT_VIEWER_MACRO();
	this->setupFirstRenderer();

    //////////////////////////////////////////////////////////////////////////
    /// Optional below, implemented according to requirements 
	//this->GetRenderWindow()->SetNumberOfLayers(2);
 //   vtkRenderer* ren = this->addRenderer();
 //   ren->SetLayer(1);
}

Viewer2::~Viewer2()
{
	delete this->ui;
}

vtkRenderWindow * Viewer2::getRenderWindow()
{
	return GetGenericOpenGLRenderWindow();
}

vtkGenericOpenGLRenderWindow *Viewer2::GetGenericOpenGLRenderWindow()
{
	return this->ui->qvtkWidget->GetRenderWindow();
}

vtkRenderWindowInteractor* Viewer2::getInteractor()
{
	return GetQVTKInteractor();
}

QVTKInteractor *Viewer2::GetQVTKInteractor()
{
	return this->ui->qvtkWidget->GetInteractor();
}

QVTKWidget2* Viewer2::GetQVTKWidget()
{
	return this->ui->qvtkWidget;
}
}
}
