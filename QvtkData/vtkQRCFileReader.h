#ifndef __VTK_QRC_FILE_READER_H__
#define __VTK_QRC_FILE_READER_H__

// vtk
#include <vtkDataSetAlgorithm.h>
class vtkInformation;
class vtkInformationVector;

template<typename vtkReaderType>
class vtkQRCFileReader : public vtkDataSetAlgorithm
{
public:
	static vtkQRCFileReader* New() { return new vtkQRCFileReader; }
	vtkTypeMacro(vtkQRCFileReader, vtkDataSetAlgorithm);
	virtual void PrintSelf(ostream& os, vtkIndent indent) VTK_OVERRIDE;

	vtkGetObjectMacro(Reader, vtkReaderType);

	vtkSetStringMacro(FileName);
	vtkGetStringMacro(FileName);

protected:
	vtkQRCFileReader();
	virtual ~vtkQRCFileReader() VTK_OVERRIDE;


	virtual int RequestDataObject(vtkInformation* request,
		vtkInformationVector** inputVector,
		vtkInformationVector* outputVector) VTK_OVERRIDE;

	virtual int RequestData(vtkInformation* request,
		vtkInformationVector **inputVector,
		vtkInformationVector *outputVector) VTK_OVERRIDE;

	virtual int FillOutputPortInformation(int port, vtkInformation* info) VTK_OVERRIDE;

	virtual int FillInputPortInformation(int port, vtkInformation* info) VTK_OVERRIDE;

	vtkReaderType *Reader;

	char *FileName;

private:
	vtkQRCFileReader(const vtkQRCFileReader&) VTK_DELETE_FUNCTION;
	void operator=(const vtkQRCFileReader&) VTK_DELETE_FUNCTION;
};

// Implementation
#include "vtkQRCFileReader.hxx"

#endif // !__VTK_QRC_FILE_READER_H__
