#ifndef __SCB_ITK_TYPE_CAST_FILTER_H__
#define __SCB_ITK_TYPE_CAST_FILTER_H__

#include "SCBAbstractITKImageToImageFilter.h"


#include <itkCastImageFilter.h>

template<typename TInputImageType, typename TOutputImageType>
class SCBITKTypeCastFilter :
	public SCBAbstractITKImageToImageFilter<TInputImageType, TOutputImageType>
{
public:
	SCBITKTypeCastFilter();
	~SCBITKTypeCastFilter();

	typedef itk::CastImageFilter<TInputImageType, TOutputImageType> FilterType;

	/* No UI for this filter */
	virtual void LoadUI();
	virtual int ConnectUI();

	virtual typename FilterType* GetITKFilter();

	virtual void Update();

	virtual void slotsFilterSettingsUpdate() {};

protected:

};

template<typename TInputImageType, typename TOutputImageType>
SCBITKTypeCastFilter<TInputImageType, TOutputImageType>::SCBITKTypeCastFilter()
{
	FilterType::Pointer newptr = FilterType::New();
	newptr->SetReferenceCount(2);

	this->m_itkfilter = newptr.GetPointer();
	this->m_filterNameString = QString("SCB ITK Type Cast Filter");
}

template<typename TInputImageType, typename TOutputImageType>
int SCBITKTypeCastFilter<TInputImageType, TOutputImageType>::ConnectUI()
{
	return 0;
}

template<typename TInputImageType, typename TOutputImageType>
void SCBITKTypeCastFilter<TInputImageType, TOutputImageType>::LoadUI()
{
}

template<typename TInputImageType, typename TOutputImageType>
void SCBITKTypeCastFilter<TInputImageType, TOutputImageType>::Update()
{
	if (!this->ErrorCheck())
	{
		this->GetITKFilter()->Update();
	}
}

template<typename TInputImageType, typename TOutputImageType>
typename itk::CastImageFilter<TInputImageType, TOutputImageType>*
SCBITKTypeCastFilter<TInputImageType, TOutputImageType>::GetITKFilter()
{
	return (FilterType*)(this->m_itkfilter);
}

template<typename TInputImageType, typename TOutputImageType>
SCBITKTypeCastFilter<TInputImageType, TOutputImageType>::~SCBITKTypeCastFilter()
{

}

#endif