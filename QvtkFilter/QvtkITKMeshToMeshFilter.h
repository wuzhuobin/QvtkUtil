/*!
 * \file QvtkAbstractITKMeshToMeshFilter.h
 * \date 2017/07/28 14:35
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong, 
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief This class wraps mesh to mesh itk filters.
 *        Note that there are restriction to this class. This itk filter
 *        wrapped with this class can only connect to 1 prefilter due the
 *        the lack of a convertion filter to convert mesh between ITK and 
 *        VTK format.
 *
 * \note This class is unstable
 *
*/

#ifndef __Qvtk_ABSTRACT_ITK_MESH_TO_MESH_FILTER_H__
#define __Qvtk_ABSTRACT_ITK_MESH_TO_MESH_FILTER_H__

#include "QvtkAbstractITKFilter.h"


#include <itkMesh.h>
#include <itkMeshToMeshFilter.h>


#include <vtkPolyData.h>
#include <vtkAlgorithmOutput.h>
#include <vtkPassThroughFilter.h>

template <typename TInputMeshType, typename TOutputMeshType>
class /*ABSTRACTFILTER_EXPORT*/ QvtkAbstractITKMeshToMeshFilter
    : public QvtkAbstractITKFilter
{
public:
    QvtkAbstractITKImageToImageFilter();
    ~QvtkAbstractITKImageToImageFilter();

    typedef itk::MeshToMeshFilter<TInputMeshType, TOutputMeshType> FilterType;


    /**
    * void SetInputData
    *
    * @brief
    *
    * Convert input to TInputMeshType. Be warned that only triangles and
    * points are kepts.
    *
    * @param vtkPolyData * input
    * @return void
    */
    virtual void SetInputData(vtkPolyData* input);


    /**
     * void SetInputData
     *
     * @brief 
     *
     * ITK Method wrapped
     *
     * @param TInputMeshType::Pointer input 
     * @return void
     */
    virtual void SetInputData(TInputMeshType::Pointer input);

    /**
    * void SetInputConnection
    *
    * @brief
    *
    * Override SetInputConnection to convert input into ITK format first. Strictly
    * for vtk input algorithm.
    *
    * @param vtkAlgorithmOutput * input
    * @param int port
    * @return void
    */
    virtual void SetInputConnection(vtkAlgorithmOutput* input, int port = 0);

    /**
    * vtkAlgorithmOutput* GetOutputPort
    *
    * @brief
    *
    * Override GetOutputPort to convert output back to VTK format. Strictly for
    * vtk algorithm output.
    *
    * @param int port
    * @return vtkAlgorithmOutput*
    */
    virtual vtkAlgorithmOutput* GetOutputPort(int port = 0);

    ///**
    // * FilterType GetITKFilter
    // *
    // * @brief 
    // *
    // * Return the ITK filter 
    // *
    // * @return FilterType
    // */
    virtual FilterType* GetITKFilter();

    /**
    * void PreConnectFilter
    *
    * @brief
    *
    * Override this method for compatibility to QvtkPipeline
    *
    * @param QvtkAbstractFilter * preFilter
    * @param int prePort
    * @param int thisPort
    * @return void
    */
    virtual void PreConnectFilter(QvtkAbstractFilter* preFilter, int prePort = 0, int thisPort = 0);


    /**
    * void Update
    *
    * @brief
    *
    * Update ITK then update vtkPassThroughFilter
    *
    * @return void
    */
    virtual void Update();


    /**
    * TOutputMeshType GetITKOutputData
    *
    * @brief
    *
    * Get ITK output, return as TOutputMeshType
    *
    * @return TOutputMeshType
    */
    virtual TOutputMeshType::Pointer GetITKOutputData();

    /**
    * vtkPolyData* GetVTKOutputData
    *
    * @brief
    *
    * Get VTK output as vtkPolyData*
    *
    * @return vtkPolyData*
    */
    virtual vtkPolyData* GetVTKOutputData();

protected:
    /**
    * void UpdateOutputPort
    *
    * @brief
    *
    * Update the port output. The port require update every time the ITKFilter changes.
    *
    * @return void
    */
    void UpdateOutputPort();

    /**
     * void UpdateInputPort
     *
     * @brief 
     *
     * Update the input port. This for mesh only as there are no filter for 
     * converting vtkpolydata to itkmesh
     *
     * @param int inPort 
     * @return void
     */
    void UpdateInputPort(int inPort = 0);

    virtual bool ErrorCheck();

    template<typename MeshType>
    static void ConvertMeshTovtkPolyData(MeshType mesh,vtkPolyData* outpd);

    template<typename MeshType>
    static void ConvertVtkPolyDataToMesh(vtkPolyData* inpd, MeshType mesh);
};



// WongQvtkAbstractITKMeshToMeshFilterMatthew7fa25dae-e60b-46be-af78-3b19ed464a31Lun  
//[ 7/27/2017-19:44:32 Author: Wong, Matthew Lun]

#include "QvtkAbstractITKImageToMeshFilter.hxx"

#endif