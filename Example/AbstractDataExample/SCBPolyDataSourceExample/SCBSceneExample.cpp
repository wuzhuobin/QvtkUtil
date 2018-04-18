#include "SCBSceneExample.h"

//me
#include "ui_SCBSceneExample.h"
#include "SCBPolyDataActor.h"
#include "SCBPolyDataSource.h"
#include "SCBOrthogonalViewer.h"

//vtk
#include <vtkPolyDataMapper.h>

//qt
#include <QStandardItemModel>
#include <QDebug>

SCBSceneExample::SCBSceneExample(QWidget * parent)
	:QMainWindow(parent)
{
	this->m_ui = new Ui::SCBSceneExample;
	this->m_ui->setupUi(this);

	this->m_viewer = new SCBOrthogonalViewer(this);
	this->m_ui->verticalLayout->addWidget(this->m_viewer);

	//this->m_sceneWidget = new SCBSceneWidget(this);
	//this->m_ui->verticalLayout->addWidget(this->m_sceneWidget);

	//this->m_scene = SCBScene::getCurrentScene();

	for (int i = 0; i < 5; ++i) {
		this->m_sources[i] = new SCBPolyDataSource;
		this->m_sources[i]->setSourceType(i);
		this->m_sources[i]->readData();
		this->m_sources[i]->setColor(0.2*i, 1-0.2*i, i *0.1);
		this->m_sources[i]->setPosition(i, i, i);

		this->m_actors[i] = new SCBPolyDataActor;
		this->m_actors[i]->setRenderDataSet(this->m_sources[i]);
	}
	//connect(this->m_ui->action_New, &QAction::triggered, this, &SCBSceneExample::on_action_New_triggered);
}

SCBSceneExample::~SCBSceneExample()
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

void SCBSceneExample::on_action_New_triggered(bool checked)
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
