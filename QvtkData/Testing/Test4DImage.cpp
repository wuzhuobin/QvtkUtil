// qt
#include <QTest>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
// itk 
#include <itkVectorImage.h>
#include <itkImageSeriesReader.h>
#include <itkImageFileWriter.h>
#include <itkImageIOBase.h>
#include <itkGDCMSeriesFileNames.h>
class Test4DImage: public QObject
{
	Q_OBJECT;
private Q_SLOTS:
	void initTestCase(){}
	void cleanupTestCase() {}
	void init() {}
	void cleanup() {}
	void test() {
		QDir folder("C:/Users/jieji/Desktop/T2_RTHANDMOTOR_BOLD/RTHANDMOTOR");
		QFileInfoList list = folder.entryInfoList(QDir::Files);
		std::vector<std::string> fileNames;
		for (QFileInfo info : list) {
			qDebug() << info.absoluteFilePath();
			fileNames.push_back(info.absoluteFilePath().toStdString());
		}

		itk::GDCMSeriesFileNames::Pointer nameGenerator = 
			itk::GDCMSeriesFileNames::New();
		nameGenerator->SetUseSeriesDetails(true);
		nameGenerator->SetDirectory(folder.absolutePath().toStdString());
		nameGenerator->GetSeriesHelper();
		//typedef itk::Image<itk::Vector<short>, 3> Image4DType;
		//typedef itk::VectorImage<short, 3> Image4DType;
		//typedef itk::ImageSeriesReader<Image4DType> Image4DSeriesReader;
		//auto image4DSeriesReader = Image4DSeriesReader::New();
		//image4DSeriesReader->SetFileNames(fileNames);
		//image4DSeriesReader->Update();

		//typedef itk::ImageFileWriter<Image4DType> Image4DWriter;
		//auto image4DWriter = Image4DWriter::New();
		//image4DWriter->SetInput(image4DSeriesReader->GetOutput());
		//image4DWriter->SetFileName("4dImage.nii.gz");
		//image4DWriter->Write();
	}
};
QTEST_APPLESS_MAIN(Test4DImage);
#include "Test4DImage.moc"