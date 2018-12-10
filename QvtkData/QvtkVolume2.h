
#ifndef __QVTK_VOLUME2_H__
#define __QVTK_VOLUME2_H__ 
#pragma once
// me 
#include "QvtkProp.h"
#include "QvtkLabelHelper.h"
// vtk
class vtkVolume;
class vtkSmartVolumeMapper;
namespace Q {
namespace vtk {
class QVTKDATA_EXPORT Volume2 :
  public Prop,
  public LabelHelper
{
  Q_OBJECT;
  Q_VTK_DATA_H(
    Volume2,
    Q_VTK_KEY(DefaultColorFile)
    Q_VTK_KEY(ColorFile)
  );
public:
  Volume2();
  virtual ~Volume2() override;
  virtual void printSelf() const override;
  virtual vtkVolume* getVolume() const;
  virtual void readXML(const QDomElement& xml, QString directoryPath = QString()) override;
  virtual void writeXML(QDomElement& xml, QString directoryPath = QString()) const override;
  virtual int getDefaultColorFile() const;
  virtual QString getColorFile() const;
public Q_SLOTS:
  virtual void setDisplayRegion(const double region[6]) override;
  /**
   * @brief  Change the preseting of vtkVolumeProperty
   */
  virtual void setRenderDataSet(DataSet* data) override;
  virtual void setDefaultColorFile(int i);
  virtual void setColorFile(QString colorFile);
  virtual void setOpacity(double opacity) override {}
  virtual void setLevel(double level);
  virtual void setWindow(double window);
protected:
  virtual Data* newInstance() const override;
  vtkSmartVolumeMapper* smartVolumeMapper;
  static void setDefaultColorFile(Data *self, QStandardItem *item);
  static void setColorFile(Data *self, QStandardItem *item);
private:
  QStandardItem *colorFile;
  QStandardItem *defaultColorFile;
};
}
}
#endif// !__QVTK_VOLUME2_H__

