// me
#include "QvtkImage.h"
#include "QvtkImageIO_readITKImage.hpp"
bool Q::vtk::Image::readITKImage(QStringList paths, vtkImageData * image, double orientation[3], double position[3], double scale[3])
{
	itk::ImageIOBase::Pointer imageIO = itk::ImageIOFactory::CreateImageIO(
		paths.first().toStdString().c_str(), itk::ImageIOFactory::ReadMode);

	if (!imageIO)
	{
		qCritical() << "Could not Create ImageIO for:" << paths.first();
		return false;
	}

	imageIO->SetFileName(paths.first().toStdString());
	imageIO->ReadImageInformation();

	typedef itk::ImageIOBase::IOComponentType IOComponentType;

	bool returnValue = false;
	switch (imageIO->GetComponentType())
	{
		Qvtk_ITK_TEMPLATE_MACRO(
			switch (imageIO->GetNumberOfComponents()) 
			{
				Qvtk_ITK_TEMPLATE_MACRO2(
					ScalarType,
					return Image::readImage<ScalarType>(paths, image, orientation, position, scale),
					return Image::readImage<PixelType>(paths, image, orientation, position, scale)
					);
			}
		);
	}
	//switch (imageIO->GetNumberOfComponents())
	//{
	//	Qvtk_ITK_TEMPLATE_MACRO2(
	//		ITK_IMAGE_IO_BASE_IO_COMPONENT_TYPE, 
	//			int a = 1;
	//		//switch (imageIO->GetComponentType())
	//		//{
	//		//	Qvtk_ITK_TEMPLATE_MACRO(return readImage<itk::Vector<ITK_IMAGE_IO_BASE_IO_COMPONENT_TYPE, COMPONENT_N>>(
	//		//		paths, image, orientation, position, scale));
	//		//}
	//	);
	//}
	return false;
}
