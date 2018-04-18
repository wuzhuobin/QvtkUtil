#ifndef __Qvtk_ABSTRACT_ITK_FILTER_H__
#define __Qvtk_ABSTRACT_ITK_FILTER_H__


#include "QvtkAbstractFilter.h"

// vtk 
class vtkPassThroughFilter;

// itk
namespace itk { class ProcessObject; }

class ABSTRACTFILTER_EXPORT QvtkAbstractITKFilter : public QvtkAbstractFilter
{
    Q_OBJECT
public:
	explicit QvtkAbstractITKFilter(QWidget* parent = nullptr);
	virtual ~QvtkAbstractITKFilter() override;

    /**
     * itk::ProcessObject::Pointer GetITKFilter
     *
     * @brief 
     *
     * Implement this method in derive class
     *
     * @return itk::ProcessObject::Pointer
     */
    virtual itk::ProcessObject* GetITKFilter() = 0;

    /**
     * void PostConnectFilter
     *
     * @brief 
     *
     * >>> DO NOT USE THIS FUNCTION! <<<
     * Because its impossible to predict the output type of postFilter, this function will not be allowed to use. 
     * To connect the filter, use PreConnectFilter instead. 
     *
     * @param QvtkAbstractFilter * postFilter 
     * @param int postPort 
     * @param int thisPort 
     * @return void
     */
    virtual void PostConnectFilter(QvtkAbstractFilter* postFilter, int postPort = 0, int thisPort = 0);


    /**
    * vtkPassThroughFilter* GetFilter
    *
    * @brief
    *
    * Return a dummy vtkPassthroughFilter
    *
    * @return vtkPassThroughFilter*
    */
    vtkPassThroughFilter* GetFilter();

public slots:
    virtual void slotsFilterSettingsUpdate() = 0;

protected:

    QList<std::pair<int, int>> prePortConnection;

    itk::ProcessObject* itkfilter;
};

#endif