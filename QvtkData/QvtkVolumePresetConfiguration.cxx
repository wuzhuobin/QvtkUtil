// vtk
#include <vtkVolumeProperty.h>
#include <vtkVolume.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
typedef enum
{
    NONE = 0,
    CT_AAA,
    CT_AAA2,
    CT_BONE,
    CT_BONES,
    CT_CARDIAC,
    CT_CARDIAC2,
    CT_CARDIAC3,
    CT_CHEST_CONTRAST_ENHANCED,
    CT_CHEST_VESSELS,
    CT_CORONARY_ARTERIES,
    CT_CORONARY_ARTERIES2,
    CT_CORONARY_ARTERIES3,
    CT_CROPPED_VOLUME_BONE,
    CT_FAT,
    CT_LIVER_VASCULATURE,
    CT_LUNG,
    CT_MIP,
    CT_MUSCLE,
    CT_PULMONARY_ARTERIES,
    CT_SOFT_TISSUE,
    MR_ANGIO,
    MR_DEFAULT,
    MR_MIP,
    MRT2_BRAIN,
    CBCT_DENTAL,
    CBCT_DENTAL_PHANTOM
} EnumPreset;
void set_volume_property(vtkVolume *&volume, const int & preset, const double & shift, const double & opacity)
{
	switch (preset)
	{
	case NONE:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.1);
		volume->GetProperty()->SetDiffuse(0.9);
		volume->GetProperty()->SetSpecular(0.2);
		volume->GetProperty()->SetSpecularPower(10.0);
		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(0, 1.0 * opacity);
		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		break;

	case CT_AAA:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.1);
		volume->GetProperty()->SetDiffuse(0.9);
		volume->GetProperty()->SetSpecular(0.2);
		volume->GetProperty()->SetSpecularPower(10.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-3024.0 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(143.556 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(166.222 + shift, 0.686275 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(214.389 + shift, 0.696078 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(419.736 + shift, 0.833333 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(3071.00 + shift, 0.803922 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-3024.0 + shift, 0.000000, 0.000000, 0.000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(143.556 + shift, 0.615686, 0.356863, 0.184314);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(166.222 + shift, 0.882353, 0.603922, 0.290196);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(214.389 + shift, 1.000000, 1.000000, 1.000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(419.736 + shift, 1.000000, 0.937033, 0.954531);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3071.00 + shift, 0.827451, 0.658824, 1.000000);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(255 + shift, 1.00);
		break;
	case CT_AAA2:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.1);
		volume->GetProperty()->SetDiffuse(0.9);
		volume->GetProperty()->SetSpecular(0.2);
		volume->GetProperty()->SetSpecularPower(10.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-3024.0 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(129.542 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(145.244 + shift, 0.166667 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(157.020 + shift, 0.500000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(169.918 + shift, 0.627451 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(395.575 + shift, 0.812500 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(1578.73 + shift, 0.812500 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(3071.00 + shift, 0.812500 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-3024.0 + shift, 0.000000, 0.000000, 0.000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(129.542 + shift, 0.549020, 0.250980, 0.149020);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(145.244 + shift, 0.600000, 0.627451, 0.843137);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(157.020 + shift, 0.890196, 0.474510, 0.600000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(169.918 + shift, 0.992157, 0.870588, 0.392157);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(395.575 + shift, 1.000000, 0.886275, 0.658824);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(1578.73 + shift, 1.000000, 0.829256, 0.957922);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3071.00 + shift, 0.827451, 0.658824, 1.000000);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(255 + shift, 1.00);
		break;
	case CT_BONE:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.1);
		volume->GetProperty()->SetDiffuse(0.9);
		volume->GetProperty()->SetSpecular(0.2);
		volume->GetProperty()->SetSpecularPower(10.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-3024.00 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-16.4458 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(641.3850 + shift, 0.715686 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(3071.000 + shift, 0.705882 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-3024.00 + shift, 0.000000, 0.000000, 0.000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-16.4458 + shift, 0.729412, 0.254902, 0.301961);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(641.3850 + shift, 0.905882, 0.815686, 0.552941);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3071.000 + shift, 1.000000, 1.000000, 1.000000);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(255 + shift, 1.00);
		break;
	case CT_BONES:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.2);
		volume->GetProperty()->SetDiffuse(1.0);
		volume->GetProperty()->SetSpecular(0.0);
		volume->GetProperty()->SetSpecularPower(1.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-1000.0 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(152.190 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(278.930 + shift, 0.190476 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(952.000 + shift, 0.200000 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-1000.0 + shift, 0.3, 0.300000, 1.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-488.00 + shift, 0.3, 1.000000, 0.3000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(463.280 + shift, 1.0, 0.000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(659.150 + shift, 1.0, 0.912535, 0.0374849);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(953.000 + shift, 1.0, 0.300000, 0.3000000);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0000 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(985.12 + shift, 1.00);
		break;
	case CT_CARDIAC:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.1);
		volume->GetProperty()->SetDiffuse(0.9);
		volume->GetProperty()->SetSpecular(0.2);
		volume->GetProperty()->SetSpecularPower(10.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-3024.00 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-77.6875 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(94.95180 + shift, 0.285714 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(179.0520 + shift, 0.553571 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(260.4390 + shift, 0.848214 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(3071.000 + shift, 0.875000 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-3024.00 + shift, 0.000000, 0.000000, 0.000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-77.6875 + shift, 0.549020, 0.250980, 0.149020);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(94.95180 + shift, 0.882353, 0.603922, 0.290196);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(179.0520 + shift, 1.000000, 0.937033, 0.954531);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(260.4390 + shift, 0.615686, 0.000000, 0.000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3071.000 + shift, 0.827451, 0.658824, 1.000000);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(255, 1.00);
		break;
	case CT_CARDIAC2:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.1);
		volume->GetProperty()->SetDiffuse(0.9);
		volume->GetProperty()->SetSpecular(0.2);
		volume->GetProperty()->SetSpecularPower(10.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-3024.00 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(42.89640 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(163.4880 + shift, 0.428571 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(277.6420 + shift, 0.776786 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(1587.000 + shift, 0.754902 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(3071.000 + shift, 0.754902 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-3024.00 + shift, 0.000000, 0.000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(42.89640 + shift, 0.549020, 0.250980, 0.1490200);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(163.4880 + shift, 0.917647, 0.639216, 0.0588235);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(277.6420 + shift, 1.000000, 0.878431, 0.6235290);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(1587.000 + shift, 1.000000, 1.000000, 1.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3071.000 + shift, 0.827451, 0.658824, 1.0000000);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(255 + shift, 1.00);
		break;
	case CT_CARDIAC3:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.1);
		volume->GetProperty()->SetDiffuse(0.9);
		volume->GetProperty()->SetSpecular(0.2);
		volume->GetProperty()->SetSpecularPower(10.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-3024.00 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-86.9767 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(45.37910 + shift, 0.169643 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(139.9190 + shift, 0.589286 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(347.9070 + shift, 0.607143 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(1224.160 + shift, 0.607143 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(3071.000 + shift, 0.616071 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-3024.00 + shift, 0.000000, 0.000000, 0.000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-86.9767 + shift, 0.000000, 0.250980, 1.000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(45.37910 + shift, 1.000000, 0.000000, 0.000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(139.9190 + shift, 1.000000, 0.894893, 0.894893);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(347.9070 + shift, 1.000000, 1.000000, 0.250980);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(1224.160 + shift, 1.000000, 1.000000, 1.000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3071.000 + shift, 0.827451, 0.658824, 1.000000);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(255 + shift, 1.00);
		break;
	case CT_CHEST_CONTRAST_ENHANCED:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.1);
		volume->GetProperty()->SetDiffuse(0.9);
		volume->GetProperty()->SetSpecular(0.2);
		volume->GetProperty()->SetSpecularPower(10.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-3024.00 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(67.01060 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(251.1050 + shift, 0.446429 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(439.2910 + shift, 0.625000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(3071.000 + shift, 0.616071 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-3024.00 + shift, 0.000000, 0.000000, 0.000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(67.01060 + shift, 0.549020, 0.250980, 0.149020);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(251.1050 + shift, 0.882353, 0.603922, 0.290196);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(439.2910 + shift, 1.000000, 0.937033, 0.954531);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3071.000 + shift, 0.827451, 0.658824, 1.000000);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(255 + shift, 1.00);
		break;
	case CT_CHEST_VESSELS:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.2);
		volume->GetProperty()->SetDiffuse(1.0);
		volume->GetProperty()->SetSpecular(0.0);
		volume->GetProperty()->SetSpecularPower(1.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-3024.00 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-1278.35 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(22.82770 + shift, 0.428571 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(439.2910 + shift, 0.625000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(3071.000 + shift, 0.616071 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-3024.00 + shift, 0.000000, 0.000000, 0.000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-1278.35 + shift, 0.549020, 0.250980, 0.149020);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(22.82770 + shift, 0.882353, 0.603922, 0.290196);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(439.2910 + shift, 1.000000, 0.937033, 0.954531);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3071.000 + shift, 0.827451, 0.658824, 1.000000);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(255 + shift, 1.00);
		break;
	case CT_CORONARY_ARTERIES:
		volume->GetProperty()->SetShade(0);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.2);
		volume->GetProperty()->SetDiffuse(1.0);
		volume->GetProperty()->SetSpecular(0.0);
		volume->GetProperty()->SetSpecularPower(1.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-2048.0 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(36.4700 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(159.215 + shift, 0.258929 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(318.430 + shift, 0.571429 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(478.693 + shift, 0.776786 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(3661.00 + shift, 1.000000 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-2048.0 + shift, 0.000000, 0.000000, 0.000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(136.470 + shift, 0.000000, 0.000000, 0.000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(159.215 + shift, 0.159804, 0.159804, 0.159804);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(318.430 + shift, 0.764706, 0.764706, 0.764706);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(478.693 + shift, 1.000000, 1.000000, 1.000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3661.00 + shift, 1.000000, 1.000000, 1.000000);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(255 + shift, 1.00);
		break;
	case CT_CORONARY_ARTERIES2:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.2);
		volume->GetProperty()->SetDiffuse(1.0);
		volume->GetProperty()->SetSpecular(0.0);
		volume->GetProperty()->SetSpecularPower(1.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-2048.0 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(142.677 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(145.016 + shift, 0.116071 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(192.174 + shift, 0.562500 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(384.347 + shift, 0.830357 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(3661.00 + shift, 0.830357 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-2048.0 + shift, 0.000000, 0.000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(142.677 + shift, 0.000000, 0.000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(145.016 + shift, 0.615686, 0.000000, 0.0156863);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(192.174 + shift, 0.909804, 0.454902, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(217.240 + shift, 0.972549, 0.807843, 0.6117650);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(384.347 + shift, 0.909804, 0.909804, 1.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3661.00 + shift, 1.000000, 1.000000, 1.000000);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(255 + shift, 1.00);
		break;
	case CT_CORONARY_ARTERIES3:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.1);
		volume->GetProperty()->SetDiffuse(0.9);
		volume->GetProperty()->SetSpecular(0.2);
		volume->GetProperty()->SetSpecularPower(10.0);


		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-2048.0 + shift, 0.0000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(128.643 + shift, 0.0000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(129.982 + shift, 0.0982143 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(173.636 + shift, 0.6696430 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(255.884 + shift, 0.8571430 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(584.878 + shift, 0.8660710 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(3661.00 + shift, 1.0000000 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-2048.0 + shift, 0.000000, 0.000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(128.643 + shift, 0.000000, 0.000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(129.982 + shift, 0.615686, 0.000000, 0.0156863);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(173.636 + shift, 0.909804, 0.454902, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(255.884 + shift, 0.886275, 0.886275, 0.8862750);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(584.878 + shift, 0.968627, 0.968627, 0.9686270);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3661.00 + shift, 1.000000, 1.000000, 1.000000);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(255 + shift, 1.00);
		break;
	case CT_CROPPED_VOLUME_BONE:
		volume->GetProperty()->SetShade(0);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.2);
		volume->GetProperty()->SetDiffuse(1.0);
		volume->GetProperty()->SetSpecular(0.0);
		volume->GetProperty()->SetSpecularPower(1.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-2048.0 + shift, 0.0000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-451.00 + shift, 0.0000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-450.00 + shift, 1.0000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(1050.00 + shift, 1.0000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(3661.00 + shift, 1.0000000 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-2048.0 + shift, 0.0000000, 0.0000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-451.00 + shift, 0.0000000, 0.0000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-450.00 + shift, 0.0556356, 0.0556356, 0.0556356);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(1050.00 + shift, 1.0000000, 1.0000000, 1.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3661.00 + shift, 1.0000000, 1.0000000, 1.0000000);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(255 + shift, 1.00);
		break;
	case CT_FAT:
		volume->GetProperty()->SetShade(0);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.2);
		volume->GetProperty()->SetDiffuse(1.0);
		volume->GetProperty()->SetSpecular(0.0);
		volume->GetProperty()->SetSpecularPower(1.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-1000.0 + shift, 0.00 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-100.00 + shift, 0.00 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-99.000 + shift, 0.15 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-60.000 + shift, 0.15 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-59.000 + shift, 0.00 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(101.200 + shift, 0.00 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(952.000 + shift, 0.00 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-1000.0 + shift, 0.300000, 0.300000, 1.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-497.50 + shift, 0.300000, 1.000000, 0.3000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-99.000 + shift, 0.000000, 0.000000, 1.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-76.946 + shift, 0.000000, 1.000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-65.481 + shift, 0.835431, 0.888889, 0.0165387);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(83.8900 + shift, 1.000000, 0.000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(463.280 + shift, 1.000000, 0.000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(659.150 + shift, 1.000000, 0.912535, 0.0374849);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0000, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(985.12, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(988.00, 1.00);
		break;
	case CT_LIVER_VASCULATURE:
		volume->GetProperty()->SetShade(0);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.2);
		volume->GetProperty()->SetDiffuse(1.0);
		volume->GetProperty()->SetSpecular(0.0);
		volume->GetProperty()->SetSpecularPower(1.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-2048.0 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(149.113 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(157.884 + shift, 0.482143 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(339.960 + shift, 0.660714 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(388.526 + shift, 0.830357 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(1197.95 + shift, 0.839286 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(3661.00 + shift, 0.848214 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-2048.0 + shift, 0.0000000, 0.0000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(149.113 + shift, 0.0000000, 0.0000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(157.884 + shift, 0.5019610, 0.2509800, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(339.960 + shift, 0.6953860, 0.5960300, 0.3688600);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(388.526 + shift, 0.8549020, 0.8509800, 0.8274510);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(1197.95 + shift, 1.0000000, 1.0000000, 1.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3661.00 + shift, 1.0000000, 1.0000000, 1.0000000);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(255 + shift, 1.00);
		break;
	case CT_LUNG:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.2);
		volume->GetProperty()->SetDiffuse(1.0);
		volume->GetProperty()->SetSpecular(0.0);
		volume->GetProperty()->SetSpecularPower(1.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-1000.0 + shift, 0.00 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-600.00 + shift, 0.00 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-599.00 + shift, 0.15 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-400.00 + shift, 0.15 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-399.00 + shift, 0.00 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(2952.00 + shift, 0.00 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-1000.0 + shift, 0.300000, 0.300000, 1.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-600.00 + shift, 0.000000, 0.000000, 1.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-530.00 + shift, 0.134704, 0.781726, 0.0724558);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-460.00 + shift, 0.929244, 1.000000, 0.1094730);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-400.00 + shift, 0.888889, 0.254949, 0.0240258);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(2952.00 + shift, 1.000000, 0.300000, 0.3000000);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0000 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(985.12 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(988.00 + shift, 1.00);
		break;
	case CT_MIP:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.1);
		volume->GetProperty()->SetDiffuse(0.9);
		volume->GetProperty()->SetSpecular(0.2);
		volume->GetProperty()->SetSpecularPower(10.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-3024.0 + shift, 0 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-637.62 + shift, 0 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(700.000 + shift, 1 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(3071.00 + shift, 1 * opacity);


		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-3024.0 + shift, 0, 0, 0);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-637.62 + shift, 1, 1, 1);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(700.000 + shift, 1, 1, 1);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3071.00 + shift, 1, 1, 1);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(255 + shift, 1.00);
		break;
	case CT_MUSCLE:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.1);
		volume->GetProperty()->SetDiffuse(0.9);
		volume->GetProperty()->SetSpecular(0.2);
		volume->GetProperty()->SetSpecularPower(10.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-3024.00 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-155.407 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(217.6410 + shift, 0.676471 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(419.7360 + shift, 0.833333 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(3071.000 + shift, 0.803922 * opacity);


		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-3024.00 + shift, 0.000000, 0.000000, 0.000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-155.407 + shift, 0.549020, 0.250980, 0.149020);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(217.6410 + shift, 0.882353, 0.603922, 0.290196);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(419.7360 + shift, 1.000000, 0.937033, 0.954531);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3071.000 + shift, 0.827451, 0.658824, 1.000000);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(255, 1.00);
		break;
	case CT_PULMONARY_ARTERIES:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.2);
		volume->GetProperty()->SetDiffuse(1.0);
		volume->GetProperty()->SetSpecular(0.0);
		volume->GetProperty()->SetSpecularPower(1.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-2048.00 + shift, 0.0000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-568.625 + shift, 0.0000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-364.081 + shift, 0.0714286 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-244.813 + shift, 0.4017860 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(18.27750 + shift, 0.6071430 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(447.7980 + shift, 0.8303570 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(3592.730 + shift, 0.8392860 * opacity);


		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-2048.00 + shift, 0.000000, 0.0000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-568.625 + shift, 0.000000, 0.0000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-364.081 + shift, 0.396078, 0.3019610, 0.1803920);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-244.813 + shift, 0.611765, 0.3529410, 0.0705882);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(18.27750 + shift, 0.843137, 0.0156863, 0.1568630);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(447.7980 + shift, 0.752941, 0.7529410, 0.7529410);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3592.730 + shift, 1.000000, 1.0000000, 1.0000000);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(255 + shift, 1.00);
		break;
	case CT_SOFT_TISSUE:
		volume->GetProperty()->SetShade(0);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.2);
		volume->GetProperty()->SetDiffuse(1.0);
		volume->GetProperty()->SetSpecular(0.0);
		volume->GetProperty()->SetSpecularPower(1.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-2048.0 + shift, 0.00 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-167.01 + shift, 0.00 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-160.00 + shift, 1.00 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(240.000 + shift, 1.00 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(3661.00 + shift, 1.00 * opacity);


		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-2048.0 + shift, 0.0000000, 0.0000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-167.01 + shift, 0.0000000, 0.0000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-160.00 + shift, 0.0556356, 0.0556356, 0.0556356);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(240.000 + shift, 1.0000000, 1.0000000, 1.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3661.00 + shift, 1.0000000, 1.0000000, 1.0000000);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(255 + shift, 1.00);
		break;
	case MR_ANGIO:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.2);
		volume->GetProperty()->SetDiffuse(1.0);
		volume->GetProperty()->SetSpecular(0.0);
		volume->GetProperty()->SetSpecularPower(1.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-2048.0 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(151.354 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(158.279 + shift, 0.437500 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(190.112 + shift, 0.580357 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(200.873 + shift, 0.732143 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(3661.00 + shift, 0.741071 * opacity);


		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-2048.0 + shift, 0.0000000, 0.0000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(151.354 + shift, 0.0000000, 0.0000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(158.279 + shift, 0.7490200, 0.3764710, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(190.112 + shift, 1.0000000, 0.8666670, 0.7333330);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(200.873 + shift, 0.9372550, 0.9372550, 0.9372550);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3661.00 + shift, 1.0000000, 1.0000000, 1.0000000);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(255 + shift, 1.00);
		break;
	case MR_DEFAULT:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.2);
		volume->GetProperty()->SetDiffuse(1.0);
		volume->GetProperty()->SetSpecular(0.0);
		volume->GetProperty()->SetSpecularPower(1.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(0.0000 + shift, 0.000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(20.000 + shift, 0.000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(40.000 + shift, 0.015 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(120.00 + shift, 0.300 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(220.00 + shift, 0.375 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(1024.0 + shift, 0.500 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(0.0000 + shift, 0.000000, 0.000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(20.000 + shift, 0.168627, 0.000000, 0.0000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(40.000 + shift, 0.403922, 0.145098, 0.0784314);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(120.00 + shift, 0.780392, 0.607843, 0.3803920);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(220.00 + shift, 0.847059, 0.835294, 0.7882350);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(1024.0 + shift, 1.000000, 1.000000, 1.0000000);


		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(255 + shift, 1.00);
		break;
	case MR_MIP:
		volume->GetProperty()->SetShade(0);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.2);
		volume->GetProperty()->SetDiffuse(1.0);
		volume->GetProperty()->SetSpecular(0.0);
		volume->GetProperty()->SetSpecularPower(1.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(0.00000 + shift, 0.000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(98.3725 + shift, 0.000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(416.637 + shift, 1.000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(2800.00 + shift, 1.000 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(0.00000 + shift, 1.00, 1.00, 1.00);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(98.3725 + shift, 1.00, 1.00, 1.00);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(416.637 + shift, 1.00, 1.00, 1.00);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(2800.00 + shift, 1.00, 1.00, 1.00);


		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(255 + shift, 1.00);
		break;
	case MRT2_BRAIN:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.3);
		volume->GetProperty()->SetDiffuse(0.6);
		volume->GetProperty()->SetSpecular(0.5);
		volume->GetProperty()->SetSpecularPower(40.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(0.00000 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(36.0500 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(218.302 + shift, 0.171429 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(412.406 + shift, 1.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(641.000 + shift, 1.000000 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(0.00000 + shift, 0.000000, 0.000000, 0.000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(98.7223 + shift, 0.956863, 0.839216, 0.192157);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(412.406 + shift, 0.000000, 0.592157, 0.807843);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(641.000 + shift, 1.000000, 1.000000, 1.000000);


		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0000 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(106.25 + shift, 1.00);
		break;
	case CBCT_DENTAL:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.9);
		volume->GetProperty()->SetDiffuse(0.1);
		volume->GetProperty()->SetSpecular(0.0);
		volume->GetProperty()->SetSpecularPower(1.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-1000.0 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-7.8099 + shift, 0.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(317.680 + shift, 0.349206358194351 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(695.867 + shift, 0.30158731341362 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(954.626 + shift, 1.000000 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(1229.55 + shift, 0.190476208925247 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-1000.00 + shift, 0.300000, 0.300000, 1.000000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-340.000 + shift, 0.300000, 1.000000, 0.300000);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-127.201 + shift, 0.821191029141014, 0.255441386941408, 0.0766324160824225);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(203.6522 + shift, 0.525490196078431, 0.396078431372549, 0.282352941176471);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(538.9766 + shift, 1.000000, 0.854901960784314, 0.752941176470588);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(953.0000 + shift, 1.000000, 0.99607843172549, 0.913725490196078);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0000 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(985.12 + shift, 1.00);
		break;
	case CBCT_DENTAL_PHANTOM:
		volume->GetProperty()->SetShade(1);
		volume->GetProperty()->SetInterpolationTypeToLinear();
		volume->GetProperty()->SetAmbient(0.2);
		volume->GetProperty()->SetDiffuse(1.0);
		volume->GetProperty()->SetSpecular(0.0);
		volume->GetProperty()->SetSpecularPower(1.0);

		volume->GetProperty()->GetScalarOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetScalarOpacity()->AddPoint(-27.670 + shift, 0.00 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(1127.01 + shift, 0.14 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(1623.96 + shift, 0.19 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(2150.14 + shift, 0.31 * opacity);
		volume->GetProperty()->GetScalarOpacity()->AddPoint(3027.12 + shift, 1.00 * opacity);

		volume->GetProperty()->GetRGBTransferFunction()->RemoveAllPoints();
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(-553.86 + shift, 1, 0.5255, 0.5255);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(1945.52 + shift, 0.9254, 0.2823, 0.0862);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(2033.21 + shift, 1, 0.855, 0.753);
		volume->GetProperty()->GetRGBTransferFunction()->AddRGBPoint(3085.58 + shift, 1, 0.996, 0.9698);

		volume->GetProperty()->GetGradientOpacity()->RemoveAllPoints();
		volume->GetProperty()->GetGradientOpacity()->AddPoint(0.0 + shift, 1.00);
		volume->GetProperty()->GetGradientOpacity()->AddPoint(214 + shift, 1.00);
		break;
	default:
		break;
	}
}