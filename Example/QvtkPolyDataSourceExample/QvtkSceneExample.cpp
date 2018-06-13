//me
#include "QvtkSceneExample.h"
#include "ui_QvtkSceneExample.h"
#include "QvtkPolyDataActor.h"
#include "QvtkPolyDataSource.h"
#include "QvtkViewer2.h"
//vtk
#include <vtkPolyDataMapper.h>
//qt
#include <QStandardItemModel>
#include <QDebug>

QvtkSceneExample::QvtkSceneExample(QWidget * parent)
	:QMainWindow(parent)
{
	this->m_ui = new Ui::QvtkSceneExample;
	this->m_ui->setupUi(this);
	this->m_viewer = new Q::vtk::Viewer2(this);
	this->m_ui->verticalLayout->addWidget(this->m_viewer);
	for (int i = 0; i < 5; ++i) {
		this->m_sources[i] = new Q::vtk::PolyDataSource;
		this->m_sources[i]->setSourceType(i);
		this->m_sources[i]->setColor(0.2*i, 1-0.2*i, i *0.1);
		this->m_sources[i]->setPosition(i, i, i);

		this->m_actors[i] = new Q::vtk::PolyDataActor;
		this->m_actors[i]->setRenderDataSet(this->m_sources[i]);
	}
	//connect(this->m_ui->action_New, &QAction::triggered, this, &QvtkSceneExample::on_action_New_triggered);
}

QvtkSceneExample::~QvtkSceneExample()
{
	//this->m_viewer->RemoveAllProp();
	for (int i = 0; i < 5; ++i) {
		//this->m_viewer->RemoveProp(this->m_actors[i]);
		this->m_actors[i]->setRenderDataSet(nullptr);
		delete this->m_actors[i];
		delete this->m_sources[i];
	}

	delete this->m_ui;
}

void QvtkSceneExample::on_action_New_triggered(bool checked)
{
	for (int i = 0; i < 5; ++i) {
		this->m_viewer->AddProp(this->m_actors[i]);
	}
	this->m_viewer->ResetCamera(0);
	//this->m_viewer->Render();
	this->m_viewer->ResetCameraClippingRange(0);
	this->m_viewer->Render();
	qDebug() << __FUNCTION__;
}
