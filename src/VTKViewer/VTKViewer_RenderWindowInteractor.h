//  File      : VTKViewer_RenderWindowInteractor.h
//  Created   : Wed Mar 20 11:31:36 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : VTKViewer
//  Copyright : Open CASCADE 2002
//  $Header$

#ifndef VTKViewer_RenderWindowInteractor_h
#define VTKViewer_RenderWindowInteractor_h

//===========================================================
// now we define the C++ class

#include "SALOME_InteractiveObject.hxx"
#include "SALOME_Actor.h"

// QT Includes
#include <qobject.h>
#include <qtimer.h>

// VTK Includes
#include <vtkVersion.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkUnstructuredGrid.h>

// Open CASCADE Includes
#include <TColStd_MapOfInteger.hxx>
#include <TColStd_MapIteratorOfMapOfInteger.hxx>

class VTKViewer_RenderWindow ;

// ------------------------------------------------------------
// :TRICKY: Fri Apr 21 22:19:27 2000 Pagey
// The Signal/Slot mechanism used by Qt requires that QObject 
// appear as the first class when using multiple inheritance. 
// Hence the order of the two classes QObject and vtkRenderWindowInteractor
// matters here. Be careful not to change it by accident. 
// ------------------------------------------------------------
class VTK_EXPORT VTKViewer_RenderWindowInteractor : 
  public QObject, public vtkRenderWindowInteractor
{
  Q_OBJECT 
public:

  static VTKViewer_RenderWindowInteractor *New() ; 

  vtkTypeMacro(vtkRenderWindowInteractor,vtkObject);

  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Initializes the event handlers without an XtAppContext.  This is
  // good for when you don`t have a user interface, but you still
  // want to have mouse interaction.
  virtual void Initialize();

  // Description:
  // This will start up the X event loop and never return. If you
  // call this method it will loop processing X events until the
  // application is exited.
  virtual void Start();
  
  // Description:
  // Enable/Disable interactions.  By default interactors are enabled when
  // initialized.  Initialize() must be called prior to enabling/disabling
  // interaction. These methods are used when a window/widget is being
  // shared by multiple renderers and interactors.  This allows a "modal"
  // display where one interactor is active when its data is to be displayed
  // and all other interactors associated with the widget are disabled
  // when their data is not displayed.
  virtual void Enable();
  virtual void Disable();

  // Description:
  // Event loop notification member for Window size change
  virtual void UpdateSize(int x,int y);

  // Description:
  // Timer methods must be overridden by platform dependent subclasses.
  // flag is passed to indicate if this is first timer set or an update
  // as Win32 uses repeating timers, whereas X uses One shot more timer
  // if flag==VTKXI_TIMER_FIRST Win32 and X should createtimer
  // otherwise Win32 should exit and X should perform AddTimeOut()
  virtual int CreateTimer(int ) ; 
  virtual int DestroyTimer() ; 
  
  // Description:
  // This function is called on 'q','e' keypress if exitmethod is not
  // specified and should be overidden by platform dependent subclasses
  // to provide a termination procedure if one is required.
  virtual void TerminateApp(void) { /* empty */ }
  
  // Description:
  // These methods correspond to the the Exit, User and Pick
  // callbacks. They allow for the Style to invoke them.
  //virtual void ExitCallback();
  //virtual void UserCallback();
  //virtual void StartPickCallback();
  //virtual void EndPickCallback();
  
  /* Selection Management */
  bool highlightCell(const Handle(SALOME_InteractiveObject)& IObject, bool hilight, 
		     const TColStd_MapOfInteger& MapIndex, bool immediatly = true );
  bool highlightEdge(const Handle(SALOME_InteractiveObject)& IObject, bool hilight, 
		     const TColStd_MapOfInteger& MapIndex, bool immediatly = true ); //NB
  bool highlightPoint(const Handle(SALOME_InteractiveObject)& IObject, bool hilight, 
		      const TColStd_MapOfInteger& MapIndex, bool immediatly = true );

  bool highlight(const Handle(SALOME_InteractiveObject)& IObject, bool hiligth, bool immediatly = true );
  bool unHighlightAll();
  bool isInViewer( const Handle(SALOME_InteractiveObject)& IObject);
  bool isVisible( const Handle(SALOME_InteractiveObject)& IObject);
  void rename(const Handle(SALOME_InteractiveObject)& IObject, QString newName);


  void SetSelectionMode(int mode);

  // Displaymode management
  int GetDisplayMode();
  void SetDisplayMode(int);

  // Switch representation wireframe/shading
  void SwitchRepresentation(const Handle(SALOME_InteractiveObject)& IObject, bool update = true);

  // Change all actors to wireframe or surface
  void ChangeRepresentationToWireframe();
  void ChangeRepresentationToSurface();

  // Change to wireframe or surface a list of vtkactor
  void ChangeRepresentationToWireframe(vtkActorCollection* ListofActors);
  void ChangeRepresentationToSurface(vtkActorCollection* ListofActors);

  // Erase Display functions
  void EraseAll();
  void DisplayAll();
  void Erase(const Handle(SALOME_InteractiveObject)& IObject, bool immediatly=true);
  void Remove(const Handle(SALOME_InteractiveObject)& IObject, bool immediatly=true);
  void Display(const Handle(SALOME_InteractiveObject)& IObject, bool immediatly=true);

  void Display( SALOME_Actor* SActor, bool immediatly = true );

  // Transparency
  void SetTransparency(const Handle(SALOME_InteractiveObject)& IObject,float trans);
  float GetTransparency(const Handle(SALOME_InteractiveObject)& IObject);

  // Color
  void SetColor(const Handle(SALOME_InteractiveObject)& IObject,QColor thecolor);
  QColor GetColor(const Handle(SALOME_InteractiveObject)& IObject);

  void Update();

  vtkRenderer* GetRenderer();

 protected:

  VTKViewer_RenderWindowInteractor();
  ~VTKViewer_RenderWindowInteractor();
  //
  // Timer used during various mouse events to figure 
  // out mouse movements. 
  //
  QTimer *mTimer ;

  //CAI: Display mode
  int displaymode;

  //NRI: Selection mode
  int selectionmode;
  vtkActor* Point_Actor;
  vtkActor* Edge_Actor; //NB
  vtkActor* Cell_Actor;
  
  //
  // User for switching to stereo mode.
  //
  int PositionBeforeStereo[2];
  //
  // Connect/disconnect all slots which allow events on the
  // render window to be passed to the interactor. 
  //
  void ConnectSlots() ;
  void DisconnectSlots() ;
  void ProcessSlotConnections(bool conn) ;

 public slots:
  void MouseMove(const QMouseEvent *event) ;
  void LeftButtonPressed(const QMouseEvent *event) ;
  void LeftButtonReleased(const QMouseEvent *event) ;
  void MiddleButtonPressed(const QMouseEvent *event) ;
  void MiddleButtonReleased(const QMouseEvent *event) ;
  void RightButtonPressed(const QMouseEvent *event) ;
  void RightButtonReleased(const QMouseEvent *event) ;
  void ButtonPressed(const QMouseEvent *event) ;
  void ButtonReleased(const QMouseEvent *event) ;
  void KeyPressed(QKeyEvent *event) ;

  private slots:
    //
    // Not all of these slots are needed in VTK_MAJOR_VERSION=3,
    // but moc does not understand "#if VTK_MAJOR_VERSION". Hence, 
    // we have to include all of these for the time being. Once,
    // this bug in MOC is fixed, we can separate these. 
    //
    void TimerFunc() ;
 signals:
  void RenderWindowModified() ;
};

#endif

