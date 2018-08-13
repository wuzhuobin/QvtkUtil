// me
#include "QvtkImage.h"
// qt 
#include <QTest>
#include <QDebug>
#include <QFile>
// itk
#include <itkImage.h>
#include <itkImageFileWriter.h>
class TestQvtkImage : public QObject
{
	Q_OBJECT;
	typedef typename itk::Image<unsigned char, 3> ImageType;
	typedef typename itk::ImageFileWriter<ImageType> ImageFileWriter;
	private Q_SLOTS:
	void init() 
	{
		this->image = new Q::vtk::Image;
	}

	void cleanup() 
	{
		delete this->image;
	}
	void initTestCase()
	{
		this->testImage = ImageType::New();
		ImageType::IndexType index;
		index.Fill(0);
		ImageType::SizeType size;
		size.Fill(100);
		ImageType::RegionType region;
		region.SetIndex(index);
		region.SetSize(size);
		this->testImage->SetRegions(region);
		this->testImage->Allocate(true);

		ImageFileWriter::Pointer writer = ImageFileWriter::New();
		writer->SetInput(this->testImage);
		writer->SetFileName(DCM_IMAGE.toStdString());
		writer->Write();
		writer->SetFileName(NII_IMAGE.toStdString());
		writer->Write();
		writer->SetFileName(NII_GZ_IMAGE.toStdString());
		writer->Write();
	}
	void cleanupTestCase()
	{
		QVERIFY(QFile(DCM_IMAGE).remove());
		QVERIFY(QFile(NII_IMAGE).remove());
		QVERIFY(QFile(NII_GZ_IMAGE).remove());
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
private:
	ImageType::Pointer testImage;
	Q::vtk::Image *image;
	const QString DCM_IMAGE = "testImage.dcm";
	const QString NII_IMAGE = "testImage.nii";
	const QString NII_GZ_IMAGE = "testImage.nii.gz";
};
QTEST_APPLESS_MAIN(TestQvtkImage)
#include "TestQvtkImage.moc"