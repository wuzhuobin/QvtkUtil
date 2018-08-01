#ifndef __VTK_CONTOUR_WIDGET_SERIES_ORIENTED_GLYPH_REPRESENTATION_H__
#define __VTK_CONTOUR_WIDGET_SERIES_ORIENTED_GLYPH_REPRESENTATION_H__
// me 
#include "qvtkinteractorobserver_export.h"
// vtk
#include <vtkContourWidgetSeriesRepresentation.h>
class vtkOrientedGlyphContourRepresentation;
class QVTKINTERACTOROBSERVER_EXPORT vtkContourWidgetSeriesOrientedGlyphRepresentation: public vtkContourWidgetSeriesRepresentation
{
public:
	static vtkContourWidgetSeriesOrientedGlyphRepresentation* New();

	vtkTypeMacro(vtkContourWidgetSeriesOrientedGlyphRepresentation, vtkContourWidgetSeriesRepresentation);
	void PrintSelf(ostream& os, vtkIndent indent);

	// Description:
	// Get the Contour representations used for a particular seed. A side effect of
	// this method is that it will create a Contour representation in the list of
	// representations if one has not yet been created.
	virtual vtkContourRepresentation *GetContourRepresentation(unsigned int num) override;

protected:
	vtkContourWidgetSeriesOrientedGlyphRepresentation();
	virtual ~vtkContourWidgetSeriesOrientedGlyphRepresentation();

	//vtkOrientedGlyphContourRepresentation* ContourRepresentation = nullptr;

private:
	vtkContourWidgetSeriesOrientedGlyphRepresentation(const vtkContourWidgetSeriesOrientedGlyphRepresentation&);  //Not implemented
	void operator=(const vtkContourWidgetSeriesOrientedGlyphRepresentation&);  //Not implemented

};


#endif // !__VTK_CONTOUR_WIDGET_SERIES_ORIENTED_GLYPH_REPRESENTATION_H__
