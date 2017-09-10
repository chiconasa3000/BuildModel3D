#include "gestorperspectiva.h"
#define PI 3.14159265
#define DISTFOCAL 50

GestorPerspectiva::GestorPerspectiva()
{
    coordEjesModel.resize(4,5);
    coordEjesModel << 1, 0, 0, 0, 0,
                      0, 0, 1, 0, 0,
                      0, 0, 0, 0, 1,
                      1, 1, 1, 1, 1;
    numPuntos = 0;
}

void GestorPerspectiva::setDataMalla(MatrixXd &dataMalla){

    this->dataMalla = &dataMalla;
    //cout<<"matrizPuntos: "<<*(this->dataMalla);
}

void GestorPerspectiva::setVectTrasRot(VectorXd &tra, VectorXd &rot){
    vectRot = &rot;
    //cout<<*vectRot;
    vectTra = &tra;
    //cout<<*vectTra;
}

void GestorPerspectiva::printCoordModel(){
    std::cout<<"Malla: "<<endl;
    std::cout<< *dataMalla;
    //std::cout<< *vectRot <<""<< *vectTra;
}

void GestorPerspectiva::calcMatRotGen(){
    matRotGen.resize(3,3);
    float wx = vectRot[0][0];
    float wy = vectRot[0][1];
    float wz = vectRot[0][2];

    matRotGen << cos(wy)*cos(wz),                          cos(wy)*sin(wz),                         -sin(wy),
                 sin(wx)*sin(wy)*cos(wz)-cos(wx)*sin(wz),  sin(wx)*sin(wy)*sin(wz)+cos(wx)*cos(wz),  sin(wx)*cos(wy),
                 cos(wx)*sin(wy)*cos(wz)+sin(wx)*sin(wz), cos(wx)*sin(wy)*sin(wz)-sin(wx)*cos(wz),  cos(wx)*cos(wy);

}

void GestorPerspectiva::calcMatTransformacion(){
    matTrans.resize(4,4);
    matTrans.block(0,0,3,3) << matRotGen;
    matTrans.col(3).head(3) << *vectTra;
    matTrans.row(3) << 0,0,0,1;
}

void GestorPerspectiva::aplicarBasicTranf(){
    int numPuntos = dataMalla->rows();
    MatrixXd dataMallaHomg(4, numPuntos);
    //cout<<"MallaOrig: "<<*dataMalla;

    MatrixXd vectUnos = MatrixXd::Ones(1,numPuntos);
    MatrixXd transp = dataMalla->transpose();
    dataMallaHomg << transp, vectUnos;

    calcMatRotGen();
    calcMatTransformacion();

    MatrixXd newMalla = matTrans*dataMallaHomg;

    MatrixXd reducedMalla(3,numPuntos);
    reducedMalla = newMalla.block(0,0,3,numPuntos);

    MatrixXd mallaOrigen(numPuntos,3);
    mallaOrigen = reducedMalla.transpose();

    dataMalla->block(0,0,numPuntos,3) = mallaOrigen.block(0,0,numPuntos,3);
}

void GestorPerspectiva::calcMatProyeccion(){
    matProy.resize(3,4);
    matProy = MatrixXd::Zero(3,4);
    matProy(0,0) = DISTFOCAL;
    matProy(1,1) = DISTFOCAL;
    matProy(2,2) = 1;
}



void GestorPerspectiva::inicioModelo(){
    //cout<<"DATAMALLA:"<<endl;
    //cout<<*dataMalla<<endl;
    numPuntos = dataMalla->rows();
    MatrixXd dataMallaHomg(4, numPuntos);

    //reservando la coordenada Z del espacio 3D
    //MatrixXd coordsZ(1,numPuntos);
    coordsZ3d.resize(1,numPuntos);
    coordsZ3d.block(0,0,1,numPuntos) << dataMalla->transpose().block(2,0,1,numPuntos);
    //cout<<coordsZ<<endl;


    MatrixXd vectUnos = MatrixXd::Ones(1,dataMalla->rows());
    MatrixXd transp = dataMalla->transpose();
    dataMallaHomg << transp, vectUnos;

   //cout<<dataMallaHomg.transpose().block(0,0,1,4)<<endl;

    //calcVectRotGen();
    //calcMatTransformacion();
    //MatrixXd nuevoDataMalla(4,numPuntos);
    //nuevoDataMalla = matProy*dataMallaHomg;

    calcMatProyeccion();
    mallaProy2d.resize(3,numPuntos);
    mallaProy2d = matProy*dataMallaHomg;
    //std::cout<<"MallaProy2D: "<<mallaNueva.transpose().row(2)<<endl;


    //mallaProy3d.block(0,0,2,numPuntos) << mallaNueva.transpose().block(0,0,2,numPuntos);
    //mallaProy2d.block(0,0,2,numPuntos) << mallaProy2d.block(0,0,2,numPuntos);
    //mallaProy3d.row(2) << DISTFOCAL*vectUnos;
    //mallaProy2d.row(2) << mallaProy2d.row(2);

    //coordsZ2d.resize(1,numPuntos);
    //coordsZ2d.block(0,0,1,numPuntos) << mallaProy2d.row(2);

    //dividiendo a todas la coordenadas por el valor Z 3D
    for(int j=0; j<3; j++){
        for(int i = 0; i < dataMalla->rows(); i++){

            mallaProy2d(j,i) = mallaProy2d(j,i) / coordsZ3d(0,i);
        }

    }

    //mallaProy2d y coordsZ3d son los elementos a guardar
    //filas de 1 y 3 con numPtos como cantidad de columnas

}

MatrixXd GestorPerspectiva::getMallaProy3d(){
    //MatrixXd reducedMalla(3,numPuntos);
    //reducedMalla = mallaProy3d.block(0,0,3,numPuntos);
    MatrixXd finalMalla(numPuntos,3);
    finalMalla = mallaProy2d.transpose();
    //std::cout<<"FinalMalla: "<<finalMalla<<endl;
    return finalMalla;

}

MatrixXd GestorPerspectiva::reproyeccion(MatrixXd &dMalla,MatrixXd &z3d){

    //MatrixXd vectUnos = MatrixXd::Ones(1,dMalla.rows());
    MatrixXd dataMallaHomg = dMalla.transpose();
    //dataMallaHomg << transp, vectUnos;
    int numPuntos = dMalla.rows();
    //calcVectRotGen();
    //calcMatTransformacion();
    //MatrixXd nuevoDataMalla(4,numPuntos);
    //nuevoDataMalla = matProy*dataMallaHomg;
    calcMatProyeccion();
    MatrixXd mallaProy3d(3,numPuntos);

    MatrixXd newMatProy(3,3);
    //la matriz matProy es la unica que se renueva
    newMatProy = matProy.block(0,0,3,3).inverse();

    mallaProy3d = newMatProy*dataMallaHomg;

    //multiplicando a todas la coordenadas por el valor Z 3D
    for(int j=0; j<3; j++){
        //iteramos por columna
        for(int i = 0; i < dMalla.rows(); i++){
            //iteramos todas las filas de la columna seleccionada
            mallaProy3d(j,i) = mallaProy3d(j,i) * z3d(0,i);
        }

    }

    MatrixXd finalMalla(numPuntos,3);
    finalMalla = mallaProy3d.transpose();
    //std::cout<<"BACKMalla: "<<finalMalla<<endl;
    return finalMalla;
}

void GestorPerspectiva::recalcularZ3d(MatrixXd &silueta){
    //buscamos item by item en la lista de mallaproy2d

    int numPtosSilueta = silueta.rows();
    MatrixXd mallaTran = mallaProy2d.transpose();
    int numPtosMallaProy = mallaTran.rows();

    //cout<<"Silueta"<<endl;
    //cout<<silueta;

    coordsZ3dr.resize(1,numPtosSilueta);
    int contZ = 0;


    for(int i=0;i<numPtosSilueta;i++){
        for(int j=0;j<numPtosMallaProy;j++){
            bool a = mallaTran(j,0)==silueta(i,0);
            bool b = mallaTran(j,1)==silueta(i,1);
            bool c = mallaTran(j,2)==silueta(i,2);
            if(a == true && b==true && c==true){
                //almacenamos como nueva coordenada Z3d
                cout<<"estoy aqui";
                coordsZ3dr(0,contZ) = coordsZ3d(0,j);
                contZ++;
            }else
                continue;
        }
    }

    cout<<"la coordenadas Z reducidas son:"<<coordsZ3dr.transpose();
}

MatrixXd GestorPerspectiva::getCoordZ3d(){
    return coordsZ3d;
}

MatrixXd GestorPerspectiva::getCoordZ3dr(){
    return coordsZ3dr;
}


