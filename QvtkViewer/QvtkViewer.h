/*!
 * \file Viewer.h
 * \date 2017/07/10 17:58
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong,
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief  This document is the abstract class for all viewers.
 *
 * TODO: long description
 *
 * \note
*/

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
/* This macro should be called after setupUi in the constructor of the subclasses */
//#define QVTK_INIT_VIEWER_MACRO() \
//    this->GetRenderWindow()->AddRenderer(this->firstRenderer); \
//    this->GetInteractor()->Initialize();
//#define QVTK_INIT_VIEWER_MACRO() \
//	this->AddRenderer(this->firstRenderer); \
//    this->GetRenderWindow()->AddRenderer(this->firstRenderer); \
//    this->GetInteractor()->Initialize();
namespace Q {
	namespace vtk {

		class QVTKVIEWER_EXPORT Viewer : public QWidget
		{
			Q_OBJECT
		public:
			typedef QHash<Prop*, vtkRenderer*> PropToRenderer;
			typedef QList<Viewer*> ViewerList;

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
			//{ return Viewer::s_cursorSource; }

			/************************************************************************/
			/* Membership functions
			/************************************************************************/

			explicit Viewer(QWidget* parent = nullptr);
			virtual ~Viewer() override;
			virtual void setupFirstRenderer(vtkRenderer *firstRenderer = nullptr);
			/**
			 * @brief vtkRenderer* AddRenderer
			 *
			 * Add a vtkRenderer to the vtkRenderWindow of this viewer
			 *
			 * @param int layer
			 * @return vtkRenderer*
			 */
			vtkRenderer* AddRenderer(int layer = -1);
			void         AddRenderer(vtkRenderer*, int layer = -1);

			void RemoveRenderer(int);
			void RemoveRenderer(vtkRenderer*);
			const QList<vtkRenderer*> GetRenderers() { return this->renderers; }
			//virtual vtkRenderer *getFirstRenderer() { return GetRenderers().first(); }

			/**
			 * @brief vtkCamera* GetActiveCamera
			 *
			 * Return the pointer to vtkCamera used by all layers of renderers.
			 *
			 * @return vtkCamera*
			 */

			vtkCamera* GetActiveCamera();

			/**
			 * @brief void ResetCamera
			 *
			 * Reset camera of this viewer. Specify layer if necessary.
			 *
			 * @param int layer
			 * @return void
			 */

			virtual void ResetCamera();
			virtual void ResetCamera(int layer);


			/**
			 * @brief void ResetCameraClippingRange
			 *
			 * Reset the clipping range of this viewer. Specify layer if necessary
			 *
			 * @param int layer
			 * @return void
			 */

			virtual void ResetCameraClippingRange();
			virtual void ResetCameraClippingRange(int layer);


			/**
			 * @brief void RenderAllViewersOfThisClass
			 *
			 * Find in the list all the viewers with the same class as the method caller and call Render()
			 *
			 * @return void
			 */

			 //virtual void update();
			virtual void RenderAllViewersOfThisClass();
			/**
			 * @brief AddProp
			 *
			 * Add a Prop to the renderer of layer = renlayer
			 *
			 * @param prop      Prop*
			 * @param renlayer  int
			 * @return void
			 */

			virtual void AddProp(Prop* prop);
			virtual void AddProp(Prop* prop, vtkRenderer* renderer);

			virtual QList<Prop*> GetProps();

			/**
			 * @brief void RemoveProp
			 *
			 * Remove prop from renlayer if exist.
			 * If renlayer = -1, the prop is searched and removed from all layers
			 *
			 * @param Prop * prop
			 * @param int renlayer
			 * @return void
			 */
			virtual void RemoveProp(Prop* prop);

			/**
			 * @brief RemoveProp
			 *
			 * Remove Prop in the list of rendering props
			 *
			 * @param propIndex Put argument desc here
			 * @return void
			 */

			 //void RemoveProp(int propIndex);

			 /**
			  * @brief RemoveAllProp
			  *
			  * Remove all the existing props from all renderers except for the cursor.
			  * Child class can re-implement this to keep their own actors untouched.
			  *
			  * @return void
			  */
			virtual void RemoveAllProp();
			virtual void RemoveAllProp(vtkRenderer* renderer);

			/**
			 * @brief SetCursorAlwaysFaceCamera
			 *
			 * Set whether the cursor should always face the camera and is along display-xy coordinate
			 *
			 * @param  bool
			 * @return void
			 */
			 //////////////////////////////////////////////////////////////////////////
			virtual vtkRenderWindow* GetRenderWindow() = 0;
			virtual vtkRenderWindowInteractor* GetInteractor() = 0;
			//////////////////////////////////////////////////////////////////////////
			virtual void SetCursorAlwaysFaceCamera(bool);

			/**
			 * @brief SetCursorDesync
			 *
			 * Set whether this viewer should use the static synced cursor
			 *
			 * @param  bool Set true to sync the cursor. False to desync the cursor
			 * @return void
			 */

			virtual void SetCursorDesyncFlag(bool desync);
			virtual bool GetCursorDesyncFlag() { return this->desyncCursorFlag; }

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
			/**
			 * @brief GetCursorTransform
			 *
			 * Obtain vtkTransform used by cursor
			 *
			 * @return vtkTransform*
			 */

			 //virtual vtkTransform* GetCursorTransform();
			vtkCursor3D* GetCursorSource() { return this->cursorSource; }
			vtkActor* GetCursorActor() { return this->cursorActor; }



			/**
			 * @brief void setEnableCornerAnnotation
			 *
			 * Show or hide the corner annotation text
			 *
			 * @param bool b True to show. False to hide
			 * @return void
			 */
			virtual void setEnableCornerAnnotation(bool b);

			/**
			 * @brief void SetCornerAnnotation
			 *
			 * Wrapper of the vtkCornerAnnotation::SetText() method
			 *
			 * @param int textPosition
			 * @param std::string text
			 * @return void
			 */

			virtual void AppendCornerAnnotation(int textPosition, QString text);
			/**
			 * @brief vtkCornerAnnotation* GetCornerAnnotation
			 *
			 * Return this vtkCornerAnnotation object. Use this to set properties to the text.
			 *
			 * @return vtkCornerAnnotation*
			 */
			virtual vtkCornerAnnotation* GetCornerAnnotation() { return this->cornerAnnotation; }


			virtual void SetEnableDepthPeeling(bool flag);

			virtual void SetMaxNoOfPeelings(int i);
			virtual int GetMaxNoOfPeeling() { return this->maxNoOfPeels; }

			virtual void SetOcclusionRatio(double ratio);
			virtual double GetOcclusionRatio() { return this->occlusionRatio; }

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
			/**
			 * @brief GetCursorPosition
			 *
			 * Get/Set the group index of this viewer. Viewers of the same group are linked when
			 * setting interactors styles
			 *
			 * @return double*
			 */
			int getViewerGroup() { return this->viewerGroup; }


			public slots:

			/**
			 * @brief SetCursorPosition
			 *
			 * Manually set cursor position.
			 *
			 * @param bool
			 * @return void
			 */
			virtual void SetCursorPosition(double x, double y, double z);
			void SetCursorPosition(const double pos[3]) { this->SetCursorPosition(pos[0], pos[1], pos[2]); }
		protected Q_SLOTS:
			virtual void UpdateCursorPosition(double x, double y, double z);
			void UpdateCursorPosition(const double xyz[3]) { this->UpdateCursorPosition(xyz[0], xyz[1], xyz[2]); }
		signals:
			void CursorPositionChanged(double x, double y, double z);
		protected:
			virtual void UpdateDepthPeeling();
			static ViewerList viewerList;
			/* This vector holds all the renderers layers */
			vtkCamera* camera;
			//vtkRenderer* firstRenderer;
			QList<vtkRenderer*> renderers;
			/// Membership variables
			CursorCallbackCommand* cursorCallback;
			vtkCursor3D*  cursorSource;
			vtkActor* cursorActor;
			vtkCornerAnnotation* cornerAnnotation;
			bool desyncCursorFlag;
			int viewerGroup;
			// prop's things. 
			PropToRenderer* propToRenderer;
			bool depthPeelingFlag;
			int maxNoOfPeels;
			double occlusionRatio;
		private:
			Q_DISABLE_COPY(Viewer);
		};

	}
}
#endif

