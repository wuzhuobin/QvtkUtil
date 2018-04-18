/*!
 * \file QvtkAbstractITKMeshToImageFilter.hxx
 * \date 2017/07/28 15:20
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong,
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief This class wraps Mesh to Image filter as itk::ImageSource.
 *        Note that there are restriction to this class. This itk filter
 *        wrapped with this class can only connect to 1 prefilter due the
 *        the lack of a convertion filter to convert mesh between ITK and
 *        VTK format.
 *
 * \note  This class is unstable
*/

#ifdef __Qvtk_ABSTRACT_MESH_TO_IMAGE_FILTER_H__

template <typename TInputMeshType, typename TOutputImageType>
void QvtkAbstractMeshToImageFilter<TInputMeshType, TOutputImageType>::SetInputData(TInputMeshType::Pointer input)
{
    if (this->GetITKFilter())
    {
        this->GetITKFilter()->SetInput(input);
    }
    else
    {
        qWarning() << "[Warning] ITK filter was not created!";
        return;
    }
}

template <typename TInputMeshType, typename TOutputImageType>
void QvtkAbstractMeshToImageFilter<TInputMeshType, TOutputImageType>::SetInputConnection(vtkAlgorithmOutput* input, int port /*= 0*/)
{
    if (!this->GetITKFilter())
    {
        qWarning() << "[Warning] ITK filter was not created!";
        return;
    }

    vtkPolyData* inPD = input->GetProducer()->GetOutputDataObject(input->GetIndex());

    TInputMeshType::Pointer mesh = TInputMeshType::New();

    ConvertVtkPolyDataToMesh(inPD, mesh);

    this->GetITKFilter()->SetInput(mesh);
}

template <typename TInputMeshType, typename TOutputImageType>
typename itk::ImageSource<TOutputImageType>* QvtkAbstractMeshToImageFilter<TInputMeshType, TOutputImageType>::GetITKFilter()
{
    return (FilterType*)(this->itkfilter);
}

template <typename TInputMeshType, typename TOutputImageType>
void QvtkAbstractMeshToImageFilter<TInputMeshType, TOutputImageType>::PreConnectFilter(QvtkAbstractFilter* preFilter, int prePort /*= 0*/, int thisPort /*= 0*/)
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
            itk::MeshSource<TInputMeshType>* meshSource
                = dynamic_cast<itk::MeshSource<TInputMeshType>*>(castedPostFilter->GetITKFilter());
            if (!meshSource)
            {
                qCritical() << "Input's itk filter cannot be casted to mesh source.";
                return;
            }
            else {
                qDebug() << "Type cast success. Treating it as ITK filter.";
                this->GetITKFilter()->SetInput(thisPort, meshSource->GetOutput());
                this->prefilter = preFilter;
                this->prePortConnection.append(std::pair<int, int>(prePort, thisPort));
            }

        }
        else {
            this->SetInputConnection(preFilter->GetOutputPort(prePort), thisPort);
            this->prefilter = preFilter;
            this->prePortConnection.append(std::pair<int, int>(prePort, thisPort));
        }
    }
    catch (...)
    {
        qDebug() << "[Error] Error check failed!";
        return;
    }
}

template <typename TInputMeshType, typename TOutputImageType>
TOutputImageType::Pointer QvtkAbstractMeshToImageFilter<TInputMeshType, TOutputImageType>::GetITKOutputData()
{
    if (!this->ErrorCheck())
        return this->GetFilter()->GetOutput();
}

template <typename TInputMeshType, typename TOutputImageType>
typename vtkImageData* QvtkAbstractMeshToImageFilter<TInputMeshType, TOutputImageType>::GetVTKOutputData()
{
    return vtkImageData::SafeDownCast(this->GetFilter()->GetOutput());
}

template <typename TInputMeshType, typename TOutputImageType>
QvtkAbstractMeshToImageFilter<TInputMeshType, TOutputImageType>::QvtkAbstractITKImageToImageFilter()
{
    this->outputConnector = ConnectorType::New();
}

template <typename TInputMeshType, typename TOutputImageType>
void QvtkAbstractMeshToImageFilter<TInputMeshType, TOutputImageType>::Update()
{
    if (!this->ErrorCheck())
    {
        this->UpdateInputPort();
        this->UpdateOutputPort();

        QvtkAbstractITKFilter->Update();
    }
}

template <typename TInputMeshType, typename TOutputImageType>
void QvtkAbstractMeshToImageFilter<TInputMeshType, TOutputImageType>::UpdateOutputPort()
{
    if (!this->ErrorCheck())
    {
        this->outputConnector->Update();

        this->GetFilter()->SetInputData(this->outputConnector->GetOutput());
        this->GetFilter()->Update();
    }
}

template <typename TInputMeshType, typename TOutputImageType>
bool QvtkAbstractMeshToImageFilter<TInputMeshType, TOutputImageType>::ErrorCheck()
{
    if (!this->GetITKFilter())
    {
        QString errmsg("[Error] " << this->GetFilterName() << " ITK filter is not created!");
        qCritical() << errmsg;
        throw std::logic_error(errmsg.toStdString());
        return true;
    }

    if (!this->GetFilter())
    {
        QString errmsg("[Error] " << this->GetFilterName() << " VTK filter is not created!");
        qCritical() << errmsg;
        throw std::logic_error(errmsg.toStdString());
        return true;
    }

    if (!this->GetFilter()->GetInput())
    {
        QString errmsg("[Warning!] " << this->GetFilterName() << " VTK filter has no input!");
        qWarning() << errmsg;
        throw std::runtime_error(errmsg.toStdString());
        return true;
    }

    return false;
}

template<typename MeshType>
void QvtkAbstractMeshToImageFilter::ConvertMeshTovtkPolyData(MeshType mesh, vtkPolyData* outpd)
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
    MeshType::PointsContainer::Pointer points = mesh->GetPoints();

    // In ITK the point container is not necessarily a vector, but in VTK it is
    vtkIdType VTKId = 0;
    std::map< vtkIdType, int > IndexMap;

    for (MeshType::PointsContainer::Iterator i = points->Begin();
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
        float, MeshType::CellTraits,
        itk::LineCell< itk::CellInterface<MeshType::PixelType, MeshType::CellTraits > >,
        VisitVTKCellsClass> LineVisitor;
    LineVisitor::Pointer lv = LineVisitor::New();
    lv->SetTypeArray(types);
    lv->SetCellCounter(&vtkCellCount);
    lv->SetCellArray(cells);

    // Setup the triangle visitor
    typedef itk::CellInterfaceVisitorImplementation<
        float, MeshType::CellTraits,
        itk::TriangleCell< itk::CellInterface<MeshType::PixelType, MeshType::CellTraits > >,
        VisitVTKCellsClass> TriangleVisitor;
    TriangleVisitor::Pointer tv = TriangleVisitor::New();
    tv->SetTypeArray(types);
    tv->SetCellCounter(&vtkCellCount);
    tv->SetCellArray(cells);

    // Setup the quadrilateral visitor
    typedef itk::CellInterfaceVisitorImplementation<
        float, MeshType::CellTraits,
        itk::QuadrilateralCell< itk::CellInterface<MeshType::PixelType, MeshType::CellTraits > >,
        VisitVTKCellsClass> QuadrilateralVisitor;
    QuadrilateralVisitor::Pointer qv = QuadrilateralVisitor::New();
    qv->SetTypeArray(types);
    qv->SetCellCounter(&vtkCellCount);
    qv->SetCellArray(cells);

    // Add the visitors to a multivisitor

    MeshType::CellType::MultiVisitor::Pointer mv =
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

template <typename TInputMeshType, typename TOutputImageType>
QvtkAbstractMeshToImageFilter<TInputMeshType, TOutputImageType>::~QvtkAbstractITKImageToImageFilter()
{

}


template <typename TInputMeshType, typename TOutputImageType>
void QvtkAbstractMeshToImageFilter<TInputMeshType, TOutputImageType>::UpdateInputPort(int thisPort /*= 0*/)
{
    if (!this->prefilter)
    {
        qWarning() << "Trying to update non existing port!";
        return;
    }
    else
    {
        /* Determine if prefilter is an itk filter */
        QvtkAbstractITKFilter* castedPostFilter
            = dynamic_cast<QvtkAbstractITKFilter*>(this->prefilter);
        if (NULL != castedPostFilter)
        {
            itk::MeshSource<TInputMeshType>* meshSource
                = dynamic_cast<itk::MeshSource<TInputMeshType>*>(castedPostFilter->GetITKFilter());
            if (!meshSource)
            {
                qCritical() << "[Error] Cannot cast the input filter to mesh source!";
                return;
            }
            else {
                /* No need to update if pre filter is connected as ITK filter */
                return;
            }

        }
        else {
            /* Update */
            int preport;
            for (QList<std::pair<int, int>>::const_iterator iter = this->prePortConnection.cbegin();
            iter != this->prePortConnection.cend(); iter++)
            {
                if (iter->second == thisPort)
                {
                    this->SetInputConnection(this->prefilter->GetOutputPort(preport), thisPort);
                }
            }
        }
    }
}

template <typename TInputMeshType, typename TOutputImageType>
vtkAlgorithmOutput* QvtkAbstractMeshToImageFilter<TInputMeshType, TOutputImageType>::GetOutputPort(int port /*= 0*/)
{
    if (!ErrorCheck())
    {
        this->outputConnector->Update();
        this->GetFilter()->SetInputData(this->outputConnector->GetOutput());
        this->GetFilter()->Update();
        return this->GetFilter()->GetOutputPort();
    }
    else
    {
        return nullptr;
    }
}

template<typename MeshType>
void QvtkAbstractMeshToImageFilter::ConvertVtkPolyDataToMesh(vtkPolyData* inpd, MeshType mesh)
{
    const unsigned int PointDimension = 3;
    const unsigned int MaxCellDimension = 2;

    typedef itk::DefaultStaticMeshTraits<
        float,
        PointDimension,
        MaxCellDimension,
        float,
        float> MeshTraits;


    typedef itk::Mesh<
        float,
        PointDimension,
        MeshTraits> MeshType;


    MeshType::Pointer  mesh = MeshType::New();


    //
    // Transfer the points from the vtkPolyData into the itk::Mesh
    //
    const unsigned int numberOfPoints = inpd->GetNumberOfPoints();

    vtkPoints * vtkpoints = inpd->GetPoints();

    mesh->GetPoints()->Reserve(numberOfPoints);

    for (unsigned int p = 0; p < numberOfPoints; p++)
    {

        float * apoint = vtkpoints->GetPoint(p);

        mesh->SetPoint(p, MeshType::PointType(apoint));

    }

    //
    // Transfer the cells from the vtkPolyData into the itk::Mesh
    //
    vtkCellArray * triangleStrips = inpd->GetStrips();


    vtkIdType  * cellPoints;
    vtkIdType    numberOfCellPoints;

    //
    // First count the total number of triangles from all the triangle strips.
    //
    unsigned int numberOfTriangles = 0;

    triangleStrips->InitTraversal();

    while (triangleStrips->GetNextCell(numberOfCellPoints, cellPoints))
    {
        numberOfTriangles += numberOfCellPoints - 2;
    }


    vtkCellArray * polygons = inpd->GetPolys();

    polygons->InitTraversal();

    while (polygons->GetNextCell(numberOfCellPoints, cellPoints))
    {
        if (numberOfCellPoints == 3)
        {
            numberOfTriangles++;
        }
    }

    mesh->GetCells()->Reserve(numberOfTriangles);


    typedef MeshType::CellType   CellType;

    typedef itk::TriangleCell< CellType > TriangleCellType;

    int cellId = 0;

    // first copy the triangle strips
    triangleStrips->InitTraversal();
    while (triangleStrips->GetNextCell(numberOfCellPoints, cellPoints))
    {

        unsigned int numberOfTrianglesInStrip = numberOfCellPoints - 2;

        unsigned long pointIds[3];
        pointIds[0] = cellPoints[0];
        pointIds[1] = cellPoints[1];
        pointIds[2] = cellPoints[2];

        for (unsigned int t = 0; t < numberOfTrianglesInStrip; t++)
        {
            MeshType::CellAutoPointer c;
            TriangleCellType * tcell = new TriangleCellType;
            TriangleCellType::PointIdentifier itkPts[3];
            for (int ii = 0; ii < 3; ++ii)
            {
                itkPts[ii] = static_cast<TriangleCellType::PointIdentifier>(pointIds[ii]);
            }
            tcell->SetPointIds(itkPts);
            c.TakeOwnership(tcell);
            mesh->SetCell(cellId, c);
            cellId++;
            pointIds[0] = pointIds[1];
            pointIds[1] = pointIds[2];
            pointIds[2] = cellPoints[t + 3];
        }
    }


    // then copy the normal triangles
    polygons->InitTraversal();
    while (polygons->GetNextCell(numberOfCellPoints, cellPoints))
    {
        if (numberOfCellPoints != 3) // skip any non-triangle.
        {
            continue;
        }
        MeshType::CellAutoPointer c;
        TriangleCellType * t = new TriangleCellType;
        TriangleCellType::PointIdentifier itkPts[3];
        for (int ii = 0; ii < numberOfCellPoints; ++ii)
        {
            itkPts[ii] = static_cast<TriangleCellType::PointIdentifier>(cellPoints[ii]);
        }
        t->SetPointIds(itkPts);
        c.TakeOwnership(t);
        mesh->SetCell(cellId, c);
        cellId++;
    }
}

template <typename TInputMeshType, typename TOutputImageType>
void QvtkAbstractMeshToImageFilter<TInputMeshType, TOutputImageType>::SetInputData(vtkPolyData* input)
{
    if (!this->GetITKFilter())
    {
        qWarning() << "[Warning] ITK filter was not created!";
        return;
    }

    TInputMeshType::Pointer inMesh = TInputMeshType::New();

    ConvertVtkPolyDataToMesh(input, imMesh);

    this->GetITKFilter()->SetInput(imMesh);
}

#endif