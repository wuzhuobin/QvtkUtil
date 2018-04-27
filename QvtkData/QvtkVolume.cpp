#include "QvtkVolume.h"

// me
#include "QvtkDataSet.h"

// vtk
#include <vtkVolume.h>
#include <vtkImageShiftScale.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkNew.h>
#include <vtkVolumeProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkImageResample.h>

// qt
#include <QDebug>

// std
#include <sstream>
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


#if VTK_MAJOR_VERSION > 6
	VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
#else 
	VTK_MODULE_INIT(vtkRenderingVolumeOpenGL);
#endif
	vtkNew<vtkSmartVolumeMapper> mapper;
	this->smartVolumeMapper = mapper.GetPointer();
	this->smartVolumeMapper->SetRequestedRenderModeToGPU();
	//this->smartVolumeMapper->SetRequestedRenderModeToDefault();
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
}

double Volume::getShift() const
{
	return getAttribute(this->shift).toDouble();
}

vtkVolume* Volume::getVolume() const
{
	return vtkVolume::SafeDownCast(this->getProp());
}

void Volume::setPreset(unsigned int preset)
{
	if (preset > 25) {
		return;
	}
	setAttribute(this->preset, preset);

	ENUM_PRESET _preset = static_cast<ENUM_PRESET>(preset);

	switch (_preset)
	{
	case NONE:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.1);
		this->getVolume()->GetProperty()->SetDiffuse(0.9);
		this->getVolume()->GetProperty()->SetSpecular(0.2);
		this->getVolume()->GetProperty()->SetSpecularPower(10.0);
		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(0, 1.0 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		break;

	case CT_AAA:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.1);
		this->getVolume()->GetProperty()->SetDiffuse(0.9);
		this->getVolume()->GetProperty()->SetSpecular(0.2);
		this->getVolume()->GetProperty()->SetSpecularPower(10.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-3024.0 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(143.556 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(166.222 + this->getShift(), 0.686275 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(214.389 + this->getShift(), 0.696078 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(419.736 + this->getShift(), 0.833333 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(3071.00 + this->getShift(), 0.803922 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-3024.0 + this->getShift(), 0.000000, 0.000000, 0.000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(143.556 + this->getShift(), 0.615686, 0.356863, 0.184314);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(166.222 + this->getShift(), 0.882353, 0.603922, 0.290196);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(214.389 + this->getShift(), 1.000000, 1.000000, 1.000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(419.736 + this->getShift(), 1.000000, 0.937033, 0.954531);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3071.00 + this->getShift(), 0.827451, 0.658824, 1.000000);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(255 + this->getShift(), 1.00);
		break;
	case CT_AAA2:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.1);
		this->getVolume()->GetProperty()->SetDiffuse(0.9);
		this->getVolume()->GetProperty()->SetSpecular(0.2);
		this->getVolume()->GetProperty()->SetSpecularPower(10.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-3024.0 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(129.542 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(145.244 + this->getShift(), 0.166667 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(157.020 + this->getShift(), 0.500000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(169.918 + this->getShift(), 0.627451 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(395.575 + this->getShift(), 0.812500 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(1578.73 + this->getShift(), 0.812500 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(3071.00 + this->getShift(), 0.812500 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-3024.0 + this->getShift(), 0.000000, 0.000000, 0.000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(129.542 + this->getShift(), 0.549020, 0.250980, 0.149020);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(145.244 + this->getShift(), 0.600000, 0.627451, 0.843137);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(157.020 + this->getShift(), 0.890196, 0.474510, 0.600000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(169.918 + this->getShift(), 0.992157, 0.870588, 0.392157);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(395.575 + this->getShift(), 1.000000, 0.886275, 0.658824);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(1578.73 + this->getShift(), 1.000000, 0.829256, 0.957922);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3071.00 + this->getShift(), 0.827451, 0.658824, 1.000000);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(255 + this->getShift(), 1.00);
		break;
	case CT_BONE:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.1);
		this->getVolume()->GetProperty()->SetDiffuse(0.9);
		this->getVolume()->GetProperty()->SetSpecular(0.2);
		this->getVolume()->GetProperty()->SetSpecularPower(10.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-3024.00 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-16.4458 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(641.3850 + this->getShift(), 0.715686 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(3071.000 + this->getShift(), 0.705882 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-3024.00 + this->getShift(), 0.000000, 0.000000, 0.000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-16.4458 + this->getShift(), 0.729412, 0.254902, 0.301961);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(641.3850 + this->getShift(), 0.905882, 0.815686, 0.552941);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3071.000 + this->getShift(), 1.000000, 1.000000, 1.000000);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(255 + this->getShift(), 1.00);
		break;
	case CT_BONES:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.2);
		this->getVolume()->GetProperty()->SetDiffuse(1.0);
		this->getVolume()->GetProperty()->SetSpecular(0.0);
		this->getVolume()->GetProperty()->SetSpecularPower(1.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-1000.0 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(152.190 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(278.930 + this->getShift(), 0.190476 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(952.000 + this->getShift(), 0.200000 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-1000.0 + this->getShift(), 0.3, 0.300000, 1.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-488.00 + this->getShift(), 0.3, 1.000000, 0.3000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(463.280 + this->getShift(), 1.0, 0.000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(659.150 + this->getShift(), 1.0, 0.912535, 0.0374849);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(953.000 + this->getShift(), 1.0, 0.300000, 0.3000000);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0000 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(985.12 + this->getShift(), 1.00);
		break;
	case CT_CARDIAC:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.1);
		this->getVolume()->GetProperty()->SetDiffuse(0.9);
		this->getVolume()->GetProperty()->SetSpecular(0.2);
		this->getVolume()->GetProperty()->SetSpecularPower(10.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-3024.00 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-77.6875 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(94.95180 + this->getShift(), 0.285714 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(179.0520 + this->getShift(), 0.553571 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(260.4390 + this->getShift(), 0.848214 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(3071.000 + this->getShift(), 0.875000 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-3024.00 + this->getShift(), 0.000000, 0.000000, 0.000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-77.6875 + this->getShift(), 0.549020, 0.250980, 0.149020);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(94.95180 + this->getShift(), 0.882353, 0.603922, 0.290196);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(179.0520 + this->getShift(), 1.000000, 0.937033, 0.954531);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(260.4390 + this->getShift(), 0.615686, 0.000000, 0.000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3071.000 + this->getShift(), 0.827451, 0.658824, 1.000000);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0, 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(255, 1.00);
		break;
	case CT_CARDIAC2:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.1);
		this->getVolume()->GetProperty()->SetDiffuse(0.9);
		this->getVolume()->GetProperty()->SetSpecular(0.2);
		this->getVolume()->GetProperty()->SetSpecularPower(10.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-3024.00 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(42.89640 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(163.4880 + this->getShift(), 0.428571 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(277.6420 + this->getShift(), 0.776786 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(1587.000 + this->getShift(), 0.754902 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(3071.000 + this->getShift(), 0.754902 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-3024.00 + this->getShift(), 0.000000, 0.000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(42.89640 + this->getShift(), 0.549020, 0.250980, 0.1490200);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(163.4880 + this->getShift(), 0.917647, 0.639216, 0.0588235);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(277.6420 + this->getShift(), 1.000000, 0.878431, 0.6235290);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(1587.000 + this->getShift(), 1.000000, 1.000000, 1.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3071.000 + this->getShift(), 0.827451, 0.658824, 1.0000000);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(255 + this->getShift(), 1.00);
		break;
	case CT_CARDIAC3:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.1);
		this->getVolume()->GetProperty()->SetDiffuse(0.9);
		this->getVolume()->GetProperty()->SetSpecular(0.2);
		this->getVolume()->GetProperty()->SetSpecularPower(10.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-3024.00 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-86.9767 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(45.37910 + this->getShift(), 0.169643 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(139.9190 + this->getShift(), 0.589286 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(347.9070 + this->getShift(), 0.607143 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(1224.160 + this->getShift(), 0.607143 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(3071.000 + this->getShift(), 0.616071 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-3024.00 + this->getShift(), 0.000000, 0.000000, 0.000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-86.9767 + this->getShift(), 0.000000, 0.250980, 1.000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(45.37910 + this->getShift(), 1.000000, 0.000000, 0.000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(139.9190 + this->getShift(), 1.000000, 0.894893, 0.894893);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(347.9070 + this->getShift(), 1.000000, 1.000000, 0.250980);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(1224.160 + this->getShift(), 1.000000, 1.000000, 1.000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3071.000 + this->getShift(), 0.827451, 0.658824, 1.000000);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(255 + this->getShift(), 1.00);
		break;
	case CT_CHEST_CONTRAST_ENHANCED:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.1);
		this->getVolume()->GetProperty()->SetDiffuse(0.9);
		this->getVolume()->GetProperty()->SetSpecular(0.2);
		this->getVolume()->GetProperty()->SetSpecularPower(10.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-3024.00 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(67.01060 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(251.1050 + this->getShift(), 0.446429 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(439.2910 + this->getShift(), 0.625000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(3071.000 + this->getShift(), 0.616071 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-3024.00 + this->getShift(), 0.000000, 0.000000, 0.000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(67.01060 + this->getShift(), 0.549020, 0.250980, 0.149020);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(251.1050 + this->getShift(), 0.882353, 0.603922, 0.290196);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(439.2910 + this->getShift(), 1.000000, 0.937033, 0.954531);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3071.000 + this->getShift(), 0.827451, 0.658824, 1.000000);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(255 + this->getShift(), 1.00);
		break;
	case CT_CHEST_VESSELS:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.2);
		this->getVolume()->GetProperty()->SetDiffuse(1.0);
		this->getVolume()->GetProperty()->SetSpecular(0.0);
		this->getVolume()->GetProperty()->SetSpecularPower(1.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-3024.00 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-1278.35 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(22.82770 + this->getShift(), 0.428571 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(439.2910 + this->getShift(), 0.625000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(3071.000 + this->getShift(), 0.616071 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-3024.00 + this->getShift(), 0.000000, 0.000000, 0.000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-1278.35 + this->getShift(), 0.549020, 0.250980, 0.149020);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(22.82770 + this->getShift(), 0.882353, 0.603922, 0.290196);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(439.2910 + this->getShift(), 1.000000, 0.937033, 0.954531);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3071.000 + this->getShift(), 0.827451, 0.658824, 1.000000);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(255 + this->getShift(), 1.00);
		break;
	case CT_CORONARY_ARTERIES:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(0);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.2);
		this->getVolume()->GetProperty()->SetDiffuse(1.0);
		this->getVolume()->GetProperty()->SetSpecular(0.0);
		this->getVolume()->GetProperty()->SetSpecularPower(1.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-2048.0 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(36.4700 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(159.215 + this->getShift(), 0.258929 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(318.430 + this->getShift(), 0.571429 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(478.693 + this->getShift(), 0.776786 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(3661.00 + this->getShift(), 1.000000 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-2048.0 + this->getShift(), 0.000000, 0.000000, 0.000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(136.470 + this->getShift(), 0.000000, 0.000000, 0.000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(159.215 + this->getShift(), 0.159804, 0.159804, 0.159804);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(318.430 + this->getShift(), 0.764706, 0.764706, 0.764706);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(478.693 + this->getShift(), 1.000000, 1.000000, 1.000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3661.00 + this->getShift(), 1.000000, 1.000000, 1.000000);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(255 + this->getShift(), 1.00);
		break;
	case CT_CORONARY_ARTERIES2:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.2);
		this->getVolume()->GetProperty()->SetDiffuse(1.0);
		this->getVolume()->GetProperty()->SetSpecular(0.0);
		this->getVolume()->GetProperty()->SetSpecularPower(1.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-2048.0 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(142.677 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(145.016 + this->getShift(), 0.116071 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(192.174 + this->getShift(), 0.562500 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(384.347 + this->getShift(), 0.830357 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(3661.00 + this->getShift(), 0.830357 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-2048.0 + this->getShift(), 0.000000, 0.000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(142.677 + this->getShift(), 0.000000, 0.000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(145.016 + this->getShift(), 0.615686, 0.000000, 0.0156863);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(192.174 + this->getShift(), 0.909804, 0.454902, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(217.240 + this->getShift(), 0.972549, 0.807843, 0.6117650);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(384.347 + this->getShift(), 0.909804, 0.909804, 1.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3661.00 + this->getShift(), 1.000000, 1.000000, 1.000000);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(255 + this->getShift(), 1.00);
		break;
	case CT_CORONARY_ARTERIES3:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.1);
		this->getVolume()->GetProperty()->SetDiffuse(0.9);
		this->getVolume()->GetProperty()->SetSpecular(0.2);
		this->getVolume()->GetProperty()->SetSpecularPower(10.0);


		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-2048.0 + this->getShift(), 0.0000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(128.643 + this->getShift(), 0.0000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(129.982 + this->getShift(), 0.0982143 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(173.636 + this->getShift(), 0.6696430 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(255.884 + this->getShift(), 0.8571430 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(584.878 + this->getShift(), 0.8660710 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(3661.00 + this->getShift(), 1.0000000 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-2048.0 + this->getShift(), 0.000000, 0.000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(128.643 + this->getShift(), 0.000000, 0.000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(129.982 + this->getShift(), 0.615686, 0.000000, 0.0156863);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(173.636 + this->getShift(), 0.909804, 0.454902, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(255.884 + this->getShift(), 0.886275, 0.886275, 0.8862750);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(584.878 + this->getShift(), 0.968627, 0.968627, 0.9686270);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3661.00 + this->getShift(), 1.000000, 1.000000, 1.000000);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(255 + this->getShift(), 1.00);
		break;
	case CT_CROPPED_VOLUME_BONE:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(0);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.2);
		this->getVolume()->GetProperty()->SetDiffuse(1.0);
		this->getVolume()->GetProperty()->SetSpecular(0.0);
		this->getVolume()->GetProperty()->SetSpecularPower(1.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-2048.0 + this->getShift(), 0.0000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-451.00 + this->getShift(), 0.0000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-450.00 + this->getShift(), 1.0000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(1050.00 + this->getShift(), 1.0000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(3661.00 + this->getShift(), 1.0000000 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-2048.0 + this->getShift(), 0.0000000, 0.0000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-451.00 + this->getShift(), 0.0000000, 0.0000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-450.00 + this->getShift(), 0.0556356, 0.0556356, 0.0556356);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(1050.00 + this->getShift(), 1.0000000, 1.0000000, 1.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3661.00 + this->getShift(), 1.0000000, 1.0000000, 1.0000000);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(255 + this->getShift(), 1.00);
		break;
	case CT_FAT:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(0);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.2);
		this->getVolume()->GetProperty()->SetDiffuse(1.0);
		this->getVolume()->GetProperty()->SetSpecular(0.0);
		this->getVolume()->GetProperty()->SetSpecularPower(1.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-1000.0 + this->getShift(), 0.00 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-100.00 + this->getShift(), 0.00 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-99.000 + this->getShift(), 0.15 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-60.000 + this->getShift(), 0.15 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-59.000 + this->getShift(), 0.00 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(101.200 + this->getShift(), 0.00 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(952.000 + this->getShift(), 0.00 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-1000.0 + this->getShift(), 0.300000, 0.300000, 1.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-497.50 + this->getShift(), 0.300000, 1.000000, 0.3000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-99.000 + this->getShift(), 0.000000, 0.000000, 1.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-76.946 + this->getShift(), 0.000000, 1.000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-65.481 + this->getShift(), 0.835431, 0.888889, 0.0165387);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(83.8900 + this->getShift(), 1.000000, 0.000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(463.280 + this->getShift(), 1.000000, 0.000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(659.150 + this->getShift(), 1.000000, 0.912535, 0.0374849);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0000, 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(985.12, 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(988.00, 1.00);
		break;
	case CT_LIVER_VASCULATURE:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(0);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.2);
		this->getVolume()->GetProperty()->SetDiffuse(1.0);
		this->getVolume()->GetProperty()->SetSpecular(0.0);
		this->getVolume()->GetProperty()->SetSpecularPower(1.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-2048.0 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(149.113 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(157.884 + this->getShift(), 0.482143 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(339.960 + this->getShift(), 0.660714 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(388.526 + this->getShift(), 0.830357 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(1197.95 + this->getShift(), 0.839286 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(3661.00 + this->getShift(), 0.848214 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-2048.0 + this->getShift(), 0.0000000, 0.0000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(149.113 + this->getShift(), 0.0000000, 0.0000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(157.884 + this->getShift(), 0.5019610, 0.2509800, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(339.960 + this->getShift(), 0.6953860, 0.5960300, 0.3688600);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(388.526 + this->getShift(), 0.8549020, 0.8509800, 0.8274510);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(1197.95 + this->getShift(), 1.0000000, 1.0000000, 1.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3661.00 + this->getShift(), 1.0000000, 1.0000000, 1.0000000);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(255 + this->getShift(), 1.00);
		break;
	case CT_LUNG:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.2);
		this->getVolume()->GetProperty()->SetDiffuse(1.0);
		this->getVolume()->GetProperty()->SetSpecular(0.0);
		this->getVolume()->GetProperty()->SetSpecularPower(1.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-1000.0 + this->getShift(), 0.00 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-600.00 + this->getShift(), 0.00 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-599.00 + this->getShift(), 0.15 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-400.00 + this->getShift(), 0.15 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-399.00 + this->getShift(), 0.00 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(2952.00 + this->getShift(), 0.00 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-1000.0 + this->getShift(), 0.300000, 0.300000, 1.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-600.00 + this->getShift(), 0.000000, 0.000000, 1.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-530.00 + this->getShift(), 0.134704, 0.781726, 0.0724558);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-460.00 + this->getShift(), 0.929244, 1.000000, 0.1094730);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-400.00 + this->getShift(), 0.888889, 0.254949, 0.0240258);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(2952.00 + this->getShift(), 1.000000, 0.300000, 0.3000000);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0000 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(985.12 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(988.00 + this->getShift(), 1.00);
		break;
	case CT_MIP:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.1);
		this->getVolume()->GetProperty()->SetDiffuse(0.9);
		this->getVolume()->GetProperty()->SetSpecular(0.2);
		this->getVolume()->GetProperty()->SetSpecularPower(10.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-3024.0 + this->getShift(), 0 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-637.62 + this->getShift(), 0 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(700.000 + this->getShift(), 1 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(3071.00 + this->getShift(), 1 * this->volumeOpacity);


		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-3024.0 + this->getShift(), 0, 0, 0);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-637.62 + this->getShift(), 1, 1, 1);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(700.000 + this->getShift(), 1, 1, 1);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3071.00 + this->getShift(), 1, 1, 1);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(255 + this->getShift(), 1.00);
		break;
	case CT_MUSCLE:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.1);
		this->getVolume()->GetProperty()->SetDiffuse(0.9);
		this->getVolume()->GetProperty()->SetSpecular(0.2);
		this->getVolume()->GetProperty()->SetSpecularPower(10.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-3024.00 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-155.407 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(217.6410 + this->getShift(), 0.676471 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(419.7360 + this->getShift(), 0.833333 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(3071.000 + this->getShift(), 0.803922 * this->volumeOpacity);


		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-3024.00 + this->getShift(), 0.000000, 0.000000, 0.000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-155.407 + this->getShift(), 0.549020, 0.250980, 0.149020);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(217.6410 + this->getShift(), 0.882353, 0.603922, 0.290196);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(419.7360 + this->getShift(), 1.000000, 0.937033, 0.954531);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3071.000 + this->getShift(), 0.827451, 0.658824, 1.000000);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0, 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(255, 1.00);
		break;
	case CT_PULMONARY_ARTERIES:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.2);
		this->getVolume()->GetProperty()->SetDiffuse(1.0);
		this->getVolume()->GetProperty()->SetSpecular(0.0);
		this->getVolume()->GetProperty()->SetSpecularPower(1.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-2048.00 + this->getShift(), 0.0000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-568.625 + this->getShift(), 0.0000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-364.081 + this->getShift(), 0.0714286 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-244.813 + this->getShift(), 0.4017860 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(18.27750 + this->getShift(), 0.6071430 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(447.7980 + this->getShift(), 0.8303570 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(3592.730 + this->getShift(), 0.8392860 * this->volumeOpacity);


		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-2048.00 + this->getShift(), 0.000000, 0.0000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-568.625 + this->getShift(), 0.000000, 0.0000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-364.081 + this->getShift(), 0.396078, 0.3019610, 0.1803920);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-244.813 + this->getShift(), 0.611765, 0.3529410, 0.0705882);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(18.27750 + this->getShift(), 0.843137, 0.0156863, 0.1568630);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(447.7980 + this->getShift(), 0.752941, 0.7529410, 0.7529410);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3592.730 + this->getShift(), 1.000000, 1.0000000, 1.0000000);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(255 + this->getShift(), 1.00);
		break;
	case CT_SOFT_TISSUE:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(0);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.2);
		this->getVolume()->GetProperty()->SetDiffuse(1.0);
		this->getVolume()->GetProperty()->SetSpecular(0.0);
		this->getVolume()->GetProperty()->SetSpecularPower(1.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-2048.0 + this->getShift(), 0.00 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-167.01 + this->getShift(), 0.00 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-160.00 + this->getShift(), 1.00 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(240.000 + this->getShift(), 1.00 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(3661.00 + this->getShift(), 1.00 * this->volumeOpacity);


		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-2048.0 + this->getShift(), 0.0000000, 0.0000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-167.01 + this->getShift(), 0.0000000, 0.0000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-160.00 + this->getShift(), 0.0556356, 0.0556356, 0.0556356);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(240.000 + this->getShift(), 1.0000000, 1.0000000, 1.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3661.00 + this->getShift(), 1.0000000, 1.0000000, 1.0000000);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(255 + this->getShift(), 1.00);
		break;
	case MR_ANGIO:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.2);
		this->getVolume()->GetProperty()->SetDiffuse(1.0);
		this->getVolume()->GetProperty()->SetSpecular(0.0);
		this->getVolume()->GetProperty()->SetSpecularPower(1.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-2048.0 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(151.354 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(158.279 + this->getShift(), 0.437500 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(190.112 + this->getShift(), 0.580357 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(200.873 + this->getShift(), 0.732143 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(3661.00 + this->getShift(), 0.741071 * this->volumeOpacity);


		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-2048.0 + this->getShift(), 0.0000000, 0.0000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(151.354 + this->getShift(), 0.0000000, 0.0000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(158.279 + this->getShift(), 0.7490200, 0.3764710, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(190.112 + this->getShift(), 1.0000000, 0.8666670, 0.7333330);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(200.873 + this->getShift(), 0.9372550, 0.9372550, 0.9372550);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3661.00 + this->getShift(), 1.0000000, 1.0000000, 1.0000000);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(255 + this->getShift(), 1.00);
		break;
	case MR_DEFAULT:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.2);
		this->getVolume()->GetProperty()->SetDiffuse(1.0);
		this->getVolume()->GetProperty()->SetSpecular(0.0);
		this->getVolume()->GetProperty()->SetSpecularPower(1.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(0.0000 + this->getShift(), 0.000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(20.000 + this->getShift(), 0.000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(40.000 + this->getShift(), 0.015 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(120.00 + this->getShift(), 0.300 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(220.00 + this->getShift(), 0.375 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(1024.0 + this->getShift(), 0.500 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(0.0000 + this->getShift(), 0.000000, 0.000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(20.000 + this->getShift(), 0.168627, 0.000000, 0.0000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(40.000 + this->getShift(), 0.403922, 0.145098, 0.0784314);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(120.00 + this->getShift(), 0.780392, 0.607843, 0.3803920);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(220.00 + this->getShift(), 0.847059, 0.835294, 0.7882350);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(1024.0 + this->getShift(), 1.000000, 1.000000, 1.0000000);


		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(255 + this->getShift(), 1.00);
		break;
	case MR_MIP:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(0);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.2);
		this->getVolume()->GetProperty()->SetDiffuse(1.0);
		this->getVolume()->GetProperty()->SetSpecular(0.0);
		this->getVolume()->GetProperty()->SetSpecularPower(1.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(0.00000 + this->getShift(), 0.000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(98.3725 + this->getShift(), 0.000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(416.637 + this->getShift(), 1.000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(2800.00 + this->getShift(), 1.000 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(0.00000 + this->getShift(), 1.00, 1.00, 1.00);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(98.3725 + this->getShift(), 1.00, 1.00, 1.00);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(416.637 + this->getShift(), 1.00, 1.00, 1.00);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(2800.00 + this->getShift(), 1.00, 1.00, 1.00);


		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(255 + this->getShift(), 1.00);
		break;
	case MRT2_BRAIN:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.3);
		this->getVolume()->GetProperty()->SetDiffuse(0.6);
		this->getVolume()->GetProperty()->SetSpecular(0.5);
		this->getVolume()->GetProperty()->SetSpecularPower(40.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(0.00000 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(36.0500 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(218.302 + this->getShift(), 0.171429 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(412.406 + this->getShift(), 1.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(641.000 + this->getShift(), 1.000000 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(0.00000 + this->getShift(), 0.000000, 0.000000, 0.000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(98.7223 + this->getShift(), 0.956863, 0.839216, 0.192157);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(412.406 + this->getShift(), 0.000000, 0.592157, 0.807843);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(641.000 + this->getShift(), 1.000000, 1.000000, 1.000000);


		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0000 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(106.25 + this->getShift(), 1.00);
		break;
	case CBCT_DENTAL:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.9);
		this->getVolume()->GetProperty()->SetDiffuse(0.1);
		this->getVolume()->GetProperty()->SetSpecular(0.0);
		this->getVolume()->GetProperty()->SetSpecularPower(1.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-1000.0 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-7.8099 + this->getShift(), 0.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(317.680 + this->getShift(), 0.349206358194351 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(695.867 + this->getShift(), 0.30158731341362 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(954.626 + this->getShift(), 1.000000 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(1229.55 + this->getShift(), 0.190476208925247 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-1000.00 + this->getShift(), 0.300000, 0.300000, 1.000000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-340.000 + this->getShift(), 0.300000, 1.000000, 0.300000);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-127.201 + this->getShift(), 0.821191029141014, 0.255441386941408, 0.0766324160824225);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(203.6522 + this->getShift(), 0.525490196078431, 0.396078431372549, 0.282352941176471);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(538.9766 + this->getShift(), 1.000000, 0.854901960784314, 0.752941176470588);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(953.0000 + this->getShift(), 1.000000, 0.99607843172549, 0.913725490196078);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0000 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(985.12 + this->getShift(), 1.00);
		break;
	case CBCT_DENTAL_PHANTOM:
		this->getVolume()->GetProperty()->ShadeOn();
		this->getVolume()->GetProperty()->SetShade(1);
		this->getVolume()->GetProperty()->SetInterpolationTypeToLinear();
		this->getVolume()->GetProperty()->SetAmbient(0.2);
		this->getVolume()->GetProperty()->SetDiffuse(1.0);
		this->getVolume()->GetProperty()->SetSpecular(0.0);
		this->getVolume()->GetProperty()->SetSpecularPower(1.0);

		this->getVolume()->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(-27.670 + this->getShift(), 0.00 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(1127.01 + this->getShift(), 0.14 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(1623.96 + this->getShift(), 0.19 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(2150.14 + this->getShift(), 0.31 * this->volumeOpacity);
		this->getVolume()->GetProperty()->GetScalarOpacity()->AddPoint(3027.12 + this->getShift(), 1.00 * this->volumeOpacity);

		this->getVolume()->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-553.86 + this->getShift(), 1, 0.5255, 0.5255);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(1945.52 + this->getShift(), 0.9254, 0.2823, 0.0862);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(2033.21 + this->getShift(), 1, 0.855, 0.753);
		this->getVolume()->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3085.58 + this->getShift(), 1, 0.996, 0.9698);

		this->getVolume()->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + this->getShift(), 1.00);
		this->getVolume()->GetProperty()->GetGradientOpacity()->AddPoint(214 + this->getShift(), 1.00);
		break;
	default:
		break;
	}
}

int Volume::getPreset()
{
	return getAttribute(this->preset).toUInt();
}

void Volume::setRenderDataSet(DataSet* data)
{
	if (data == this->getRenderDataSet()) {
		return;
	}
	if (this->getRenderDataSet()) {
		this->getVolume()->GetMapper()->SetInputConnection(nullptr);
	}
	Prop::setRenderDataSet(data);
	if (this->getRenderDataSet()) {
		if (!data->isClass("Image")) {
			qCritical() << "data is not Image.";
		}
		this->getVolume()->GetMapper()->SetInputConnection(data->getOutputPort());
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