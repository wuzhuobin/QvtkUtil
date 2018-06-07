#include "vtkDiscreteMarchingCubesWithSmooth.h"
// vtk
#include <vtkSmartpointer.h>
#include <vtkNew.h>
#include <vtkPolyData.h>
#include <vtkPolyDataWriter.h>
#include <vtkNIFTIImageReader.h>
// std
#include <ctime>
int main(int argc, char **argv) {

	vtkNew<vtkNIFTIImageReader> reader;
	reader->SetFileName("C:/Users/jieji/Desktop/aal.nii");
	reader->Update();

	std::clock_t cc = std::clock();
	vtkNew<vtkDiscreteMarchingCubesWithSmooth> marchingCubes;
	marchingCubes->SetInputConnection(reader->GetOutputPort());
	marchingCubes->SetPassBand(0.001);
	marchingCubes->SetFeatureAngle(120);
	marchingCubes->GenerateValues(130, 0, 129); 
	marchingCubes->SetNumberOfIterations(15);

	marchingCubes->Update();
	std::cout << "Time used: " << std::clock() - cc << "ms.\n";

	vtkNew<vtkPolyDataWriter> writer;
	writer->SetInputConnection(marchingCubes->GetOutputPort());
	writer->SetFileName("asdf.vtk");
	writer->Update();
	writer->Write();
	
	std::cout << "Press any keys to continue.\n";
	std::cin.get();

	return 0;
}