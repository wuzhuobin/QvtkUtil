// me
#include "QvtkImage.h"
// qt 
#include <QTest>
#include <QDebug>
#include <QFile>
// itk
#include <itkImage.h>
#include <itkAffineTransform.h>
#include <itkMatrix.h>
#include <itkImageFileWriter.h>
// vtk
#include <vtkImageData.h>
#include <vtkTransform.h>
#include <vtkMatrix4x4.h>
#include <vtkSmartPointer.h>
class TestQvtkImage : public QObject
{
	Q_OBJECT;
	static constexpr unsigned int COMPONENT = 3;
	static constexpr unsigned int DIMENSION = 3;
	typedef typename itk::Image<unsigned char, DIMENSION> ImageType;
	typedef typename itk::Vector<unsigned char, COMPONENT> Component3;
	typedef typename itk::Image<Component3, DIMENSION> Component3ImageType;
	typedef typename itk::ImageFileWriter<ImageType> ImageFileWriter;
	typedef typename itk::ImageFileWriter<Component3ImageType> Component3ImageFileWriter;
	private Q_SLOTS:
	void init() 
	{
		// new Q::vtk::Image 
		this->image = new Q::vtk::Image;
	}
	void cleanup() 
	{
		// reomve Q::vtk::Image
		delete this->image;
	}
	void initTestCase()
	{
		// initialize variables.
		double origin[3]{ 1 ,2 ,3 };
		std::copy_n(origin, 3, this->origin);
		double spacing[3]{ 3, 2, 1 };
		std::copy_n(spacing, 3, this->spacing);
		double orientation[3]{ 1, 1, 1 };
		std::copy_n(orientation, 3, this->orientation);
		// creating testITKImage;
		this->testITKImage = ImageType::New();
		ImageType::IndexType index;
		index.Fill(0);
		ImageType::SizeType size;
		size.Fill(100);
		ImageType::RegionType region;
		region.SetIndex(index);
		region.SetSize(size);
		this->testITKImage->SetRegions(region);
		this->testITKImage->Allocate(true);
		this->testITKImage->SetOrigin(origin);
		Q::vtk::Image::AffineTransformType::Pointer affine = Q::vtk::Image::AffineTransformType::New();
		typedef Q::vtk::Image::AffineTransformType::InputVectorType VectorType;
		VectorType yAxis, xAxis, zAxis;
		xAxis[0] = yAxis[1] = zAxis[2] = 1;
		xAxis[1] = yAxis[2] = zAxis[0] = 0;
		xAxis[2] = yAxis[0] = zAxis[1] = 0;
		affine->Rotate3D(yAxis, orientation[0] / itk::Math::deg_per_rad);
		affine->Rotate3D(xAxis, orientation[1] / itk::Math::deg_per_rad);
		affine->Rotate3D(zAxis, orientation[2] / itk::Math::deg_per_rad);
		this->testITKImage->SetDirection(affine->GetMatrix());
		this->testITKImage->SetSpacing(spacing);
		// write image to disk	
		ImageFileWriter::Pointer writer = ImageFileWriter::New();
		writer->SetInput(this->testITKImage);
		writer->SetFileName(DCM_IMAGE.toStdString());
		writer->Write();
		writer->SetFileName(NII_IMAGE.toStdString());
		writer->Write();
		writer->SetFileName(NII_GZ_IMAGE.toStdString());
		writer->Write();
		// create testComponent3ITKImage
		this->testComponent3ITKImage = Component3ImageType::New();
		this->testComponent3ITKImage->SetRegions(region);
		this->testComponent3ITKImage->Allocate(true);
		// write testComponent3ITKImage to disk
		Component3ImageFileWriter::Pointer cWriter = Component3ImageFileWriter::New();
		cWriter->SetInput(this->testComponent3ITKImage);
		cWriter->SetFileName(this->COMPONENT_3_IMAGE.toStdString());
		cWriter->Write();
		// create testVTKImage;
		this->testVTKImage = vtkSmartPointer<vtkImageData>::New();
		this->testVTKImage->SetExtent(
			index[0], size[0], 
			index[1], size[1], 
			index[2], size[2] );
		this->testVTKImage->SetOrigin(origin);
		this->testVTKImage->SetSpacing(spacing);
		this->testVTKImage->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

		this->testComponent3VTKImage = vtkSmartPointer<vtkImageData>::New();
		this->testComponent3VTKImage->SetExtent(
			index[0], size[0], 
			index[1], size[1], 
			index[2], size[2] );
		this->testComponent3VTKImage->SetOrigin(origin);
		this->testComponent3VTKImage->SetSpacing(spacing);
		this->testComponent3VTKImage->AllocateScalars(VTK_UNSIGNED_CHAR, COMPONENT);
	}
	void cleanupTestCase()
	{
		QVERIFY(QFile(DCM_IMAGE).remove());
		QVERIFY(QFile(NII_IMAGE).remove());
		QVERIFY(QFile(NII_GZ_IMAGE).remove());
		QVERIFY(QFile(COMPONENT_3_IMAGE).remove());
	}
	void _VTKImageToITKImage() 
	{
		ImageType::Pointer itkImage = ImageType::New();
		Q::vtk::Image::_VTKImageToITKImage(itkImage.GetPointer(), this->testVTKImage, this->orientation, this->origin, this->spacing);
		vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
		transform->Identity();
		transform->RotateY(this->orientation[1]);
		transform->RotateX(this->orientation[0]);
		transform->RotateZ(this->orientation[2]);
		transform->Update();
		for (int i = 0; i < 3; ++i) {
			QCOMPARE(itkImage->GetSpacing()[i], this->spacing[i]);
			QCOMPARE(itkImage->GetOrigin()[i], this->origin[i]);
			for (int j = 0; j < 3; ++j) {
				QCOMPARE(itkImage->GetDirection()[i][j], transform->GetMatrix()->GetElement(i, j));
			}
		}
		// failed
		//Component3ImageType::Pointer component3ITKImage = Component3ImageType::New();
		//Q::vtk::Image::_VTKImageToITKImage(component3ITKImage.GetPointer(), this->testComponent3VTKImage, this->orientation, this->origin, this->spacing);
		//QCOMPARE(static_cast<unsigned int>(this->testComponent3VTKImage->GetNumberOfScalarComponents()), COMPONENT);
		//QCOMPARE(static_cast<unsigned int>(this->testComponent3VTKImage->GetNumberOfScalarComponents()), component3ITKImage->GetNumberOfComponentsPerPixel());
	}
	void _ITKImageToVTKImage()
	{
		vtkSmartPointer<vtkImageData> vtkImage = vtkSmartPointer<vtkImageData>::New();
		double orientation[3];
		double origin[3];
		double spacing[3];
		Q::vtk::Image::_ITKImageToVTKImage(vtkImage, this->testITKImage.GetPointer(), orientation, origin, spacing);
		for (int i = 0; i < 3; ++i) {
			QCOMPARE(spacing[i], this->spacing[i]);
			QCOMPARE(origin[i], this->origin[i]);
			QCOMPARE(orientation[i], this->orientation[i]);
		}
		Q::vtk::Image::_ITKImageToVTKImage(vtkImage, this->testComponent3ITKImage.GetPointer(), orientation, origin, spacing);
		QCOMPARE(static_cast<unsigned int>(vtkImage->GetNumberOfScalarComponents()), COMPONENT);
		QCOMPARE(static_cast<unsigned int>(vtkImage->GetNumberOfScalarComponents()), this->testComponent3ITKImage->GetNumberOfComponentsPerPixel());
	}
	void readRelativePath_data()
	{
		QTest::addColumn<QString>("fileName");
		QTest::newRow("dcm") << DCM_IMAGE;
		QTest::newRow("nii") << NII_IMAGE;
		QTest::newRow("nii.gz") << NII_GZ_IMAGE;
	}
	void readRelativePath()
	{
		QFETCH(QString, fileName);
		this->image->setRelativePath(fileName);
		QVERIFY(this->image->readData(QDir::currentPath()));
	}
	void readAbsolutePath_data()
	{
		QTest::addColumn<QString>("fileName");
		QTest::newRow("dcm") << DCM_IMAGE;
		QTest::newRow("nii") << NII_IMAGE;
		QTest::newRow("nii.gz") << NII_GZ_IMAGE;
	}
	void readAbsolutePath()
	{
		QFETCH(QString, fileName);
		this->image->setAbsolutePath(QDir::currentPath() + '/' + fileName);
		QVERIFY(this->image->readData());
	}
	void setComponent3ITKImageData()
	{
		this->image->setITKImageData(this->testComponent3ITKImage);
		QCOMPARE(static_cast<unsigned int>(this->image->getImageData()->GetNumberOfScalarComponents()), 
			this->testComponent3ITKImage->GetNumberOfComponentsPerPixel());
		QCOMPARE(COMPONENT, this->testComponent3ITKImage->GetNumberOfComponentsPerPixel());
	}
private:
	ImageType::Pointer testITKImage;
	vtkSmartPointer<vtkImageData> testVTKImage;
	Component3ImageType::Pointer testComponent3ITKImage;
	vtkSmartPointer<vtkImageData> testComponent3VTKImage;
	double origin[3];
	double spacing[3];
	double orientation[3];
	Q::vtk::Image *image;
	const QString DCM_IMAGE = "testImage.dcm";
	const QString NII_IMAGE = "testImage.nii";
	const QString NII_GZ_IMAGE = "testImage.nii.gz";
	const QString COMPONENT_3_IMAGE = "Component3Image.nii.gz";
};
// definition.
constexpr unsigned int TestQvtkImage::COMPONENT;
constexpr unsigned int TestQvtkImage::DIMENSION;
QTEST_APPLESS_MAIN(TestQvtkImage)
#include "TestQvtkImage.moc"