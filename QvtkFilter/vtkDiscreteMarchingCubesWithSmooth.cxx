#include "vtkDiscreteMarchingCubesWithSmooth.h"

// vtk
#include <vtkWindowedSincPolyDataFilter.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkCallbackCommand.h>

vtkStandardNewMacro(vtkDiscreteMarchingCubesWithSmooth);
vtkDiscreteMarchingCubesWithSmooth::vtkDiscreteMarchingCubesWithSmooth()
{
	this->WindowedSincPolyDataFilter = vtkWindowedSincPolyDataFilter::New();
	this->BoundarySmoothing = 0;
	this->FeatureEdgeSmoothing = 0;
	this->FeatureAngle = 120;
	this->PassBand = 0.001;
	this->NonManifoldSmoothing = 1;
	this->NormalizeCoordinates = 1;
}

vtkDiscreteMarchingCubesWithSmooth::~vtkDiscreteMarchingCubesWithSmooth()
{
	this->WindowedSincPolyDataFilter->Delete();
}

int vtkDiscreteMarchingCubesWithSmooth::RequestData(
	vtkInformation *request,
	vtkInformationVector **inputVector,
	vtkInformationVector *outputVector)
{
	int flag1 = vtkDiscreteMarchingCubes::RequestData(request, inputVector, outputVector);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);
	vtkPolyData *output = vtkPolyData::SafeDownCast(
		outInfo->Get(vtkDataObject::DATA_OBJECT()));
	this->WindowedSincPolyDataFilter->SetInputData(output);
	this->WindowedSincPolyDataFilter->SetNumberOfIterations(this->NumberOfIterations);
	this->WindowedSincPolyDataFilter->SetPassBand(this->PassBand);
	this->SetNormalizeCoordinates(this->NormalizeCoordinates);
	this->SetFeatureEdgeSmoothing(this->FeatureEdgeSmoothing);
	this->SetFeatureAngle(this->FeatureAngle);
	this->SetEdgeAngle(this->EdgeAngle);
	this->SetBoundarySmoothing(this->BoundarySmoothing);
	this->SetNonManifoldSmoothing(this->NonManifoldSmoothing);
	this->SetGenerateErrorScalars(this->GenerateErrorScalars);
	this->SetGenerateErrorVectors(this->GenerateErrorVectors);
	this->WindowedSincPolyDataFilter->Update();
	output->ShallowCopy(this->WindowedSincPolyDataFilter->GetOutput());
	return flag1;
}