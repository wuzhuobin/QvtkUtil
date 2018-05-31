// me 
#include "QvtkImageSliceLabel.h"
// vtk
#include <vtkLookupTable.h>
#include <vtkNamedColors.h>
#include <vtkImageActor.h>
#include <vtkImageProperty.h>
#include <vtkImageMapper3D.h>
#include <vtkDiscretizableColorTransferFunction.h>
// qt
#include <QDebug>
#include <QFile>
#include <QTextStream>
namespace Q {
	namespace vtk {
		Q_VTK_DATA_CPP(ImageSliceLabel);
		QString ImageSliceLabel::COLOR_DIR("C:/Users/jieji/Desktop/ITK-SNAP-label.txt");
		ImageSliceLabel::ImageSliceLabel()
		{
			this->readLabelFile(COLOR_DIR);
			this->getImageActor()->GetProperty()->SetLookupTable(this->transferFunction);
			this->getImageActor()->GetProperty()->SetUseLookupTableScalarRange(true);
		}

		ImageSliceLabel::~ImageSliceLabel()
		{
			//this->lookupTable->Delete();
			//this->namedColor->Delete();
		}

		void ImageSliceLabel::printSelf() const
		{
		}
	}
}
