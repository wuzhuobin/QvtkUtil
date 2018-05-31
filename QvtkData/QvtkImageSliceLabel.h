#ifndef __QVTK_IMAGE_SLICE_LABEL_H__
#define __QVTK_IMAGE_SLICE_LABEL_H__
#pragma once
// me 
#include "QvtkImageSlice.h"
#include "QvtkLabelHelper.h"
// vtk
class vtkLookupTable;
class vtkNamedColors;
namespace Q {
	namespace vtk {
		class QVTKDATA_EXPORT ImageSliceLabel : 
			public ImageSlice, 
			public LabelHelper
		{
			Q_OBJECT;
			Q_VTK_DATA_H(ImageSliceLabel)
		public:
			static QString COLOR_DIR;
			ImageSliceLabel();
			virtual ~ImageSliceLabel() override;
			virtual void printSelf() const override;
			//virtual void namedColorsToLookupTable();
		protected:
			virtual Data *newInstance() const override { return new ImageSliceLabel; }
			//vtkLookupTable *lookupTable;
			//vtkNamedColors *namedColor;
		};
	}
}

#endif // !__QVTK_IMAGE_SLICE_LABEL_H__
