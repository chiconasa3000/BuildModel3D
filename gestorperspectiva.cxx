#include "gestorperspectiva.h"
#define PI 3.14159265
#define DISTFOCAL 5

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
}

void GestorPerspectiva::setVectTrasRot(VectorXd &tra, VectorXd &rot){
    vectRot = &rot;
    vectTra = &tra;
}

void GestorPerspectiva::printCoordModel(){
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

void GestorPerspectiva::calcMatProyeccion(){
    matProy.resize(3,4);
    matProy = MatrixXd::Zero(3,4);
    matProy(0,0) = DISTFOCAL;
    matProy(1,1) = DISTFOCAL;
    matProy(2,2) = 1;
}



void GestorPerspectiva::inicioModelo(){
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

   cout<<dataMallaHomg.transpose().block(0,0,1,4)<<endl;



    //calcVectRotGen();
    //calcMatTransformacion();
    //MatrixXd nuevoDataMalla(4,numPuntos);
    //nuevoDataMalla = matProy*dataMallaHomg;

    calcMatProyeccion();
    MatrixXd mallaProy2d(3,numPuntos);
    mallaProy2d = matProy*dataMallaHomg;
    //std::cout<<"MallaProy2D: "<<mallaNueva.transpose().row(2)<<endl;

    mallaProy3d.resize(4,numPuntos);
    //mallaProy3d.block(0,0,2,numPuntos) << mallaNueva.transpose().block(0,0,2,numPuntos);
    mallaProy3d.block(0,0,2,numPuntos) << mallaProy2d.block(0,0,2,numPuntos);
    //mallaProy3d.row(2) << DISTFOCAL*vectUnos;
    mallaProy3d.row(2) << mallaProy2d.row(2);

    coordsZ2d.resize(1,numPuntos);
    coordsZ2d.block(0,0,1,numPuntos) << mallaProy2d.row(2);

    //dividiendo a todas la coordenadas por el valor Z 3D
    /*for(int j=0; j<3; j++){
        for(int i = 0; i < dataMalla->rows(); i++){

            mallaProy3d(j,i) = mallaProy3d(j,i) / coordsZ(0,i);
        }

    }*/

    //cout << "cordZ: "<< coordsZ(0,0)<<endl;

    //std::cout<<"proyeccion con z que debe valer: "<<endl;
    //cout<<mallaProy3d.transpose().block(0,0,1,4)<<endl;


    //mallaProy3d.row(2) << mallaProy2d.row(2);
    //mallaProy3d.row(2) << mallaNueva.transpose().row(2);
    mallaProy3d.row(3) << vectUnos;
}

MatrixXd GestorPerspectiva::getMallaProy3d(){
    MatrixXd reducedMalla(3,numPuntos);
    reducedMalla = mallaProy3d.block(0,0,3,numPuntos);
    MatrixXd finalMalla(numPuntos,3);
    finalMalla = reducedMalla.transpose();
    //std::cout<<"FinalMalla: "<<finalMalla<<endl;
    return finalMalla;

}

MatrixXd GestorPerspectiva::reproyeccion(MatrixXd &dMalla){
    MatrixXd dataMallaHomg(4, dMalla.rows());
    MatrixXd vectUnos = MatrixXd::Ones(1,dMalla.rows());
    MatrixXd transp = dMalla.transpose();
    dataMallaHomg << transp, vectUnos;
    int numPuntos = dMalla.rows();
    //calcVectRotGen();
    //calcMatTransformacion();
    //MatrixXd nuevoDataMalla(4,numPuntos);
    //nuevoDataMalla = matProy*dataMallaHomg;
    calcMatProyeccion();
    MatrixXd mallaProy2d(3,numPuntos);

    //la matriz matProy es la unica que se renueva
    matProy.block(0,0,3,3) << matProy.block(0,0,3,3).inverse();
    mallaProy2d = matProy*dataMallaHomg;

    MatrixXd mProy3d;
    mProy3d.resize(4,numPuntos);
    mProy3d.block(0,0,2,numPuntos) << mallaProy2d.block(0,0,2,numPuntos);
    mProy3d.row(2) << mallaProy2d.row(2);
    mProy3d.row(3) << vectUnos;

    MatrixXd reducedMalla(3,numPuntos);
    reducedMalla = mProy3d.block(0,0,3,numPuntos);
    MatrixXd finalMalla(numPuntos,3);
    finalMalla = reducedMalla.transpose();



    return finalMalla;
}



