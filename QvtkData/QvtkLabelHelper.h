#ifndef __QVTK_LABEL_HELPER_H__
#define __QVTK_LABEL_HELPER_H__
#pragma once
// me 
#include "qvtkdata_export.h"
// vtk
class vtkLookupTable;
class vtkNamedColors;
class vtkDiscretizableColorTransferFunction;
class vtkScalarsToColors;
// qt
#include <QHash>
class QDomElement;
namespace Q {
namespace vtk {
class QVTKDATA_EXPORT LabelHelper {
public:
  typedef QHash<int, QString> LabelIdToLabelName;
  static const QStringList DEFAULT_COLOR_FILES;
  static const QString DEFAULT_PRESET_FILE;
  static const QString LABEL;
  LabelHelper();
  virtual ~LabelHelper();
  virtual bool readColorFile(QString path);
  virtual bool readPresetFile(QString path);
  virtual const LabelIdToLabelName getLabelIdToLabelName() const { return this->labelIdToLabelName; }
  virtual void setColor(int id, const double* rgba) { this->setColor(id, rgba[0], rgba[1], rgba[2], rgba[3]); }
  virtual void setColor(int id, double r, double g, double b, double a);
  virtual void getColor(int id, double rgba[4]) const;
  virtual void getColor(QString labelName, double rgba[4]) const;
  virtual vtkLookupTable *getLookupTable() const { return this->lookupTable; }
  virtual vtkDiscretizableColorTransferFunction *getColorTransferFunction() const { return this->transferFunction;}
protected: 
  static void readLabel(LabelHelper *self, const QDomElement &xml, QString nameKey, QString rgbaKey);
  static void writeLabel(const LabelHelper *self, QDomElement &xml, QString nameKey, QString rgbaKey);
  virtual void namedColorsToLookupTable();
  virtual void namedColosrToTransferFunction();
  LabelIdToLabelName labelIdToLabelName;
  vtkLookupTable *lookupTable;
  vtkNamedColors *namedColors;
  vtkDiscretizableColorTransferFunction *transferFunction;
};
}
}
#endif // !__QVTK_LABEL_HELPER_H__
