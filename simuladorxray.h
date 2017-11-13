#ifndef SIMULADORXRAY_H
#define SIMULADORXRAY_H

#include "vtkDICOMImageReader.h"
#include <vtkMetaImageReader.h>
#include "vtkSmartVolumeMapper.h"

#include "vtkVolume.h"
#include "vtkVolumeProperty.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

#include <vtkImageData.h>
#include "vtkImageResample.h"
#include <vtkTransform.h>
#include <vtkImageReslice.h>
#include "vtkBoxWidget.h"

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>

#include <vtkPNGWriter.h>
#include <vtkWindowToImageFilter.h>
#include "vtkPlanes.h"
#include "vtkProperty.h"
#include <string>
#include <QStringList>

using namespace std;

class SimuladorXray
{
private:
    //QStringList *listTypesBlend;
    string pathDirectory;
    vtkDICOMImageReader* readerimage;
    vtkMetaImageReader * readerimagemha;
    //vtkPolyDataMapper *mapper;
    vtkRenderer *renderer;
    vtkRenderWindow *renWin;
    vtkRenderWindowInteractor *iren;
    vtkAlgorithm *reader=0;
    vtkImageData *input=0;
    vtkVolume *volume;
    vtkSmartVolumeMapper *mapper;//usando la GPU
    vtkVolumeProperty * volumeProperty;
    vtkPiecewiseFunction * opacityFun;
    vtkColorTransferFunction *colorFun;
    vtkImageResample *resample;
    vtkImageReslice *reslice;
    vtkTransform *transform;
    int opacityWindow = 4096;
    int opacityLevel = 2048;
    double reductionFactor = 0.5;
    double ratioFrames = 10.0;
    QString *typeBlend = new QString("MIP");
    int dim[3]; //dimensiones
    double bounds[6]; //limites
    bool clip = true;
    vtkBoxWidget *box;
public:
    SimuladorXray(vtkRenderWindow* renWin,vtkRenderWindowInteractor *iren);
    //QStringList* getlistTypesBlend();
    void setDirectory3Dimage(string namedirectory);
    void show();
    void createImageData(vtkImageData* imageData);
    void iniciarClip();
    void resampleImageData();
    void transformRotImageData(double rotx,double roty,double rotz);

    void cleanPropVol();
    void updatePropVol();
    void applyBlendMIP();
    void applyBlendCompRamp();
    void applyBlendCompShadeRamp();
    void applyBlendCTSkin();
    void applyBlendCTBone();
    void applyBlendCTMuscle();
    void applyBlendRgbComp();

    void setOpacityWindow(int opacWindow);
    void setOpacityLevel(int opacLevel);
    void setFactReduction(double facRed);
    void setRatFrames(double ratFra);
    void updateMapper();
    void setFlagClip(bool value);
    vtkRenderWindow * getRenderWindowSimXray();
};

#endif // SIMULADORXRAY_H
