#ifdef __SCB_EXAMPLE_ITK_FILTER_H__

template<typename ImageType>
SCBExampleITKFilter<ImageType>::SCBExampleITKFilter()
{
    SCB_FILTER_CONSTRUCT_MACRO(vtkPassThroughFilter);

    /* Add 1 to reference cout to prevent it being imediately deleted */
    FilterType::Pointer notsosmartFilterPointer = itk::ThresholdImageFilter<ImageType>::New();
    notsosmartFilterPointer->SetReferenceCount(2); /* Remember to set the reference count! */

    this->m_itkfilter = notsosmartFilterPointer.GetPointer();
    this->m_filterNameString = QString("sCB Example ITK Threshold Filter");
}

template<typename ImageType>
SCBExampleITKFilter<ImageType>::~SCBExampleITKFilter()
{
    this->m_itkfilter->Delete();
}

template<typename ImageType>
int SCBExampleITKFilter<ImageType>::ConnectUI()
{
    try {
        connect(m_ui->radioBtnROutside, SIGNAL(toggled(bool)),
            this, SLOT(slotsFilterSettingsUpdate()));
        connect(m_ui->radioBtnRUpper, SIGNAL(toggled(bool)),
            this, SLOT(slotsFilterSettingsUpdate()));
        connect(m_ui->radioBtnRLower, SIGNAL(toggled(bool)),
            this, SLOT(slotsFilterSettingsUpdate()));

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

template<typename ImageType>
void SCBExampleITKFilter<ImageType>::LoadUI()
{
        /* Setup UI */
        m_ui = new Ui_ExampleITKFilter;
        m_ui->setupUi(this);
}

template<typename ImageType>
typename itk::ThresholdImageFilter<ImageType>* SCBExampleITKFilter<ImageType>::GetITKFilter()
{
    return (itk::ThresholdImageFilter<ImageType>*)(m_itkfilter);
}

template<typename ImageType>
void SCBExampleITKFilter<ImageType>::slotsFilterSettingsUpdate()
{
    if (this->sender() == this->m_ui->doubleSpinBoxLowerValue)
    {
        this->m_ui->doubleSpinBoxUpperValue->setMinimum(
            this->m_ui->doubleSpinBoxLowerValue->value());
        if (this->m_ui->radioBtnROutside->isChecked())
        {
            this->GetITKFilter()->ThresholdOutside(
                this->m_ui->doubleSpinBoxLowerValue->value(),
                this->m_ui->doubleSpinBoxUpperValue->value()
                );
        }
        else
        {
            this->GetITKFilter()->ThresholdBelow(this->m_ui->doubleSpinBoxLowerValue->value());
        }
    }
    else if (this->sender() == this->m_ui->doubleSpinBoxUpperValue)
    {
        this->m_ui->doubleSpinBoxLowerValue->setMaximum(
            this->m_ui->doubleSpinBoxUpperValue->value());
        if (this->m_ui->radioBtnROutside->isChecked())
        {
            this->GetITKFilter()->ThresholdOutside(
                this->m_ui->doubleSpinBoxLowerValue->value(),
                this->m_ui->doubleSpinBoxUpperValue->value()
                );
        }
        else
        {
            this->GetITKFilter()->ThresholdAbove(this->m_ui->doubleSpinBoxUpperValue->value());
        }
    }
    else if (this->sender() == this->m_ui->radioBtnROutside ||
        this->sender() == this->m_ui->radioBtnRUpper ||
        this->sender() == this->m_ui->radioBtnRLower)
    {
        QRadioButton* sender = static_cast<QRadioButton*> (this->sender());
        this->slotsThresholdModeCheck(sender->isChecked());
    }
    this->Update();
}

template<typename ImageType>
void SCBExampleITKFilter<ImageType>::slotsThresholdModeCheck(bool b)
{
    /* Do nothing if its turning off*/
    if (!b)
        return;

    if (this->sender() == this->m_ui->radioBtnRUpper)
    {
        this->m_ui->doubleSpinBoxUpperValue->setDisabled(false);
        this->m_ui->doubleSpinBoxLowerValue->setDisabled(true);
        this->m_ui->radioBtnROutside->setChecked(false);
        this->m_ui->radioBtnRLower->setChecked(false);
        this->GetITKFilter()->ThresholdAbove(this->m_ui->doubleSpinBoxUpperValue->value());
    }
    else if (this->sender() == this->m_ui->radioBtnROutside)
    {
        this->m_ui->doubleSpinBoxUpperValue->setDisabled(false);
        this->m_ui->doubleSpinBoxLowerValue->setDisabled(false);
        this->m_ui->radioBtnRUpper->setChecked(false);
        this->m_ui->radioBtnRLower->setChecked(false);
        this->GetITKFilter()->ThresholdOutside(
            this->m_ui->doubleSpinBoxLowerValue->value(),
            this->m_ui->doubleSpinBoxUpperValue->value()
            );
    }
    else if (this->sender() == this->m_ui->radioBtnRLower)
    {
        this->m_ui->doubleSpinBoxUpperValue->setDisabled(true);
        this->m_ui->doubleSpinBoxLowerValue->setDisabled(false);
        this->m_ui->radioBtnROutside->setChecked(false);
        this->m_ui->radioBtnRUpper->setChecked(false);
        this->GetITKFilter()->ThresholdBelow(this->m_ui->doubleSpinBoxLowerValue->value());
    }
    this->Update();
}

#endif