#include "DataMalla.h"
DataMalla::DataMalla(){

}

DataMalla::DataMalla(int fil, int col){
    matrix.resize(fil,col);
}

void DataMalla::buildDataMallaWithPolyData(vtkSmartPointer<vtkPolyData> *polyd){
    for(vtkIdType i = 0; i < polyd->Get()->GetNumberOfPoints(); i++)
    {
        double p[3];
        polyd->Get()->GetPoint(i,p);
        //cada punto obtenido tiene un id unico que sera el i
        //este sera la cuarta columna
        VectorXd v(3);
        v << p[0], p[1], p[2];
        matrix.row(i) = v;
    }
}

MatrixXd& DataMalla::getMatrizDataMalla(){
    return matrix;
}

vtkSmartPointer<vtkPolyData> DataMalla::convMatrix2PolyData(MatrixXd & nuevaMalla){
    //en el polydata solo extraemos las coordenadas mas no el identificador del punto
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPolyData> pointsPolydata = vtkSmartPointer<vtkPolyData>::New();

    for(vtkIdType i = 0; i < nuevaMalla.rows(); i++)
    {
        points->InsertNextPoint(nuevaMalla(i,0), nuevaMalla(i,1), nuevaMalla(i,2));
    }

    pointsPolydata->SetPoints(points);

    return pointsPolydata;
}

MatrixXd DataMalla::convPolyData2Matrix(vtkSmartPointer<vtkPolyData> * polyd){

    //Peligro!!! no se puede recuperar el identificador del punto

    MatrixXd matrixFromPolyData(polyd->Get()->GetNumberOfPoints(),3);
    for(vtkIdType i = 0; i < polyd->Get()->GetNumberOfPoints(); i++)
    {
        double p[3];
        polyd->Get()->GetPoint(i,p);
        VectorXd v(3);
        v << p[0], p[1], p[2];
        matrixFromPolyData.row(i) = v;
    }

    return matrixFromPolyData;
}

void DataMalla::printDataMalla(){
    std::cout<<"Imprimiendo Matriz de "
    <<"["<< matrix.rows()<<","<< matrix.cols()<<"]\n";
    std::cout<<matrix<<"\n";
}

void DataMalla::saveInFile(std::string nameFile){
    ofstream myfile;
    myfile.open (nameFile);

    for(int i=0; i<matrix.rows(); i++){
        for(int j=0; j<matrix.cols(); j++){
            myfile<< matrix(i,j) <<"\t";
        }
        myfile<<endl;
    }
    myfile.close();
}

void DataMalla::setBoundsData(vtkSmartPointer<vtkPolyData> *data3d){
    data3d->Get()->GetBounds(bounds_data);
}

void DataMalla::setCenterData(vtkSmartPointer<vtkPolyData> *data3d){
    data3d->Get()->GetCenter(center_data);
}

void DataMalla::setBoundsCont(vtkSmartPointer<vtkPolyData> *data2d){
    data2d->Get()->GetBounds(bounds_contour);
}

void DataMalla::setCenterCont(vtkSmartPointer<vtkPolyData> *data2d){
    data2d->Get()->GetCenter(center_contour);
}

double* DataMalla::calcRatio(){
    double *ratios = new double[2];
    ratios[0] = (bounds_data[1]-bounds_data[0])/(bounds_contour[1]-bounds_contour[0]);
    ratios[1] = (bounds_data[3]-bounds_data[2])/(bounds_contour[3]-bounds_contour[2]);
    return ratios;
}

double* DataMalla::calcTraslacion(){
    double *vectTrasl = new double[3];
    vectTrasl[0] = center_data[0]-center_contour[0];
    vectTrasl[1] = center_data[1]-center_contour[1];
    vectTrasl[2] = center_data[2]-center_contour[2];
    return vectTrasl;
}

vtkSmartPointer<vtkPolyData> DataMalla::setAlignEscala(vtkSmartPointer<vtkPolyData> *polydata){
    double *ratioModel2Proy = calcRatio();
    //Alineacion del modelo con su respectiva proyeccion
    vtkSmartPointer<vtkTransform>transform1 = vtkSmartPointer<vtkTransform>::New();
    transform1->Scale( ratioModel2Proy[0], ratioModel2Proy[1], 1.);

    vtkSmartPointer<vtkTransformPolyDataFilter> tfilter1 = vtkSmartPointer<vtkTransformPolyDataFilter>::New();

#if VTK_MAJOR_VERSION <= 5
    tfilter1->SetInput(polydata->Get());
#else
    tfilter1->SetInputData(polydata->Get());
#endif
    tfilter1->SetTransform(transform1);
    tfilter1->Update();

    //Escala actualizada
    return tfilter1->GetOutput();

}

vtkSmartPointer<vtkPolyData> DataMalla::setAlignPosicion(vtkSmartPointer<vtkPolyData> *polydata){
    double *traslModel2Proy = calcTraslacion();
    //ALineacion del modelo con su respectiva proyeccion
    vtkSmartPointer<vtkTransform> transform2 = vtkSmartPointer<vtkTransform>::New();
    transform2->Translate( traslModel2Proy[0], traslModel2Proy[1], traslModel2Proy[2]);

    vtkSmartPointer<vtkTransformPolyDataFilter> tfilter2 = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
#if VTK_MAJOR_VERSION <= 5
    tfilter2->SetInput(polydata->Get());
#else
    tfilter2->SetInputData(polydata->Get());
#endif

    tfilter2->SetTransform(transform2);
    tfilter2->Update();

    //Traslacion Actualizada
    return tfilter2->GetOutput();
}

void DataMalla::setPolyData(vtkSmartPointer<vtkPolyData> mypoly){
    mypolyd=mypoly;
}

vtkSmartPointer<vtkPolyData> DataMalla::getPolyData(){
    return mypolyd;
}

void DataMalla::setMapEdges(std::unordered_map<double*,double**> &contours){
    mapEdgesContornoMalla = &contours;

    /*std::unordered_map<double*,double*>::iterator ite( mapEdgesContornoMalla->begin() );
    //std::prev( mapEdgesContornoMalla->end() );

    //double * p= mapEdgesContornoMalla->begin()->second;

    //Primer elemento de la lista
    orderContourMalla.push_back(mapEdgesContornoMalla->begin()->second);
    int posPivote = 0;
    for ( auto it = mapEdgesContornoMalla->begin(); ++it !=  mapEdgesContornoMalla->end(); ++it ){
        //Encontrando los puntos semejantes
        std::unordered_map<double*,double*>::const_iterator itFind = mapEdgesContornoMalla.find (input);
        if(it->second[posPivote] == (++it)->second[0]){
            posPivote = 1;
        }else if(it->second[posPivote] == (++it)->second[1]){
            posPivote = 1;
        }

    }*/
}

std::unordered_map<double*,double**> & DataMalla::getMapEdges(){
    //std::cout<<"tamaenfuncion"<< mapEdgesContornoMalla->size()<<std::endl;
    return *mapEdgesContornoMalla;
}


/*std::unordered_map<double*,double*>  DataMalla::getMapEdges(){
    return mapEdgesContornoMalla;
}*/

std::list<double> DataMalla::calcPtosControl(double salto,double a,double b){
    std::list<double> pctrl;
    for(int i = 0;i <= salto; i++){
        pctrl.push_back(a+(i/salto)*(b-a));
    }
    return pctrl;
}

vtkSmartPointer<vtkPolyData> DataMalla::build_mesh_grid(double s1,double s2,double s3){
    double minx = bounds_data[0];
    double maxx = bounds_data[1];
    double miny = bounds_data[2];
    double maxy = bounds_data[3];
    double minz = bounds_data[4];
    double maxz = bounds_data[5];

    double saltox = s1;
    double saltoy = s2;
    double saltoz = s3;

    std::list<double> pctrlX = calcPtosControl(saltox,minx - saltox,maxx + saltox);
    std::list<double> pctrlY = calcPtosControl(saltoy,miny - saltoy,maxy + saltoy);
    std::list<double> pctrlZ = calcPtosControl(saltoz,minz - saltoz,maxz + saltoz);

    vtkSmartPointer<vtkPoints> points =  vtkSmartPointer<vtkPoints>::New();
    meshgridControl =  vtkSmartPointer<vtkPolyData>::New();

    for (std::list<double>::iterator itx=pctrlX.begin(); itx != pctrlX.end(); ++itx){
        for (std::list<double>::iterator ity=pctrlY.begin(); ity != pctrlY.end(); ++ity){
            for (std::list<double>::iterator itz=pctrlZ.begin(); itz != pctrlZ.end(); ++itz){
                points->InsertNextPoint (*itx, *ity, *itz);
    }}}



    meshgridControl->SetPoints(points);

    return meshgridControl;
}

vtkSmartPointer<vtkPolyData> DataMalla::getMeshGridControl(){
    return meshgridControl;
}

