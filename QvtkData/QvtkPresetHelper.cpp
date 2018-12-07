// // me 
// #include "QvtkPresetHelper.h"
// // qt 
// #include <QFile>
// #include <QDomDocument>
// using namespace Q::vtk;
// const QString PresetHelper::PRESET_FILE(":/VolumeRendering/presets.xml");
// const struct PresetHelperInit
// {
//   PresetHelperInit()
//   {
//     Q_INIT_RESOURCE(QvtkPresetHelper);
//   }
// } PRESET_HELPER_INIT;
// PresetHelper::PresetHelper()
// {
// }

// PresetHelper::~PresetHelper()
// {
// }

// bool PresetHelper::readPresetFile(QString path)
// {
//   QFile file(path);
//   if(!file.open(QFile::ReadOnly | QFile::Text))
//   {
//     return false;
//   }
//   QDomDocument doc(path);
//   if(!doc.setContent(&file))
//   {
//     file.close();
//     return;
//   }
//   file.close();
// }