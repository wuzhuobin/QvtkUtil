#ifndef __VTK_DISCRETE_MARCHING_CUBES_WITH_SMOOTH_H__
#define __VTK_DISCRETE_MARCHING_CUBES_WITH_SMOOTH_H__

// me
#include "abstractfilter_export.h"

// vtk
#include <vtkDiscreteMarchingCubes.h>

class vtkWindowedSincPolyDataFilter;
class ABSTRACTFILTER_EXPORT vtkDiscreteMarchingCubesWithSmooth : public vtkDiscreteMarchingCubes
{
public:
	static vtkDiscreteMarchingCubesWithSmooth* New();
	vtkTypeMacro(vtkDiscreteMarchingCubesWithSmooth, vtkDiscreteMarchingCubes);
	//@{
	/**
	* Specify the number of iterations (or degree of the polynomial
	* approximating the windowed sinc function).
	*/
	vtkSetClampMacro(NumberOfIterations, int, 0, VTK_INT_MAX);
	vtkGetMacro(NumberOfIterations, int);
	//@}

	//@{
	/**
	* Set the passband value for the windowed sinc filter
	*/
	vtkSetClampMacro(PassBand, double, 0.0, 2.0);
	vtkGetMacro(PassBand, double);
	//@}

	//@{
	/**
	* Turn on/off coordinate normalization.  The positions can be
	* translated and scaled such that they fit within a [-1, 1] prior
	* to the smoothing computation. The default is off.  The numerical
	* stability of the solution can be improved by turning
	* normalization on.  If normalization is on, the coordinates will
	* be rescaled to the original coordinate system after smoothing has
	* completed.
	*/
	vtkSetMacro(NormalizeCoordinates, int);
	vtkGetMacro(NormalizeCoordinates, int);
	vtkBooleanMacro(NormalizeCoordinates, int);
	//@}

	//@{
	/**
	* Turn on/off smoothing along sharp interior edges.
	*/
	vtkSetMacro(FeatureEdgeSmoothing, int);
	vtkGetMacro(FeatureEdgeSmoothing, int);
	vtkBooleanMacro(FeatureEdgeSmoothing, int);
	//@}

	//@{
	/**
	* Specify the feature angle for sharp edge identification.
	*/
	vtkSetClampMacro(FeatureAngle, double, 0.0, 180.0);
	vtkGetMacro(FeatureAngle, double);
	//@}

	//@{
	/**
	* Specify the edge angle to control smoothing along edges (either interior
	* or boundary).
	*/
	vtkSetClampMacro(EdgeAngle, double, 0.0, 180.0);
	vtkGetMacro(EdgeAngle, double);
	//@}

	//@{
	/**
	* Turn on/off the smoothing of vertices on the boundary of the mesh.
	*/
	vtkSetMacro(BoundarySmoothing, int);
	vtkGetMacro(BoundarySmoothing, int);
	vtkBooleanMacro(BoundarySmoothing, int);
	//@}

	//@{
	/**
	* Smooth non-manifold vertices.
	*/
	vtkSetMacro(NonManifoldSmoothing, int);
	vtkGetMacro(NonManifoldSmoothing, int);
	vtkBooleanMacro(NonManifoldSmoothing, int);
	//@}

	//@{
	/**
	* Turn on/off the generation of scalar distance values.
	*/
	vtkSetMacro(GenerateErrorScalars, int);
	vtkGetMacro(GenerateErrorScalars, int);
	vtkBooleanMacro(GenerateErrorScalars, int);
	//@}

	//@{
	/**
	* Turn on/off the generation of error vectors.
	*/
	vtkSetMacro(GenerateErrorVectors, int);
	vtkGetMacro(GenerateErrorVectors, int);
	vtkBooleanMacro(GenerateErrorVectors, int);

protected:
	vtkDiscreteMarchingCubesWithSmooth();
	~vtkDiscreteMarchingCubesWithSmooth() VTK_OVERRIDE;

	int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
		vtkInformationVector* outputVector) VTK_OVERRIDE;

	vtkWindowedSincPolyDataFilter* WindowedSincPolyDataFilter;
	int NumberOfIterations;
	double PassBand;
	int FeatureEdgeSmoothing;
	double FeatureAngle;
	double EdgeAngle;
	int BoundarySmoothing;
	int NonManifoldSmoothing;
	int GenerateErrorScalars;
	int GenerateErrorVectors;
	int NormalizeCoordinates;
private:
	vtkDiscreteMarchingCubesWithSmooth(const vtkDiscreteMarchingCubesWithSmooth&) VTK_DELETE_FUNCTION;
	void operator=(const vtkDiscreteMarchingCubesWithSmooth&) VTK_DELETE_FUNCTION;
};

#endif // !__VTK_DISCRETE_MARCHING_CUBES_WITH_SMOOTH_H__
