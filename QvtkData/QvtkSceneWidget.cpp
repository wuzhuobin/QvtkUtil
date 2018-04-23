// me
#include "QvtkSceneWidget.h"
#include "ui_QvtkSceneWidget.h"
#include "QvtkScene.h"
#include "QvtkData.h"

//qt
#include <QStandardItemModel>
#include <QDebug>

namespace Q {
	namespace vtk {


SceneWidget::SceneWidget(QWidget * parent)
	:QWidget(parent)
{
	this->ui = new Ui::SceneWidget;
	this->ui->setupUi(this);

	this->tmpData = nullptr;

	Scene* scene = Scene::getCurrentScene();

	// connection 
	//connect(scene, &Scene::signalImported, this, &SceneWidget::slotSceneReloaded);
	connect(scene, &Scene::signalCleared, this, &SceneWidget::slotSceneReloaded);
	//connect(scene, SIGNAL(Scene::signalAddedData(QString)), this, SLOT(SceneWidget::slotSceneAddedData(QString)));
	connect(scene, &Scene::signalAddedData, this, &SceneWidget::slotSceneAddedData);
	connect(scene, &Scene::signalRemovedData, this, &SceneWidget::slotSceneRemoveData);
	connect(this->ui->listWidgetQvtkScene, &QListWidget::itemClicked, 
		this, static_cast<void(SceneWidget::*)(QListWidgetItem*)>(&SceneWidget::slotSceneClicked));

	
	
}

SceneWidget::~SceneWidget()
{
	delete this->ui;
}

void SceneWidget::slotSceneReloaded()
{
	Scene* scene = Scene::getCurrentScene();
	this->ui->listWidgetQvtkScene->clear();
	// checking for removing warning of disconnect nullptr
	if (this->tmpData) {
		disconnect(this->tmpData, &Data::aliasChanged,
			this->ui->lineEditAlias, &QLineEdit::setText);
		disconnect(this->ui->lineEditAlias, &QLineEdit::textChanged,
			this->tmpData, &Data::setAlias);
	}

	QStringList allData = scene->getAllSelectableData();

	for (QStringList::iterator it = allData.begin();
		it != allData.end(); ++it) {
		(*it) = uniqueNameToItemName(*it);
	}

	this->ui->listWidgetQvtkScene->addItems(allData);

	// need to set it to nullptr, because everything is destroyed.
	this->tmpData = nullptr;

}

void SceneWidget::slotSceneAddedData(QString uniqueName)
{
	Scene* scene = Scene::getCurrentScene();
	if (scene->getDataByUniqueName(uniqueName)->getSelectable()) {
		uniqueName = uniqueNameToItemName(uniqueName);
		this->ui->listWidgetQvtkScene->addItem(uniqueName);
	}
}

void SceneWidget::slotSceneRemoveData(QString uniqueName)
{
	// check whether #tmpData is data to be removed.
	if (this->tmpData && 
		this->tmpData->getUniqueName() == uniqueName) {
		disconnect(this->tmpData, &Data::aliasChanged,
			this->ui->lineEditAlias, &QLineEdit::setText);
		disconnect(this->ui->lineEditAlias, &QLineEdit::textChanged,
			this->tmpData, &Data::setAlias);
		this->tmpData = nullptr;
	}
	uniqueName = uniqueNameToItemName(uniqueName);
	QList<QListWidgetItem*> items =
		this->ui->listWidgetQvtkScene->findItems(uniqueName, Qt::MatchExactly);
	
	if (items.isEmpty()) {
		qWarning() << "There is no " << uniqueName << "in the list";
		qWarning() << "No items is deleted.";
		return;
	}

	if (items.count() > 1) {
		qWarning() << "Multiple items are found. ";
		qWarning() << "Supposed there is only one uniqueName" << uniqueName << 
			"should be found";
	}

	for (QList<QListWidgetItem*>::const_iterator cit = items.cbegin();
		cit != items.cend(); ++cit)
	{
		this->ui->listWidgetQvtkScene->removeItemWidget(*cit);
		// delete it 
		delete *cit;
	}
}

void SceneWidget::slotSceneClicked(QListWidgetItem * item)
{
	slotSceneClicked(itemNameToUniqueName(item->text()));
}

void SceneWidget::slotSceneClicked(QString uniqueName)
{
	Scene* scene = Scene::getCurrentScene();
	Data* data = scene->getDataByUniqueName(uniqueName);
	if (this->tmpData == data) {
		return;
	}
	if (this->tmpData) {
		this->tmpData->selectedOff();
		disconnect(this->tmpData, &Data::aliasChanged,
			this->ui->lineEditAlias, &QLineEdit::setText);
		disconnect(this->ui->lineEditAlias, &QLineEdit::textChanged,
			this->tmpData, &Data::setAlias);
	}
	this->tmpData = data;
	this->tmpData->selectedOn();
	this->ui->lineEditAlias->setText(this->tmpData->getAlias());
	connect(this->tmpData, &Data::aliasChanged, 
		this->ui->lineEditAlias, &QLineEdit::setText);
	connect(this->ui->lineEditAlias, &QLineEdit::textChanged,
		this->tmpData, &Data::setAlias);
	this->ui->treeViewQvtkData->setModel(this->tmpData->getModel());
	this->ui->treeViewQvtkData->expandAll();
}

QString SceneWidget::uniqueNameToItemName(QString uniqueName)
{
	Scene* scene = Scene::getCurrentScene();
	QString itemName = scene->getTagByClassName(scene->getDataByUniqueName(uniqueName)->getClassName());
	itemName += ": ";
	itemName += uniqueName;
	return itemName;
}

QString SceneWidget::itemNameToUniqueName(QString uniqueName)
{
	return uniqueName.split(": ").last();
}


}
}
