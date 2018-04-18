/*!
 * \file Example_Pipeline.h
 * \date 2017/07/20 19:20
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong, 
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief This class is demonstrate how to use SCBAbstractPipeline to create a usable pipeline.
 *
 *
 * \note
*/

#ifndef __SCB_EXAMPLE_PIPELINE_H__
#define __SCB_EXAMPLE_PIPELINE_H__

#include "SCBAbstractPipeline.h"


#include <QVBoxLayout>

class ExamplePDTransfromFilter;
class ExampleConnectivityFilter;

class ExamplePipeline final : public SCBAbstractPipeline
{
    Q_OBJECT
public:
	explicit ExamplePipeline(QWidget* parent = nullptr);
    virtual ~ExamplePipeline() override;

    /**
     * QVBoxLayout* GetLayout
     *
     * @brief 
     *
     * Override this virtual function to allow other class to control the layout
     *
     * @return QT_NAMESPACE::QVBoxLayout*
     */
    QVBoxLayout* GetLayout();

protected:

};

#endif