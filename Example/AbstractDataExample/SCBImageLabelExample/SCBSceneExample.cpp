//me
#include "SCBSceneExample.h"
#include "ui_SCBSceneExample.h"
#include "SCBPlanarViewer.h"
#include "SCBImage.h"
#include "SCBImageSlice.h"
#include "SCBImageLabel.h"
#include "SCBScene.h"

//vtk
#include <vtkActor.h>

//qt
#include <QDebug>

SCBSceneExample::SCBSceneExample(QWidget * parent)
	:QMainWindow(parent),
	m_viewer(new SCBPlanarViewer(this)),
	m_image(new SCBImage),
	m_imageSlice(new SCBImageSlice),
	m_imageLabelSlice(new SCBImageSlice)
{
	this->m_ui = new Ui::SCBSceneExample;
	this->m_ui->setupUi(this);
	this->m_ui->verticalLayout->addWidget(this->m_viewer);

	SCBScene::getCurrentScene()->registerData(new SCBImageLabel);


	this->m_viewer->GetCursorActor()->VisibilityOff();
	this->m_image->setAbsolutePath(QStringList() << "image.nii.gz");
	this->m_image->readData();

	this->m_imageSlice->setRenderDataSet(this->m_image);
	//this->m_sceneWidget = new SCBSceneWidget(this);
	//this->m_ui->verticalLayout->addWidget(this->m_sceneWidget);

	//this->m_scene = SCBScene::getCurrentScene();


	//connect(this->m_ui->action_New, &QAction::triggered, this, &SCBSceneExample::on_action_New_triggered);
}

SCBSceneExample::~SCBSceneExample()
{
	delete this->m_ui;
	delete this->m_image;
	delete this->m_imageSlice;
	SCBScene::getCurrentScene()->removeData("This is a label. ");
	delete this->m_imageLabelSlice;
}

void SCBSceneExample::on_action_New2_triggered(bool checked)
{
	SCBImageLabel* imageLabel = new SCBImageLabel;
	imageLabel->setAbsolutePath(QStringList() << "label2.nii.gz");
	imageLabel->setRelativePath(QStringList() << "label2.nii.gz");
	imageLabel->readData();

	this->m_imageLabelSlice->setRenderDataSet(imageLabel);

	SCBScene* scene = SCBScene::getCurrentScene();
	scene->clear();
	scene->addData(imageLabel, "This is a label. ");

	this->m_viewer->AddProp(this->m_imageLabelSlice, this->m_viewer->GetRenderers()[1]);

	this->m_viewer->ResetCamera(1);
	this->m_viewer->ResetCameraClippingRange(1);
	this->m_viewer->Render();
}

void SCBSceneExample::on_action_New_triggered(bool checked)
{
	this->m_viewer->AddProp(this->m_imageSlice);

	this->m_viewer->ResetCamera(0);
	this->m_viewer->ResetCameraClippingRange(0);
	this->m_viewer->Render();
}

void SCBSceneExample::on_action_Opacity_triggered(bool checked)
{
	SCBScene* scene = SCBScene::getCurrentScene();
	SCBImageLabel* imageLabel = scene->getDataByUniqueName<SCBImageLabel>("This is a label. ");
	if (!imageLabel) return;
	imageLabel->setColor("red", 1, 0, 0, 0.3);
	imageLabel->setColor("green", 0, 1, 0, 0.5);
	imageLabel->setColor("blue", 0, 0, 1, 0.7);

}

void SCBSceneExample::on_action_Import_triggered(bool checked)
{
	SCBScene* scene = SCBScene::getCurrentScene();
	scene->setURL("Scene.xml");
	scene->import();

	SCBImageLabel* imageLabel = scene->getDataByUniqueName<SCBImageLabel>("This is a label. ");
	this->m_imageLabelSlice->setRenderDataSet(imageLabel);
	this->m_viewer->Render();
}

void SCBSceneExample::on_action_Commit_triggered(bool checked)
{
	SCBScene* scene = SCBScene::getCurrentScene();
	scene->setURL("Scene.xml");
	scene->commit("Scene");
	
}