#ifndef Create3DBones_H
#define Create3DBones_H
 
#include <vtk-7.1/vtkSmartPointer.h>
#include <QMainWindow>
#include <string>
#include "DataMalla.h"
#include "simuladorxray.h"
#include "delaunay.h"
#include "DelaunayUtil/arista.h"
//#include "QuickView.h"
//#include "perspecdata.h"
#include "gestorperspectiva.h"
#include "ui_Create3DBones.h"
// Forward Qt class declarations
QT_BEGIN_NAMESPACE
class Ui_Create3DBones;
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
QT_END_NAMESPACE

class Create3DBones : public QMainWindow
{
  Q_OBJECT
public:
 
  // Constructor/Destructor
  Create3DBones(); 
  ~Create3DBones() {};
  void cargarModelo(std::string,std::string filetype);
  void cargarFileVTK(std::string);
  void cargar3Dimage(std::string);
  void setRutaImagen(QString filename);
  int calcularNumCifras(int idPoint);
  int calcFactorDecimal(int numCifras);
  vtkSmartPointer<vtkPolyData> ejec_delaunay();
  void reproyeccionMalla();
  vtkSmartPointer<vtkRenderWindow> getRenderWindow(QVTKWidget *w);
  vtkSmartPointer<vtkRenderWindowInteractor> getInteractor(QVTKWidget *w);
  std::string getRutaImagen();

  //simulador de rayos x
  void prepGraphSimXray();
  QStringList * getTypesBlend();

  QMenu *fileMenu;
  QMenu *viewMenu;
  QMenu *helpMenu;
public slots:
 
  virtual void slotExit();

private:
  // Designer form
  Ui_Create3DBones *ui;

  //funciones para imagen
  void createActions();
  void createMenus();
  void updateActions();
  void scaleImage(double factor);
  void adjustScrollBar(QScrollBar *scrollBar, double factor);

  //Cuadro de imagen
  QLabel *image_label;
  QLabel *cont_imag_label;
  QString rutaImagen;
  QScrollArea *scrollArea,*scrollArea2;
  double scaleFactor;

  //Variables de Proyeccion Geometrica
  DataMalla *dataMalla;
  DataMalla *gridPtosCtrl;
  //PerspecData *perspecData;
  GestorPerspectiva *gp;
  GestorPerspectiva *gp1;
  GestorPerspectiva *grp;
  GestorPerspectiva *grp1;

  //Simulador de rayos x
  QStringList *listTypesBlend;
  SimuladorXray *simXray;

  //Qaction
    QAction *openAct;
    QAction *exitAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;

private slots:
  void load_image();
  void load_model();
  void load_promedio();
  void findContornoOnImage();
  void ejec_proyeccion();
  void ejec_proyeccionGrid();
  void load_model_grid();
  void reproyeccion();
//simulador de rayos x
  void load_3dimage();
  void applyBlend(const QString&);
  void applyTransSimulXray();
  void setLevelXray(int);
  void setWindowXray(int);
  void setRatFramesXray(double);
  void setFactRedXray(double);
  void applyOpacityValues();
  void evalClip(bool valClip);
  void loadProbeFilter();
  void saveVirtualXray();
  int getBinMask();

  //slots de imagenes
  void open();
  void zoomIn();
  void zoomOut();
  void normalSize();
  void fitToWindow();
};
 
#endif
