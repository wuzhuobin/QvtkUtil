#ifndef __Qvtk_VOLUME_H__
#define __Qvtk_VOLUME_H__

#include "QvtkAbstractProp.h"

class vtkVolume;
class vtkSmartVolumeMapper;
class QStandardItem;
class vtkImageShiftScale;

class ABSTRACTDATA_EXPORT QvtkVolume: public QvtkAbstractProp
{
	Q_OBJECT;
	Q_VTK_DATAH(
		QvtkVolume,
		Q_VTK_KEY(Preset)
		Q_VTK_KEY(Shift)
	)

public:

	/**
	 * @brief preset property from slicer3D
	 */
	enum ENUM_PRESET
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
	};


	QvtkVolume();
	virtual ~QvtkVolume() override;


	virtual void printSelf() const override;
	virtual double getShift() const;
	virtual int getPreset();

	virtual vtkVolume* getVolume() const;

	virtual void readXML(const QDomElement& xml, QString directoryPath = QString()) override;
	virtual void writeXML(QDomElement& xml, QString directoryPath = QString()) const override;

public slots:
	virtual void setDisplayRegion(const double region[6]) override;

	/**
	 * @brief	Change the preseting of vtkVolumeProperty
	 */
	virtual void setPreset(unsigned int preset);
	void setPresetToNone() { setPreset(NONE); }
	void setPresetToCTAAA() { setPreset(CT_AAA); }
	void setPresetToCTAAA2() { setPreset(CT_AAA2); }
	void setPresetToCTBone() { setPreset(CT_BONE); }
	void setPresetToCTBones() { setPreset(CT_BONES); }
	void setPresetToCTCardiac() { setPreset(CT_CARDIAC); }
	void setPresetToCTCardiac2() { setPreset(CT_CARDIAC2); }
	void setPresetToCTCardiac3() { setPreset(CT_CARDIAC3); }
	void setPresetToCTChestContrastEnhanced() { setPreset(CT_CHEST_CONTRAST_ENHANCED); }
	void setPresetToCTChestVessels() { setPreset(CT_CHEST_VESSELS); }
	void setPresetToCTCoronaryArteries() { setPreset(CT_CORONARY_ARTERIES); }
	void setPresetToCTCoronaryArteries2() { setPreset(CT_CORONARY_ARTERIES2); }
	void setPresetToCTCoronaryArteries3() { setPreset(CT_CORONARY_ARTERIES3); }
	void setPresetToCTCroppedVolumeBone() { setPreset(CT_CROPPED_VOLUME_BONE); }
	void setPresetToCTFat() { setPreset(CT_FAT); }
	void setPresetToCTLiverVasculature() { setPreset(CT_LIVER_VASCULATURE); }
	void setPresetToCTLung() { setPreset(CT_LUNG); }
	void setPresetToCTMIP() { setPreset(CT_MIP); }
	void setPresetToCTMuscle() { setPreset(CT_MUSCLE); }
	void setPresetToCTPulmonaryArteries() { setPreset(CT_PULMONARY_ARTERIES); }
	void setPresetToCTSoftTissue() { setPreset(CT_SOFT_TISSUE); }
	void setPresetToMRAngio() { setPreset(MR_ANGIO); }
	void setPresetToMRDefault() { setPreset(MR_DEFAULT); }
	void setPresetToMRMIP() { setPreset(MR_MIP); }
	void setPresetToMRT2Brain() { setPreset(MRT2_BRAIN); }
	void setPresetToCBCTDental() { setPreset(CBCT_DENTAL); }
	void setPresetToCBCTDentalPhanton() { setPreset(CBCT_DENTAL_PHANTOM); }

	virtual void setShift(double shift);


	virtual void setRenderDataSet(DataSet* data) override;

	virtual void setOpacity(double opacity) override;

protected:

	virtual Data* newInstance() const override;

	static void setShift(Data* self, QStandardItem* item);
	static void setPreset(Data* self, QStandardItem* item);

	double volumeOpacity;

	QStandardItem* shift;

	QStandardItem* preset;

	//vtkImageResample* imageResample;
	vtkImageShiftScale* imageShiftScale;
	
	vtkSmartVolumeMapper* smartVolumeMapper;


private:




};

#endif// !__Qvtk_VOLUME_H__
