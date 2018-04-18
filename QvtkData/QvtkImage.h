/**
 * @file	QvtkUtilProject\Project\code\system\core\QvtkImage.h.
 *
 * @brief	Declares the Qvtko scene class.
 *
 * Qvtk for QvtkUtil
 */
#ifndef __Qvtk_IMAGE_H__
#define __Qvtk_IMAGE_H__

#define Qvtk_TEMPLATE_MACRO_CASE(typeN, type, call)     \
  case typeN: { typedef type ITK_IMAGE_IO_BASE_IO_COMPONENT_TYPE; call; }; break
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
Qvtk_TEMPLATE_MACRO_CASE(itk::ImageIOBase::DOUBLE, double, call); \

// me
#include "DataSet.h"

// vtk
class vtkImageData;
class vtkImageReslice;
class vtkImageChangeInformation;

// itk
namespace itk {template< typename TPixel, unsigned int VImageDimension > class Image; }
// qt
class QStandardItem;

class ABSTRACTDATA_EXPORT QvtkImage: public DataSet
{
	Q_OBJECT;
	Q_VTK_DATAH(
		QvtkImage,
		Q_VTK_KEY(Window)
		Q_VTK_KEY(Level)
	);
public:
	enum IMAGE_SUFFIX
	{
		UNKNOWN = 0,
		VTK = 1,
		VTI = 2,
		NIFTI = 3,
		DICOM = 4
	};

	template<typename PixelType>
	static bool _VTKImageToITKImage(itk::Image<PixelType, 3>* output, vtkImageData* input, const double orientation[3], const double position[3], const double scale[3]);

	template<typename PixelType>
	static bool _ITKImageToVTKImage(vtkImageData* output, itk::Image<PixelType, 3>* input, double orientation[3], double position[3], double scale[3]);

	static bool readITKImage(QStringList paths, vtkImageData* image, double orientation[3], double position[3], double scale[3]);

	static bool writeITKImage(QStringList paths, vtkImageData* image, const double orientation[3], const double position[3], const double scale[3]);

	QvtkImage();

	virtual ~QvtkImage() override;

	virtual void printSelf() const override;

	virtual bool readData(QString rootDirectory = QString()) override;

	virtual bool writeData(QString rootDirectory = QString()) const override;

	virtual void addReference(QvtkAbstractProp* prop) override;

	virtual double getWindow() const;

	virtual double getLevel() const;

	virtual vtkAlgorithmOutput* getTransformOutputPort() const;

	virtual vtkImageData* getImageData() const;

	virtual vtkImageData* getTransformImageData() const;

	template<typename PixelType>
	void getITKImageData(itk::Image<PixelType, 3>* itkImage) const;

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

	virtual Data* newInstance() const override { return new QvtkImage; }

	static void setWindow(Data* self, QStandardItem* item);

	static void setLevel(Data* self, QStandardItem* item);

	template<typename PixelType>
	static bool writeImage(QStringList paths, vtkImageData* image, const double orientation[3], const double position[3], const double scale[3]);

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

#endif // !__Qvtk_IMAGE_H__
