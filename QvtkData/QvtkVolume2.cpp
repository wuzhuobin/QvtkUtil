// me
#include "QvtkVolume2.h"
#include "QvtkDataSet.h"
void set_volume_property(vtkVolume *&volume, const int & preset, const double & shift, const double & opacity);
// vtk
#include <vtkVolume.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkNew.h>
// qt
#include <QDebug>
// std
#include <sstream>
const struct QvtkVolume2Init{
	QvtkVolume2Init(){
		VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
	}
} Init;
namespace Q {
namespace vtk{
Q_VTK_DATA_CPP(Volume2)
Volume2::Volume2()
{
	this->volumeOpacity = 1;
	this->shift = createAttribute(K.Shift, static_cast<double>(0), true);
	insertSlotFunction(this->shift, &Volume2::setShift);
	// this->preset = createAttribute(K.Preset, static_cast<int>(0), true);
	insertSlotFunction(this->preset, &Volume2::setPreset);
	vtkNew<vtkSmartVolumeMapper> mapper;
	this->smartVolumeMapper = mapper.GetPointer();
	this->smartVolumeMapper->SetRequestedRenderModeToGPU();
	//this->smartVolumeMapper->SetRequestedRenderModeToDefault();
	this->smartVolumeMapper->CroppingOn();
	vtkVolume* volume = vtkVolume::New();
	volume->SetMapper(this->smartVolumeMapper);
	this->setProp(volume);

}

Volume2::~Volume2()
{
	setRenderDataSet(nullptr);
}

void Volume2::printSelf() const
{
	Prop::printSelf();

	std::stringstream ss;


	ss.clear();
	this->smartVolumeMapper->Print(ss);
	qDebug() << "smartVolumeMapper" << '=' << QString::fromStdString(ss.str());

	ss. clear();


}

void Volume2::readXML(const QDomElement& xml, QString directoryPath)
{
	Prop::readXML(xml, directoryPath);
}

void Volume2::writeXML(QDomElement& xml, QString directoryPath) const
{
	Prop::writeXML(xml, directoryPath);
}

void Volume2::setShift(double shift)
{
	setAttribute(this->shift, shift);
	this->setPreset(this->getPreset());
}

double Volume2::getShift() const
{
	return getAttribute(this->shift).toDouble();
}

vtkVolume* Volume2::getVolume() const
{
	return vtkVolume::SafeDownCast(this->getProp());
}

void Volume2::setPreset(int preset)
{
	if (preset > 25) {
		return;
	}
	setAttribute(this->preset, preset);
	vtkVolume * volume = this->getVolume();
	unsigned int shift = this->getShift();
	set_volume_property(volume, preset, shift, this->volumeOpacity);
}

int Volume2::getPreset() const
{
	return getAttribute(this->preset).toInt();
}

void Volume2::setRenderDataSet(DataSet* data)
{
	if (data == this->getRenderDataSet()) {
		return;
	}
	if (this->getRenderDataSet()) {
		this->getVolume()->GetMapper()->SetInputConnection(nullptr);
	}
	Prop::setRenderDataSet(data);
	if (this->getRenderDataSet()) {
		if (!data->isClass("Q::vtk::Image")) {
			qCritical() << "data is not Q::vtk::Image.";
		}
		this->getVolume()->GetMapper()->SetInputConnection(data->getOutputPort());
		// Make shift work;
		this->setShift(this->getShift());
	}
}

void Volume2::setOpacity(double opacity)
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

void Volume2::setDisplayRegion(const double region[6])
{
	Prop::setDisplayRegion(region);
	double _region[6];
	if (this->getRenderDataSet()) {
		/*this->getRenderDataSet()->*/worldRegionToDataSetRegion(region, _region);
		this->smartVolumeMapper->SetCroppingRegionPlanes(_region);
	}

}

Data* Volume2::newInstance() const
{
	return new Volume2;
}

void Volume2::setShift(Data * self, QStandardItem * item)
{
	Volume2* _self = static_cast<Volume2*>(self);
	_self->setShift(getAttribute(item).toDouble());
}

void Volume2::setPreset(Data* self, QStandardItem* item)
{
	Volume2* _self = static_cast<Volume2*>(self);
	_self->setPreset(getAttribute(item).toUInt());
}
}
}
