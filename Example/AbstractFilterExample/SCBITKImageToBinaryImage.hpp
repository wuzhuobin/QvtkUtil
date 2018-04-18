#ifdef __SCB_ITK_IMAGE_TO_BINARY_IMAGE_H__

#include "SCBITKImageToBinaryImage.h"
#include <vtkAlgorithmOutput.h>
#include <vtkPassThroughFilter.h>


template <typename TInputScalarType>
SCBITKImageToBinaryImage<TInputScalarType>::~SCBITKImageToBinaryImage()
{

}


template <typename TInputScalarType>
void SCBITKImageToBinaryImage<TInputScalarType>::SetInputData(vtkImageData* im)
{
	if (!this->GetITKFilter())
	{
		qDebug() << "ITK filter not created!";
		return;
	}

	/* Use smart pointer here */
	typedef itk::VTKImageToImageFilter<itk::Image<TInputScalarType, 3>> ConnectorType;
	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(im);
	connector->Update();

	itk::Image<TInputScalarType, 3>::Pointer image = connector->GetOutput();

	this->GetITKFilter()->SetInput(image);
	this->GetITKFilter()->Update();
}

template <typename TInputScalarType>
int SCBITKImageToBinaryImage<TInputScalarType>::ConnectUI()
{
	try {
		connect(m_ui->doubleSpinBoxLowerValue, SIGNAL(valueChanged(double)),
			this, SLOT(slotsFilterSettingsUpdate()));
		connect(m_ui->doubleSpinBoxUpperValue, SIGNAL(valueChanged(double)),
			this, SLOT(slotsFilterSettingsUpdate()));
		return 0;
	}
	catch (...)
	{
		return 1;
	}
}

template <typename TInputScalarType>
void SCBITKImageToBinaryImage<TInputScalarType>::LoadUI()
{
		/* Setup UI */
		m_ui = new Ui_BinaryThresholdFilter;
		m_ui->setupUi(this);
}

template <typename TInputScalarType>
void SCBITKImageToBinaryImage<TInputScalarType>::slotsFilterSettingsUpdate()
{
	if (this->sender() == this->m_ui->doubleSpinBoxLowerValue)
	{
		this->m_ui->doubleSpinBoxUpperValue->setMinimum(
			this->m_ui->doubleSpinBoxLowerValue->value());
		this->GetITKFilter()->SetLowerThreshold(
			this->m_ui->doubleSpinBoxLowerValue->value()
		);
	}
	else if (this->sender() == this->m_ui->doubleSpinBoxUpperValue)
	{
		this->m_ui->doubleSpinBoxLowerValue->setMaximum(
			this->m_ui->doubleSpinBoxUpperValue->value());
		this->GetITKFilter()->SetUpperThreshold(
			this->m_ui->doubleSpinBoxUpperValue->value()
		);
	}

	this->Update();
}

template <typename TInputScalarType>
typename vtkAlgorithmOutput* SCBITKImageToBinaryImage<TInputScalarType>::
ConvertOutputToVTKPort(itk::Image<unsigned char, 3>::Pointer outputPtr)
{
	vtkImageData* im = vtkImageData::SafeDownCast(this->ConvertOutputToVTKObject(outputPtr));

	this->GetFilter()->SetInputData(im);
	this->GetFilter()->Update();
	return this->GetFilter()->GetOutputPort();
}

template <typename TInputScalarType>
typename vtkDataObject* SCBITKImageToBinaryImage<TInputScalarType>::
ConvertOutputToVTKObject(itk::Image<unsigned char, 3>::Pointer outputPtr)
{
	this->m_outConnector->SetInput(this->GetITKFilter()->GetOutput());
	this->m_outConnector->Update();

	return this->m_outConnector->GetOutput();
}

template <typename TInputScalarType>
typename itk::Image<TInputScalarType, 3>::Pointer SCBITKImageToBinaryImage<TInputScalarType>::ConvertInputType(vtkAlgorithmOutput* input)
{
	vtkImageData* im
		= vtkImageData::SafeDownCast(input->GetProducer()->GetOutputDataObject(input->GetIndex()));

	this->m_inConnector->SetInput(im);
	this->m_inConnector->Update();

	return this->m_inConnector->GetOutput();
}

template <typename TInputScalarType>
SCBITKImageToBinaryImage<TInputScalarType>::SCBITKImageToBinaryImage()
{
	SCB_FILTER_CONSTRUCT_MACRO(vtkPassThroughFilter);


	FilterType::Pointer tempFilter = FilterType::New();
	tempFilter->SetReferenceCount(2);
	tempFilter->SetInsideValue(255);
	tempFilter->SetOutsideValue(0);

	this->m_itkfilter = tempFilter.GetPointer();


	this->m_inConnector = InConnectorType::New();
	this->m_outConnector = OutConnectorType::New();
}

template <typename TInputScalarType>
typename SCBITKImageToBinaryImage<TInputScalarType>::FilterType*
SCBITKImageToBinaryImage<TInputScalarType>::GetITKFilter()
{
	return (FilterType*)(this->m_itkfilter);
}

#endif 