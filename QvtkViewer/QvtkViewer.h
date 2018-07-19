
#ifndef __QVTK_VIEWER_H__
#define __QVTK_VIEWER_H__
#pragma once
// me
#include "qvtkviewer_export.h"
namespace Q {
	namespace vtk {
		class CursorCallbackCommand;
		class Prop;
	}
}
// qt
#include <QWidget>
template <class Key, class T>
class QHash;
// vtk
class vtkRenderer;
class vtkInteractorStyle;
class vtkCursor3D;
class vtkCornerAnnotation;
class vtkCamera;
class vtkRenderWindow;
class vtkRenderWindowInteractor;
class vtkActor;
class vtkMatrix4x4;
// std
#include <functional>
namespace Q {
	namespace vtk {
		class QVTKVIEWER_EXPORT Viewer : public QWidget
		{
			Q_OBJECT
		public:
			typedef QHash<Prop*, vtkRenderer*> PropToRenderer;
			typedef QList<Viewer*> ViewerList;
			//typedef void(*CursorTransformFunction)(double *pos, vtkMatrix4x4 *matrix, const Viewer *self);
			typedef std::function<void(double *pos, vtkMatrix4x4 *cursorMatrix, Viewer *self)> CursorTransformFunction;
			/************************************************************************/
			/* Static Functions
			/************************************************************************/

			/**
			 * @brief Viewer* getViewerOfInteractor
			 *
			 * Find in the list of viewers which uses the input interactorstyle
			 *
			 * @param vtkInteractorStyle *
			 * @return Viewer*
			 */
			static Viewer* getViewerOfInteractor(vtkInteractorStyle*);

			/**
			 * @brief void updateAllViewers
			 *
			 * Call Render() to all the viewers in the list
			 *
			 * @return void
			 */

			static void updateAllViewers();

			/**
			 * @brief updateViewersByGroup
			 *
			 * Call RendererWindow->Render() for all viewers with group input
			 *
			 * @param group int Input group
			 * @return void
			 */

			static void updateViewersByGroup(int group);


			/**
			 * @brief setGroupInteractorStyle
			 *
			 * Not Implemented. DO NOT USE
			 *
			 * @return void
			 */

			static void setGroupInteractorStyle(vtkInteractorStyle* style, int group);


			/**
			 * @brief GetAllViewers
			 *
			 * Return static vector of all viewers
			 *
			 * @return const QT_NAMESPACE::std::vector<QvtkABstractViewer*>
			 */

			static const ViewerList getAllViewers() { return Viewer::viewerList; }

			static unsigned int numOfViewers() { return Viewer::viewerList.size(); }

			static vtkCursor3D* getSyncCursorSource();

			/************************************************************************/
			/* Membership functions
			/************************************************************************/

			explicit Viewer(QWidget* parent = nullptr);
			virtual ~Viewer() override;
			virtual void setupFirstRenderer(vtkRenderer *firstRenderer = nullptr);
			/**
			 * @brief vtkRenderer* addRenderer
			 *
			 * Add a vtkRenderer to the vtkRenderWindow of this viewer
			 *
			 * @param int layer
			 * @return vtkRenderer*
			 */
			vtkRenderer* addRenderer(int layer = -1);
			void         addRenderer(vtkRenderer*, int layer = -1);

			void removeRenderer(int);
			void removeRenderer(vtkRenderer*);
			const QList<vtkRenderer*> getRenderers() { return this->renderers; }

			/**
			 * @brief vtkCamera* getActiveCamera
			 *
			 * Return the pointer to vtkCamera used by all layers of renderers.
			 *
			 * @return vtkCamera*
			 */

			vtkCamera* getActiveCamera() { return this->camera; }

			/**
			 * @brief void resetCamera
			 *
			 * Reset camera of this viewer. Specify layer if necessary.
			 *
			 * @param int layer
			 * @return void
			 */

			virtual void resetCamera();
			virtual void resetCamera(int layer);


			/**
			 * @brief void resetCameraClippingRange
			 *
			 * Reset the clipping range of this viewer. Specify layer if necessary
			 *
			 * @param int layer
			 * @return void
			 */

			virtual void resetCameraClippingRange();
			virtual void resetCameraClippingRange(int layer);


			/**
			 * @brief void updateAllViewersOfThisClass
			 *
			 * Find in the list all the viewers with the same class as the method caller and call Render()
			 *
			 * @return void
			 */

			virtual void update();
			virtual void updateAllViewersOfThisClass();
			/**
			 * @brief addProp
			 *
			 * Add a Prop to the renderer of layer = renlayer
			 *
			 * @param prop      Prop*
			 * @param renlayer  int
			 * @return void
			 */

			virtual void addProp(Prop* prop);
			virtual void addProp(Prop* prop, vtkRenderer* renderer);

			virtual QList<Prop*> getProps();

			/**
			 * @brief void removeProp
			 *
			 * Remove prop from renlayer if exist.
			 * If renlayer = -1, the prop is searched and removed from all layers
			 *
			 * @param Prop * prop
			 * @param int renlayer
			 * @return void
			 */
			virtual void removeProp(Prop* prop);

			/**
			 * @brief removeProp
			 *
			 * Remove Prop in the list of rendering props
			 *
			 * @param propIndex Put argument desc here
			 * @return void
			 */

			 //void removeProp(int propIndex);

			 /**
			  * @brief removeAllProp
			  *
			  * Remove all the existing props from all renderers except for the cursor.
			  * Child class can re-implement this to keep their own actors untouched.
			  *
			  * @return void
			  */
			virtual void removeAllProp();
			virtual void removeAllProp(vtkRenderer* renderer);

			/**
			 * @brief setCursorAlwaysFaceCamera
			 *
			 * Set whether the cursor should always face the camera and is along display-xy coordinate
			 *
			 * @param  bool
			 * @return void
			 */
			 //////////////////////////////////////////////////////////////////////////
			virtual vtkRenderWindow* getRenderWindow() = 0;
			virtual vtkRenderWindowInteractor* getInteractor() = 0;
			//////////////////////////////////////////////////////////////////////////

			/**
			 * @brief SetCursorDesync
			 *
			 * Set whether this viewer should use the static synced cursor
			 *
			 * @param  bool Set true to sync the cursor. False to desync the cursor
			 * @return void
			 */

			virtual bool getCursorSyncFlag()const { return this->cursorSyncFlag; }
			virtual bool getCursorAnnotationFlag() const { return this->cursorAnnotationFlag; }
			/**
			 * @brief GetCursorPosition
			 *
			 * Returns the pointer of the physical location of current cursor actor
			 *
			 * @return double*
			 */
			virtual const double* getCursorPosition() const;
			virtual void getCursorPosition(double pos[3]) const {
				pos[0] = this->getCursorPosition()[0];
				pos[1] = this->getCursorPosition()[1];
				pos[2] = this->getCursorPosition()[2];
			}
			vtkCursor3D* getCursorSource() const;
			vtkActor* getCursorActor() const { return this->cursorActor; }
			/**
			 * @brief vtkCornerAnnotation* getCornerAnnotation
			 *
			 * Return this vtkCornerAnnotation object. Use this to set properties to the text.
			 *
			 * @return vtkCornerAnnotation*
			 */
			virtual vtkCornerAnnotation* getCornerAnnotation() { return this->cornerAnnotation; }


			virtual int getMaxNoOfPeeling() { return this->maxNoOfPeels; }

			virtual double getOcclusionRatio() { return this->occlusionRatio; }
			/**
			 * @brief GetCursorPosition
			 *
			 * Get/Set the group index of this viewer. Viewers of the same group are linked when
			 * setting interactors styles
			 *
			 * @return double*
			 */
			int getViewerGroup() { return this->viewerGroup; }


		public Q_SLOTS:
			virtual void setCursorAlwaysFaceCamera(bool);
			virtual void setCursorSyncFlag(bool desync);
			/**
			 * @brief setCursorPosition
			 *
			 * Manually set cursor position.
			 *
			 * @param bool
			 * @return void
			 */
			virtual void setCursorPosition(double x, double y, double z);
			void setCursorPosition(const double pos[3]) { this->setCursorPosition(pos[0], pos[1], pos[2]); }
			virtual void setCursorTransformFunction(CursorTransformFunction cursorTransformFunction) { this->cursorTransformFunction = cursorTransformFunction; }
			virtual void setEnableDepthPeeling(bool flag);
			virtual void setMaxNoOfPeelings(int i);
			virtual void setOcclusionRatio(double ratio);
			/**
			 * @brief void setCursorAnnotation
			 *
			 * Show or hide the corner annotation text
			 *
			 * @param bool b True to show. False to hide
			 * @return void
			 */
			virtual void setCursorAnnotation(bool b);

			/**
			 * @brief SetViewerGroup
			 *
			 * Get/Set the group index of this viewer. Viewers of the same group are linked when
			 * setting interactors styles
			 *
			 * @param group Put argument desc here
			 * @return void
			 */
			virtual void setViewerGroup(int group) { this->viewerGroup = group; }
		Q_SIGNALS:
			void cursorPositionChanged(double x, double y, double z);
		protected:
			virtual void updateDepthPeeling();
			// prop's things. 
			PropToRenderer* propToRenderer;
		protected Q_SLOTS:
			virtual void updateCursorPosition(double x, double y, double z);
			void updateCursorPosition(const double xyz[3]) { this->updateCursorPosition(xyz[0], xyz[1], xyz[2]); }
		private:
			Q_DISABLE_COPY(Viewer);
			static ViewerList viewerList;
			int viewerGroup;
			bool cursorSyncFlag;
			bool cursorAnnotationFlag;
			bool depthPeelingFlag;
			int maxNoOfPeels;
			double occlusionRatio;
			vtkCamera* camera;
			QList<vtkRenderer*> renderers;
			/// Membership variables
			vtkCursor3D*  cursorSource;
			vtkActor* cursorActor;
			mutable double cursorPosition[4];
			friend class CursorCallbackCommand;
			CursorCallbackCommand* cursorCallback;
			CursorTransformFunction cursorTransformFunction;
			vtkCornerAnnotation* cornerAnnotation;
		};

	}
}
#endif

