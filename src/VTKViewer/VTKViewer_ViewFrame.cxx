using namespace std;
//  File      : VTKViewer_ViewFrame.cxx
//  Created   : Wed Mar 20 11:29:40 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : VTKViewer
//  Copyright : Open CASCADE 2002
//  $Header$

#include "VTKViewer_ViewFrame.h"
#include "VTKViewer_RenderWindow.h"
#include "VTKViewer_InteractorStyleSALOME.h"

#include "QAD_Settings.h"
#include "QAD_Config.h"
#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "SALOME_Selection.h"
#include "SALOME_InteractiveObject.hxx"

#include "utilities.h"

//QT Include
#include <qlayout.h>
#include <qcolordialog.h>
#include <qfiledialog.h>
#include <qapplication.h>

// VTK Includes
#include <vtkMath.h>
#include <vtkTextSource.h>
#include <vtkLine.h>
#include <vtkConeSource.h>
#include <vtkTextMapper.h>
#include <vtkMapper2D.h>
#include <vtkActor2D.h>
#include <vtkWindowToImageFilter.h>
#include <vtkTIFFWriter.h>

#include <vtkTransformPolyDataFilter.h>
#include <vtkDataSetMapper.h> 

/*!
    Constructor
*/
VTKViewer_ViewFrame::VTKViewer_ViewFrame(QWidget* parent, const char* name) 
  :  QAD_ViewFrame(parent, name)
{
  m_ViewUp[0] = 0; m_ViewUp[1] = 0; m_ViewUp[2] = -1;
  m_ViewNormal[0] = 0; m_ViewNormal[1] = 0; m_ViewNormal[2] = 1;

  //  m_InitialSetupDone = false ;
  InitialSetup();

  m_NonIsometricTransform = NULL;

}


static vtkActor* CreateTextActor(char *text) {
    
  vtkTextSource* XLabel = vtkTextSource::New();
  XLabel->SetForegroundColor(1,1,1);
  XLabel->SetBackgroundColor(0,0,0);
  XLabel->SetText(text);

  vtkActor* XLabelActor =  vtkActor::New();
  
  vtkPolyDataMapper* text1Mapper = vtkPolyDataMapper::New();
  text1Mapper->SetInput(XLabel->GetOutput());

  XLabelActor->SetMapper(text1Mapper);
  XLabelActor->SetScale(1,1,1);

  return XLabelActor;
}

void VTKViewer_ViewFrame::AddVector(float* o,float* p,vtkRenderer* renderer) {
  
  // Create line

  vtkPoints* myPoints = vtkPoints::New();
  vtkLine* myLine = vtkLine::New();

  myPoints->InsertNextPoint(o);
  myPoints->InsertNextPoint(p);

  (myLine->GetPointIds())->InsertNextId(0);
  (myLine->GetPointIds())->InsertNextId(1);

  vtkActor* lineActor = vtkActor::New();

  vtkCellArray* cell = vtkCellArray::New();

  cell->InsertNextCell(myLine);

  vtkPolyData* output = vtkPolyData::New();
  
  output->SetPoints(myPoints);
  output->SetLines(cell);
 
  vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();

  mapper->SetInput(output);

  lineActor->SetMapper(mapper);

  // Create CONE

  vtkConeSource* acone =  vtkConeSource::New();

  float dim;
  QString Size = QAD_CONFIG->getSetting("Viewer:TrihedronSize");
  if( Size.isEmpty() ) {
    dim = 100;
  } else {
    dim = Size.toFloat();
  }

  acone->SetResolution(2);
  //  acone->SetAngle(70);
  acone->SetRadius(0.02*dim);
  acone->SetHeight(0.08*dim);

  vtkActor* coneActor = vtkActor::New();
 
  vtkPolyDataMapper* coneMapper = vtkPolyDataMapper::New();
  coneMapper->SetInput(acone->GetOutput());

  coneActor->SetMapper(coneMapper);
  float rot[3];
  rot[0]=0; rot[1]=0; rot[2]=0;

  if(p[0]!=0) {
    // x
    coneActor->AddPosition(p);
  }
  else {
    if(p[1]!=0) {
      // y
      coneActor->AddPosition(p);
      rot[2]=90;
      coneActor->AddOrientation(rot);
    }
    else {
      if(p[2]!=0) {
	// z
	coneActor->AddPosition(p);   
	rot[1]=-90;
	coneActor->AddOrientation(rot);
      }
    }
  }

  // Create 2d TEXT

  coneActor->GetProperty()->SetInterpolation(1);
  coneActor->GetProperty()->SetRepresentationToSurface();
  coneActor->GetProperty()->SetAmbient(1);
  coneActor->GetProperty()->SetAmbientColor(1,1,1);
  coneActor->GetProperty()->SetDiffuseColor(0.7,0.7,0.7);
  coneActor->GetProperty()->SetSpecularColor(0.7,0.7,0.7);

  lineActor->GetProperty()->SetInterpolation(1);
  lineActor->GetProperty()->SetRepresentationToSurface();
  lineActor->GetProperty()->SetAmbient(1);
  lineActor->GetProperty()->SetAmbientColor(1,1,1);
  lineActor->GetProperty()->SetDiffuseColor(0.7,0.7,0.7);
  lineActor->GetProperty()->SetSpecularColor(0.7,0.7,0.7);
     
  coneActor->PickableOff();
  lineActor->PickableOff();
  
  m_Triedron->AddItem(coneActor);
  m_Triedron->AddItem(lineActor);

  renderer->AddActor(coneActor);
  renderer->AddActor(lineActor);
}  

bool VTKViewer_ViewFrame::isTrihedronDisplayed()
{
  m_Triedron->InitTraversal();
  vtkActor *ac = m_Triedron->GetNextActor();
  while(!(ac==NULL)) {
    if(ac->GetVisibility()) return true;
    ac = m_Triedron->GetNextActor();
  }
  return false;
}

void VTKViewer_ViewFrame::SetTrihedronSize(int size)
{
  m_Triedron->InitTraversal();
  vtkActor* anActor = m_Triedron->GetNextActor();
  while(!(anActor==NULL)) {  
    m_Renderer->RemoveActor( anActor );
    anActor = m_Triedron->GetNextActor();
  }

  m_Triedron->RemoveAllItems();
  AddAxis(m_Renderer);
  m_RW->updateGL();
}


void VTKViewer_ViewFrame::AddAxis(vtkRenderer* renderer) {
  
  float origine[3];
  float X[3];
  float Y[3];
  float Z[3];
  float dim;

  QString Size = QAD_CONFIG->getSetting("Viewer:TrihedronSize");
  if( Size.isEmpty() ){
    dim = 100;
  } else {
    dim = Size.toFloat();
  }

  origine[0]=0;        origine[1]=0;        origine[2]=0;
  X[0]=origine[0]+dim; X[1]=origine[0];     X[2]=origine[0];
  Y[0]=origine[0];     Y[1]=origine[0]+dim; Y[2]=origine[0];
  Z[0]=origine[0];     Z[1]=origine[0];     Z[2]=origine[0]+dim;

  AddVector(origine,X,renderer);
  AddVector(origine,Y,renderer);
  AddVector(origine,Z,renderer);
 
}

/*!
  Returns widget containing 3D-Viewer
*/
QWidget* VTKViewer_ViewFrame::getViewWidget() 
{
  return (QWidget*)getRW();
}


void VTKViewer_ViewFrame::setRW(VTKViewer_RenderWindow* rw) {
  m_RW = rw;
}


void VTKViewer_ViewFrame::InitialSetup() {
  //
  // Create a render window.
  //
  m_RW = new VTKViewer_RenderWindow(this,"");
  //
  // Create a renderer for this widget.
  //
  m_Renderer = vtkRenderer::New() ;
  m_RW->AddRenderer(m_Renderer) ;


  vtkCamera* camera = vtkCamera::New();
  camera->SetPosition(1,-1,1);
  camera->SetParallelProjection(true);
  camera->SetRoll(-60);

  // CREATE AXIS
  m_Triedron = vtkActorCollection::New();
  AddAxis(m_Renderer);
 
  // Set BackgroundColor
  QString BackgroundColorRed   = QAD_CONFIG->getSetting("VTKViewer:BackgroundColorRed");
  QString BackgroundColorGreen = QAD_CONFIG->getSetting("VTKViewer:BackgroundColorGreen");
  QString BackgroundColorBlue  = QAD_CONFIG->getSetting("VTKViewer:BackgroundColorBlue");

  if( !BackgroundColorRed.isEmpty() && !BackgroundColorGreen.isEmpty() && !BackgroundColorBlue.isEmpty() ) 
    m_Renderer->SetBackground( BackgroundColorRed.toInt()/255., BackgroundColorGreen.toInt()/255., BackgroundColorBlue.toInt()/255. );
  else
    m_Renderer->SetBackground( 0, 0 , 0 );

  //
  // Create an interactor.
  //
  m_RWInteractor = VTKViewer_RenderWindowInteractor::New() ;
  m_RWInteractor->SetRenderWindow(m_RW) ;
  //
  // :TRICKY: Tue May  2 19:29:36 2000 Pagey
  // The order of the next two statements is very 
  // important. The interactor must be initialized
  // before rendering. 
  //
  m_RWInteractor->Initialize();
  m_Renderer->Render() ;
  m_Renderer->SetActiveCamera(camera);
  m_Renderer->ResetCamera();  
  camera->Zoom(0.3);
  VTKViewer_InteractorStyleSALOME* RWS = dynamic_cast<VTKViewer_InteractorStyleSALOME*>(getRWInteractor()->GetInteractorStyle());
  if (RWS) {
    RWS->setTriedron( m_Triedron );
    //SRN: additional initialization, to init CurrentRenderer of vtkInteractorStyle 
    RWS->FindPokedRenderer(0, 0);
  }

  m_RW->updateGL() ;

  setCentralWidget( m_RW );
}

VTKViewer_ViewFrame::~VTKViewer_ViewFrame() {
  //
  // In order to ensure that the interactor unregisters
  // this RenderWindow, we assign a NULL RenderWindow to 
  // it before deleting it.
  //
  if ( m_NonIsometricTransform )
    m_NonIsometricTransform->Delete() ;
    
  m_RWInteractor->SetRenderWindow(NULL) ;
  m_RWInteractor->Delete() ;
  
  m_RW->Delete() ;

  // NRI : BugID 1137.
  //  m_Renderer->Delete() ;
}


/*!
  Display/hide Trihedron
*/
void VTKViewer_ViewFrame::onViewTrihedron()
{
  if (isTrihedronDisplayed()) {
    m_Triedron->InitTraversal();
    vtkActor *ac = m_Triedron->GetNextActor();
    while(!(ac==NULL)) {
      ac->VisibilityOff();
      ac = m_Triedron->GetNextActor();
    }
  }
  else {
    m_Triedron->InitTraversal();
    vtkActor *ac = m_Triedron->GetNextActor();
    while(!(ac==NULL)) {
      ac->VisibilityOn();
      ac = m_Triedron->GetNextActor();
    }
    m_TriedronVisible = true;
  }  
  m_RW->updateGL();
}

/*!
  Provides top projection of the active view
*/
void VTKViewer_ViewFrame::onViewTop() {
  vtkCamera* camera = vtkCamera::New();
  camera->SetPosition(0,0,1);
  camera->SetParallelProjection(true);
  m_Renderer->SetActiveCamera(camera);
//  m_Renderer->ResetCamera();  
  onViewFitAll();
//  m_RW->updateGL();
}

/*!
  Provides bottom projection of the active view
*/
void VTKViewer_ViewFrame::onViewBottom()
{
  vtkCamera* camera = vtkCamera::New();
  camera->SetPosition(0,0,-1);
  camera->SetParallelProjection(true);
  m_Renderer->SetActiveCamera(camera);
//  m_Renderer->ResetCamera();  
  onViewFitAll();
//  m_RW->updateGL();
}

/*!
  Provides left projection of the active view
*/
void VTKViewer_ViewFrame::onViewLeft()    
{
  vtkCamera* camera = vtkCamera::New();
  camera->SetPosition(0,1,0);
  camera->SetParallelProjection(true);
  m_Renderer->SetActiveCamera(camera);
//  m_Renderer->ResetCamera();  
  onViewFitAll();
//  m_RW->updateGL(); 
}

/*!
  Provides right projection of the active view
*/
void VTKViewer_ViewFrame::onViewRight()
{
  vtkCamera* camera = vtkCamera::New();
  camera->SetPosition(0,-1,0);
  camera->SetParallelProjection(true);
  m_Renderer->SetActiveCamera(camera);
//  m_Renderer->ResetCamera();  
  onViewFitAll();
//  m_RW->updateGL();
}

/*!
  Provides back projection of the active view
*/
void VTKViewer_ViewFrame::onViewBack()
{
  vtkCamera* camera = vtkCamera::New();
  camera->SetPosition(-1,0,0);
  camera->SetParallelProjection(true);
  m_Renderer->SetActiveCamera(camera);
//  m_Renderer->ResetCamera();  
  onViewFitAll();
//  m_RW->updateGL();
}

/*!
  Provides front projection of the active view
*/
void VTKViewer_ViewFrame::onViewFront()
{
  vtkCamera* camera = vtkCamera::New();
  camera->SetPosition(1,0,0);
  camera->SetParallelProjection(true);
  m_Renderer->SetActiveCamera(camera);
//  m_Renderer->ResetCamera();  
  onViewFitAll();
//  m_RW->updateGL();
}

/*!
  Reset the active view
*/
void VTKViewer_ViewFrame::onViewReset()    
{
  vtkCamera* camera = vtkCamera::New();
  camera->SetPosition(1,-1,1);
  camera->SetParallelProjection(true);
  camera->SetRoll(-60);
  m_Renderer->SetActiveCamera(camera);
  m_Renderer->ResetCamera();  
  m_Renderer->ResetCameraClippingRange();
  camera->Zoom(0.3);
  m_RW->updateGL();
}

/*!
  Rotates the active view
*/
void VTKViewer_ViewFrame::onViewRotate()
{
  VTKViewer_InteractorStyleSALOME* RWS = dynamic_cast<VTKViewer_InteractorStyleSALOME*>(getRWInteractor()->GetInteractorStyle());
  if (RWS)
    RWS->startRotate();
}

/*!
  Sets a new center of the active view
*/
void VTKViewer_ViewFrame::onViewGlobalPan()
{
  VTKViewer_InteractorStyleSALOME* RWS = dynamic_cast<VTKViewer_InteractorStyleSALOME*>(getRWInteractor()->GetInteractorStyle());
  if (RWS)
    RWS->startGlobalPan();
}

/*!
  Zooms the active view
*/
void VTKViewer_ViewFrame::onViewZoom()
{
  VTKViewer_InteractorStyleSALOME* RWS = dynamic_cast<VTKViewer_InteractorStyleSALOME*>(getRWInteractor()->GetInteractorStyle());
  if (RWS)
    RWS->startZoom();
}

/*!
  Moves the active view
*/
void VTKViewer_ViewFrame::onViewPan()
{
  VTKViewer_InteractorStyleSALOME* RWS = dynamic_cast<VTKViewer_InteractorStyleSALOME*>(getRWInteractor()->GetInteractorStyle());
  if (RWS)
    RWS->startPan();
}

/*!
  Fits all obejcts within a rectangular area of the active view
*/
void VTKViewer_ViewFrame::onViewFitArea()
{

  VTKViewer_InteractorStyleSALOME* RWS = dynamic_cast<VTKViewer_InteractorStyleSALOME*>(getRWInteractor()->GetInteractorStyle());
  if (RWS)
    RWS->startFitArea();
}

/*!
  Fits all objects in the active view
*/
// Reset the camera clipping range to include this entire bounding box
static void ResetCameraClippingRange(vtkRenderer* theRenderer, float bounds[6] )
{
  //see vtkRenderer::ResetCameraClippingRange(float bounds[6]) method
  double  vn[3], position[3], a, b, c, d;
  double  range[2], dist;
  int     i, j, k;
  float center[3];
  float distance;
  float width;

  vtkCamera* anActiveCamera = theRenderer->GetActiveCamera();
  if ( anActiveCamera == NULL )
    {
      //vtkErrorMacro(<< "Trying to reset clipping range of non-existant camera");
    return;
    }
  
  // Find the plane equation for the camera view plane
  anActiveCamera->GetViewPlaneNormal(vn);
  anActiveCamera->GetPosition(position);
//  a = -vn[0];
//  b = -vn[1];
//  c = -vn[2];
//  d = -(a*position[0] + b*position[1] + c*position[2]);

  // Set the max near clipping plane and the min far clipping plane
//  range[0] = a*bounds[0] + b*bounds[2] + c*bounds[4] + d;
//  range[1] = 1e-18;

  // Find the closest / farthest bounding box vertex
//  for ( k = 0; k < 2; k++ )
//    {
//    for ( j = 0; j < 2; j++ )
//	  {
//	  for ( i = 0; i < 2; i++ )
//	    {
//	    dist = a*bounds[i] + b*bounds[2+j] + c*bounds[4+k] + d;
//	    range[0] = (dist<range[0])?(dist):(range[0]);
//	    range[1] = (dist>range[1])?(dist):(range[1]);
//	    }
//	  }
//    }
  
  center[0] = (bounds[0] + bounds[1])/2.0;
  center[1] = (bounds[2] + bounds[3])/2.0;
  center[2] = (bounds[4] + bounds[5])/2.0;
  width = sqrt((bounds[1]-bounds[0])*(bounds[1]-bounds[0]) +
	       (bounds[3]-bounds[2])*(bounds[3]-bounds[2]) +
	       (bounds[5]-bounds[4])*(bounds[5]-bounds[4]));
  distance = sqrt((position[0]-center[0])*(position[0]-center[0]) +
		  (position[1]-center[1])*(position[1]-center[1]) +
		  (position[2]-center[2])*(position[2]-center[2]));
  range[0] = distance - width/2.0;
  range[1] = distance + width/2.0;

  // Give ourselves a little breathing room
  range[0] = 0.99*range[0] - (range[1] - range[0])*0.5;
  range[1] = 1.01*range[1] + (range[1] - range[0])*0.5;

  // Make sure near is not bigger than far
  range[0] = (range[0] >= range[1])?(0.01*range[1]):(range[0]);

  // Make sure near is at least some fraction of far - this prevents near
  // from being behind the camera or too close in front. How close is too
  // close depends on the resolution of the depth buffer
  int ZBufferDepth = 16;
  vtkRenderWindow* aRenderWindow = theRenderer->GetRenderWindow();
  if (aRenderWindow)
    {
      ZBufferDepth = aRenderWindow->GetDepthBufferSize();
    }
  //
  if ( ZBufferDepth <= 16 )
    {
    range[0] = (range[0] < 0.01*range[1])?(0.01*range[1]):(range[0]);
    }
  else if ( ZBufferDepth <= 24 )
    {
    range[0] = (range[0] < 0.01*range[1])?(0.01*range[1]):(range[0]);
    }
  else
    {
    range[0] = (range[0] < 0.01*range[1])?(0.01*range[1]):(range[0]);
    }
  anActiveCamera->SetClippingRange( range );
}

static void ResetCamera(vtkRenderer* theRenderer){  
  //see vtkRenderer::ResetCamera(float bounds[6]) method
  float      bounds[6];
  if(!theRenderer) return;
  theRenderer->ComputeVisiblePropBounds( bounds );

  float center[3];
  float distance;
  float width;
  double vn[3], *vup;
  
  if ( theRenderer->GetActiveCamera() != NULL )
    {
    theRenderer->GetActiveCamera()->GetViewPlaneNormal(vn);
    }
  else
    {
    MESSAGE("Trying to reset non-existant camera");
    return;
    }

  center[0] = (bounds[0] + bounds[1])/2.0;
  center[1] = (bounds[2] + bounds[3])/2.0;
  center[2] = (bounds[4] + bounds[5])/2.0;
  width = sqrt((bounds[1]-bounds[0])*(bounds[1]-bounds[0]) +
	       (bounds[3]-bounds[2])*(bounds[3]-bounds[2]) +
	       (bounds[5]-bounds[4])*(bounds[5]-bounds[4]));
  double ang = theRenderer->GetActiveCamera()->GetViewAngle();
  distance = 2.0*width/tan(ang*vtkMath::Pi()/360.0);
  // check view-up vector against view plane normal
  vup = theRenderer->GetActiveCamera()->GetViewUp();
  if ( fabs(vtkMath::Dot(vup,vn)) > 0.999 )
    {
    MESSAGE("Resetting view-up since view plane normal is parallel");
    theRenderer->GetActiveCamera()->SetViewUp(-vup[2], vup[0], vup[1]);
    }

  // update the camera
  theRenderer->GetActiveCamera()->SetFocalPoint(center[0],center[1],center[2]);
  theRenderer->GetActiveCamera()->SetPosition(center[0]+distance*vn[0],
                                  center[1]+distance*vn[1],
                                  center[2]+distance*vn[2]);
  // setup default parallel scale
  theRenderer->GetActiveCamera()->SetParallelScale(width/2.0);
  //workaround on VTK
  //theRenderer->ResetCameraClippingRange(bounds);
  ResetCameraClippingRange(theRenderer,bounds);
}

void VTKViewer_ViewFrame::onViewFitAll()
{
  Standard_Boolean TriedronWasVisible = false;
  if (isTrihedronDisplayed()) {
    m_Triedron->InitTraversal();
    vtkActor *ac = m_Triedron->GetNextActor();
    while(!(ac==NULL)) {
      ac->VisibilityOff();
      ac = m_Triedron->GetNextActor();
    }
    TriedronWasVisible = true;
  }
  bool hasVisibleActors = m_Renderer->VisibleActorCount() > 0;
  if ( hasVisibleActors )    // if there are visible actors, not to take into account Trihedron
    ResetCamera(m_Renderer); 
  if(TriedronWasVisible) {
    m_Triedron->InitTraversal();
    vtkActor *ac = m_Triedron->GetNextActor();
    while(!(ac==NULL)) {
      ac->VisibilityOn();
      ac = m_Triedron->GetNextActor();
    }
    if ( !hasVisibleActors ) // if there are NO visible actors, fit view to see only Trihedron
      ResetCamera(m_Renderer); 
  }
  //m_Renderer->ResetCameraClippingRange();
  m_RW->updateGL();
}

/*!
    Set background of the viewport
*/
void VTKViewer_ViewFrame::setBackgroundColor( const QColor& color)
{
  if ( m_Renderer )
    m_Renderer->SetBackground( color.red()/255., color.green()/255., color.blue()/255. );
}

/*!
    Returns background of the viewport
*/
QColor VTKViewer_ViewFrame::backgroundColor() const
{
  float backint[3];
  if ( m_Renderer ) {
    m_Renderer->GetBackground(backint);
    return QColorDialog::getColor ( QColor(int(backint[0]*255), int(backint[1]*255), int(backint[2]*255)), NULL );
  }
  return QMainWindow::backgroundColor();
}


void VTKViewer_ViewFrame::SetSelectionMode( int mode )
{
  m_RWInteractor->SetSelectionMode( mode );
}

void VTKViewer_ViewFrame::rename( const Handle(SALOME_InteractiveObject)& IObject, QString newName )
{
  m_RWInteractor->rename(IObject, newName);
}

void VTKViewer_ViewFrame::unHighlightAll() 
{
  m_RWInteractor->unHighlightAll();
}

void VTKViewer_ViewFrame::highlight( const Handle(SALOME_InteractiveObject)& IObject, 
				     bool highlight, 
				     bool update ) 
{
  QAD_Study* ActiveStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel = SALOME_Selection::Selection( ActiveStudy->getSelection() );

  if ( Sel->SelectionMode() == 4 )
    m_RWInteractor->highlight(IObject, highlight, update);
  else if ( Sel->SelectionMode() == 3 ) {
    m_RWInteractor->highlight(IObject, highlight, update);
    if ( Sel->HasIndex( IObject ) ) {
      TColStd_MapOfInteger MapIndex;
      Sel->GetIndex( IObject, MapIndex );
      m_RWInteractor->highlightCell(IObject, highlight, MapIndex, update);
    }
  } 
  else if ( Sel->SelectionMode() == 2 ) {
    m_RWInteractor->highlight(IObject, highlight, update);
    if ( Sel->HasIndex( IObject ) ) {
      TColStd_MapOfInteger MapIndex;
      Sel->GetIndex( IObject, MapIndex );
      m_RWInteractor->highlightEdge(IObject, highlight, MapIndex, update);
    }
  }
  else if ( Sel->SelectionMode() == 1 ) {
    m_RWInteractor->highlight(IObject, highlight, update);
    if ( Sel->HasIndex( IObject ) ) {
      TColStd_MapOfInteger MapIndex;
      Sel->GetIndex( IObject, MapIndex );
      m_RWInteractor->highlightPoint(IObject, highlight, MapIndex, update);
    }
  }
}

bool VTKViewer_ViewFrame::isInViewer( const Handle(SALOME_InteractiveObject)& IObject ) 
{
  return m_RWInteractor->isInViewer( IObject );
}

bool VTKViewer_ViewFrame::isVisible( const Handle(SALOME_InteractiveObject)& IObject ) 
{
  return m_RWInteractor->isVisible( IObject );
}

void VTKViewer_ViewFrame::setPopupServer( QAD_Application* App )
{
  m_RW->setPopupServer( App );
}

void VTKViewer_ViewFrame::undo(SALOMEDS::Study_var aStudy,
			       const char* StudyFrameEntry)
{
  vtkActorCollection* theActors = m_Renderer->GetActors();
  theActors->InitTraversal();
  vtkActor *ac = theActors->GetNextActor();
  while(!(ac==NULL)) {
    if ( ac->IsA("SALOME_Actor") ) {
      SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
      if ( anActor->hasIO() ) {
	Handle(SALOME_InteractiveObject) IO = anActor->getIO();
	if ( IO->hasEntry() ) { 
	  if (!QAD_ViewFrame::isInViewer(aStudy, IO->getEntry(), StudyFrameEntry)) {
	    m_RWInteractor->Erase(IO);
	  }
	}
      }
    }
    ac = theActors->GetNextActor();
  }
}

void VTKViewer_ViewFrame::redo(SALOMEDS::Study_var aStudy,
			       const char* StudyFrameEntry)
{
  SALOMEDS::SObject_var RefSO;
  SALOMEDS::SObject_var SO = aStudy->FindObjectID( StudyFrameEntry );
  SALOMEDS::ChildIterator_var it = aStudy->NewChildIterator(SO);
  for (; it->More();it->Next()){
    SALOMEDS::SObject_var CSO= it->Value();
    if (CSO->ReferencedObject(RefSO)) {
      vtkActorCollection* theActors = m_Renderer->GetActors();
      theActors->InitTraversal();
      vtkActor *ac = theActors->GetNextActor();
      while(!(ac==NULL)) {
	if ( ac->IsA("SALOME_Actor") ) {
	  SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	  if ( anActor->hasIO() ) {
	    Handle(SALOME_InteractiveObject) IO = anActor->getIO();
	    if ( IO->hasEntry() ) { 
	      if ( strcmp(IO->getEntry(),RefSO->GetID()) == 0 )
		m_RWInteractor->Display(IO);
	    }
	  }
	}
	ac = theActors->GetNextActor();
      }
    }
  }
}


/* selection */
Handle(SALOME_InteractiveObject) VTKViewer_ViewFrame::FindIObject(const char* Entry)
{
  Handle(SALOME_InteractiveObject) IO;
  vtkActorCollection* theActors = m_Renderer->GetActors();
  theActors->InitTraversal();
  vtkActor *ac = theActors->GetNextActor();
  while(!(ac==NULL)) {
    if ( ac->IsA("SALOME_Actor") ) {
      SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
      if ( anActor->hasIO() ) {
	IO = anActor->getIO();
	if ( IO->hasEntry() ) {
	  if ( strcmp( IO->getEntry(), Entry ) == 0 ) {
	    return IO;
	  }
	}
      }
    }
    ac = theActors->GetNextActor();
  }
  return IO;
}

/* display */		
void VTKViewer_ViewFrame::Display(const Handle(SALOME_InteractiveObject)& IObject, bool update)
{
  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel
    = SALOME_Selection::Selection( myStudy->getSelection() );

  vtkActorCollection* theActors = m_Renderer->GetActors();
  theActors->InitTraversal();
  vtkActor *ac = theActors->GetNextActor();
  while(!(ac==NULL))
    {
      if ( ac->IsA("SALOME_Actor") )
	{
	  SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	  if ( anActor->hasIO() ) 
	    {
	      Handle(SALOME_InteractiveObject) IO = anActor->getIO();
	      if ( IO->isSame(IObject) )
		{
		  m_RWInteractor->Display(IO, false);
		  Sel->AddIObject(IO, false);
		  break;
		}
	    }
	}
      ac = theActors->GetNextActor();
    }
  if (update)
    Repaint();
}


void VTKViewer_ViewFrame::DisplayOnly(const Handle(SALOME_InteractiveObject)& IObject)
{
  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel
    = SALOME_Selection::Selection( myStudy->getSelection() );

  vtkActorCollection* theActors = m_Renderer->GetActors();
  theActors->InitTraversal();
  vtkActor *ac = theActors->GetNextActor();
  while(!(ac==NULL)) {
    if ( ac->IsA("SALOME_Actor") ) {
      SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
      if ( anActor->hasIO() ) {
	Handle(SALOME_InteractiveObject) IO = anActor->getIO();
	if ( !IO->isSame(IObject) ) {
	  m_RWInteractor->Erase(IO, false);
	  Sel->RemoveIObject(IO, false);
	} else {
	  anActor->SetVisibility(true);
	  Sel->AddIObject(IO, false);
	}
      }
    }
    ac = theActors->GetNextActor();
  }
  Repaint();
}

void VTKViewer_ViewFrame::Erase(const Handle(SALOME_InteractiveObject)& IObject, bool update)
{
  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel
    = SALOME_Selection::Selection( myStudy->getSelection() );

  vtkActorCollection* theActors = m_Renderer->GetActors();
  theActors->InitTraversal();
  vtkActor *ac = theActors->GetNextActor();
  while(!(ac==NULL)) 
    {
      if ( ac->IsA("SALOME_Actor") )
	{
	  SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	  if ( anActor->hasIO() )
	    {
	      Handle(SALOME_InteractiveObject) IO = anActor->getIO();
	      if ( IO->isSame( IObject ) ) {
		m_RWInteractor->Erase(IO, false);
		Sel->RemoveIObject(IO, false);
		break;
	      }
	    }
	}
      ac = theActors->GetNextActor();
    }
  if (update)
    Repaint();
}


void VTKViewer_ViewFrame::DisplayAll()
{
  m_RWInteractor->DisplayAll();
}


void VTKViewer_ViewFrame::EraseAll()
{
  m_RWInteractor->EraseAll();
}


void VTKViewer_ViewFrame::Repaint()
{
  m_RWInteractor->Render();
  m_RW->updateGL();
}

void VTKViewer_ViewFrame::AddActor( SALOME_Actor* theActor, bool update /*=false*/ )
{
  m_Renderer->AddActor( theActor );
  if (m_NonIsometricTransform != NULL)
    {
      vtkPolyDataMapper* mapper = NULL;
      vtkMapper* initialMapper = theActor->GetInitialMapper();
      if ( initialMapper == NULL )
	initialMapper =   theActor->GetMapper();
      if ( initialMapper->IsA("vtkDataSetMapper") )
	{
	  mapper = vtkDataSetMapper::SafeDownCast( initialMapper )->GetPolyDataMapper ();
	  if (!mapper)
	    {
	      initialMapper->Render(m_Renderer,theActor);
	      mapper = vtkDataSetMapper::SafeDownCast( initialMapper )->GetPolyDataMapper ();
	    }
	}
      else
	mapper = vtkPolyDataMapper::SafeDownCast( initialMapper );
      if (mapper)
	{
	  //create Filter
	  vtkTransformPolyDataFilter *aTransformFilter = vtkTransformPolyDataFilter::New();
	  aTransformFilter->SetInput ( mapper->GetInput() );
	  aTransformFilter->SetTransform (m_NonIsometricTransform);

	  //create new mapper
	  vtkPolyDataMapper *aMapper = vtkPolyDataMapper::New();
	  aMapper->SetInput (aTransformFilter->GetOutput());
	  aMapper->ShallowCopy ( theActor->GetMapper());

	  //set new mapper
	  theActor->SetMapper (aMapper);
		  
	  aTransformFilter->Delete();
	  aMapper->Delete();
	}
    } 
  theActor->SetVisibility( true );
  
  if (update)
    m_RWInteractor->Render();
}
