//
//
//#ifndef __Qvtk_ABSTRACT_ITK_IMAGE_FILTER_H__
//#define __Qvtk_ABSTRACT_ITK_IMAGE_FILTER_H__
//
//#include "QvtkAbstractImageFilter.h"
//
//
//#include <itkImage.h>
//#include <itkImageToImageFilter.h>
//#include <itkImageToVTKImageFilter.h>
//#include <itkVTKImageToImageFilter.h>
//
//template <typename T>
//class QvtkAbstractITKImageFilter : public QvtkAbstractImageFilter
//{
//public:
//	QvtkAbstractITKImageFilter(QObject* parent = nullptr);
//	~QvtkAbstractITKImageFilter();
//
//	virtual void SetInputImage(itk::Image<T, 3> inImage);
//
//	virtual itk::ImageToVTKImageFilter<T> GetFilter();
//
//protected:
//	itk::ImageToImageFilter<T, T> itkfilter
//};
//
//template <typename T>
//QvtkAbstractITKImageFilter<T>::QvtkAbstractITKImageFilter(QObject* parent /*= nullptr*/)
//{
//	
//}
//
//#include "QvtkAbstractITKImageFilter.cpp"
//
//#endif