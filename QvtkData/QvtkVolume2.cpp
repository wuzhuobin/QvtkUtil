// me
#include "QvtkVolume2.h"
#include "QvtkDataSet.h"
// vtk
#include <vtkVolume.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkNew.h>
#include <vtkVolumeProperty.h>
#include <vtkDiscretizableColorTransferFunction.h>
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
  vtkNew<vtkSmartVolumeMapper> mapper;
  this->smartVolumeMapper = mapper.GetPointer();
  this->smartVolumeMapper->SetRequestedRenderModeToDefault();
  this->smartVolumeMapper->CroppingOn();
  vtkVolume *volume = vtkVolume::New();
  volume->SetMapper(this->smartVolumeMapper);
  volume->GetProperty()->SetColor(this->getColorTransferFunction());
  volume->GetProperty()->SetScalarOpacity(this->getColorTransferFunction()->GetScalarOpacityFunction());
  this->setProp(volume);
  this->defaultColorFile = this->createAttribute(K.ColorFile, static_cast<int>(0), true);
  this->insertSlotFunction(this->defaultColorFile, &Volume2::setDefaultColorFile);
  this->colorFile = createAttribute(K.ColorFile, "", true);
  this->insertSlotFunction(this->colorFile, &Volume2::setColorFile);
  this->setDefaultColorFile(DEFAULT_COLOR_FILES.size());
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

int Volume2::getDefaultColorFile() const
{
  return Data::getAttribute(this->defaultColorFile).toInt();
}

QString Volume2::getColorFile() const
{
  return Data::getAttribute(this->colorFile).toString();
}

vtkVolume * Volume2::getVolume() const
{
  return vtkVolume::SafeDownCast(this->getProp());
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
      return;
    }
    this->getVolume()->GetMapper()->SetInputConnection(data->getOutputPort());
  }
}

void Volume2::setDefaultColorFile(int i)
{
  this->setAttribute(this->defaultColorFile, i);
  if (i > -1 && i < DEFAULT_COLOR_FILES.size())
  {
    this->setColorFile(DEFAULT_COLOR_FILES[i]);
  }
  else if (i >= DEFAULT_COLOR_FILES.size())
  {
    //this->getImageActor()->GetProperty()->SetUseLookupTableScalarRange(false);
    //this->getVolume->GetProperty()->SetLookupTable(nullptr);
  }
}

void Volume2::setColorFile(QString colorFile)
{
  Data::setAttribute(this->colorFile, colorFile);
  if (!DEFAULT_COLOR_FILES.contains(colorFile))
  {
    this->setDefaultColorFile(-1);
  }
  if (!this->readColorFile(colorFile))
  {
    qCritical() << "Cannot find color file: " << colorFile;
    return;
  }
  this->getVolume()->GetProperty()->SetColor(this->getColorTransferFunction());
  this->getVolume()->GetProperty()->SetScalarOpacity(this->getColorTransferFunction()->GetScalarOpacityFunction());
  //   this->getImageActor()->GetProperty()->SetUseLookupTableScalarRange(true);
  this->namedColorsToLookupTable();
  this->namedColosrToTransferFunction();
}

void Volume2::setLevel(double level)
{
}

void Volume2::setWindow(double window)
{

}

void Volume2::setDefaultColorFile(Data *self, QStandardItem *item)
{
  Volume2 *_self = static_cast<Volume2*>(self);
  _self->setDefaultColorFile(getAttribute(item).toInt());
}

void Volume2::setColorFile(Data *self, QStandardItem *item)
{
  Volume2 *_self = static_cast<Volume2*>(self);
  _self->setColorFile(getAttribute(item).toString());
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
}
}
