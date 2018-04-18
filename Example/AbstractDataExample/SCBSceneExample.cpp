#include "SCBSceneExample.h"

//me
#include "ui_SCBSceneExample.h"
#include "SCBScene.h"
#include "SCBPolyData.h"
#include "SCBImage.h"
#include "SCBVolume.h"
#include "SCBPolyDataActor.h"

//vtk
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <QVTKWidget2.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkNew.h>

//qt
#include <QStandardItemModel>
#include <QDebug>

SCBSceneExample::SCBSceneExample(QWidget * parent)
	:QMainWindow(parent)
{

	this->m_ui = new Ui::SCBSceneExample;
	this->m_ui->setupUi(this);

	vtkNew<vtkRenderer> renderer;
	m_ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer.GetPointer());

	connect(this->m_ui->actionPrint, &QAction::triggered,
			this, &SCBSceneExample::slotPrint);
	connect(this->m_ui->pushButton, &QPushButton::clicked,
			this , &SCBSceneExample::slotPushButton);
	connect(this->m_ui->actionCommit, &QAction::triggered,
			this, &SCBSceneExample::slotCommit);
	connect(this->m_ui->actionImport, &QAction::triggered,
			this, &SCBSceneExample::slotImport);


}

SCBSceneExample::~SCBSceneExample()
{
	delete this->m_ui;
}

void SCBSceneExample::slotPushButton()
{

	SCBPolyData* polydata = new SCBPolyData;
	polydata->setAbsolutePath(QStringList() << "E:/TestProgram/git/SucabotProject/test.vtp");
	polydata->setRelativePath(QStringList() << "bbb.vtp");
	polydata->readData();
	SCBScene::getCurrentScene()->addData(polydata);


	SCBImage* image = new SCBImage;
	image->setAbsolutePath(QStringList() << "E:/TestProgram/git/SucabotProject/test.nii");
	image->setRelativePath(QStringList() << "test.nii.gz");
	image->readData();
	SCBScene::getCurrentScene()->addData(image);


	SCBPolyDataActor* actor = new SCBPolyDataActor;
	actor->setRenderDataSet(polydata);
	SCBScene::getCurrentScene()->addData(actor);

	SCBVolume* volume = new SCBVolume;
	volume->setRenderDataSet(image);
	SCBScene::getCurrentScene()->addData(volume);

	m_ui->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor->getActor());
	m_ui->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddVolume(volume->getVolume());
	m_ui->qvtkWidget->GetRenderWindow()->Render();

}

void SCBSceneExample::slotImport()
{
	m_ui->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveAllViewProps();
	SCBScene* scene = SCBScene::getCurrentScene();
	scene->setURL("E:/TestProgram/git/SucabotProject/url_commit.xml");
	scene->import(true);

	QStringList allKeys = scene->getAllData();
	if(allKeys.size() < 0){
		qCritical() << "Have no data at all.";
		return ;
	}

	for(int i = 0; i < allKeys.size(); ++i){
		SCBAbstractProp* prop = qobject_cast<SCBAbstractProp*>(scene->getDataByUniqueName(allKeys[i]));
		if(prop){
			m_ui->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(prop->getProp());
		}


	}

	SCBAbstractData* data = scene->getDataByUniqueName(allKeys.first());
	this->m_ui->treeView->setModel(data->getModel());
	//this->m_ui->listView->setModel(data->getModel());
}

void SCBSceneExample::slotCommit()
{
	SCBScene* scene = SCBScene::getCurrentScene();
	scene->setURL("E:/TestProgram/git/SucabotProject/url_commit.xml");
	scene->commit();
}

void SCBSceneExample::slotPrint()
{
	SCBScene::getCurrentScene()->printSelf();
}
