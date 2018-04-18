#ifdef __Qvtk_ABSTRACT_ITK_GENERAL_FILTER_H__


template<typename TInputType, typename TOutputType, typename TOutputFilterType>
void QvtkAbstractITKGeneralFilter<TInputType, TOutputType, TOutputFilterType>::Update()
{
	if (!this->ErrorCheck())
	{
		this->UpdateInputPort();
		this->GetITKFilter()->Update();
		this->UpdateOutputPort();
		emit signalFilterModified();
	}
}

template<typename TInputType, typename TOutputType, typename TOutputFilterType>
void QvtkAbstractITKGeneralFilter<TInputType, TOutputType, TOutputFilterType>::UpdateOutputPort()
{
	if (!this->ErrorCheck())
	{
		this->GetFilter()->SetInputData(this->ConvertOutputToVTKObject(this->GetITKFilter()->GetOutput()));
		this->GetFilter()->Update();
	}
}

template<typename TInputType, typename TOutputType, typename TOutputFilterType>
void QvtkAbstractITKGeneralFilter<TInputType, TOutputType, TOutputFilterType>::UpdateInputPort()
{
	if (!this->ErrorCheck())
	{
		if (!this->prefilter)
			return;

		TInputType::Pointer dataobj = this->ConvertInputType(this->prefilter->GetOutputPort());

		this->GetITKFilter()->SetInput(dataobj);
	}
}

template<typename TInputType, typename TOutputType, typename TOutputFilterType>
void QvtkAbstractITKGeneralFilter<TInputType, TOutputType, TOutputFilterType>::PreConnectFilter(QvtkAbstractFilter* preFilter, int prePort /*= 0*/, int thisPort /*= 0*/)
{
	if (this->GetITKFilter())
	{
		this->SetInputConnection(preFilter->GetOutputPort(prePort), thisPort);
		this->prefilter = preFilter;
	}
}

template<typename TInputType, typename TOutputType, typename TOutputFilterType>
vtkDataObject* QvtkAbstractITKGeneralFilter<TInputType, TOutputType, TOutputFilterType>::
GetVTKOutputData(int port)
{
	return this->ConvertOutputToVTKObject(this->GetITKFilter()->GetOutput(port));
}

template<typename TInputType, typename TOutputType, typename TOutputFilterType>
typename vtkAlgorithmOutput* QvtkAbstractITKGeneralFilter<TInputType, TOutputType, TOutputFilterType>::GetOutputPort(int port/* =0 */)
{
	if (!this->ErrorCheck())
	{
		return this->ConvertOutputToVTKPort(this->GetITKFilter()->GetOutput(port));
	}
}

template<typename TInputType, typename TOutputType, typename TOutputFilterType>
void QvtkAbstractITKGeneralFilter<TInputType, TOutputType, TOutputFilterType>::
	SetInputConnection(vtkAlgorithmOutput* input, int port /*= 0*/)
{
	if (!this->ErrorCheck())
	{
		TInputType::Pointer inputitk = this->ConvertInputType(input);
		this->GetITKFilter()->SetInput(inputitk);
	}
}

template<typename TInputType, typename TOutputType, typename TOutputFilterType>
QvtkAbstractITKGeneralFilter<TInputType, TOutputType, TOutputFilterType>::QvtkAbstractITKGeneralFilter()
{
	TOutputFilterType::Pointer itkfilterSmart = TOutputFilterType::New();
	itkfilterSmart->SetReferenceCount(2);

	this->itkfilter = itkfilterSmart.GetPointer();
}


template<typename TInputType, typename TOutputType, typename TOutputFilterType>
QvtkAbstractITKGeneralFilter<TInputType, TOutputType, TOutputFilterType>::~QvtkAbstractITKGeneralFilter()
{

}


#endif