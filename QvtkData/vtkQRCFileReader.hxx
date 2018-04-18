#ifndef __VTK_QRC_FILE_READER_HXX__
#define __VTK_QRC_FILE_READER_HXX__

// vtk
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkErrorCode.h>
#include <vtkDataObject.h>

// qt
#include <QFile>
#include "vtkQRCFileReader.h"

template<typename vtkReaderType>
vtkQRCFileReader<vtkReaderType>::vtkQRCFileReader()
{
	this->FileName = nullptr;
	this->Reader = vtkReaderType::New();
	this->SetNumberOfInputPorts(0);
}

template<typename vtkReaderType>
vtkQRCFileReader<vtkReaderType>::~vtkQRCFileReader()
{
	this->Reader->Delete();
}

template<typename vtkReaderType>
void vtkQRCFileReader<vtkReaderType>::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);
	os << indent << "Reader: ";
	if (this->Reader)
	{
		this->Reader->PrintSelf(os << endl, indent.GetNextIndent());
	}
	else
	{
		os << "(none)\n";
	}
}

template<typename vtkReaderType>
int vtkQRCFileReader<vtkReaderType>::RequestDataObject(vtkInformation* request,
	vtkInformationVector** inputVector,
	vtkInformationVector* outputVector) {

	return 1;

}

template<typename vtkReaderType>
int vtkQRCFileReader<vtkReaderType>::RequestData(vtkInformation* request,
	vtkInformationVector **inputVector,
	vtkInformationVector *outputVector) {
	
	vtkInformation *outInfo = outputVector->GetInformationObject(0);
	vtkDataObject *output = outInfo->Get(vtkDataObject::DATA_OBJECT());


	if (!this->FileName || *this->FileName == 0)
	{
		vtkErrorMacro(<< "A FileName must be specified.");
		this->SetErrorCode(vtkErrorCode::NoFileNameError);
		return 0;
	}

	QFile tmpSource("vtkQRCFileReaderTMP");
	// try to rmove the file if it has already been existed. 
	if (tmpSource.exists()) {
		vtkWarningMacro(<< tmpSource.fileName().toStdString() << "has already existed. \n" <<
			"The temporary file cannot be created and try to delete it .");
		if (tmpSource.setPermissions(QFileDevice::WriteOwner) && tmpSource.remove()) {
			vtkErrorMacro(<< "Cannot remove temporary file. " << tmpSource.fileName().toStdString() <<
				"need to be removed manually. ");
			this->SetErrorCode(vtkErrorCode::NoFileNameError);
			return 0;
		}
	}
	// try to write a file from qrc to the computer ROM.
	if (!QFile::copy(this->FileName, tmpSource.fileName())) {
		vtkErrorMacro(<< "Cannot create temporary file " << this->FileName <<
			".");
		this->SetErrorCode(vtkErrorCode::NoFileNameError);
		return 0;
	}

	tmpSource.setPermissions(QFileDevice::WriteOwner);
	this->Reader->SetFileName(tmpSource.fileName().toStdString().c_str());
	this->Reader->Update();

	if (!tmpSource.remove()) {
		vtkWarningMacro(<< "Cannot remove temporary file " << this->FileName << ".\n" << 
			"file" << tmpSource.fileName().toStdString() << "need to be removed manually.");
	}
	output->ShallowCopy(this->Reader->GetOutput());


	return 1;

}

template<typename vtkReaderType>
int vtkQRCFileReader<vtkReaderType>::FillOutputPortInformation(int port, vtkInformation* info)
{

	vtkInformation* outInfo = this->Reader->GetOutputPortInformation(port);
	info->Set(vtkDataObject::DATA_TYPE_NAME(), outInfo->Get(vtkDataObject::DATA_TYPE_NAME()));
	return 1;
}

template<typename vtkReaderType>
int vtkQRCFileReader<vtkReaderType>::FillInputPortInformation(int port, vtkInformation* info)
{
	//vtkInformation* inInfo = this->Reader->GetInputInformation(port, 0);
	//info->Set(vtkDataObject::DATA_TYPE_NAME(), inInfo->Get(vtkDataObject::DATA_TYPE_NAME()));
	return 1;
}





#endif // !__VTK_QRC_FILE_READER_HXX__
