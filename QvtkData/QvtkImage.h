/**
 * @file	QvtkUtilProject\Project\code\system\core\Image.h.
 *
 * @brief	Declares the Qvtko scene class.
 *
 * Qvtk for QvtkUtil
 */
#ifndef __QVTK_IMAGE_H__
#define __QVTK_IMAGE_H__
#pragma once
#define Qvtk_TEMPLATE_MACRO_CASE(typeN, type, call)     \
  case typeN: { typedef type ScalarType; call; }; break
#define Qvtk_ITK_TEMPLATE_MACRO(call) \
default: \
case itk::ImageIOBase::UNKNOWNCOMPONENTTYPE: \
std::cerr << "Unknown and unsupported component type!" << std::endl; \
Qvtk_TEMPLATE_MACRO_CASE(itk::ImageIOBase::UCHAR, unsigned char, call); \
Qvtk_TEMPLATE_MACRO_CASE(itk::ImageIOBase::CHAR, char, call); \
Qvtk_TEMPLATE_MACRO_CASE(itk::ImageIOBase::USHORT, unsigned short, call); \
Qvtk_TEMPLATE_MACRO_CASE(itk::ImageIOBase::SHORT, short, call);   \
Qvtk_TEMPLATE_MACRO_CASE(itk::ImageIOBase::UINT, unsigned int, call); \
Qvtk_TEMPLATE_MACRO_CASE(itk::ImageIOBase::INT, int, call); \
Qvtk_TEMPLATE_MACRO_CASE(itk::ImageIOBase::ULONG, unsigned long, call); \
Qvtk_TEMPLATE_MACRO_CASE(itk::ImageIOBase::LONG, long, call); \
Qvtk_TEMPLATE_MACRO_CASE(itk::ImageIOBase::FLOAT, float, call); \
Qvtk_TEMPLATE_MACRO_CASE(itk::ImageIOBase::DOUBLE, double, call); 
#define Qvtk_TEMPLATE_MACRO2_CASE(component, type, call) \
case component: { \
constexpr unsigned int COMPONENT = component; \
typedef itk::Vector<type, component> PixelType; \
call; } break
#define Qvtk_ITK_TEMPLATE_MACRO2(type, call1, callN) \
default: \
case 10: \
std::cerr << "Unsupported number of components! Only 1~9 are supported. " << std::endl; break;\
Qvtk_TEMPLATE_MACRO2_CASE(1, type, call1); \
Qvtk_TEMPLATE_MACRO2_CASE(2, type, callN); \
Qvtk_TEMPLATE_MACRO2_CASE(3, type, callN); \
Qvtk_TEMPLATE_MACRO2_CASE(4, type, callN); \
Qvtk_TEMPLATE_MACRO2_CASE(5, type, callN); \
Qvtk_TEMPLATE_MACRO2_CASE(6, type, callN); \
Qvtk_TEMPLATE_MACRO2_CASE(7, type, callN); \
Qvtk_TEMPLATE_MACRO2_CASE(8, type, callN); \
Qvtk_TEMPLATE_MACRO2_CASE(9, type, callN); 
// me
#include "QvtkDataSet.h"

// vtk
class vtkImageData;
class vtkImageReslice;
class vtkImageChangeInformation;

// itk
namespace itk {
	template< typename TPixel, unsigned int VImageDimension > class Image;
	template< typename TParametersValueType, unsigned int NDimensions> class AffineTransform;
}
namespace Q {
namespace vtk{

class QVTKDATA_EXPORT Image: public DataSet
{
	Q_OBJECT;
	Q_VTK_DATA_H(
		Image,
		Q_VTK_KEY(Window)
		Q_VTK_KEY(Level)
	);
public:
	typedef enum IMAGE_SUFFIX
	{
		UNKNOWN = 0,
		VTK = 1,
		VTI = 2,
		NIFTI = 3,
		DICOM = 4
	}IMAGE_SUFFIX;

	//template<typename TParametersValueType, unsigned int NDimensions> 
	typedef itk::AffineTransform<double, 3> AffineTransformType;
	static void vtkMatrix4x4ToitkAffineTransform(AffineTransformType *itkTransform, vtkMatrix4x4 *vtkMatrix);

	template<typename PixelType>
	static bool _VTKImageToITKImage(itk::Image<PixelType, 3>* output, vtkImageData* input, const double orientation[3], const double position[3], const double scale[3], vtkMatrix4x4* userMatrix = nullptr);

	template<typename PixelType>
	static bool _ITKImageToVTKImage(vtkImageData* output, itk::Image<PixelType, 3>* input, double orientation[3], double position[3], double scale[3]);

	static bool readITKImage(QStringList paths, vtkImageData* image, double orientation[3], double position[3], double scale[3]);

	static bool writeITKImage(QStringList paths, vtkImageData* image, const double orientation[3], const double position[3], const double scale[3], vtkMatrix4x4* userMatrix = nullptr);

	Image();

	virtual ~Image() override;

	virtual void printSelf() const override;

	virtual bool readData(QString rootDirectory = QString()) override;

	virtual bool writeData(QString rootDirectory = QString()) const override;

	//virtual void addReference(Prop* prop) override;

	virtual double getWindow() const;
	virtual double getResetWindow() const;
	virtual double getLevel() const;
	virtual double getResetLevel() const;
	virtual vtkAlgorithmOutput* getTransformOutputPort() const;

	virtual vtkImageData* getImageData() const;

	virtual vtkImageData* getTransformImageData() const;

	template<typename PixelType>
	void getITKImageData(itk::Image<PixelType, 3>* itkImage) const;
	template<typename PixelType>
	void setITKImageData(itk::Image<PixelType, 3>* itkImage);
	static bool readDataSuffix(
		QStringList fileNames,
		vtkImageData* data,
		double orientation[3],
		double position[3],
		double scale[3],
		int suffix = -1);

	static bool writeDataSuffix(
		QString fileName,
		vtkImageData* data,
		int suffix = -1);

	static bool writeDataSuffix(
		QStringList fileNames,
		vtkImageData* data,
		double orientation[3],
		double position[3],
		double scale[3],
		int suffix = -1
	);

public slots:

	virtual void setWindow(double window);
	virtual void setLevel(double level);
	virtual void resetWindowLevel();

signals:
	void windowChanged(double window);
	void levelChanged(double level);

protected:

	virtual Data* newInstance() const override { return new Image; }

	static void setWindow(Data* self, QStandardItem* item);

	static void setLevel(Data* self, QStandardItem* item);

	template<typename PixelType>
	static bool writeImage(QStringList paths, vtkImageData* image, const double orientation[3], const double position[3], const double scale[3], vtkMatrix4x4 *userMatrix = nullptr);

	template<typename PixelType>
	static bool readImage(QStringList paths, vtkImageData* image, double orientation[3], double position[3], double scale[3]);

	template<typename OPixelType, typename IPixelType>
	static void _ITKImageCasting(itk::Image<OPixelType, 3>* output, vtkImageData* input);

	static unsigned int suffixTranslate(QString suffix);

	vtkImageReslice* imageReslice;

	vtkImageChangeInformation* imageChangeInformation;

private:

	QStandardItem* window;

	QStandardItem* level;

};

}
}
#endif // !__QVTK_IMAGE_H__
