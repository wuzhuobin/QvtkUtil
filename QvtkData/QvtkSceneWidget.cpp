// me
#include "QvtkSceneWidget.h"
#include "ui_QvtkSceneWidget.h"
#include "QvtkScene.h"
#include "Data.h"

//qt
#include <QStandardItemModel>
#include <QDebug>



QvtkSceneWidget::QvtkSceneWidget(QWidget * parent)
	:QWidget(parent)
{
	this->ui = new Ui::QvtkSceneWidget;
	this->ui->setupUi(this);

	this->tmpData = nullptr;

	QvtkScene* scene = QvtkScene::getCurrentScene();

	// connection 
	//connect(scene, &QvtkScene::signalImported, this, &QvtkSceneWidget::slotSceneReloaded);
	connect(scene, &QvtkScene::signalCleared, this, &QvtkSceneWidget::slotSceneReloaded);
	//connect(scene, SIGNAL(QvtkScene::signalAddedData(QString)), this, SLOT(QvtkSceneWidget::slotSceneAddedData(QString)));
	connect(scene, &QvtkScene::signalAddedData, this, &QvtkSceneWidget::slotSceneAddedData);
	connect(scene, &QvtkScene::signalRemovedData, this, &QvtkSceneWidget::slotSceneRemoveData);
	connect(this->ui->listWidgetQvtkScene, &QListWidget::itemClicked, 
		this, static_cast<void(QvtkSceneWidget::*)(QListWidgetItem*)>(&QvtkSceneWidget::slotSceneClicked));

	
	
}

QvtkSceneWidget::~QvtkSceneWidget()
{
	delete this->ui;
}

void QvtkSceneWidget::slotSceneReloaded()
{
	QvtkScene* scene = QvtkScene::getCurrentScene();
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

void QvtkSceneWidget::slotSceneAddedData(QString uniqueName)
{
	QvtkScene* scene = QvtkScene::getCurrentScene();
	if (scene->getDataByUniqueName(uniqueName)->getSelectable()) {
		uniqueName = uniqueNameToItemName(uniqueName);
		this->ui->listWidgetQvtkScene->addItem(uniqueName);
	}
}

void QvtkSceneWidget::slotSceneRemoveData(QString uniqueName)
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

void QvtkSceneWidget::slotSceneClicked(QListWidgetItem * item)
{
	slotSceneClicked(itemNameToUniqueName(item->text()));
}

void QvtkSceneWidget::slotSceneClicked(QString uniqueName)
{
	QvtkScene* scene = QvtkScene::getCurrentScene();
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

QString QvtkSceneWidget::uniqueNameToItemName(QString uniqueName)
{
	QvtkScene* scene = QvtkScene::getCurrentScene();
	QString itemName = scene->getTagByClassName(scene->getDataByUniqueName(uniqueName)->getClassName());
	itemName += ": ";
	itemName += uniqueName;
	return itemName;
}

QString QvtkSceneWidget::itemNameToUniqueName(QString uniqueName)
{
	return uniqueName.split(": ").last();
}


