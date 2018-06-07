#include "vtkDiscreteMarchingCubesWithSmooth.h"

// vtk
#include <vtkWindowedSincPolyDataFilter.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
//#include <vtkCallbackCommand.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkAppendPolyData.h>
// std
#include <mutex>
#include <future>

vtkStandardNewMacro(vtkDiscreteMarchingCubesWithSmooth);
vtkDiscreteMarchingCubesWithSmooth::vtkDiscreteMarchingCubesWithSmooth()
{
	this->AppendPolyData = vtkAppendPolyData::New();
	this->NumberOfIterations = 20;
	this->PassBand = 0.1;

	this->FeatureAngle = 45.0;
	this->EdgeAngle = 15.0;
	this->FeatureEdgeSmoothing = 0;
	this->BoundarySmoothing = 1;
	this->NonManifoldSmoothing = 0;

	this->GenerateErrorScalars = 0;
	this->GenerateErrorVectors = 0;

	this->NormalizeCoordinates = 0;
}

static void Threading(vtkDiscreteMarchingCubesWithSmooth* _this) {

}

vtkDiscreteMarchingCubesWithSmooth::~vtkDiscreteMarchingCubesWithSmooth()
{
}

int vtkDiscreteMarchingCubesWithSmooth::RequestData(
	vtkInformation *request,
	vtkInformationVector **inputVector,
	vtkInformationVector *outputVector)
{
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);
	vtkImageData *input = vtkImageData::SafeDownCast(
		inInfo->Get(vtkDataObject::DATA_OBJECT()));
	vtkPolyData *output = vtkPolyData::SafeDownCast(
		outInfo->Get(vtkDataObject::DATA_OBJECT()));
	int numContours = this->ContourValues->GetNumberOfContours();
	double *values = this->ContourValues->GetValues();
	// initialize and check input
	vtkPointData *pd;
	vtkDataArray *inScalars;
	pd = input->GetPointData();
	if (pd == NULL)
	{
		vtkErrorMacro(<< "PointData is NULL");
		return 1;
	}
	inScalars = pd->GetScalars();
	if (inScalars == NULL)
	{
		vtkErrorMacro(<< "Scalars must be defined for contouring");
		return 1;
	}

	if (input->GetDataDimension() != 3)
	{
		vtkErrorMacro(<< "Cannot contour data of dimension != 3");
		return 1;
	}
	this->WindowedSincPolyDataFilters.reserve(numContours);
	this->DiscreteMarchingCubes.reserve(numContours);
	
	for (int i = 0; i < numContours; ++i) {
		if (this->DiscreteMarchingCubes.size() <= i) {
			this->DiscreteMarchingCubes.push_back(vtkDiscreteMarchingCubes::New());
			this->DiscreteMarchingCubes[i]->SetInputData(input);
		}
		if (this->WindowedSincPolyDataFilters.size() <= i) {
			this->WindowedSincPolyDataFilters.push_back(vtkWindowedSincPolyDataFilter::New());
			this->WindowedSincPolyDataFilters[i]->SetInputConnection(this->DiscreteMarchingCubes[i]->GetOutputPort());
		}
	}
	std::vector<std::future<void>> furtures;
	std::mutex m;
	for (int i = 0; i < numContours; ++i) {
		furtures.push_back(std::async([=, &m]() {
			this->DiscreteMarchingCubes[i]->SetComputeAdjacentScalars(this->ComputeAdjacentScalars);
			this->DiscreteMarchingCubes[i]->SetValue(0, values[i]);
			this->DiscreteMarchingCubes[i]->SetComputeNormals(this->ComputeNormals);
			this->DiscreteMarchingCubes[i]->SetComputeGradients(this->ComputeGradients);
			this->DiscreteMarchingCubes[i]->SetComputeScalars(this->ComputeScalars);
			this->DiscreteMarchingCubes[i]->SetLocator(this->Locator);
			this->DiscreteMarchingCubes[i]->Update();
			vtkPolyData *_polydata = this->DiscreteMarchingCubes[i]->GetOutput();
			if (_polydata->GetNumberOfPoints() >= 1 &&
				_polydata->GetNumberOfCells() >= 1) {
				this->WindowedSincPolyDataFilters[i]->SetNumberOfIterations(this->NumberOfIterations);
				this->WindowedSincPolyDataFilters[i]->SetPassBand(this->PassBand);
				this->WindowedSincPolyDataFilters[i]->SetFeatureAngle(this->FeatureAngle);
				this->WindowedSincPolyDataFilters[i]->SetFeatureEdgeSmoothing(this->FeatureEdgeSmoothing);
				this->WindowedSincPolyDataFilters[i]->SetBoundarySmoothing(this->BoundarySmoothing);
				this->WindowedSincPolyDataFilters[i]->SetNonManifoldSmoothing(this->NonManifoldSmoothing);
				this->WindowedSincPolyDataFilters[i]->SetNormalizeCoordinates(this->NormalizeCoordinates);
				this->WindowedSincPolyDataFilters[i]->Update();
				m.lock();
				this->AppendPolyData->AddInputConnection(this->WindowedSincPolyDataFilters[i]->GetOutputPort());
				m.unlock();
			}
		}));
	}
	for (auto &future : furtures) {
		future.get();
	}
	this->AppendPolyData->Update();
	output->ShallowCopy(this->AppendPolyData->GetOutput());
	return 1;
}