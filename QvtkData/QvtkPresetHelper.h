// #ifndef __QVTK_PRESET_HELPER_H__
// #define __QVTK_PRESET_HELPER_H__
// #pragma once
// // me 
// #include "qvtkdata_export.h"
// // qt
// #include <QString>
// #include <QHash>
// class QDomElement;
// namespace Q{
// namespace vtk{
// class QVTKDATA_EXPORT PresetHelper
// {
// public:
//   struct Preset
//   {
//     QString name;
//     QList<double> gradientOpacity;
//     QList<double> scalarOpacity;
//     QList<double> colorTransfer;
//     double ambient;
//     double diffuse;
//     double specular; 
//     double specularPower;
//     bool shade;
//     bool interpolation;
//   };
//   static const QString PRESET_FILE;
//   PresetHelper();
//   virtual ~PresetHelper();
//   virtual bool readPresetFile(QString path);
// protected:
  
// };
// }
// }
// #endif //!__QVTK_PRESET_HELPER_H__