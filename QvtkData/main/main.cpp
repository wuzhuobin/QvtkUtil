#include "vtkDiscreteMarchingCubesWithSmooth.h"
// vtk
#include <vtkSmartpointer.h>
#include <vtkNew.h>
#include <vtkPolyData.h>
#include <vtkPolyDataWriter.h>
#include <vtkNIFTIImageReader.h>
#include <vtkDisc>
// std
#include <ctime>
int main(int argc, char **argv) {

	vtkNew<vtkNIFTIImageReader> reader;
	reader->SetFileName("C:/Users/jieji/Desktop/aal.nii");
	reader->Update();

	std::clock_t cc;
	cc = std::clock();
	vtkNew<vtkDiscreteMarchingCubesWithSmooth> threadingMarchingCubes;
	threadingMarchingCubes->SetInputConnection(reader->GetOutputPort());
	threadingMarchingCubes->SetPassBand(0.001);
	threadingMarchingCubes->SetFeatureAngle(120);
	threadingMarchingCubes->GenerateValues(130, 0, 129); 
	threadingMarchingCubes->SetNumberOfIterations(2);
	threadingMarchingCubes->Update();
	std::cout << "Threading marching cubes time used: " << std::clock() - cc << "ms.\n";
	cc = std::clock();
	vtkNew<vtkDiscreteMarchingCubes> marchingCubes;
	marchingCubes->SetInputConnection(reader->GetOutputPort());
	marchingCubes->GenerateValues(130, 0, 129);
	marchingCubes->Update();
	std::cout << "marching cubes time used: " << std::clock() - cc << "ms.\n";
	cc = std::clock();
	vtkNew<vtkFlyingEdges3D> flyingEdges3d;
	flyingEdges3d->SetInputConnection(reader->GetOutputPort());
	flyingEdges3d->GenerateValues(130, 0, 129);
	flyingEdges3d->Update();
	std::cout << "Flying edges time used: " << std::clock() - cc << "ms.\n";



	vtkNew<vtkPolyDataWriter> writer;
	writer->SetInputConnection(flyingEdges3d->GetOutputPort());
	writer->SetFileName("asdf.vtk");
	writer->Update();
	writer->Write();
	
	std::cout << "Press any keys to continue.\n";
	std::cin.get();

	return 0;
}