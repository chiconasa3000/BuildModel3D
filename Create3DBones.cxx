#include "Create3DBones.h"

// This is included here because it is forward declared in
// Create3DBones.h

#include <unordered_map>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollArea>
#include <QScrollBar>
#include <QMenu>
#include <QMenuBar>
#include <QGridLayout>
#include <QLabel>
#include <QDebug>
#include <vtk-7.1/vtkPolyDataMapper.h>
#include <vtk-7.1/vtkRenderer.h>
#include <vtk-7.1/vtkRenderWindow.h>
#include <vtk-7.1/vtkSphereSource.h>
#include <vtk-7.1/vtkSmartPointer.h>
#include <vtk-7.1/vtkActor.h>
#include <vtk-7.1/vtkOBJReader.h>
#include <vtk-7.1/vtkPolyDataReader.h>
#include <vtk-7.1/vtkAxesActor.h>
#include <vtk-7.1/vtkTransform.h>
#include <vtk-7.1/vtkVertexGlyphFilter.h>
#include <vtk-7.1/vtkTransform.h>
#include <vtk-7.1/vtkTransformPolyDataFilter.h>
#include <vtk-7.1/vtkDelaunay2D.h>
#include <vtk-7.1/vtkProperty.h>
#include <vtk-7.1/vtkWindowToImageFilter.h>
#include <vtk-7.1/vtkContourFilter.h>
#include <vtk-7.1/vtkCamera.h>
#include <vtk-7.1/vtkLine.h>
#include <vtk-7.1/vtkPolyDataSilhouette.h>
#include <vtk-7.1/vtkTriangle.h>
#include <vtk-7.1/vtkCleanPolyData.h>


#include <vtk-7.1/vtkVersion.h>
#include <vtk-7.1/vtkSmartPointer.h>
#include <vtk-7.1/vtkPoints.h>
#include <vtk-7.1/vtkPolyData.h>
#include <vtk-7.1/vtkPointData.h>
#include <vtk-7.1/vtkCellArray.h>
#include <vtk-7.1/vtkUnsignedCharArray.h>
#include <vtk-7.1/vtkPolyDataMapper.h>
#include <vtk-7.1/vtkActor.h>
#include <vtk-7.1/vtkRenderWindow.h>
#include <vtk-7.1/vtkRenderer.h>
#include <vtk-7.1/vtkRenderWindowInteractor.h>
#include <vtk-7.1/vtkVertexGlyphFilter.h>
#include <vtk-7.1/vtkProperty.h>
#include <vtk-7.1/vtkIdList.h>
#include <vtk-7.1/vtkCell.h>
#include <vtk-7.1/vtkDataSetMapper.h>
#include <vtk-7.1/vtkIdTypeArray.h>
#include <vtk-7.1/vtkSelectionNode.h>
#include <vtk-7.1/vtkSelection.h>
#include <vtk-7.1/vtkExtractSelection.h>
#include <vtk-7.1/vtkOBJReader.h>
#include <vtk-7.1/vtkDelaunay2D.h>
#include <vtk-7.1/vtkEdgeTable.h>
#include <vtk-7.1/vtkExtractEdges.h>

#include <iostream>
#include <unordered_map>
#include <list>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>

#include <vtkSmartPointer.h>
#include <vtkMarchingCubes.h>
#include <vtkMetaImageReader.h>

#include <vtkSphereSource.h>
#include <vtkProbeFilter.h>
#include <vtkSphere.h>
#include <vtkClipDataSet.h>
#include <vtkImplicitVolume.h>
#include <vtkUnstructuredGrid.h>
#include <vtkLookupTable.h>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include "vtkDICOMImageReader.h"
#include <vtkSampleFunction.h>
#include <vtkCylinder.h>
#include <vtkSTLReader.h>

// For compatibility with new VTK generic data arrays
#ifdef vtkGenericDataArray_h
#define InsertNextTupleValue InsertNextTypedTuple
#endif



#include "itkMesh.h"
#include "itkMeshFileReader.h"

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkCastImageFilter.h"
#include "itkTriangleMeshToBinaryImageFilter.h"

//#include "QuickView.h"

//#include "interactorstyle.h"
//#include "mouseinteractorstyle.h"

//vtkStandardNewMacro(InteractorStyle);

vtkSmartPointer<vtkPolyData> pointsSilhouette;

// Constructor
Create3DBones::Create3DBones()
{
    this->ui = new Ui_Create3DBones;
    this->ui->setupUi(this);

    //grupo de acciones y menus de la ui
    createActions();
    createMenus();

    //Graficos para el visor de imagenes
    image_label = new QLabel;
    image_label->setBackgroundRole(QPalette::Base);
    image_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    image_label->setScaledContents(true);

    cont_imag_label = new QLabel;
    cont_imag_label->setBackgroundRole(QPalette::Base);
    cont_imag_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    cont_imag_label->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(image_label);

    scrollArea2 = new QScrollArea;
    scrollArea2->setBackgroundRole(QPalette::Dark);
    scrollArea2->setWidget(cont_imag_label);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(scrollArea,0,0);
    this->ui->box_image->setLayout(layout);

    /*********+ ui simulador de rayos x ****************/
    prepGraphSimXray();

    // Set up action signals and slots
    connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(this->ui->but_find_cont,  SIGNAL(clicked()), this, SLOT(findContornoOnImage()));
    connect(this->ui->but_load_image,  SIGNAL(clicked()), this, SLOT(load_image()));
    connect(this->ui->but_load_meanmd,  SIGNAL(clicked()), this, SLOT(load_model()));
    connect(this->ui->but_load_model,  SIGNAL(clicked()), this, SLOT(load_model_grid()));
    connect(this->ui->but_exe_pers,  SIGNAL(clicked()), this, SLOT(ejec_proyeccion()));
    connect(this->ui->but_silueta, SIGNAL(clicked()), this, SLOT(ejec_proyeccionGrid()));
    connect(this->ui->but_delaunay,  SIGNAL(clicked()), this, SLOT(reproyeccion()));
    connect(this->ui->but_load_3dimage, SIGNAL(clicked()), this, SLOT(load_3dimage()));
    //simulador de rayos x
    connect(this->ui->comboBox_typeBlend,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(applyBlend(const QString&)));
    connect(this->ui->but_trans_xray, SIGNAL(clicked()), this, SLOT(applyTransSimulXray()));
    //par ael cambio de spin box
    connect(this->ui->spinbox_window, SIGNAL(valueChanged(int)), this, SLOT(setWindowXray(int)));
    connect(this->ui->spinbox_level, SIGNAL(valueChanged(int)), this, SLOT(setLevelXray(int)));
    connect(this->ui->butApplyOpacity, SIGNAL(clicked()), this, SLOT(applyOpacityValues()));
    connect(this->ui->checkbox_clip, SIGNAL(clicked(bool)), this, SLOT(evalClip(bool)));

    //test con probefilter
    connect(this->ui->butDic, SIGNAL(clicked()), this, SLOT(loadProbeFilter()));

    //get binary mask from volumen usign mesh
    connect(this->ui->butGetBinMas, SIGNAL(clicked()),this, SLOT(getBinMask()));

}

void Create3DBones::prepGraphSimXray(){
    listTypesBlend = new QStringList();
    listTypesBlend->append("MIP");
    listTypesBlend->append("CompositeRamp");
    listTypesBlend->append("CompositeShadeRamp");
    listTypesBlend->append("CTSkin");
    listTypesBlend->append("CTBone");
    listTypesBlend->append("CTMuscle");
    listTypesBlend->append("RGBComposite");
    //this->ui->comboBox_typeBlend->addItems(*(simXray->getlistTypesBlend()));
    this->ui->comboBox_typeBlend->addItems(*listTypesBlend);
}

QStringList * Create3DBones::getTypesBlend(){
    return listTypesBlend;
}

vtkSmartPointer<vtkRenderWindow> Create3DBones::getRenderWindow(QVTKWidget *w){
    return w->GetRenderWindow();
}

vtkSmartPointer<vtkRenderWindowInteractor> Create3DBones::getInteractor(QVTKWidget *w){
    return w->GetRenderWindow()->GetInteractor();
}

void Create3DBones::slotExit()
{
    qApp->exit();
}

void Create3DBones::load_image(){
   open();
}

//REPROYECCION DE GRILLA Y CONTORNO a 3D
void Create3DBones::reproyeccion(){
    //leer la nube de puntos de la cubo proyectado
    //leer la nube de puntos del modelo proyectado

    /********************************************/
    Delaunay miDelaunay;
    //convertir a un vector la nube de puntos
    miDelaunay.nubeMatrix2nubeVect();
    //ordenando la nube de puntos
    miDelaunay.ordenarNubePtos();
    //mostrando la nueba nube de puntos
    //miDelaunay.printNubeVect();
    //diviendo la nube en grupos
    miDelaunay.dividirNubePtos();
    miDelaunay.estructurarInGrafo();

    Grafo *grafDelaunay = miDelaunay.getGrafoDelaunay();
    //grafDelaunay->printDataGrafo();
    grafDelaunay->doTriangBase();
    grafDelaunay->printDataGrafo();
    //grafDelaunay->drawGrafo(getRenderWindow(this->ui->qvtkWidgetDelaunay));
    grafDelaunay->mergeGrafo();
    grafDelaunay->genNuevoIdsGraphGen();
    grafDelaunay->drawGrafo(getRenderWindow(this->ui->qvtkWidgetDelaunay));
    //reproyeccionMalla();
}

//REPROYECCION SOLO DE CONTORNO DE PLANTILLA A 3D
void Create3DBones::reproyeccionMalla(){

    //MatrixXd silhouette = dataMalla->convPolyData2Matrix(&pointsSilhouette);
    //Delaunay miDelaunay(silhoutte);

    //cout<<silhouette;
    //gp->recalcularZ3d(silhouette);

//    MatrixXd newCoordZ3d = gp->getCoordZ3dr();
//    MatrixXd silhouetteRep = gp->reproyeccion(silhouette,newCoordZ3d);
//    vtkSmartPointer<vtkPolyData> dataSilhouette3D =  vtkSmartPointer<vtkPolyData>::New();
//    dataSilhouette3D = gridPtosCtrl->convMatrix2PolyData(silhouetteRep);

//    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter1 = vtkSmartPointer<vtkVertexGlyphFilter>::New();
//#if VTK_MAJOR_VERSION <= 5
//    vertexFilter1->SetInputConnection(dataSilhouette3D->GetProducerPort());
//#else
//    vertexFilter1->SetInputData(dataSilhouette3D);
//#endif
//    vertexFilter1->Update();

//    vtkSmartPointer<vtkPolyData> mypolyData1 = vtkSmartPointer<vtkPolyData>::New();
//    mypolyData1->ShallowCopy(vertexFilter1->GetOutput());

//    //Visualizando
//    vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
//    mapper1->SetInputData(mypolyData1);

//    vtkSmartPointer<vtkActor> actor1 = vtkSmartPointer<vtkActor>::New();
//    actor1->SetMapper(mapper1);
//    actor1->GetProperty()->SetColor(1,0,0);
//    actor1->GetProperty()->SetPointSize(4);


//    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
//    renderer->AddActor(actor1);

//    // VTK/Qt wedded
//    this->ui->qvtkWidgetSilueta->GetRenderWindow()->AddRenderer(renderer);

}

//PROYECCION DE CUBO Y CONTORNO DE PLANTILLA a 2D
void Create3DBones::ejec_proyeccionGrid(){
    vtkSmartPointer<vtkPolyData> ptosCtrl =  vtkSmartPointer<vtkPolyData>::New();
    ptosCtrl = dataMalla->getMeshGridControl();

    gridPtosCtrl = new DataMalla(ptosCtrl->GetNumberOfPoints(),3);
    gridPtosCtrl->buildDataMallaWithPolyData(&ptosCtrl);
    gridPtosCtrl->setPolyData(ptosCtrl);
    gridPtosCtrl->setBoundsData(&ptosCtrl);
    gridPtosCtrl->setCenterData(&ptosCtrl);
    //gridPtosCtrl->printDataMalla();
    //Iniciando puntos de perspectiva

    gp1 = new GestorPerspectiva();

    MatrixXd temp = gridPtosCtrl->getMatrizDataMalla();
    //MatrixXd temp2 = temp.block(0,0, temp.rows(),3);

    gp1->setDataMalla(temp);
    VectorXd vtrasl(3); vtrasl <<0,0,50;
    VectorXd vRot(3); vRot << 0,0,0;
    gp1->setVectTrasRot(vtrasl,vRot);
    gp1->aplicarBasicTranf();

    gp1->inicioModelo();


    MatrixXd mallaProy = gp1->getMallaProy3d();
//    MatrixXd vectUnos = MatrixXd::Ones(mallaProy.rows(),1)*5;
//    mallaProy.col(2) << vectUnos;


    vtkSmartPointer<vtkPolyData> polydata =  vtkSmartPointer<vtkPolyData>::New();
    polydata = gridPtosCtrl->convMatrix2PolyData(mallaProy);

    //polydata = gridPtosCtrl->setAlignEscala(&polydata);
    //polydata = gridPtosCtrl->setAlignPosicion(&polydata);

    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
#if VTK_MAJOR_VERSION <= 5
    vertexFilter->SetInputConnection(polydata->GetProducerPort());
#else
    vertexFilter->SetInputData(polydata);
#endif
    vertexFilter->Update();

    vtkSmartPointer<vtkPolyData> mypolyData = vtkSmartPointer<vtkPolyData>::New();
    mypolyData->ShallowCopy(vertexFilter->GetOutput());

    //Visualizando
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(mypolyData);

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);

    //Ejecutamos la funcion para obtener el contorno
    //EJECUCION DE LA DETECCION DE CONTORNO MEDIANTE DELAUNAY
    vtkSmartPointer<vtkPolyData> dataContorno = ejec_delaunay();

    vtkSmartPointer<vtkPolyDataMapper> mapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION <= 5
    mapper2->SetInputConnection(dataContorno->GetProducerPort());
#else
    mapper2->SetInputData(dataContorno);
#endif

    vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
    actor2->SetMapper(mapper2);
    actor2->GetProperty()->SetColor(1,0,0);
    actor2->GetProperty()->SetPointSize(4);


    //renderer->AddActor(meshActor);
    renderer->AddActor(actor2);

    // VTK/Qt wedded
    this->ui->qvtkWidgetProyeccion->GetRenderWindow()->AddRenderer(renderer);
}



//EJECUCION DE PROYECCION SOLO DE MALLA a 2D
void Create3DBones::ejec_proyeccion(){
    //Iniciando puntos de perspectiva

    MatrixXd temp = dataMalla->getMatrizDataMalla();
    MatrixXd temp2 = temp.block(0,0, temp.rows(),3);

    gp = new GestorPerspectiva();
    gp->setDataMalla(temp2);
    VectorXd vtrasl(3); vtrasl <<0,0,50;
    VectorXd vRot(3); vRot << 0,0,0;
    gp->setVectTrasRot(vtrasl,vRot);
    gp->aplicarBasicTranf();

    //dataMalla->saveInFile("ptosMalla.txt");

    gp->inicioModelo();
    //gp->printCoordModel();
    MatrixXd mallaProy = gp->getMallaProy3d();

    //MatrixXd vectUnos = MatrixXd::Ones(mallaProy.rows(),1)*5;
    //mallaProy.col(2) << vectUnos;

    //std::cout<<"Malla de la plantilla: "<<mallaProy<<endl;

    vtkSmartPointer<vtkPolyData> polydata =  vtkSmartPointer<vtkPolyData>::New();
    polydata = dataMalla->convMatrix2PolyData(mallaProy);

    dataMalla->setBoundsCont(&polydata);
    dataMalla->setCenterCont(&polydata);

    polydata = dataMalla->setAlignEscala(&polydata);
    polydata = dataMalla->setAlignPosicion(&polydata);

    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
#if VTK_MAJOR_VERSION <= 5
    vertexFilter->SetInputConnection(polydata->GetProducerPort());
#else
    vertexFilter->SetInputData(polydata);
#endif
    vertexFilter->Update();

    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->ShallowCopy(vertexFilter->GetOutput());

    //Visualizando
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(polyData);

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);

    // VTK/Qt wedded
    this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
}

//NUBE DE PUNTOS DE PLANTILLA Y CUBO DE PUNTOS DE CONTROL
void Create3DBones::load_model_grid(){
    /*MatrixXd mallaProy = gp->getMallaProy3d();
    vtkSmartPointer<vtkPolyData> polydata =  vtkSmartPointer<vtkPolyData>::New();
    polydata = dataMalla->convMatrix2PolyData(mallaProy);*/

    vtkSmartPointer<vtkPolyData> ptosCtrl =  vtkSmartPointer<vtkPolyData>::New();

    vtkSmartPointer<vtkPolyData> polydata;
    polydata = dataMalla->getPolyData();

    //TODO //reusa el mismo puntos de control a la hora de proyectar al 2D
    ptosCtrl = dataMalla->build_mesh_grid(15,15,15);

    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter1 = vtkSmartPointer<vtkVertexGlyphFilter>::New();
#if VTK_MAJOR_VERSION <= 5
    vertexFilter1->SetInputConnection(polydata->GetProducerPort());
#else
    vertexFilter1->SetInputData(polydata);
#endif
    vertexFilter1->Update();

    vtkSmartPointer<vtkPolyData> polyData1 = vtkSmartPointer<vtkPolyData>::New();
    polyData1->ShallowCopy(vertexFilter1->GetOutput());

    // Visualize
    vtkSmartPointer<vtkPolyDataMapper> meshMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION <= 5
    meshMapper->SetInputConnection(polyData1->GetProducerPort());
#else
    meshMapper->SetInputData(polyData1);
#endif

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(meshMapper);
    actor->GetProperty()->SetColor(1,0,0);
    actor->GetProperty()->SetPointSize(4);

    //meshActor->GetProperty()->SetRepresentationToWireframe();

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);


    // Visualize
    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
#if VTK_MAJOR_VERSION <= 5
    vertexFilter->SetInputConnection(ptosCtrl->GetProducerPort());
#else
    vertexFilter->SetInputData(ptosCtrl);
#endif
    vertexFilter->Update();

    vtkSmartPointer<vtkPolyData> polydataCont = vtkSmartPointer<vtkPolyData>::New();
    polydataCont->ShallowCopy(vertexFilter->GetOutput());

    //create mapper and actor for silouette
    vtkSmartPointer<vtkPolyDataMapper> mapper2 =  vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper2->SetInputData(polydataCont);

    vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
    actor2->SetMapper(mapper2);
    //actor2->GetProperty()->SetColor(1.0, 0.3882, 0.2784); // tomato
    //actor2->GetProperty()->SetLineWidth(5);
    renderer->AddActor(actor2);
    //renderer->SetBackground(.1, .2, .3);
    renderer->ResetCamera();

    // VTK/Qt wedded
    this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
}

int Create3DBones::calcularNumCifras(int idPoint){
    int contador=1;

    while(idPoint/10>0)
    {
        idPoint=idPoint/10;
        contador++;
    }
    return contador;
}

int Create3DBones::calcFactorDecimal(int idPoint){
    int numCifras = calcularNumCifras(idPoint);
    int it = 0;
    int factDec = 1;
    while(it<numCifras){
        factDec = factDec * 10;
        it++;
    }
    return factDec;
}

//DETECCION DE CONTORNO DE PLANTILLA MEDIANTE DELAUNAY
vtkSmartPointer<vtkPolyData> Create3DBones::ejec_delaunay(){
    MatrixXd mallaProy = gp->getMallaProy3d();
    //cout<<mallaProy;

    /*MatrixXd vectUnos = MatrixXd::Ones(mallaProy.rows(),1)*5;
    mallaProy.col(2) << vectUnos;*/

    vtkSmartPointer<vtkPolyData> polydata =  vtkSmartPointer<vtkPolyData>::New();
    polydata = dataMalla->convMatrix2PolyData(mallaProy);

    // Triangulate the grid points
    vtkSmartPointer<vtkDelaunay2D> delaunay = vtkSmartPointer<vtkDelaunay2D>::New();
    delaunay->SetAlpha(5);

    //dataMalla->ordenarContornos();

#if VTK_MAJOR_VERSION <= 5
    delaunay->SetInput(polydata);
    //delaunay->SetSource(boundary);
#else
    delaunay->SetInputData(polydata);
    //delaunay->SetSourceData(boundary);
#endif
    delaunay->Update();

    vtkSmartPointer<vtkPolyData> datosGrilla =  vtkSmartPointer<vtkPolyData>::New();
    datosGrilla->ShallowCopy(delaunay->GetOutput());


    //mapa que almacenara las aristas por la informacion de sus puntos
    //clave : suma de sus puntos value : vector de puntos

    std::unordered_map<int,double**> mapaAristas;

    //Creando archivo que almacena los puntos de la silueta
    ofstream myfile;
    myfile.open ("ptosCont.txt");

    //Imprimiendo valores
    for(vtkIdType i = 0; i < datosGrilla->GetNumberOfCells(); i++)
    {
        vtkCell* cell = datosGrilla->GetCell(i);
        //std::cout<<"Ide la celda: "<<i<<endl;

        if(cell->IsA("vtkTriangle")){
            vtkTriangle* triangle = dynamic_cast<vtkTriangle*>(cell);

            //std::cout << "The cell has " << triangle->GetNumberOfEdges() << " edges." << std::endl;

            for(vtkIdType i = 0; i < triangle->GetNumberOfEdges(); i++)
            {
                //Obviamente siempre tendra 3 aristas
                vtkCell* edge = triangle->GetEdge(i);
                vtkIdList* pointIdList = edge->GetPointIds();

                //vtkIdType newId = triangle->GetEdge(i)->GetId(i);
                //std::cout << "Edge " << i << " has " << pointIdList->GetNumberOfIds()<< " points."  << std::endl;

                double **puntosEdge = new double*[2];
                int *puntosEdgeID = new int[2];
                puntosEdge[0] = new double[3];
                puntosEdge[1] = new double[3];
                //double *puntoA = new double[3];
                //double *puntoB = new double[3];
                for(vtkIdType p = 0; p < pointIdList->GetNumberOfIds(); p++)
                {
                    edge->GetPoints()->GetPoint(p, puntosEdge[p]);
                    puntosEdgeID[p] = pointIdList->GetId(p);
                    //std::cout << "Edge " << i << " uses point " << pointIdList->GetId(p)<< std::endl;
                }

                //obtenemos el numero de cifras de cada ide de los puntos

                //std::cout<<"AA: "<<puntosEdgeID[0]<<endl;
                //std::cout<<"BB: "<<puntosEdgeID[1]<<endl;

                //formando id de arista a partir de ids de sus ptos
                //posibles ids de arista
                int AnewIdEdge = puntosEdgeID[0]*calcFactorDecimal(puntosEdgeID[0]) + puntosEdgeID[1];
                int BnewIdEdge = puntosEdgeID[1]*calcFactorDecimal(puntosEdgeID[1]) + puntosEdgeID[0];

                //std::cout<<"A: "<<AnewIdEdge<<endl;
                //std::cout<<"B: "<<BnewIdEdge<<endl;

                std::unordered_map<int,double**>::iterator it1;
                std::unordered_map<int,double**>::iterator it2;

                //preguntamos por el primer elemento
                it1 = mapaAristas.find(AnewIdEdge);
                it2 = mapaAristas.find(BnewIdEdge);

                if (it1 != mapaAristas.end()){
                    //en caso de encontrar alguna arista igual
                    //eliminamos la q ya se encuentra en el mapa y no lo insertamos
                    mapaAristas.erase (it1);
                    //std::cout<<"supr: "<<it1->first<<endl;
                }else if(it2 != mapaAristas.end()){
                    //en caso de encontrar alguna arista igual
                    //eliminamos la q ya se encuentra en el mapa y no lo insertamos
                    //std::cout<<"supr: "<<it2->first<<endl;
                    mapaAristas.erase (it2);
                }else{
                    //en cualquier otro caso solo insertamos la nueva arista
                    //se inserta por defecto el AnewIdEdge
                    std::pair<int,double**> tmpEdge (AnewIdEdge,puntosEdge);
                    //std::cout<<"insert: "<<std::get<0>(tmpEdge)<<endl;
                    mapaAristas.insert(tmpEdge);
                }
            }
        }
    }

    //colocando ahora en un polydata
    vtkSmartPointer<vtkPoints> points =  vtkSmartPointer<vtkPoints>::New();

    //Imprimir el tamaño del mapa
    //std::cout<<"Nro de Aristas en el mapa: "<<mapaAristas.size()<<endl;

    vtkSmartPointer<vtkExtractEdges> extractEdges = vtkSmartPointer<vtkExtractEdges>::New();
    extractEdges->SetInputConnection(delaunay->GetOutputPort());
    extractEdges->Update();

    vtkCellArray* lines= extractEdges->GetOutput()->GetLines();
    //std::cout << "There are " << lines->GetNumberOfCells() << " aristas en total." << std::endl;

    for ( auto it = mapaAristas.begin(); it != mapaAristas.end(); ++it ){
        //Recorriendo arista
        points->InsertNextPoint (it->second[0][0], it->second[0][1], it->second[0][2]);
        myfile << it->second[0][0] << "\t"<< it->second[0][1] << std::endl;
        //std::cout<<it->second[0][0]<<" "<<it->second[0][1]<<" "<<it->second[0][2]<<std::endl;
    }

    vtkSmartPointer<vtkPolyData> pointsPolydata =    vtkSmartPointer<vtkPolyData>::New();
    pointsPolydata->SetPoints(points);

    // Visualize
    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
#if VTK_MAJOR_VERSION <= 5
    vertexFilter->SetInputConnection(pointsPolydata->GetProducerPort());
#else
    vertexFilter->SetInputData(pointsPolydata);
#endif
    vertexFilter->Update();

    pointsSilhouette = vtkSmartPointer<vtkPolyData>::New();
    pointsSilhouette->ShallowCopy(vertexFilter->GetOutput());

    //cerrando el archivo de puntos de contorno
    myfile.close();

    return pointsSilhouette;
}

//Interactor del estilo del mouse
//vtkStandardNewMacro(MouseInteractorStyle);

//CARGAR EL MODELO O PLANTILLA PARA VISUALIZACION formato (VTK,OBJ)
void Create3DBones::cargarModelo(std::string filename, std::string filetype){

    std::cout << "FileName: "<< filename <<std::endl;

    vtkSmartPointer<vtkOBJReader> readerObj = vtkSmartPointer<vtkOBJReader>::New();
    vtkPolyDataReader* readerVtk = vtkPolyDataReader::New();
    vtkMetaImageReader * readerimagemha = vtkMetaImageReader::New();
    //Conseguiendo puntos del modelo
    vtkSmartPointer<vtkPolyData> polydata =  vtkSmartPointer<vtkPolyData>::New();

    if(filetype.compare("obj")==0){
        readerObj->SetFileName(filename.c_str());
        readerObj->Update();
        polydata->ShallowCopy(readerObj->GetOutput());

    }else if(filetype.compare("vtk")==0){
        readerVtk->SetFileName(filename.c_str());
        readerVtk->Update();
        polydata->ShallowCopy(readerVtk->GetOutput());
    }else if(filetype.compare("mha")==0){
        readerimagemha->SetFileName(filename.c_str());
        readerimagemha->Update();
        polydata->ShallowCopy(readerimagemha->GetOutput());
    }

    //Pasando puntos de malla a matriz general
    //dataMalla = new DataMalla();
    dataMalla = new DataMalla(polydata->GetNumberOfPoints(),3);
    dataMalla->buildDataMallaWithPolyData(&polydata);
    dataMalla->setPolyData(polydata);
    dataMalla->setBoundsData(&polydata);
    dataMalla->setCenterData(&polydata);



    //Visualizando
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

    if(filetype.compare("obj")==0){
        mapper->SetInputConnection(readerObj->GetOutputPort());
    }else if(filetype.compare("vtk")==0){
        mapper->SetInputConnection(readerVtk->GetOutputPort());
    }


    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);

    //Dibujo de ejes
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->Translate(5.0, 0.0, 0.0);
    vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();

    renderer->AddActor(axes);
    // VTK/Qt wedded
    this->ui->qvtkWidget_2->GetRenderWindow()->AddRenderer(renderer);
}


//CARGAR MODELOS CON FORMATO SOLO VTK
void Create3DBones::cargarFileVTK(std::string filename)
{
   vtkPolyDataReader* reader = vtkPolyDataReader::New();
   reader->SetFileName(filename.c_str());
   reader->Update();

   //Visualizando
   vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
   mapper->SetInputConnection(reader->GetOutputPort());

   vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
   actor->SetMapper(mapper);

   vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
   renderer->AddActor(actor);

   // VTK/Qt wedded
   this->ui->qvtkWidget_2->GetRenderWindow()->AddRenderer(renderer);
}

//menu abrir para archivos VTK para su visualizacion
void Create3DBones::load_promedio()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Model"), "/home", tr("Model Files (*.vtk *.obj)"));
    if (!fileName.isEmpty()) {
        cargarFileVTK(fileName.toStdString());
    }else if (fileName.isEmpty()) {
        QMessageBox::information(this, tr("Model Viewer"), tr("No puede cargar el modelos %1.").arg(fileName));
        return;
    }
}

//menu abrir para archivos OBJ para su visualizacion
void Create3DBones::load_model(){
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Model"), "/home", tr("Model Files (*.vtk *.obj )"));
    QFileInfo fi(fileName);
    QString ext = fi.suffix();
    std::cout<<ext.toStdString();
    if (!fileName.isEmpty()) {
        cargarModelo(fileName.toStdString(),ext.toStdString());
    }else if (fileName.isEmpty()) {
        QMessageBox::information(this, tr("Model Viewer"), tr("No puede cargar el modelos %1.").arg(fileName));
        return;
    }

}

void Create3DBones::open()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"), QDir::currentPath());
    setRutaImagen(fileName);//guardando la ruta de la imagen
    if (!fileName.isEmpty()) {
        QImage image(fileName);
        if (image.isNull()) {
            QMessageBox::information(this, tr("Image Viewer"), tr("Cannot load %1.").arg(fileName));
            return;
        }

        image_label->setPixmap(QPixmap::fromImage(image));
        scaleFactor = 1.0;
        fitToWindowAct->setEnabled(true);
        updateActions();

        if (!fitToWindowAct->isChecked())
            image_label->adjustSize();
    }
}


void Create3DBones::zoomIn()
{
    scaleImage(1.25);
}

void Create3DBones::zoomOut()
{
    scaleImage(0.8);
}

void Create3DBones::normalSize()
{
    image_label->adjustSize();
    scaleFactor = 1.0;
}

void Create3DBones::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}


void Create3DBones::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    zoomInAct = new QAction(tr("Zoom &In (25%)"), this);
    zoomInAct->setShortcut(tr("Ctrl++"));
    zoomInAct->setEnabled(false);
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Zoom &Out (25%)"), this);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    zoomOutAct->setEnabled(false);
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    normalSizeAct = new QAction(tr("&Normal Size"), this);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

    fitToWindowAct = new QAction(tr("&Fit to Window"), this);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

}

void Create3DBones::createMenus()
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(normalSizeAct);
    viewMenu->addSeparator();
    viewMenu->addAction(fitToWindowAct);

    this->menuBar()->addMenu(fileMenu);
    this->menuBar()->addMenu(viewMenu);
}

void Create3DBones::updateActions()
{
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void Create3DBones::scaleImage(double factor)
{
    Q_ASSERT(image_label->pixmap());
    scaleFactor *= factor;
    image_label->resize(scaleFactor * image_label->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void Create3DBones::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));
}

void Create3DBones::findContornoOnImage(){

    /*typedef itk::Image<double, 2>  DoubleImageType;

    std::cout << "AplicarCanny: "<< getRutaImagen().c_str() <<std::endl;
    typedef itk::ImageFileReader<DoubleImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(getRutaImagen());
    //el reader solo tiene una salida de tipo para itk mas no para el tipo para qt
    //se tiene que pasarlo solo por ruta de imagen

    //a menos que construyas la salida del filtro canny como imagen
    QuickView viewer;
    viewer.AddImage<DoubleImageType>(reader->GetOutput());
    viewer.Visualize();*/
}

void Create3DBones::setRutaImagen(QString filename){
    //guardar la ruta de la imagen
    this->rutaImagen = filename;
}

std::string Create3DBones::getRutaImagen(){
    return rutaImagen.toStdString();
}

void Create3DBones::cargar3Dimage(std::string filename){
    //creacion del simulador de rayos x
    vtkRenderWindow *renWin = getRenderWindow(this->ui->qvtkWidget_3);
    //este tipo interactor es vtkrenderwindowinteractor no es qvtkitneractors
    vtkRenderWindowInteractor *iren = renWin->GetInteractor();
    simXray = new SimuladorXray(renWin,iren);

    //asignamos la ruta de la imagen 3D a cargar
    simXray->setDirectory3Dimage(filename);

    //renderizamos la imagen 3D
    simXray->show();
}

void Create3DBones::load_3dimage(){

    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Model"), "/home", tr("Model Files (*.vtk *.mha )"));
    QFileInfo fi(fileName);
    QString ext = fi.suffix();
    std::cout<<ext.toStdString();
    if (!fileName.isEmpty()) {
        cargar3Dimage(fileName.toStdString());
    }else if (fileName.isEmpty()) {
        QMessageBox::information(this, tr("Model Viewer"), tr("No puede cargar el modelos %1.").arg(fileName));
        return;
    }

    //cargamos el dicom
    /*QString fileName = QFileDialog::getExistingDirectory(this, tr("Abrir Directorio"),"/home", QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    if (!fileName.isEmpty()) {
        cargar3Dimage(fileName.toStdString());
    }else if (fileName.isEmpty()) {
        QMessageBox::information(this, tr("Model Viewer"), tr("No puede cargar el modelos %1.").arg(fileName));
        return;
    }*/

}

void Create3DBones::applyBlend(const QString& typeblend){
    if(!QString::compare(typeblend, "MIP", Qt::CaseInsensitive)){
        simXray->applyBlendMIP();
    }else if(!QString::compare(typeblend, "CompositeRamp", Qt::CaseInsensitive)){
        simXray->applyBlendCompRamp();
    }else if(!QString::compare(typeblend, "CompositeShadeRamp", Qt::CaseInsensitive)){
        simXray->applyBlendCompShadeRamp();
    }else if(!QString::compare(typeblend, "CTSkin", Qt::CaseInsensitive)){
        simXray->applyBlendCTSkin();
    }else if(!QString::compare(typeblend, "CTBone", Qt::CaseInsensitive)){
        simXray->applyBlendCTBone();
    }else if(!QString::compare(typeblend, "CTMuscle", Qt::CaseInsensitive)){
        simXray->applyBlendCTMuscle();
    }else if(!QString::compare(typeblend, "RGBComposite", Qt::CaseInsensitive)){
        simXray->applyBlendRgbComp();
    }
}

void Create3DBones::applyTransSimulXray(){
    QString rotx = this->ui->editRotX->text();
    QString roty = this->ui->editRotY->text();
    QString rotz = this->ui->editRotZ->text();
    simXray->transformRotImageData(rotx.toDouble(),roty.toDouble(),rotz.toDouble());
}

void Create3DBones::setWindowXray(int opacwin){
    simXray->setOpacityWindow(opacwin);
}
void Create3DBones::setLevelXray(int opaclev){
    simXray->setOpacityLevel(opaclev);
}

void Create3DBones::applyOpacityValues(){
    //aplicando los valores actuales de window y level
    //dependiendo del tipo de blend
    simXray->updateMapper();
}

void Create3DBones::evalClip(bool valClip){
    simXray->setFlagClip(valClip);
}

void Create3DBones::loadProbeFilter(){
    string filenameDic = "/home/cris/Documentos/DICOMsamples/HipPelvisM";
    //string filenameVtk = "/home/cris/Documentos/DICOMsamples/pelvisFemenina.vtk";
    //string filenameDic = "/home/cris/Descargas/PolyDataToImageData/build/SphereVolume.mhd";
    string filenameVtk = "/home/cris/Documentos/DICOMsamples/pelvisFemenina.vtk";
    vtkDICOMImageReader *dicomReader = vtkDICOMImageReader::New();
    dicomReader->SetDirectoryName(filenameDic.c_str());
    dicomReader->Update();

    //Marching cubes seudo marching cubes
    vtkPolyDataReader* readerVtk = vtkPolyDataReader::New();
    readerVtk->SetFileName(filenameVtk.c_str());
    readerVtk->Update();

    // An isosurface, or contour value of 500 is known to correspond to the
    // skin of the patient. MARCHING CUBES HERE
    vtkSmartPointer<vtkMarchingCubes> skinExtractor = vtkSmartPointer<vtkMarchingCubes>::New();
    skinExtractor->SetInputConnection(dicomReader->GetOutputPort());
    //skinExtractor->SetInputConnection(readerVtk->GetOutputPort());
    skinExtractor->SetValue(0, 250);



    //SIN LA PARTE DEL CLIP
    // Define a spherical clip function to clip the isosurface
    vtkSmartPointer<vtkSphere> clipFunction =
            vtkSmartPointer<vtkSphere>::New();
    clipFunction->SetRadius(50);
    clipFunction->SetCenter(150, 250, 500);


    // Clip the isosurface with a sphere
    vtkSmartPointer<vtkClipDataSet> skinClip =
            vtkSmartPointer<vtkClipDataSet>::New();
    //skinClip->SetInputConnection(skinExtractor->GetOutputPort()); //STRUCTURE WILL BE CLIPED
    skinClip->SetInputConnection(readerVtk->GetOutputPort());
    skinClip->SetClipFunction(clipFunction);// FORM OF THE CLIP (SPHERE)
    skinClip->SetValue(0);
    skinClip->GenerateClipScalarsOn();
    skinClip->Update();

    vtkSmartPointer<vtkDataSetMapper> skinMapper =
            vtkSmartPointer<vtkDataSetMapper>::New();
    skinMapper->SetInputConnection(skinClip->GetOutputPort());
    //skinMapper->SetInputConnection(skinExtractor->GetOutputPort());
    skinMapper->ScalarVisibilityOff();

    vtkSmartPointer<vtkActor> skin = vtkSmartPointer<vtkActor>::New();
    skin->SetMapper(skinMapper);
    //skin->GetProperty()->SetDiffuseColor(1, .49, .25);
    skin->GetProperty()->SetDiffuseColor(0, 1, 0); //GREEN CONTOUR MARCHING CUBES

    vtkSmartPointer<vtkProperty> backProp =
            vtkSmartPointer<vtkProperty>::New();
    //backProp->SetDiffuseColor(0.8900, 0.8100, 0.3400);
    backProp->SetDiffuseColor(1, 0, 0);//RED INTERIOR OF MARCHING CUBES
    skin->SetBackfaceProperty(backProp);//setbackfaceproperty

    // Define a model for the "lens". Its geometry matches the implicit
    // sphere used to clip the isosurface
    vtkSmartPointer<vtkSphereSource> lensModel =
            vtkSmartPointer<vtkSphereSource>::New();
    lensModel->SetRadius(50);
    lensModel->SetCenter(150, 250, 500);
    lensModel->SetPhiResolution(300);
    lensModel->SetThetaResolution(150);

    // Sample the input volume with the lens model geometry
    vtkSmartPointer<vtkProbeFilter> lensProbe =
            vtkSmartPointer<vtkProbeFilter>::New();
    //lensProbe->SetInputConnection(lensModel->GetOutputPort()); //ESTRUCTURA A INTEPORLAR SPHERESOURCE
    lensProbe->SetInputConnection(readerVtk->GetOutputPort());
    lensProbe->SetSourceConnection(dicomReader->GetOutputPort()); //DICOM SOURCE INTENSITY

    // Clip the lens data with the isosurface value
    vtkSmartPointer<vtkClipDataSet> lensClip =
            vtkSmartPointer<vtkClipDataSet>::New();
    lensClip->SetInputConnection(lensProbe->GetOutputPort());
    lensClip->SetValue(250);
    lensClip->GenerateClipScalarsOff();
    lensClip->Update();

    // Define a suitable grayscale lut
    vtkSmartPointer<vtkLookupTable> bwLut =
            vtkSmartPointer<vtkLookupTable>::New();
    bwLut->SetTableRange (0, 2048);
    bwLut->SetSaturationRange (0, 0);
    bwLut->SetHueRange (0, 0);
    bwLut->SetValueRange (.2, 1);
    bwLut->Build();

    vtkSmartPointer<vtkDataSetMapper> lensMapper =
            vtkSmartPointer<vtkDataSetMapper>::New();
    lensMapper->SetInputConnection(lensClip->GetOutputPort());
    lensMapper->SetScalarRange(lensClip->GetOutput()->GetScalarRange());
    /*lensMapper->SetInputConnection(lensProbe->GetOutputPort());
    lensMapper->SetScalarRange(lensProbe->GetOutput()->GetScalarRange());*/
    lensMapper->SetLookupTable(bwLut);

    vtkSmartPointer<vtkActor> lens = vtkSmartPointer<vtkActor>::New();
    lens->SetMapper(lensMapper);

    // It is convenient to create an initial view of the data. The FocalPoint
    // and Position form a vector direction. Later on (ResetCamera() method)
    // this vector is used to position the camera to look at the data in
    // this direction.
    /*vtkSmartPointer<vtkCamera> aCamera =
            vtkSmartPointer<vtkCamera>::New();
    aCamera->SetViewUp (0, 0, -1);
    aCamera->SetPosition (0, -1, 0);
    aCamera->SetFocalPoint (0, 0, 0);
    aCamera->ComputeViewPlaneNormal();
    aCamera->Azimuth(30.0);
    aCamera->Elevation(30.0);*/

    // Create the renderer, the render window, and the interactor. The renderer
    // draws into the render window, the interactor enables mouse- and
    // keyboard-based interaction with the data within the render window.
    //
    vtkSmartPointer<vtkRenderer> aRenderer =
            vtkSmartPointer<vtkRenderer>::New();

    ui->qvtkWidget_2->GetRenderWindow()->AddRenderer(aRenderer);


    // Actors are added to the renderer. An initial camera view is created.
    // The Dolly() method moves the camera towards the FocalPoint,
    // thereby enlarging the image.
    aRenderer->AddActor(lens);
    aRenderer->AddActor(skin);
    //aRenderer->SetActiveCamera(aCamera);
    aRenderer->ResetCamera ();
    //aCamera->Dolly(1.5);

    // Set a background color for the renderer and set the size of the
    // render window (expressed in pixels).
    aRenderer->SetBackground(.2, .3, .4);
    ui->qvtkWidget_2->GetRenderWindow()->SetSize(640, 480);

    // Note that when camera movement occurs (as it does in the Dolly()
    // method), the clipping planes often need adjusting. Clipping planes
    // consist of two planes: near and far along the view direction. The
    // near plane clips out objects in front of the plane; the far plane
    // clips out objects behind the plane. This way only what is drawn
    // between the planes is actually rendered.
    aRenderer->ResetCameraClippingRange ();


}


void Create3DBones::setRatFramesXray(double ratFrames){
    simXray->setRatFrames(ratFrames);
}
void Create3DBones::setFactRedXray(double factReduction){
    simXray->setFactReduction(factReduction);
}

void Create3DBones::saveVirtualXray(){
    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(simXray->getRenderWindowSimXray());
    windowToImageFilter->SetMagnification(3); //set the resolution of the output image (3 times the current resolution of vtk render window)
    windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
    windowToImageFilter->ReadFrontBufferOff(); // read from the back buffer
    windowToImageFilter->Update();

    vtkSmartPointer<vtkPNGWriter> writer = vtkSmartPointer<vtkPNGWriter>::New();
    writer->SetFileName("simulatedXray.png");
    writer->SetInputConnection(windowToImageFilter->GetOutputPort());
    writer->Write();
    QMessageBox Msgbox;
    Msgbox.setText("Se guardo la imagen exitosamente");
    Msgbox.exec();
}

int Create3DBones::getBinMask(){
    cout<<"Binary Mask"<<endl;

    //const char * inputImageName   = "/home/cris/Escritorio/MHAVolumeVisibleHuman/volOriBase.mha";
    const char * inputImageName   = "/home/cris/Escritorio/pelvisFull.mha";
    //const char * inputMeshName    = "/home/cris/Escritorio/MHAVolumeVisibleHuman/pelvisFemIzq.vtk";
    const char * inputMeshName    = "/home/cris/Escritorio/MHAVolumeVisibleHuman/pelvisFemIzqSeg.obj";
    //const char * inputMeshName    = "/home/cris/Documentos/PelvisFem.vtk";
    //const char * inputImageName   = "/home/cris/Escritorio/MHAVolumeVisibleHuman/HeadMRVolume.mha";
    //const char * inputMeshName    = "/home/cris/Escritorio/MHAVolumeVisibleHuman/torus.vtk";
    const char * outputImageName  = "/home/cris/Escritorio/MHAVolumeVisibleHuman/volumeBinFinal.mha"    ;

    const unsigned int Dimension = 3;
    typedef double MeshPixelType;

    vtkSmartPointer<vtkMetaImageReader> readerVtk = vtkSmartPointer<vtkMetaImageReader>::New();

    //vtkPolyDataReader* readerVtk = vtkPolyDataReader::New();
    //Conseguiendo puntos del modelo
    vtkSmartPointer<vtkPolyData> polydata =  vtkSmartPointer<vtkPolyData>::New();

    readerVtk->SetFileName(inputImageName);

    readerVtk->Update();
    polydata->ShallowCopy(readerVtk->GetOutput());

    //Visualizando
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(readerVtk->GetOutputPort());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);

    // VTK/Qt wedded
    this->ui->qvtkWidget_2->GetRenderWindow()->AddRenderer(renderer);

    typedef itk::Mesh< MeshPixelType, Dimension > MeshType;

    typedef itk::MeshFileReader< MeshType >  MeshReaderType;
    MeshReaderType::Pointer meshReader = MeshReaderType::New();
    meshReader->SetFileName( inputMeshName );

    typedef unsigned char                           InputPixelType;
    typedef itk::Image< InputPixelType, Dimension > InputImageType;
    typedef itk::ImageFileReader< InputImageType >  ImageReaderType;

    ImageReaderType::Pointer imageReader = ImageReaderType::New();
    imageReader->SetFileName( inputImageName );

    typedef unsigned char                             OutputPixelType;
    typedef itk::Image< OutputPixelType, Dimension >  OutputImageType;

    typedef itk::CastImageFilter< InputImageType, OutputImageType > CastFilterType;
    CastFilterType::Pointer cast = CastFilterType::New();
    cast->SetInput( imageReader->GetOutput() );

    typedef itk::TriangleMeshToBinaryImageFilter< MeshType, OutputImageType > FilterType;
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput( meshReader->GetOutput() );
    filter->SetInfoImage( cast->GetOutput() );
    filter->SetInsideValue( itk::NumericTraits< OutputPixelType >::max() );
    try
    {
        filter->Update();
    }
    catch( itk::ExceptionObject & error )
    {
        std::cerr << "Error: " << error << std::endl;
        return EXIT_FAILURE;
    }

    typedef itk::ImageFileWriter< OutputImageType > WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName( outputImageName );
    writer->SetInput( filter->GetOutput() );
    try
    {
        writer->Update();
    }
    catch( itk::ExceptionObject & error )
    {
        std::cerr << "Error: " << error << std::endl;
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;

}

