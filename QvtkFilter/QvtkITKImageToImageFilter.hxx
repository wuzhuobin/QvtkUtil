/*!
* \file QvtkAbstractITKImageFilter.h
* \date 2017/07/25 14:42
*
* \author 		Wong, Matthew Lun
* Occupation	Chinese University of Hong Kong,
* 				Department of Imaging and Interventional Radiology,
* 				M.Phil Student
* Contact: 	fromosia@gmail.com
*
* \brief This class wraps an itk pipeline into the structure. A vtk input is expected.
*
* TODO: long description
*
* \note
*/

#ifdef __Qvtk_ABSTRACT_ITK_IMAGE_TO_IMAGE_FILTER_H__

template <typename TInputImageType, typename TOutputImageType>
void QvtkAbstractITKImageToImageFilter<TInputImageType, TOutputImageType>::Update()
{
    this->GetITKFilter()->Update();
    QvtkAbstractITKFilter::Update();
}

template <typename TInputImageType, typename TOutputImageType>
void QvtkAbstractITKImageToImageFilter<TInputImageType, TOutputImageType>::SetInputData(vtkImageData* input)
{
    /* cast the input into vtkimagedata, then convert to itkimage */
    typedef itk::VTKImageToImageFilter<TInputImageType> InputConnectorType;
    InputConnectorType::Pointer connector = InputConnectorType::New();
    connector->SetInput(input);
    connector->Update();

    /* Set the converted image into the itk filter */
    if (this->GetITKFilter())
        this->GetITKFilter()->SetInput(connector->GetOutput());
}

template <typename TInputImageType, typename TOutputImageType>
typename QvtkAbstractITKImageToImageFilter<TInputImageType, TOutputImageType>::FilterType*
QvtkAbstractITKImageToImageFilter<TInputImageType, TOutputImageType>::GetITKFilter()
{
    return (FilterType*)(this->itkfilter);
}


template <typename TInputImageType, typename TOutputImageType>
typename TOutputImageType::Pointer QvtkAbstractITKImageToImageFilter<TInputImageType, TOutputImageType>::GetITKOutputData()
{
    return this->GetITKFilter()->GetOutput();
}

template <typename TInputImageType, typename TOutputImageType>
typename vtkImageData* QvtkAbstractITKImageToImageFilter<TInputImageType, TOutputImageType>::GetVTKOutputData()
{
    if (!ErrorCheck())
    {
        return this->outputConnector->GetOutput();
    }
    else
    {
        return nullptr;
    }
}

template <typename TInputImageType, typename TOutputImageType>
void QvtkAbstractITKImageToImageFilter<TInputImageType, TOutputImageType>::PreConnectFilter(QvtkAbstractFilter* preFilter, int prePort /*= 0*/, int thisPort /*= 0*/)
{
    try {
        this->ErrorCheck();
    }
    catch(std::runtime_error &e) /* Allows lack of input */
    {
        /* Check if prefilter is an ITK Filter */
        QvtkAbstractITKFilter* castedPostFilter
            = dynamic_cast<QvtkAbstractITKFilter*>(preFilter);
        if (NULL != castedPostFilter)
        {
            /* Further cast the itkfilter to itk::ImageSource */
            itk::ImageSource<TInputImageType>* imSource
                = dynamic_cast<itk::ImageSource<TInputImageType>*>(castedPostFilter->GetITKFilter());
            if (!imSource)
            {
                qCritical() << "Input's itk filter cannot be casted to image source.";
                return;
            }
            else {
                qDebug() << "Type cast success. Treating it as ITK filter.";
                this->GetITKFilter()->SetInput(thisPort, imSource->GetOutput());
                this->prefilter = preFilter;
            }

        }
        else {
            this->SetInputConnection(preFilter->GetOutputPort(prePort), thisPort);
            this->prefilter = preFilter;
        }
    }
    catch (...)
    {
        qDebug() << "[Error] Error check failed!";
        return;
    }
}

template <typename TInputImageType, typename TOutputImageType>
QvtkAbstractITKImageToImageFilter<TInputImageType, TOutputImageType>::~QvtkAbstractITKImageToImageFilter()
{
}


template <typename TInputImageType, typename TOutputImageType>
QvtkAbstractITKImageToImageFilter<TInputImageType, TOutputImageType>::QvtkAbstractITKImageToImageFilter()
{
    this->outputConnector = OutputConnectorType::New();
    this->inputConnector = InputConnectorType::New();
}

template <typename TInputImageType, typename TOutputImageType>
vtkAlgorithmOutput* QvtkAbstractITKImageToImageFilter<TInputImageType, TOutputImageType>::GetOutputPort(int port /*= 0*/)
{
    if (!this->GetITKFilter())
    {
        qCritical() << "Filter not created!";
        return nullptr;
    }
    OutputConnectorType::Pointer connector = OutputConnectorType::New();
    this->outputConnector->SetInput(this->GetITKFilter()->GetOutput());
    this->outputConnector->Update();

    this->GetFilter()->SetInputData(this->outputConnector->GetOutput());
    this->GetFilter()->Update();

    return this->GetFilter()->GetOutputPort();
}

template <typename TInputImageType, typename TOutputImageType>
void QvtkAbstractITKImageToImageFilter<TInputImageType, TOutputImageType>::SetInputConnection(vtkAlgorithmOutput* input, int port /*= 0*/)
{
    /* cast the input into vtkimagedata, then convert to itkimage */
    this->inputConnector->
        SetInput(vtkImageData::SafeDownCast(input->GetProducer()->GetOutputDataObject(input->GetIndex())));
    this->inputConnector->Update();

    /* Set the converted image into the itk filter */
    if (this->GetITKFilter())
        this->GetITKFilter()->SetInput(this->inputConnector->GetOutput());
}


template <typename TInputImageType, typename TOutputImageType>
bool QvtkAbstractITKImageToImageFilter<TInputImageType, TOutputImageType>::ErrorCheck()
{
    if (!this->GetITKFilter())
    {
        QString errmsg("[Error] " + this->GetFilterName() + " ITK filter is not created!");
        throw std::logic_error(errmsg.toStdString());
        qCritical() << errmsg;
        return true;
    }

    if (!this->GetFilter())
    {
        QString errmsg("[Error] " + this->GetFilterName() + " VTK filter is not created!");
        throw std::logic_error(errmsg.toStdString());
        qCritical() << errmsg;
        return true;
    }

    if (!this->GetFilter()->GetInput())
    {
        QString errmsg("[Warning!] " + this->GetFilterName() + " VTK filter has no input!");
        throw std::runtime_error(errmsg.toStdString());
        qWarning() << errmsg;
        return true;
    }

    return false;
}

#endif