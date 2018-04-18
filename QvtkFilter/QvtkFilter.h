/*!
 * \file QvtkAbstractFilter.h
 * \date 2017/07/04 14:05
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong, 
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief	Base class for filters AbstractFilter
 *
 * TODO: This class should be inherited by all filters when using this system.
 *		 Devs should not inherit it directly, instead, inherit its subclass, namely,
 *		 AbstractImageFilter and AbstractPolyDataFilter based on the output type of 
 *		 the filter.
 *		 
 *		 The following function must be inherited in subclasses: 
 *		 - void LoadUI()
 *		 - void ConnectUI()
 *		 - virtual QPointer<QLayout> GetUI()
 *
 * \note
*/


#ifndef __Qvtk_ABSTRACT_FILTER_H__
#define __Qvtk_ABSTRACT_FILTER_H__

#include "abstractfilter_export.h"

// qt
#include <QWidget>
#include <QFuture>
	
// vtk
class vtkAlgorithm;
class vtkAlgorithmOutput;

// Error message of handled error
#define ERRMSG_ABSTRACT_FILTER_NULL_VTK_FILTER	"Variabel vtkfilter points to null/nullptr."
#define ERRMSG_ABSTRACT_FILTER_NUM_OF_IO		"Number of inputs/outputs are incorrect."

// Define macro for subclass constructor call3t
#define Qvtk_FILTER_CONSTRUCT_MACRO(SUB_CLASS_FILTER_NAME) \
	this->vtkfilter = SUB_CLASS_FILTER_NAME::New(); \
	LoadUI(); \
	ConnectUI();

class ABSTRACTFILTER_EXPORT QvtkAbstractFilter : public QWidget
{
	Q_OBJECT;
public:
    explicit QvtkAbstractFilter(QWidget* parent = nullptr);
    virtual ~QvtkAbstractFilter() override;

    /**
     * QString GetFilterName
     *
     * @brief 
     *	if (this->GetWidget()) {
		delete this->GetWidget();
		this->widget = nullptr;
	}
     * This method will be used in BuildUI() function of pipeline when using 
     * some of the layouts such as the StackWidgetLayout
     *
     * @return QT_NAMESPACE::QString
     */
    virtual QString GetFilterName();

	
	/**
	 * @brief	Convenient method to get filter
	 * @return	#vtkFilter
	 */
	 vtkAlgorithm* GetFilter();

	/*
	 Update functions, should be straight forward
	*/
	virtual QFuture<int*> ThreadedUpdate();
	virtual void Update();


	/*
	 Wrap the vtk algorithm interface
	*/
	virtual int GetNumberOfInputs();
	virtual int GetNumberOfOutputs();
	virtual vtkAlgorithmOutput* GetOutputPort(int port=0);
	virtual void SetInputConnection(vtkAlgorithmOutput* input, int port = 0);
	
	/**
	 * @brief PreConnectFilter
	 *
	 * Connect vtkFilter of preFilter to this object
	 *
	 * @param preFilter QvtkAbstractFilter to connect to this object
	 * @param prePort	Output port of the preFilter 
	 * @param thisPort  Input port of this filter
	 * @return void
	 */
	virtual void PreConnectFilter(QvtkAbstractFilter* preFilter, int prePort=0, int thisPort=0);
	
	/**
	 * @brief PostConnectFilter
	 *
	 * Connect vtkFilter of this object to postFilter
	 *
	 * @param postFilter Connect this filter to another QvtkAbstractFilter
	 * @param postPort   Input port of the postFilter
	 * @param thisPort   Output port of this filter
	 * @return void
	 */
	virtual void PostConnectFilter(QvtkAbstractFilter* postFilter, int postPort=0, int thisPort=0);

	/*
	 Points to print function of the member vtkfilter
	*/
	void Print(std::ostream& stream);

	/*
	 Serve as an error reference.
	*/
	enum ErrorCode {
		NORMAL = 0,
		ERROR_NULL_VTK_FILTER = 1,
		ERROR_INCORRECT_NUM_OF_INPUTS = 2,
		ERROR_INCORRECT_NUM_OF_OUTPUTS = 3
	};

signals:
	void signalFilterModified();

public slots:

protected:

	/**
	 * @brief LoadUI
	 *
	 * Developer should either load a ui from file or dynamically creates an 
	 * UI with this function
	 *
	 * @return void
	 */

	virtual void LoadUI() = 0;

	/**
	 * @brief ConnectUI
	 *
	 * This virtual function must be inherited
	 * This functio should be called when this object is focused
	 *
	 * @return int
	 */

	virtual int ConnectUI() = 0;

	/**
	 * bool ErrorCheck
	 *
	 * @brief 
	 *
	 * Perform logic checking, 0 if pass, 1 if failed
	 *
	 * @return bool
	 */
	virtual bool ErrorCheck();



    QvtkAbstractFilter* prefilter; // if prefilter was called this will have value
    QvtkAbstractFilter* postfilter; // if postfilter was called this will have value

	vtkAlgorithm* vtkfilter; // Declare and cast this in the child class

    /* Set this in subclass to extend the allowed input/output number */
    int maxNumOfInput;
    int maxNumOfOutput;

    QString filterNameString; // Change this for automatic windows/tab naming
};


#endif
