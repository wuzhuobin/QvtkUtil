// me
#include "QvtkDiscreteMarchingCubes.h"
#include "vtkDiscreteMarchingCubesWithSmooth.h"
#include "ui_QvtkDiscreteMarchingCubes.h"

// qt 
#include <QWidget>

// vtk 
//#include <vtkWindowedSincPolyDataFilter.h>

QvtkDiscreteMarchingCubes::QvtkDiscreteMarchingCubes(QObject * parent)
{
	Qvtk_FILTER_CONSTRUCT_MACRO(vtkDiscreteMarchingCubesWithSmooth);
}

QvtkDiscreteMarchingCubes::~QvtkDiscreteMarchingCubes()
{
	this->vtkfilter->Delete();
	delete this->ui;
}

void QvtkDiscreteMarchingCubes::LoadUI()
{
		/* Setup UI */
		ui = new Ui::QvtkDiscreteMarchingCubes;
		ui->setupUi(this);
}

int QvtkDiscreteMarchingCubes::ConnectUI()
{
	QObject::connect(this->ui->spinBoxStart, SIGNAL(valueChanged(int)),
		this, SLOT(slotGenerateValues()));
	QObject::connect(this->ui->spinBoxEnd, SIGNAL(valueChanged(int)),
		this, SLOT(slotGenerateValues()));
	QObject::connect(this->ui->spinBoxSmoothingIterations, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		this, &QvtkDiscreteMarchingCubes::slotSetNumberOfIterations);
	return 0;
}

void QvtkDiscreteMarchingCubes::slotSetNumberOfIterations(int iteractions)
{
	GetDiscreteMarchingCubesWithSmooth()->SetNumberOfIterations(iteractions);
}
void QvtkDiscreteMarchingCubes::slotGenerateValues()
{
	int start = this->ui->spinBoxStart->value();
	int end = this->ui->spinBoxEnd->value();
	int range = end - start + 1;
	GetDiscreteMarchingCubesWithSmooth()->GenerateValues(range, start, end);

}

vtkDiscreteMarchingCubesWithSmooth * QvtkDiscreteMarchingCubes::GetDiscreteMarchingCubesWithSmooth()
{
	return static_cast<vtkDiscreteMarchingCubesWithSmooth*>(this->vtkfilter);
}
