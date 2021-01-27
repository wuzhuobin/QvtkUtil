// me
#include "QvtkVolume.h"
#include "QvtkDataSet.h"
void set_volume_property(vtkVolume *&volume, const int & preset, const double & shift, const double & opacity);
// vtk
#include <vtkVolume.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkNew.h>
#include <vtkDataSet.h>

// qt
#include <QDebug>
// std
#include <sstream>
const struct QvtkVolumeInit{
	QvtkVolumeInit(){
		VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
	}
} Init;
namespace Q {
namespace vtk{
Q_VTK_DATA_CPP(Volume)
Volume::Volume()
{
	this->volumeOpacity = 1;
	this->shift = createAttribute(K.Shift, static_cast<double>(0), true);
	insertSlotFunction(this->shift, &Volume::setShift);
	this->preset = createAttribute(K.Preset, static_cast<int>(0), true);
	insertSlotFunction(this->preset, &Volume::setPreset);
	vtkNew<vtkSmartVolumeMapper> mapper;
	this->smartVolumeMapper = mapper.GetPointer();
	// this->smartVolumeMapper->SetRequestedRenderModeToGPU();
	this->smartVolumeMapper->SetRequestedRenderModeToDefault();
	this->smartVolumeMapper->CroppingOn();
	vtkVolume* volume = vtkVolume::New();
	volume->SetMapper(this->smartVolumeMapper);
	this->setProp(volume);
	setPreset(CT_AAA);
}

Volume::~Volume()
{
	setRenderDataSet(nullptr);
}

void Volume::printSelf() const
{
	Prop::printSelf();

	std::stringstream ss;
	ss.clear();
	this->smartVolumeMapper->Print(ss);
	qDebug() << "smartVolumeMapper" << '=' << QString::fromStdString(ss.str());
	qDebug() << "shift" << '=' << this->getShift();
	ss. clear();
}

void Volume::readXML(const QDomElement& xml, QString directoryPath)
{
	Prop::readXML(xml, directoryPath);
}

void Volume::writeXML(QDomElement& xml, QString directoryPath) const
{
	Prop::writeXML(xml, directoryPath);
}

void Volume::setShift(double shift)
{
	setAttribute(this->shift, shift);
	this->setPreset(this->getPreset());
}

double Volume::getShift() const
{
	return getAttribute(this->shift).toDouble();
}

vtkVolume* Volume::getVolume() const
{
	return vtkVolume::SafeDownCast(this->getProp());
}

void Volume::setPreset(int preset)
{
	if (preset > 25) {
		return;
	}
	setAttribute(this->preset, preset);
	vtkVolume * volume = this->getVolume();
	unsigned int shift = this->getShift();
	set_volume_property(volume, preset, shift, this->volumeOpacity);
}

int Volume::getPreset() const
{
	return getAttribute(this->preset).toInt();
}

void Volume::setRenderDataSet(DataSet* data)
{
	if (data == this->getRenderDataSet()) {
		return;
	}

	// bug issue: https://gitlab.kitware.com/vtk/vtk/-/issues/17328
	data->getDataSet()->Modified();

	//if (this->getRenderDataSet()) {
	//	this->getVolume()->GetMapper()->SetInputConnection(nullptr);
	//}

	Prop::setRenderDataSet(data);

	if (this->getRenderDataSet()) {
		if (!data->isClass("Q::vtk::Image")) {
			qCritical() << "data is not Q::vtk::Image.";
		}

		this->smartVolumeMapper->SetInputData(data->getDataSet());
		//this->getVolume()->GetMapper()->SetInputData(data->getOutputPort());
		// Make shift work;
		this->setShift(this->getShift());
	}
}

void Volume::setOpacity(double opacity)
{
	if (opacity > 1) {
		opacity = 1;
	}
	if (opacity < 0) {
		opacity = 0;
	}
	this->volumeOpacity = opacity;
	this->setPreset(this->getPreset());
}

void Volume::setDisplayRegion(const double region[6])
{
	Prop::setDisplayRegion(region);
	double _region[6];
	if (this->getRenderDataSet()) {
		/*this->getRenderDataSet()->*/worldRegionToDataSetRegion(region, _region);
		this->smartVolumeMapper->SetCroppingRegionPlanes(_region);
	}

}

Data* Volume::newInstance() const
{
	return new Volume;
}

void Volume::setShift(Data * self, QStandardItem * item)
{
	Volume* _self = static_cast<Volume*>(self);
	_self->setShift(getAttribute(item).toDouble());
}

void Volume::setPreset(Data* self, QStandardItem* item)
{
	Volume* _self = static_cast<Volume*>(self);
	_self->setPreset(getAttribute(item).toUInt());
}
}
}
