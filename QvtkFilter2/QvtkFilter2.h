#ifndef __QVTK_FILTER2_H__
#define __QVTK_FILTER2_H__
#pragma once
// me
#include "qvtkfilter2_export.h"
// qt
#include <QWidget>
namespace Q {
	namespace vtk {
		class QVTKFILTER2_EXPORT Filter2 : public QWidget
		{
			Q_OBJECT;
		public:
			explicit Filter2(QWidget *parent = nullptr);
			virtual ~Filter2() override;
		public Q_SLOTS:
			virtual void Update() = 0;
		Q_SIGNALS:
			void finished();
		private:
			 void run();
			 void finish();
			Q_DISABLE_COPY(Filter2);
		};

	}
}
#endif // !__QVTK_FILTER2_H__
