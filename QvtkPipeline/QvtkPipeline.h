/*!
 * \file QvtkAbstractPipeline.h
 * \date 2017/07/17 18:40
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong, 
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief This class collects multiple QvtkAbstractFilter subclasses, put them together and
 *        assembles their UI to form a panel like interface which allows interaction with 
 *        the filter components. Support dynamic pipeline creation, but requires a concrete
 *        implementation.
 *
 *
 * \note  Note that it rest on YOU to make sure that the connections between filters are correct
 *        that the type matches. You should downcast the output yourself.
*/

#ifndef __Qvtk_ABSTRACT_PIPELINE_H__
#define __Qvtk_ABSTRACT_PIPELINE_H__

#include "QvtkAbstractFilter.h"

#include "abstractpipeline_export.h"

#include <QWidget>

class vtkDataObject;


class ABSTRACTPIPELINE_EXPORT QvtkAbstractPipeline : public QWidget
{
    Q_OBJECT
public:
    explicit QvtkAbstractPipeline(QWidget* parent=nullptr);
    virtual ~QvtkAbstractPipeline() override;


    struct FilterConenction
    {
        int preFilterIndex          = -1;
        int postFilterIndex         = -1;
        int preFilterOutputPort     = 0;
        int postFilterInputPort     = 0;
    };

    enum Qvtk_PIPELINE_LAYOUT
    {
        VBOX_LAYOUT           = 0,
        HBOX_LAYOUT           = 1,
        GRID_LAYOUT           = 2,
        FORM_LAYOUT           = 3,
        STACK_LAYOUT          = 4,
        STACKED_WIDGET_LAYOUT = 5,
        TOOLBOX_LAYOUT        = 6,
        TABWIDGET_LAYOUT      = 7
    } pipelineLayout;
    
    /**
     * void RemoveFilter
     *
     * @brief 
     *
     * Remove a filter from the pipeline. BuildUI() will be called after
     * the filter is removed. Neighbor filter will then be connected.
     *
     * @see BuildUI()
     * @param QvtkAbstractFilter * filter Filter to be removed
     * @return void
     */
    void RemoveFilter(QvtkAbstractFilter* filter);

    /**
     * void RemoveFilter
     *
     * @brief 
     *
     * Remove a filter from the pipeline by its index. 
     *
     * @see BuildUI()
     * @param int index Index of the filter in the filter list to be removed.
     * @return void
     */
    void RemoveFilter(int index);


    /**
     * void AddFilter
     *
     * @brief 
     *
     * Append a filter to the filter list. Adding same filter twice is not allowed. Note
     * that no filter connection is built with this function, use 
     *
     * @param QvtkAbstractFilter * filter  Filter to be appended.
     * @param bool withUI  True if you want the UI of the input filter to appear in the pipeline.
     * @return void
     */
    void AddFilter(QvtkAbstractFilter *filter, bool withUI = true);

    /**
     * void ConnectAllFilters
     *
     * @brief 
     *
     * Connect all filter in the list filterList, assume all filters has single port IO.
     *
     * @return void
     */
    void ConnectAllFilters();

    /**
     * void ClearConnections
     *
     * @brief 
     *
     * Clear the connection map.
     *
     * @return void
     */
    void ClearConnections();
    
    /**
     * void ConnectFilters
     *
     * @brief 
     *
     * Connects filters' input and output
     *
     * @param int filter1Index      Index of the prefilter for output
     * @param int filter2Index      Index of the postfilter for input
     * @param int filter1Outputport Port number of the prefilter
     * @param int filter2Inputport  Port number of the postfilter
     * @return void
     */
    void ConnectFilters(int filter1Index, int filter2Index, int filter1Outputport = 0,  int filter2Inputport = 0);

    /**
     * void UpdatePipeline
     *
     * @brief 
     *
     * Call Update() of all the component filters.
     *
     * @return void
     */
    void UpdatePipeline();

    /**
     * void PrintConnection
     *
     * @brief 
     *
     * Print current connection.
     *
     * @return void
     */
    void PrintConnection();

    /**
     * QvtkAbstractFilter* GetComponentFilter
     *
     * @brief 
     *
     * Return QvtkAbstractFilter of the pipeline at the specified index of the list
     *
     * @param unsigned int index  Index of the desired filter
     * @return QvtkAbstractFilter*
     */
    virtual QvtkAbstractFilter* GetComponentFilter(unsigned int index);


    /**
     * QT_NAMESPACE::QLayout* GetLayout
     *
     * @brief 
     *
     * Implement this in subclass. Subclass method should return concrete QLayout* types.
     *
     * @return QT_NAMESPACE::QLayout*
     */
    virtual QLayout* GetLayout() = 0;

    /**
     * int GetFilterIndex
     *
     * @brief 
     *
     * Find the index of the specified filter. Return -1 if the filter is not in the list.
     *
     * @param QvtkAbstractFilter * filter 
     * @return int
     */
    virtual int GetFilterIndex(QvtkAbstractFilter* filter);

    /**
     * void SetLayout
     *
     * @brief 
     *
     * Set the layout of the widget. Affects behavior of BuildUI(). Check enum Qvtk_PIPELINE_LAYOUT
     * for more details.
     *
     * @param Qvtk_PIPELINE_LAYOUT  
     * @return void
     */
    virtual void SetLayout(Qvtk_PIPELINE_LAYOUT);




signals:
    void signalPipelineUpdate();
    void signalPipelineConnectionBuilt();
    void signalPipelineUIBuilt();


protected:

    bool CheckFilterConnection();

    /**
     * void ClearUI
     *
     * @brief 
     *
     * Clear the widgets in the layout of this object. Internal use only
     *
     * @return void
     */
    virtual void ClearUI();

    /**
     * void ClearLayout
     *
     * @brief 
     *
     * Delete the layout manager of the widget. Use this to change layout type. Internal use only!
     *
     * @return void
     */
    virtual void ClearLayout();

    /**
     * void BuildConenction
     *
     * @brief 
     *
     * Build the connection of filters
     *
     * @return void
     */
    virtual void BuildConenction();


    /**
     * void BuildUI
     *
     * @brief 
     *
     * Prepare UI. Call this after this->layout is set. Inherit this to specify your out UI
     *
     * @return void
     */
    virtual void BuildUI();

	QString MatchUIToFilterName(QWidget*);


    /************************************************************************/
    /* Inherit these set of function to modify behavior of BuildUI
    /************************************************************************/
    virtual void SetStackWidgetLayout();
    virtual void SetToolboxLayout();
    virtual void SetTabWidgetLayout();
    virtual void SetHBoxLayout();
    virtual void SetVBoxLayout();
    virtual void SetGridLayout();
    virtual void SetStackLayout();
    virtual void SetFormLayout();

    bool Qvtk_PIPELINE_CONNECTED_FLAG;

    QList<FilterConenction> filterConnectionMap;
    QList<QWidget*> filterUIList;
    QList<QvtkAbstractFilter*> filterList;
    QMap<QvtkAbstractFilter*, QWidget*> filterToUIMap;
};

#endif // __Qvtk_ABSTRACT_PIPELINE_H__
