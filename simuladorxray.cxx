#include "simuladorxray.h"
#include "vtkDICOMImageReader.h"


//CLASE PARA LA INTERACCION DE TECLADO
class KeyPressInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:

    //constructor
    static KeyPressInteractorStyle* New();
    vtkTypeMacro(KeyPressInteractorStyle, vtkInteractorStyleTrackballCamera);

    /*KeyPressInteractorStyle()
    {
      this->SelectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
      this->SelectedActor = vtkSmartPointer<vtkActor>::New();
      this->SelectedActor->SetMapper(SelectedMapper);
    }*/

    virtual void OnKeyPress()
    {
        // Get the keypress
        vtkRenderWindowInteractor *rwi = this->Interactor;
        std::string key = rwi->GetKeySym();

        // Handle an arrow key
        if(key == "s")
        {
            std::cout << "Guardando Imagen en un archivo" << std::endl;
            /* get image from render vtk*/
            vtkWindowToImageFilter * windowToImageFilter = vtkWindowToImageFilter::New();
            windowToImageFilter->SetInput(this->Interactor->GetRenderWindow());
            windowToImageFilter->SetMagnification(3); //set the resolution of the output image (3 times the current resolution of vtk render window)
            windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
            windowToImageFilter->ReadFrontBufferOff(); // read from the back buffer
            windowToImageFilter->Update();

            vtkPNGWriter *writer = vtkPNGWriter::New();
            writer->SetFileName("simulatedXray.png");
            writer->SetInputConnection(windowToImageFilter->GetOutputPort());
            writer->Write();
        }

        if(key == "c")
        {

            if(!myboxwidget->GetEnabled()){
                myboxwidget->On();
                std::cout << "Recorte de Volumen Encendido." << std::endl;
            }else{
                myboxwidget->Off();
                std::cout << "Recorte de Volumen Apagado" << std::endl;
            }

        }

        // Forward events
        vtkInteractorStyleTrackballCamera::OnKeyPress();
    }

    void setBoxWidget(vtkBoxWidget  *bw){
        myboxwidget = bw;
    }
private:
    vtkBoxWidget *myboxwidget;

};
vtkStandardNewMacro(KeyPressInteractorStyle);

// Callback for moving the planes from the box widget to the mapper
class vtkBoxWidgetCallback : public vtkCommand
{
public:
    static vtkBoxWidgetCallback *New()
    { return new vtkBoxWidgetCallback; }
    void Execute(vtkObject *caller, unsigned long, void*) override
    {
        vtkBoxWidget *widget = reinterpret_cast<vtkBoxWidget*>(caller);
        if (this->Mapper)
        {
            vtkPlanes *planes = vtkPlanes::New();
            widget->GetPlanes(planes);
            this->Mapper->SetClippingPlanes(planes);
            planes->Delete();
        }
    }
    void SetMapper(vtkSmartVolumeMapper* m)
    { this->Mapper = m; }

protected:
    vtkBoxWidgetCallback()
    { this->Mapper = 0; }

    vtkSmartVolumeMapper *Mapper;
};

SimuladorXray::SimuladorXray(vtkRenderWindow* renWin,vtkRenderWindowInteractor *iren)
{
    //asignacion de memoria para el todos los controladores de visualizacion

    readerimage = vtkDICOMImageReader::New();
    mapper = vtkSmartVolumeMapper::New();
    renderer = vtkRenderer::New();
    volume = vtkVolume::New();
    volumeProperty  = vtkVolumeProperty::New();
    opacityFun = vtkPiecewiseFunction::New();
    colorFun = vtkColorTransferFunction::New();
    resample = vtkImageResample::New();
    reslice = vtkImageReslice::New();
    transform = vtkTransform::New();
    box = vtkBoxWidget::New();
    //share from gui

    this->renWin = renWin;
    this->iren = iren;

    //iren->SetDesiredUpdateRate(60/ (1+0) );
    //iren->GetInteractorStyle()->SetDefaultRenderer(renderer);
}

void SimuladorXray::setDirectory3Dimage(string namedirectory){
    this->pathDirectory = namedirectory;
    readerimage->SetDirectoryName(namedirectory.c_str());
    readerimage->Update();
    //actualizamos de paso el vtkimagedata y el vtkalgorithm
    input=readerimage->GetOutput();
    //actualizar vtkAlgorithm
    reader=readerimage;
    //Dimensiones del volumen
    input->GetDimensions(dim);
    if ( dim[0] < 2 || dim[1] < 2 || dim[2] < 2 ){
        cout << "Image Dimensions are lower than 3!" << endl;
        exit(EXIT_FAILURE);
    }
}

void SimuladorXray::show(){

    renWin->SetSize(600,600); // intentional odd and NPOT  width/heigh

    //tranformando en rotacion al volumen
    //transformRotImageData(); //primero la tranformacion salida (reslice)
    //submuestra del volumen original siempre aplicado
    //en caso de no hacer resample el factor sera puesto a uno
    resampleImageData(); //segundo el resample salida (resample)

    //asignando al render la entrada de imagen 3D
    mapper->SetInputConnection(resample->GetOutputPort());

    //GENERAL PROPERTY
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->SetIndependentComponents(true);
    volumeProperty->ShadeOn();
    volumeProperty->SetAmbient(0.1);
    volumeProperty->SetDiffuse(0.9);
    volumeProperty->SetSpecular(0.2);
    volumeProperty->SetSpecularPower(10.0);
    volumeProperty->SetScalarOpacityUnitDistance(0.8919);

    //Por Default se usa el Blend MIP
    applyBlendMIP();
    //iren->GetInteractorStyle()->SetDefaultRenderer(renderer);
    //Iniciar Clip
    iniciarClip();


    //asignando el volumen al render
    renderer->AddVolume(volume);
    renderer->ResetCamera();
    renWin->AddRenderer(renderer);


    // 3D texture mode. For coverage.
    #if !defined(VTK_LEGACY_REMOVE) && !defined(VTK_OPENGL2)
      volumeMapper->SetRequestedRenderModeToRayCastAndTexture();
    #endif // VTK_LEGACY_REMOVE
      //renWin->Render();
}

void SimuladorXray::transformRotImageData(double rotx,double roty,double rotz){
    //tomamos siempre el resample
    resample->GetOutput()->GetBounds(bounds);

    //TRANSFORMACION DE ROTACION DEL VOLUMEN

    //calculando el centro mediante los limites obtenidos
    double center[3];
    center[0] = (bounds[1] + bounds[0]) / 2.0;
    center[1] = (bounds[3] + bounds[2]) / 2.0;
    center[2] = (bounds[5] + bounds[4]) / 2.0;

    //trasladamos al centro
    transform->Translate(center[0], center[1], center[2]);
    transform->RotateX(rotx);
    transform->RotateY(roty);
    transform->RotateZ(rotz);
    //trasladamos al centro del origen de coordenadas (0,0,0)
    transform->Translate(-center[0], -center[1], -center[2]);

    //aplicamos al volumen la transformacion

    //el pasamos el vtkalgorithm del dicom
    reslice->SetInputConnection(resample->GetOutputPort());
    reslice->SetResliceTransform(transform);
    reslice->SetInterpolationModeToCubic();
    //copiamos las mismas propiedades de la original dicom ya que esta permanecen iguales (spacing, origin, extent)
    reslice->SetOutputSpacing(resample->GetOutput()->GetSpacing()[0], resample->GetOutput()->GetSpacing()[1], resample->GetOutput()->GetSpacing()[2]);
    reslice->SetOutputOrigin(resample->GetOutput()->GetOrigin()[0], resample->GetOutput()->GetOrigin()[1], resample->GetOutput()->GetOrigin()[2]);
    reslice->SetOutputExtent(resample->GetOutput()->GetExtent());

    //actualizamos el resample con este nuevo resultado del reslice
    //input = reslice->GetOutput();
    //reader = reslice;
    //resample->SetInputConnection(reslice->GetOutputPort());


    mapper->SetInputConnection(reslice->GetOutputPort());
    mapper->Update();


}

void SimuladorXray::resampleImageData(){
    //resample->SetInputData(reslice->GetOutput());
    resample->SetInputConnection( reader->GetOutputPort() );
    //factor de reduccion en cada eje (x,y,z)
    resample->SetAxisMagnificationFactor(0, reductionFactor);
    resample->SetAxisMagnificationFactor(1, reductionFactor);
    resample->SetAxisMagnificationFactor(2, reductionFactor);
}

//calculando la parte de tipos de blend
void SimuladorXray::cleanPropVol(){
    //volumeProperty  = vtkVolumeProperty::New();
    opacityFun = vtkPiecewiseFunction::New();
    colorFun = vtkColorTransferFunction::New();
}
void SimuladorXray::updatePropVol(){
    //actualizando el render
    mapper->Update();

    //actualizando las propiedades del volumen
    volumeProperty->SetColor(colorFun);
    volumeProperty->SetScalarOpacity(opacityFun);

    //asignando todo al volumen
    volume->SetMapper(mapper);
    volume->SetProperty(volumeProperty);
}
void SimuladorXray::applyBlendMIP(){
    cleanPropVol();
    colorFun->AddRGBSegment(0.0, 1.0, 1.0, 1.0, 255.0, 1.0, 1.0, 1.0 );
    opacityFun->AddSegment( opacityLevel - 0.5*opacityWindow, 0.0,opacityLevel + 0.5*opacityWindow, 1.0 );
    mapper->SetBlendModeToMaximumIntensity();
    volumeProperty->ShadeOff();
    updatePropVol();
    *typeBlend = "MIP";
}

void SimuladorXray::applyBlendCompRamp(){
    cleanPropVol();
    colorFun->AddRGBSegment(opacityLevel - 0.5*opacityWindow, 0.0, 0.0, 0.0, opacityLevel + 0.5*opacityWindow, 1.0, 1.0, 1.0 );
    opacityFun->AddSegment( opacityLevel - 0.5*opacityWindow, 0.0, opacityLevel + 0.5*opacityWindow, 1.0 );
    mapper->SetBlendModeToComposite();
    volumeProperty->ShadeOff();
    updatePropVol();
    *typeBlend = "CompositeRamp";
}

void SimuladorXray::applyBlendCompShadeRamp(){
    //Advertencia: prop de ambient,specular, diffuse, power aun se preservan
    cleanPropVol();
    colorFun->AddRGBSegment(0.0, 1.0, 1.0, 1.0, 255.0, 1.0, 1.0, 1.0 );
    opacityFun->AddSegment( opacityLevel - 0.5*opacityWindow, 0.0, opacityLevel + 0.5*opacityWindow, 1.0 );
    mapper->SetBlendModeToComposite();
    volumeProperty->ShadeOn();
    updatePropVol();
    *typeBlend = "CompositeShadeRamp";
}

void SimuladorXray::applyBlendCTSkin(){
    cleanPropVol();
    colorFun->AddRGBPoint( -3024, 0, 0, 0, 0.5, 0.0 );
    colorFun->AddRGBPoint( -1000, .62, .36, .18, 0.5, 0.0 );
    colorFun->AddRGBPoint( -500, .88, .60, .29, 0.33, 0.45 );
    colorFun->AddRGBPoint( 3071, .83, .66, 1, 0.5, 0.0 );

    opacityFun->AddPoint(-3024, 0, 0.5, 0.0 );
    opacityFun->AddPoint(-1000, 0, 0.5, 0.0 );
    opacityFun->AddPoint(-500, 1.0, 0.33, 0.45 );
    opacityFun->AddPoint(3071, 1.0, 0.5, 0.0);

    volumeProperty->ShadeOn();
    mapper->SetBlendModeToComposite();
    updatePropVol();
    *typeBlend = "CTSkin";
}

void SimuladorXray::applyBlendCTBone(){
    cleanPropVol();
    colorFun->AddRGBPoint( -3024, 0, 0, 0, 0.5, 0.0 );
    colorFun->AddRGBPoint( -16, 0.73, 0.25, 0.30, 0.49, .61 );
    colorFun->AddRGBPoint( 641, .90, .82, .56, .5, 0.0 );
    colorFun->AddRGBPoint( 3071, 1, 1, 1, .5, 0.0 );

    opacityFun->AddPoint(-3024, 0, 0.5, 0.0 );
    opacityFun->AddPoint(-16, 0, .49, .61 );
    opacityFun->AddPoint(641, .72, .5, 0.0 );
    opacityFun->AddPoint(3071, .71, 0.5, 0.0);
    volumeProperty->ShadeOn();
    mapper->SetBlendModeToComposite();
    updatePropVol();
    *typeBlend = "CTBone";
}

void SimuladorXray::applyBlendCTMuscle(){
    cleanPropVol();
    colorFun->AddRGBPoint( -3024, 0, 0, 0, 0.5, 0.0 );
    colorFun->AddRGBPoint( -155, .55, .25, .15, 0.5, .92 );
    colorFun->AddRGBPoint( 217, .88, .60, .29, 0.33, 0.45 );
    colorFun->AddRGBPoint( 420, 1, .94, .95, 0.5, 0.0 );
    colorFun->AddRGBPoint( 3071, .83, .66, 1, 0.5, 0.0 );

    opacityFun->AddPoint(-3024, 0, 0.5, 0.0 );
    opacityFun->AddPoint(-155, 0, 0.5, 0.92 );
    opacityFun->AddPoint(217, .68, 0.33, 0.45 );
    opacityFun->AddPoint(420,.83, 0.5, 0.0);
    opacityFun->AddPoint(3071, .80, 0.5, 0.0);
    volumeProperty->ShadeOn();
    mapper->SetBlendModeToComposite();
    updatePropVol();
    *typeBlend = "CTMuscle";
}

void SimuladorXray::applyBlendRgbComp(){
    cleanPropVol();
    //advertencia: vacio asignacion de colorfun
    opacityFun->AddPoint(0, 0.0);
    opacityFun->AddPoint(5.0, 0.0);
    opacityFun->AddPoint(30.0, 0.05);
    opacityFun->AddPoint(31.0, 0.0);
    opacityFun->AddPoint(90.0, 0.0);
    opacityFun->AddPoint(100.0, 0.3);
    opacityFun->AddPoint(110.0, 0.0);
    opacityFun->AddPoint(190.0, 0.0);
    opacityFun->AddPoint(200.0, 0.4);
    opacityFun->AddPoint(210.0, 0.0);
    opacityFun->AddPoint(245.0, 0.0);
    opacityFun->AddPoint(255.0, 0.5);

    mapper->SetBlendModeToComposite();
    volumeProperty->ShadeOff();
    volumeProperty->SetScalarOpacityUnitDistance(1.0);
    updatePropVol();
    //seteamos el typo de blend actual
    *typeBlend = "RGBComposite";
}

void SimuladorXray::setOpacityWindow(int opacWindow){
    opacityWindow = opacWindow;
}
void SimuladorXray::setOpacityLevel(int opacLevel){
    opacityLevel = opacLevel;
}

void SimuladorXray::updateMapper(){
    applyBlendMIP();
}

void SimuladorXray::iniciarClip(){
    if (clip)
    {
        //advertencia usao del interactor esto
        //puede ser el qvtkinteractor
        //puede ser el vtkinteractor

        box->SetInteractor(iren);
        //
        box->SetPlaceFactor(1.01);
        box->SetInputConnection(resample->GetOutputPort());
        box->SetDefaultRenderer(renderer);
        box->InsideOutOn();
        box->PlaceWidget();

        //construccion del callback o llamada de retorno
        vtkBoxWidgetCallback *callback = vtkBoxWidgetCallback::New();
        callback->SetMapper(mapper);

        //asignacion del callback al boxwidget
        box->AddObserver(vtkCommand::InteractionEvent, callback);
        callback->Delete();
        box->EnabledOn();
        box->GetSelectedFaceProperty()->SetOpacity(0.0);
    }
}

void SimuladorXray::setFlagClip(bool value){
    this->clip = value;
    if(value)
        box->On();
    else
        box->Off();

}
