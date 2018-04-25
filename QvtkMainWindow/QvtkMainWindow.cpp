#include "ui_QvtkAbstractMainWindow.h"

#include <QAction>

#include "QvtkViewer.h"
#include "QvtkAbstractMainWindow.h"
#include "QvtkAbstractInteractorObserver.h"
#include "QvtkAbstractPipeline.h"

QvtkAbstractMainWindow* QvtkAbstractMainWindow::instance;

QvtkAbstractMainWindow* QvtkAbstractMainWindow::Get()
{
    if (instance == nullptr)
    {
        qCritical() << "Singleton instance not created! Call New() before calling Get()";
    }
    return instance;
}


QvtkAbstractMainWindow* QvtkAbstractMainWindow::New()
{
    qCritical() << "Cannot new an abstract class!";
    std::logic_error("Cannot new an abstract class!");
    return nullptr;
}

void QvtkAbstractMainWindow::slotActionBarHandle()
{

}

void QvtkAbstractMainWindow::slotChangeInteractorStyle()
{
    qCritical() << "Function not implemented: " << __func__;
}

void QvtkAbstractMainWindow::slotChangePipeline()
{
    QvtkAbstractPipeline* targetObj = this->mapActionToPipeline[(QAction*)this->sender()];
    if (!this->ui->tabPipeline->layout())
    {
        QGridLayout* layout = new QGridLayout(this->ui->tabPipeline);
        layout->setMargin(3);
    }

    /* Clear current layout */
    bool targetObjInLayout = false;
    QLayout* layout = this->ui->tabPipeline->layout();
    for (int i = 0; i < layout->count();i++)
    {
        QLayoutItem* item = layout->itemAt(i);
        if (item->widget())
        {
            /* hide obj if it is not our target object! Otherwise, display it */
            if (item->widget() != targetObj)
                item->widget()->setVisible(false);
            else
            {
                item->widget()->setVisible(true);
                targetObjInLayout = true;
            }
        }
    }
    /* if the widget is not already in the layout, add it */
    if (!targetObjInLayout)
        this->ui->tabPipeline->layout()->addWidget(targetObj);
}

void QvtkAbstractMainWindow::slotTogglePanelHide()
{
    if (!dynamic_cast<QAction*>(this->sender()))
    {
        qWarning() << "Slot connection is incorrect! Trying to connect objects that are not mutable to \
            QAction to function: " << __func__;
        return;
    }

    QAction* action = (QAction*)(this->sender());
    if (this->sender() == this->ui->actionHide_Left_Panel)
    {
        if (action->isChecked())
        {
            this->ui->dockLeft->setHidden(true);
            //this->ui->tabwidgetLeft->setMaximumWidth(1);
        }
        else
        {
            this->ui->dockLeft->setHidden(false);
            //this->ui->tabwidgetLeft->setMaximumWidth(65535);
        }
    }
    else if (this->sender() == this->ui->actionHide_Right_Panel)
    {

        if (action->isChecked())
        {
            this->ui->dockRight->setHidden(true);
            //this->ui->tabwidgetRight->setMaximumWidth(1);
        }
        else
        {
            this->ui->dockRight->setHidden(false);
            //this->ui->tabwidgetRight->setMaximumWidth(65535);
        }
    }
}

void QvtkAbstractMainWindow::slotTogglePanelDock()
{
    /* Check slot type */
    if (!dynamic_cast<QAction*>(this->sender()))
    {
        qWarning() << "Slot connection is incorrect! Trying to connect objects that are not mutable to \
            QAction to function: " << __func__;
        return;
    }

    if (this->sender() == this->ui->actionDock_Left_Panel)
    {
        QAction* action = (QAction*)(this->sender());
        if (action->isChecked())
        {
            this->ui->dockLeft->setTitleBarWidget(new QWidget(this->ui->dockRight));
            this->ui->dockLeft->setAllowedAreas(Qt::RightDockWidgetArea);
            this->ui->dockLeft->setFloating(false);
        }
        else
        {
            delete this->ui->dockLeft->titleBarWidget();  /* */
            this->ui->dockLeft->setTitleBarWidget(0);     /* Show default tittle bar */
            this->ui->dockLeft->setAllowedAreas(0);
            this->ui->dockLeft->setFloating(true);
        }
    }
    else if (this->sender() == this->ui->actionDock_Right_Panel)
    {
        QAction* action = (QAction*)(this->sender());
        if (action->isChecked())
        {
            this->ui->dockRight->setTitleBarWidget(new QWidget(this->ui->dockRight));
            this->ui->dockRight->setAllowedAreas(Qt::RightDockWidgetArea);
            this->ui->dockRight->setFloating(false);
        }
        else
        {
            delete this->ui->dockRight->titleBarWidget();
            this->ui->dockRight->setTitleBarWidget(0);
            this->ui->dockRight->setAllowedAreas(0);
            this->ui->dockRight->setFloating(true);
        }
    }
    else {
        qWarning() << "Incorrect connection! Sent by: " << this->sender()->objectName();
    }
}

void QvtkAbstractMainWindow::Initialize()
{
    this->SetUpViewers();
}


Ui_MainWindow* QvtkAbstractMainWindow::GetUI()
{
	return ui;
}


void QvtkAbstractMainWindow::ConnectUI()
{
    connect(this->ui->actionHide_Left_Panel,  SIGNAL(triggered()), this, SLOT(slotTogglePanelHide()));
    connect(this->ui->actionHide_Right_Panel, SIGNAL(triggered()), this, SLOT(slotTogglePanelHide()));

    connect(this->ui->actionDock_Left_Panel,  SIGNAL(triggered()), this, SLOT(slotTogglePanelDock()));
    connect(this->ui->actionDock_Right_Panel, SIGNAL(triggered()), this, SLOT(slotTogglePanelDock()));
}

void QvtkAbstractMainWindow::AddInteractorStyle(InteractorObserver* obj,
                                               QString objName)
{
    // #TODO: QvtkAbstractMainWindow::AddInteractorStyle(obj, objName)
    qCritical() << "Function not implemented: " << __func__;

	/* Creates an action for activating this obj */
	QAction* newAction = new QAction(this);
	if (objName != "")
		newAction->setText(objName);
	else
		newAction->setText(obj->objectName());
	QString actionName = QString("action") + newAction->text();
}


void QvtkAbstractMainWindow::AddPipline(QvtkAbstractPipeline* obj, QString objName)
{
    /* Creates an action for activating this obj */
    QAction* newAction = new QAction(this);
    if (objName != "")
        newAction->setText(objName);
    else
        newAction->setText(obj->objectName());
    QString actionName = QString("action") + newAction->text();

    /// DEBUG
    qDebug() << "Adding action: " << actionName;

    /* Add the action to the mapping */
    this->mapActionToPipeline[newAction] = obj;

    /* Connects the action to a slot which triggers the change in obj */
    connect(newAction, SIGNAL(triggered()), this, SLOT(slotChangePipeline()));

    /* Add this action to list */
    this->ui->menuFunctions->addAction(newAction);
}

QvtkAbstractMainWindow::QvtkAbstractMainWindow(QObject* parent/*=nullptr*/)
{
    /* Create an instance if there are none */
    this->ui = new Ui_MainWindow;
    this->ui->setupUi(this);

    /* Hide the titlebar of the dock widgets */
    QWidget* titleWidgetLeft = new QWidget(this->ui->dockLeft);
    QWidget* titleWidgetRight = new QWidget(this->ui->dockRight);
    this->ui->dockLeft->setTitleBarWidget(titleWidgetLeft);
    this->ui->dockRight->setTitleBarWidget(titleWidgetRight);

	/* Add seperators to menubar */
	this->menuBarSeperators << this->ui->toolBar->addSeparator();
	this->menuBarSeperators << this->ui->toolBar->addSeparator();

    this->ConnectUI();
}
