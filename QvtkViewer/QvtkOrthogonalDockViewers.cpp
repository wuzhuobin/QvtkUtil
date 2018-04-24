// me
#include "QvtkOrthogonalDockViewers.h"
#include "QvtkOrthogonalViewer.h"
#include "ui_QvtkOrthogonalViewer.h"

// qt
#include <QDockWidget>
#include <QSignalMapper>
#include <QDebug>
namespace Q {
namespace vtk{
OrthogonalDockViewers::OrthogonalDockViewers(QWidget * parent)
	:QMainWindow(parent)
{
	this->viewers = new QList<OrthogonalViewer*>;
	this->dockers = new QList<QDockWidget*>;
	this->mapper = new QSignalMapper(this);

}

OrthogonalDockViewers::~OrthogonalDockViewers()
{
	delete this->viewers;
	delete this->dockers;
}

void OrthogonalDockViewers::deleteAllViewer()
{
	for (int i = 0; i < getNumberOfViewers(); ++i) {
		delete this->viewers->at(i);
		delete this->dockers->at(i);
	}
}

void OrthogonalDockViewers::setViewer(int i)
{
	setViewer(i, new OrthogonalViewer(this));
}

void OrthogonalDockViewers::setViewer(int index, OrthogonalViewer * viewer)
{
	if (index >getNumberOfViewers()) {
		setViewer(index - 1, new OrthogonalViewer(this));
	}

	if (index < getNumberOfViewers()) {
		delete this->viewers->takeAt(index);
		this->viewers->insert(index, viewer);
	}
	else if (index == getNumberOfViewers()) {
		this->viewers->push_back(viewer);
		this->dockers->push_back(new QDockWidget(this));
		this->dockers->at(index)->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
		this->dockers->at(index)->setAllowedAreas(Qt::AllDockWidgetAreas);

		if (index % 2 == 0) {
			this->addDockWidget(Qt::BottomDockWidgetArea, this->dockers->at(index));
		}
		else {
			this->addDockWidget(Qt::TopDockWidgetArea, this->dockers->at(index));
		}
	}

	this->dockers->at(index)->setWidget(this->viewers->at(index));
	this->mapper->setMapping(this->viewers->at(index)->GetUi()->pushBtnMaximize, index * 2 + 1);
	this->mapper->setMapping(this->viewers->at(index)->GetUi()->pushBtnNormal, index * 2);
	// connection
	connect(this->viewers->at(index)->GetUi()->pushBtnMaximize, SIGNAL(clicked()),
		this->mapper, SLOT(map()));
	connect(this->viewers->at(index)->GetUi()->pushBtnNormal, SIGNAL(clicked()),
		this->mapper, SLOT(map()));
	connect(this->mapper, SIGNAL(mapped(int)),
		this, SLOT(showMaximizedOrNormal(int)));
}

OrthogonalViewer * OrthogonalDockViewers::getViewers(int i) const
{
	return this->viewers->at(i);
}

QDockWidget * OrthogonalDockViewers::getDocker(int i) const
{
	return this->dockers->at(i);
}

int OrthogonalDockViewers::getNumberOfViewers() const
{
	return this->viewers->count();
}

void OrthogonalDockViewers::showMaximizedOrNormal(int i) const
{
	int num = i / 2;
	bool maximize = i % 2;

	if (this->dockers->at(num)->isFloating()) {
		if (maximize) {
			qWarning() << "showMaximized() function is not working in linux.";
			this->dockers->at(num)->showMaximized();
		}
		else {
			qWarning() << "showNormal() function is not working in linux.";
			this->dockers->at(num)->showNormal();
		}
	}
	else {
		for (QList<QDockWidget*>::iterator it = this->dockers->begin();
			it != this->dockers->end(); ++it) {
			(*it)->setHidden(maximize);
		}
		if (maximize) {
			this->dockers->at(num)->setHidden(!maximize);
		}
	}
}

}
}