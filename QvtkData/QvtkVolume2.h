
#ifndef __QVTK_VOLUME2_H__
#define __QVTK_VOLUME2_H__ 
#pragma once
// me 
#include "QvtkProp.h"
// vtk
class vtkVolume;
class vtkSmartVolumeMapper;
namespace Q {
namespace vtk {
class QVTKDATA_EXPORT Volume2 : public Prop
{
  Q_OBJECT;
  Q_PROPERTY(
    double    Shift
    READ    getShift
    WRITE    setShift);
  Q_VTK_DATA_H(
    Volume2,
    Q_VTK_KEY(Shift))
public:
  Volume2();
  virtual ~Volume2() override;
  virtual void printSelf() const override;
  virtual double getShift() const;
  virtual int getPreset() const;
  virtual vtkVolume* getVolume() const;
  virtual void readXML(const QDomElement& xml, QString directoryPath = QString()) override;
  virtual void writeXML(QDomElement& xml, QString directoryPath = QString()) const override;
public Q_SLOTS:
  virtual void setDisplayRegion(const double region[6]) override;
  /**
   * @brief  Change the preseting of vtkVolumeProperty
   */
  virtual void setPreset(int preset);
  virtual void setShift(double shift);
  virtual void setRenderDataSet(DataSet* data) override;
  virtual void setOpacity(double opacity) override;
protected:
  virtual Data* newInstance() const override;
  static void setShift(Data* self, QStandardItem* item);
  static void setPreset(Data* self, QStandardItem* item);
  vtkSmartVolumeMapper* smartVolumeMapper;
private:
  double volumeOpacity;
  QStandardItem* shift;
  QStandardItem* preset;
};
}
}
#endif// !__QVTK_VOLUME2_H__

