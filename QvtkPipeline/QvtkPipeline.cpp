/*!
 * \file QvtkAbstractPipeline.cpp
 * \date 2017/07/19 15:17
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong, 
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief This class collects multiple QvtkAbstractFilter subclasses, put them together and
 *        assembles their UI to form a panel like interface which allows interaction with
 *        the filter components. Support dynamic pipeline creation, but requires a concrete
 *        implementation.
 *
 *
 * \note
*/

#include "QvtkAbstractPipeline.h"

#include <QSpacerItem>
#include <QLayout>
#include <QDebug>
#include <QStackedLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QStackedWidget>
#include <QToolBox>
#include <QTabWidget>

#include <vtkSmartPointer.h>
#include <vtkPointData.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkCellArray.h>
#include <vtkLine.h>
#include <vtkArrayIteratorIncludes.h>

QvtkAbstractPipeline::QvtkAbstractPipeline(QWidget* parent/*=nullptr*/)
	:QWidget(parent)
{
    this->Qvtk_PIPELINE_CONNECTED_FLAG = false;
}

QvtkAbstractPipeline::~QvtkAbstractPipeline()
{
    qDeleteAll(this->children());
}

void QvtkAbstractPipeline::RemoveFilter(QvtkAbstractFilter* filter)
{
    /* Get filter index */
    int filterIndex = this->filterList.indexOf(filter);


    /* Detect any change in map */
    QList<FilterConenction> newList;
    for (QList<FilterConenction>::iterator iter = this->filterConnectionMap.begin();
    iter != this->filterConnectionMap.end(); iter++)
    {
        // 
        if (iter->postFilterIndex == filterIndex || iter->preFilterIndex == filterIndex)
        {
#ifdef _DEBUG // #TODO: _DEBUG not supported by linux
            QString msg = QString("Removing connection: [%1]%2-port%3 -> [%4]%5-port%6").arg(
                QString(iter->preFilterIndex),
                typeid(this->filterList[iter->preFilterIndex]).name(),
                QString(iter->preFilterOutputPort),
                QString(iter->postFilterIndex),
                typeid(this->filterList[iter->postFilterIndex]).name(),
                QString(iter->postFilterInputPort));
            qDebug() << msg;
#endif
            continue;
        }
        else
        {
            // Shift the index according to ths list
            if (iter->postFilterIndex > filterIndex)
                iter->postFilterIndex -= 1;
            if (iter->preFilterIndex > filterIndex)
                iter->preFilterIndex -= 1;
            newList.append(*iter);
        }
    }
    this->filterConnectionMap = newList;

    /* Remove UI and filter */
    this->filterUIList.removeOne(this->filterToUIMap[filter]);
    this->filterList.removeOne(filter);

    /* Force rebuilding connection */
    this->Qvtk_PIPELINE_CONNECTED_FLAG = false;

}

void QvtkAbstractPipeline::RemoveFilter(int index)
{
    this->RemoveFilter(this->filterList[index]);
}

void QvtkAbstractPipeline::AddFilter(QvtkAbstractFilter *filter, bool withUI /*= true*/)
{
    /* Adding same filter twice will mess with vtkAlgorithmOutput */
    if (this->filterList.indexOf(filter) != -1)
    {
        QString errmsg("Adding same filter object twice is not allowed!");
        qCritical() << errmsg;
        throw std::logic_error(errmsg.toStdString());
        return;
    }

    /* Add ui to the list for later BuildUI() use */
    if (withUI)
    {
        this->filterUIList.append(filter);
        this->filterToUIMap[filter] = filter;
    }

    this->filterList.append(filter);

    /* Force rebuilding connection */
    this->Qvtk_PIPELINE_CONNECTED_FLAG = false;
}

void QvtkAbstractPipeline::ConnectAllFilters()
{
    /* Clear connection map */
    this->ClearConnections();

    for (QList<QvtkAbstractFilter*>::const_iterator iter = this->filterList.cbegin() + 1;
    		 iter != this->filterList.cend(); iter++)
    {
        FilterConenction con;
        con.preFilterIndex = iter - this->filterList.cbegin() - 1;
        con.postFilterIndex = iter - this->filterList.cbegin();
        con.preFilterOutputPort = 0;
        con.postFilterInputPort = 0;
        this->filterConnectionMap.append(con);
    }

    this->BuildConenction();
}

void QvtkAbstractPipeline::ClearConnections()
{
    this->filterConnectionMap.clear();

    this->Qvtk_PIPELINE_CONNECTED_FLAG = false;
}

void QvtkAbstractPipeline::ConnectFilters(int filter1Index, int filter2Index, int filter1Outputport /*= 0*/, int filter2InputPort /*= 0*/)
{
    // #TODO: type checking
    if (filter1Index >= this->filterList.size() || filter2Index >= this->filterList.size())
    {
        QString errmsg("Specify connection failed. Filters doesn't exist!");
        qCritical() << errmsg;
        throw std::logic_error(errmsg.toStdString());
        return;
    }

    FilterConenction fc;
    fc.preFilterIndex = filter1Index;
    fc.postFilterIndex = filter2Index;
    fc.preFilterOutputPort = filter1Outputport;
    fc.postFilterInputPort = filter2InputPort;

    this->filterConnectionMap.append(fc);

    /* Force rebuilding connection */
    this->Qvtk_PIPELINE_CONNECTED_FLAG = false;
}

void QvtkAbstractPipeline::UpdatePipeline()
{
    try {
        this->BuildConenction();
    }
    catch (std::logic_error &e) {
        qCritical() << e.what();
    }
    catch (...)
    {
        qCritical() << "Unknown error! Build connection failed!";
    }

    if (!Qvtk_PIPELINE_CONNECTED_FLAG)
    {
        QString errmsg("Filters I/O are not connected, call BuildConnection() first!");
        qCritical() << errmsg;
        throw std::logic_error(errmsg.toStdString());
        return;
    }

    for (QList<QvtkAbstractFilter*>::const_iterator iter = this->filterList.cbegin();
    		 iter != this->filterList.cend(); iter++)
    {
        (*iter)->Update();
    }

    emit signalPipelineUpdate();
}


void QvtkAbstractPipeline::PrintConnection()
{
    for (QList<FilterConenction>::iterator iter = this->filterConnectionMap.begin();
    iter != this->filterConnectionMap.end(); iter++)
    {
        // 
        QString msg = QString("[%1]%2-port%3 -> [%4]%5-port%6").arg(
            QString::number(iter->preFilterIndex),
            typeid(*(this->filterList[iter->preFilterIndex])).name(),
            QString::number(iter->preFilterOutputPort),
            QString::number(iter->postFilterIndex),
            typeid(*(this->filterList[iter->postFilterIndex])).name(),
            QString::number(iter->postFilterInputPort));
        qDebug() << msg;
    }
}

QvtkAbstractFilter* QvtkAbstractPipeline::GetComponentFilter(unsigned int index)
{
    return this->filterList[index];
}

int QvtkAbstractPipeline::GetFilterIndex(QvtkAbstractFilter* filter)
{
    int index = this->filterList.indexOf(filter);
    if (index >= 0)
        return index;
    else 
    {
        qWarning() << "Filter is not in the filter list!";
        return -1;
    }
}

void QvtkAbstractPipeline::SetLayout(Qvtk_PIPELINE_LAYOUT e)
{
    if (e == STACKED_WIDGET_LAYOUT)
        this->SetStackWidgetLayout();
    if (e == TOOLBOX_LAYOUT)
        this->SetToolboxLayout();
    if (e == TABWIDGET_LAYOUT)
        this->SetTabWidgetLayout();
    if (e == HBOX_LAYOUT)
        this->SetHBoxLayout();
    if (e == VBOX_LAYOUT)
        this->SetVBoxLayout();
    if (e == GRID_LAYOUT)
        this->SetGridLayout();
    if (e == STACK_LAYOUT)
        this->SetStackLayout();
    if (e == FORM_LAYOUT)
        this->SetFormLayout();

    if (this->filterUIList.size() != 0)
    {
        this->BuildUI();
    }
}

void QvtkAbstractPipeline::SetStackWidgetLayout()
{
    this->ClearLayout();

    QBoxLayout* layout = new QBoxLayout(QBoxLayout::Down, this);
    this->setLayout(layout);

    this->pipelineLayout = STACKED_WIDGET_LAYOUT;
}

void QvtkAbstractPipeline::SetToolboxLayout()
{
    this->ClearLayout();

    QBoxLayout* layout = new QBoxLayout(QBoxLayout::Down, this);
    this->setLayout(layout);

    this->pipelineLayout = TOOLBOX_LAYOUT;
}

void QvtkAbstractPipeline::SetTabWidgetLayout()
{
    this->ClearLayout();

    QBoxLayout* layout = new QBoxLayout(QBoxLayout::Down, this);
    this->setLayout(layout);

    this->pipelineLayout = TABWIDGET_LAYOUT;
}

bool QvtkAbstractPipeline::CheckFilterConnection()
{
    vtkSmartPointer<vtkPoints> pts
        = vtkSmartPointer<vtkPoints>::New();
    for (int i = 0; i < this->filterList.size();i++)
    {
        pts->InsertNextPoint(i, i, i);
    }

    vtkSmartPointer<vtkCellArray> cells
        = vtkSmartPointer<vtkCellArray>::New();
    for (int i = 0; i < this->filterConnectionMap.size(); i++)
    {
        vtkSmartPointer<vtkLine> line
            = vtkSmartPointer<vtkLine>::New();

        line->GetPointIds()->InsertNextId(this->filterConnectionMap[i].preFilterIndex);
        line->GetPointIds()->InsertNextId(this->filterConnectionMap[i].postFilterIndex);

        cells->InsertNextCell(line);
    }

    vtkSmartPointer<vtkPolyData> pd
        = vtkSmartPointer<vtkPolyData>::New();
    pd->SetPoints(pts);
    pd->SetLines(cells);
    pd->BuildCells();

    vtkSmartPointer<vtkPolyDataConnectivityFilter> conn
        = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
    conn->SetInputData(pd);
    conn->ColorRegionsOn();
    conn->SetExtractionModeToAllRegions();
    conn->Update();

    if (conn->GetNumberOfExtractedRegions() != 1)
    {
        qDebug() << "Connectivity check of filters failed: ";
        this->PrintConnection();
        conn->GetOutput()->Print(cout);
        vtkIdTypeArray* regionIDs = vtkIdTypeArray::SafeDownCast(conn->GetOutput()->GetPointData()->GetArray("RegionID"));
        vtkArrayIteratorTemplate<vtkIdType>* iter 
            = vtkArrayIteratorTemplate<vtkIdType>::SafeDownCast(regionIDs->NewIterator());
        int filterId = 0;
        while (iter)
        {
            qDebug() << "Filter" << filterId << ":" << iter->GetValue(0);
            iter++;
            filterId++;
        }
        return 1; //failed
    }
    else
        return 0; //success

}

void QvtkAbstractPipeline::ClearUI()
{
    if (!this->GetLayout())
    {
        qDebug() << "Attempting to clear non-existing layout!";
        return;
    }

    QLayoutItem* item;
    while (item = this->GetLayout()->takeAt(0))
    {
        this->GetLayout()->removeItem(item);
    }
}

void QvtkAbstractPipeline::ClearLayout()
{
    if (this->layout())
    {
        this->ClearUI();
        delete this->layout();
    }
    else
        qWarning() << "Trying to delete non-existing layout!";
}

void QvtkAbstractPipeline::BuildConenction()
{
    if (this->Qvtk_PIPELINE_CONNECTED_FLAG)
        return;

    /* Check if all filters are connected */
    if (this->CheckFilterConnection())
    {
        QString errmsg("Connection is not properly set, not all filters are linked!");
        qCritical() << errmsg;
        throw std::logic_error(errmsg.toStdString());
        return;
    }

    for (QList<FilterConenction>::const_iterator iter = this->filterConnectionMap.cbegin();
    		 iter != this->filterConnectionMap.cend(); iter++)
    {
        /* Check connectin is valid */
        if (iter->postFilterIndex >= this->filterList.size() ||
            iter->preFilterIndex >= this->filterList.size() ||
            iter->postFilterIndex < 0 ||
            iter->preFilterIndex < 0)
        {
            QString errmsg("Trying to make connection with incorrect filter index!");
            qCritical() << errmsg;
            throw std::logic_error(errmsg.toStdString());
            return;
        }
        this->filterList[iter->postFilterIndex]->PreConnectFilter(
            this->filterList[iter->preFilterIndex], iter->preFilterOutputPort, iter->postFilterInputPort);
    }
    this->Qvtk_PIPELINE_CONNECTED_FLAG = true;

    emit signalPipelineConnectionBuilt();
}

void QvtkAbstractPipeline::BuildUI()
{
    if (!this->GetLayout())
    {
        qCritical() << "Layout must be set before calling BuildUI().";
        return;
    }

    if (this->pipelineLayout == VBOX_LAYOUT  ||
        this->pipelineLayout == HBOX_LAYOUT  || 
        this->pipelineLayout == GRID_LAYOUT  ||
        this->pipelineLayout == STACK_LAYOUT || 
        this->pipelineLayout == FORM_LAYOUT)
    {
        for (QList<QWidget*>::const_iterator iter = this->filterUIList.cbegin();
        iter != this->filterUIList.cend(); iter++)
        {
            /* Avoid adding same widget twice */
            if (this->GetLayout()->children().contains((*iter)))
                continue;

			if (*iter)
				this->GetLayout()->addWidget(*iter);
			else
				qWarning() << "Cannot add nullptr as UI of :" << this->objectName();
        }
        this->GetLayout()->addItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));
    }
    else if (this->pipelineLayout == STACKED_WIDGET_LAYOUT)
    {
        QStackedWidget* widget = new QStackedWidget(this);
        for (QList<QWidget*>::const_iterator iter = this->filterUIList.cbegin();
        iter != this->filterUIList.cend(); iter++)
        {
            /* Avoid adding same widget twice */
            if (widget->children().contains((*iter)))
                continue;

			if (*iter)
				widget->addWidget(*iter);
			else
				qWarning() << "Cannot add nullptr as UI of :" << this->objectName();
        }
        this->GetLayout()->addWidget(widget);
    }
    else if (this->pipelineLayout == TOOLBOX_LAYOUT)
    {
        QToolBox* toolbox = new QToolBox(this);
        for (QList<QWidget*>::const_iterator iter = this->filterUIList.cbegin();
        iter != this->filterUIList.cend(); iter++)
        {
            /* Avoid adding same widget twice */
            if (toolbox->children().contains((*iter)))
                continue;

			if (*iter)
				toolbox->addItem(*iter, this->MatchUIToFilterName(*iter));
			else
				qWarning() << "Cannot add nullptr as UI of :" << this->objectName();
        }
        this->GetLayout()->addWidget(toolbox);
    }
    else if (this->pipelineLayout == TABWIDGET_LAYOUT)
    {
        QTabWidget* tabWidget = new QTabWidget(this);
        tabWidget->setTabPosition(QTabWidget::East);
        for (QList<QWidget*>::const_iterator iter = this->filterUIList.cbegin();
        iter != this->filterUIList.cend(); iter++)
        {
            /* Avoid adding same widget twice */
            if (tabWidget->children().contains((*iter)))
                continue;

			if (*iter)
				tabWidget->addTab(*iter, this->MatchUIToFilterName(*iter));
			else
				qWarning() << "Cannot add nullptr as UI of :" << this->objectName();
        }
        this->GetLayout()->addWidget(tabWidget);
    }
    this->GetLayout()->setMargin(1);

    emit signalPipelineUIBuilt();
}



QString QvtkAbstractPipeline::MatchUIToFilterName(QWidget* widget)
{
	for (QList<QvtkAbstractFilter*>::const_iterator iter = this->filterList.cbegin();
		iter != this->filterList.cend(); iter++)
	{
		if ((*iter) == widget)
			return (*iter)->GetFilterName();
	}
}

void QvtkAbstractPipeline::SetHBoxLayout()
{
    this->ClearLayout();

    QHBoxLayout* layout = new QHBoxLayout(this);
    this->setLayout(layout);

    this->pipelineLayout = HBOX_LAYOUT;
}
 
void QvtkAbstractPipeline::SetVBoxLayout()
{
    this->ClearLayout();

    QVBoxLayout* layout = new QVBoxLayout(this);
    this->setLayout(layout);

    this->pipelineLayout = VBOX_LAYOUT;
}

void QvtkAbstractPipeline::SetGridLayout()
{
    this->ClearLayout();

    QGridLayout* layout = new QGridLayout(this);
    this->setLayout(layout);

    this->pipelineLayout = GRID_LAYOUT;
}

void QvtkAbstractPipeline::SetStackLayout()
{
    this->ClearLayout();

    QStackedLayout* layout = new QStackedLayout(this);
    this->setLayout(layout);

    this->pipelineLayout = STACK_LAYOUT;
}

void QvtkAbstractPipeline::SetFormLayout()
{
    this->ClearLayout();

    QFormLayout* layout = new QFormLayout(this);
    this->setLayout(layout);

    this->pipelineLayout = FORM_LAYOUT;
}

QLayout* QvtkAbstractPipeline::GetLayout()
{
    return this->layout();
}

