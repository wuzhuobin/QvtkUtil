/*!
 * \file QvtkAbstractITKImageToMeshFilter.hxx
 * \date 2017/07/27 19:19
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong, 
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief Implementation of the abstract class QvtkAbstractITKImageToMeshFilter
 *
 * \note
*/
#ifdef __Qvtk_ABSTRACT_ITK_IMAGE_TO_MESH_FILTER_H__

#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkUnstructuredGrid.h>
#include <itkMesh.h>
#include <itkLineCell.h>
#include <itkTriangleCell.h>
#include <itkCellInterface.h>
#include <itkQuadrilateralCell.h>
#include <itkCellInterfaceVisitor.h>


template <typename TInputImageType, typename TOutputMeshType>
QvtkAbstractImageToMeshFilter<TInputImageType, TOutputMeshType>::
~QvtkAbstractITKImageToImageFilter()
{

}

template <typename TInputImageType, typename TOutputMeshType>
QvtkAbstractImageToMeshFilter<TInputImageType, TOutputMeshType>::
QvtkAbstractITKImageToImageFilter()
{

}

template <typename TInputImageType, typename TOutputMeshType>
typename vtkPolyData* QvtkAbstractImageToMeshFilter<TInputImageType, TOutputMeshType>::
GetVTKOutputData()
{
    if (!ErrorCheck())
    {
        return vtkPolyData::SafeDownCast(this->GetFilter()->GetOutput());
    }
}

// WongQvtkAbstractITKImageToMeshFilterMatthewb289a0e0-ac2d-40f7-a11c-78c5d7cf00eeLun  
//[ 7/27/2017-19:30:57 Author: Wong, Matthew Lun]

template <typename TInputImageType, typename TOutputMeshType>
typename TOutputMeshType::Pointer QvtkAbstractImageToMeshFilter<TInputImageType, TOutputMeshType>::GetITKOutputData()
{
    if (!ErrorCheck())
        return this->GetITKFilter()->GetOutput();
    else
        return nullptr;
}

template <typename TInputImageType, typename TOutputMeshType>
void QvtkAbstractImageToMeshFilter<TInputImageType, TOutputMeshType>::
PreConnectFilter(QvtkAbstractFilter* preFilter, int prePort /*= 0*/, int thisPort /*= 0*/)
{
    try {
        this->ErrorCheck();
    }
    catch (std::runtime_error &e) /* Allows lack of input */
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
            }

        }
        else {
            this->SetInputConnection(preFilter->GetOutputPort(prePort), thisPort);
        }
    }
    catch (...)
    {
        qDebug() << "[Error] Error check failed!";
        return;
    }
}

template <typename TInputImageType, typename TOutputMeshType>
typename  itk::ImageToMeshFilter<TInputImageType, TOutputImageType>*
QvtkAbstractImageToMeshFilter<TInputImageType, TOutputMeshType>::GetITKFilter()
{
    return (FilterType*)this->itkfilter;
}

template <typename TInputImageType, typename TOutputMeshType>
void QvtkAbstractImageToMeshFilter<TInputImageType, TOutputMeshType>::UpdateOutputPort()
{
    if (!this->ErrorCheck())
    {
        TOutputMeshType::Pointer mesh = this->GetITKFilter()->GetOutput();

        vtkSmartPointer<vtkPolyData> PD
            = vtkSmartPointer<vtkPolyData>::New();
        ConvertMeshTovtkPolyData(mesh, PD);
        this->GetFilter()->SetInputData(PD);
        this->GetFilter()->Update();
    }
}

template <typename TInputImageType, typename TOutputMeshType>
typename vtkAlgorithmOutput* QvtkAbstractImageToMeshFilter<TInputImageType, TOutputMeshType>::GetOutputPort(int port /*= 0*/)
{
    if (!this->ErrorCheck())
    {
        return nullptr;
    }

    if (!this->GetITKFilter())
    {
        qCritical() << "[Error] Filter" << this->GetITKFilter()->GetNameOfClass() << " not created!";
        return nullptr;
    }

    if (!this->GetITKFilter()->GetOutput())
    {
        qCritical() << "[Error] Attempting to get output port before output is ready!";
        qCritical() << "\tOutput of filter " << this->GetITKFilter()->GetNameOfClass() << " is not ready";
        return nullptr;
    }

    return this->GetFilter()->GetOutputPort();
}

template <typename TInputImageType, typename TOutputMeshType>
void QvtkAbstractImageToMeshFilter<TInputImageType, TOutputMeshType>::Update()
{
    /* Update the mesh output of the filter */
    this->UpdateOutputPort();

    QvtkAbstractITKFilter::Update();
}

template <typename TInputImageType, typename TOutputMeshType>
void QvtkAbstractImageToMeshFilter<TInputImageType, TOutputMeshType>::
ConvertMeshTovtkPolyData(TOutputMeshType::Pointer mesh, vtkPolyData* outpd)
{
    // Get the number of points in the mesh
    int numPoints = mesh->GetNumberOfPoints();
    if (numPoints == 0)
    {
        mesh->Print(std::cerr);
        std::cerr << "no points in Grid " << std::endl;
        exit(-1);
    }

    // Create the vtkPoints object and set the number of points
    vtkPoints* vpoints = vtkPoints::New();
    vpoints->SetNumberOfPoints(numPoints);
    // Iterate over all the points in the itk mesh filling in
    // the vtkPoints object as we go
    TOutputMeshType::PointsContainer::Pointer points = mesh->GetPoints();

    // In ITK the point container is not necessarily a vector, but in VTK it is
    vtkIdType VTKId = 0;
    std::map< vtkIdType, int > IndexMap;

    for (TOutputMeshType::PointsContainer::Iterator i = points->Begin();
    i != points->End(); ++i, VTKId++)
    {
        // Get the point index from the point container iterator
        IndexMap[VTKId] = i->Index();

        // Set the vtk point at the index with the the coord array from itk
        // itk returns a const pointer, but vtk is not const correct, so
        // we have to use a const cast to get rid of the const
        vpoints->SetPoint(VTKId, const_cast<float*>(i->Value().GetDataPointer()));
    }

    // Set the points on the vtk grid
    outpd->SetPoints(vpoints);

    // Setup some VTK things
    int vtkCellCount = 0; // running counter for current cell being inserted into vtk
    int numCells = mesh->GetNumberOfCells();
    int *types = new int[numCells]; // type array for vtk
                                    // create vtk cells and estimate the size
    vtkCellArray* cells = vtkCellArray::New();
    cells->EstimateSize(numCells, 4);

    // Setup the line visitor
    typedef itk::CellInterfaceVisitorImplementation<
        float, TOutputMeshType::CellTraits,
        itk::LineCell< itk::CellInterface<TOutputMeshType::PixelType, TOutputMeshType::CellTraits > >,
        VisitVTKCellsClass> LineVisitor;
    LineVisitor::Pointer lv = LineVisitor::New();
    lv->SetTypeArray(types);
    lv->SetCellCounter(&vtkCellCount);
    lv->SetCellArray(cells);

    // Setup the triangle visitor
    typedef itk::CellInterfaceVisitorImplementation<
        float, TOutputMeshType::CellTraits,
        itk::TriangleCell< itk::CellInterface<TOutputMeshType::PixelType, TOutputMeshType::CellTraits > >,
        VisitVTKCellsClass> TriangleVisitor;
    TriangleVisitor::Pointer tv = TriangleVisitor::New();
    tv->SetTypeArray(types);
    tv->SetCellCounter(&vtkCellCount);
    tv->SetCellArray(cells);

    // Setup the quadrilateral visitor
    typedef itk::CellInterfaceVisitorImplementation<
        float, TOutputMeshType::CellTraits,
        itk::QuadrilateralCell< itk::CellInterface<TOutputMeshType::PixelType, TOutputMeshType::CellTraits > >,
        VisitVTKCellsClass> QuadrilateralVisitor;
    QuadrilateralVisitor::Pointer qv = QuadrilateralVisitor::New();
    qv->SetTypeArray(types);
    qv->SetCellCounter(&vtkCellCount);
    qv->SetCellArray(cells);

    // Add the visitors to a multivisitor

    TOutputMeshType::CellType::MultiVisitor::Pointer mv =
        TOutputMeshType::CellType::MultiVisitor::New();

    mv->AddVisitor(tv);
    mv->AddVisitor(qv);
    mv->AddVisitor(lv);

    // Now ask the mesh to accept the multivisitor which
    // will Call Visit for each cell in the mesh that matches the
    // cell types of the visitors added to the MultiVisitor
    mesh->Accept(mv);

    // Now set the cells on the vtk grid with the type array and cell array
    outpd->SetCells(types, cells);

    // Clean up vtk objects
    cells->Delete();
    vpoints->Delete();

}

template <typename TInputImageType, typename TOutputMeshType>
void QvtkAbstractImageToMeshFilter<TInputImageType, TOutputMeshType>::SetInputConnection(vtkAlgorithmOutput* input, int port /*= 0*/)
{
    /* cast the input into vtkimagedata, then convert to itkimage */
    this->inputConnector->
        SetInput(vtkImageData::SafeDownCast(input->GetProducer()->GetOutputDataObject(input->GetIndex())));
    this->inputConnector->Update();

    /* Set the converted image into the itk filter */
    if (this->GetITKFilter())
        this->GetITKFilter()->SetInput(this->inputConnector->GetOutput());
}

// WongQvtkAbstractITKImageToMeshFilterMatthew98dd7b38-d2e5-4c73-8b18-7148478fe8cdLun  
//[ 7/27/2017-19:30:46 Author: Wong, Matthew Lun]

template <typename TInputImageType, typename TOutputMeshType>
void QvtkAbstractImageToMeshFilter<TInputImageType, TOutputMeshType>::SetInputData(vtkImageData* input)
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

#endif 