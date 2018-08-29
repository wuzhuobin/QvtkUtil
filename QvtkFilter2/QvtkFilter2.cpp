// me 
#include "QvtkFilter2.h"
#include <QtConcurrentRun>
#include <QFutureWatcher>
Q::vtk::Filter2::Filter2(QWidget * parent):
	QWidget(parent)
{
}

Q::vtk::Filter2::~Filter2()
{
}

void Q::vtk::Filter2::run()
{
	QFutureWatcher<void> *watcher = new QFutureWatcher<void>;
	connect(watcher, &QFutureWatcher<void>::finished, this, &Filter2::finish);
	watcher->setFuture(QtConcurrent::run(this, &Filter2::Update));
}

void Q::vtk::Filter2::finish()
{
	emit this->finished();
	delete sender();
}
