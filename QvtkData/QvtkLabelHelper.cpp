// me 
#include "QvtkLabelHelper.h"
// vtk
#include <vtkLookupTable.h>
#include <vtkDiscretizableColorTransferFunction.h>
#include <vtkNamedColors.h>
#include <vtkSmartPointer.h>
#include <vtkPiecewiseFunction.h>
// qt
#include <QDebug>
#include <QTextStream>
#include <QFile>
using namespace Q::vtk;
const QStringList LabelHelper::DEFAULT_COLOR_FILES{
	":/ColorFiles/64Color-Nonsemantic.txt",
	":/ColorFiles/AbdomenColors.txt",
	":/ColorFiles/ColdToHotRainbow.txt",
	":/ColorFiles/DarkBrightChartColors.txt",
	":/ColorFiles/GenericAnatomyColors.txt",
	":/ColorFiles/GenericColors.txt",
	":/ColorFiles/HotToColdRainbow.txt",
	":/ColorFiles/Inferno.txt",
	":/ColorFiles/LightPaleChartColors.txt",
	":/ColorFiles/Magma.txt",
	":/ColorFiles/MediumChartColors.txt",
	":/ColorFiles/PelvisColor.txt",
	":/ColorFiles/Plasma.txt",
	":/ColorFiles/Slicer3_2010_Brain_Labels.txt",
	":/ColorFiles/Slicer3_2010_Label_Colors.txt",
	":/ColorFiles/SPL-BrainAtlas-2009-ColorFile.txt",
	":/ColorFiles/SPL-BrainAtlas-2012-ColorFile.txt",
	":/ColorFiles/SPL-BrainAtlas-ColorFile.txt",
	":/ColorFiles/Viridis.txt",
	":/ColorFiles/ITK-SNAP-label.txt"
};
const struct LabelHelperInit {
	LabelHelperInit() {
		Q_INIT_RESOURCE(QvtkLabelHelper);
	}
}LABEL_HELPER_INIT;
LabelHelper::LabelHelper()
{
	this->lookupTable = vtkLookupTable::New();
	this->namedColors = vtkNamedColors::New();
	vtkSmartPointer<vtkPiecewiseFunction> function = vtkSmartPointer<vtkPiecewiseFunction>::New();
	this->transferFunction = vtkDiscretizableColorTransferFunction::New();
	this->transferFunction->RemoveAllPoints();
	this->transferFunction->SetEnableOpacityMapping(true);
	this->transferFunction->SetScalarOpacityFunction(function);
	this->namedColorsToLookupTable();
	this->namedColosrToTransferFunction();
}

LabelHelper::~LabelHelper()
{
	this->lookupTable->Delete();
	this->namedColors->Delete();
	this->transferFunction->Delete();
}

bool Q::vtk::LabelHelper::readLabelFile(QString path)
{
	QFile labelFile(path);
	if (!labelFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qCritical() << "Cannot read label file: " << path <<
			"." << "Failed to use correct label file. ";
		return false;
	}
	QTextStream labelStream(&labelFile);
	QString labelLine = labelStream.readLine();
	//int min = VTK_INT_MAX;
	//int max = VTK_INT_MIN;
	int lineIndex = 0;
	this->labelIdToLabelName.clear();
	for (; !labelLine.isEmpty(); ++lineIndex, labelLine = labelStream.readLine()) {
		if (!labelLine.startsWith('#')) {
			QStringList values = labelLine.simplified().split(' ');
			int labelId;
			QString labelName;
			unsigned char rgba[4];
			try {
				bool ok = false;
				ok = labelLine.size() >= 6;
				if (!ok) {
					throw std::out_of_range("Values in this line is less than 6.");
				}
				labelId = values[0].toInt(&ok);
				if (!ok) {
					throw QString("Not correct label id. ");
				}
				labelName = values[1];
				rgba[0] = values[2].toUShort(&ok);
				if (!ok) {
					throw QString("Not correct red value. ");
				}
				rgba[1] = values[3].toUShort(&ok);
				if (!ok) {
					throw QString("Not correct green value. ");
				}
				rgba[2] = values[4].toUShort(&ok);
				if (!ok) {
					throw QString("Not correct blue value. ");
				}
				rgba[3] = values[5].toUShort(&ok);
				if (!ok) {
					throw QString("Not correct alpha value. ");
				}
				this->labelIdToLabelName.insert(labelId, labelName);
				this->namedColors->SetColor(labelName.toStdString(), rgba);
			}
			catch (QString error) {
				qWarning() << error << 
					"Fail to read this line number:" << lineIndex + 1 << ".";
			}
			catch (std::out_of_range &e) {
				qWarning() << e.what() << 
					"This line does not contain 6 values. " << 
					"Fail to read this line number:" << lineIndex + 1 << ".";
			}
		}
	}

	labelFile.close();
	if (lineIndex == 0)
	{
		qCritical() << "Empty label file. ";
		return false;
	}
	else {
		this->namedColorsToLookupTable();
		this->namedColosrToTransferFunction();
		//this->lookupTable->SetTableRange(min, max);
		//this->lookupTable->Build();
		//this->transferFunction->Build();
		return true;
	}
}

void LabelHelper::setColor(int id, double r, double g, double b, double a)
{
	if (!this->labelIdToLabelName.contains(id)) {
		qCritical() << "Label Id:" << id << " does not exist. ";
		return;
	}
	this->namedColors->SetColor(this->labelIdToLabelName[id].toStdString(), r, g, b, a);
	//this->namedColorsToLookupTable();
	//this->namedColosrToTransferFunction();
	this->lookupTable->SetTableValue(id, r, g, b, a);
	this->lookupTable->Build();
	this->transferFunction->RemovePoint(id);
	this->transferFunction->GetScalarOpacityFunction()->RemovePoint(id);
	this->transferFunction->AddRGBPoint(id, r, g, b);
	this->transferFunction->GetScalarOpacityFunction()->AddPoint(id, a);
	this->transferFunction->Build();
}

void LabelHelper::getColor(QString labelName, double rgba[4]) const
{
	this->namedColors->GetColor(labelName.toStdString(), rgba);
}

void LabelHelper::getColor(int id, double rgba[4]) const
{
	//this->getLookupTable()->GetTableValue(id, rgba);
	if (!this->labelIdToLabelName.contains(id)) {
		qCritical() << "Label Id:" << id << " does not exist. ";
		return;
	}
	this->getColor(this->labelIdToLabelName[id], rgba);
}

void LabelHelper::namedColorsToLookupTable()
{
	this->lookupTable->SetNumberOfTableValues(1);
	this->lookupTable->SetTableRange(0, 0);
	this->lookupTable->SetTableValue(0, 0, 0, 0, 0);
	this->lookupTable->Build();
	QList<int> keys = this->labelIdToLabelName.keys();
	std::pair<QList<int>::const_iterator, QList<int>::const_iterator> min_max = std::minmax_element(keys.cbegin(), keys.cend());
	this->lookupTable->SetTableRange(*min_max.first, *min_max.second);
	this->lookupTable->SetNumberOfTableValues(*min_max.second + 1);
	this->lookupTable->Build();
	for (QList<int>::const_iterator cit = keys.cbegin(); cit != keys.cend(); ++cit) {
		double rgba[4];
		this->namedColors->GetColor(this->labelIdToLabelName[*cit].toStdString(), rgba);
		this->lookupTable->SetTableValue(*cit, rgba);
	}
	this->lookupTable->Build();
}

void LabelHelper::namedColosrToTransferFunction()
{
	this->transferFunction->GetScalarOpacityFunction()->RemoveAllPoints();
	this->transferFunction->RemoveAllPoints();
	QList<int> keys = this->labelIdToLabelName.keys();
	//int min = VTK_INT_MAX;
	//int max = VTK_INT_MIN;
	for (QList<int>::const_iterator cit = keys.cbegin(); cit != keys.cend(); ++cit) {
		//min = std::min(min, *cit);
		//max = std::max(max, *cit);
		double rgba[4];
		this->namedColors->GetColor(this->labelIdToLabelName[*cit].toStdString(), rgba);
		this->transferFunction->AddRGBPoint(*cit, rgba[0], rgba[1], rgba[2]);
		this->transferFunction->GetScalarOpacityFunction()->AddPoint(*cit, rgba[3]);
	}
}
