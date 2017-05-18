#ifndef GESTORPERSPECTIVA_H
#define GESTORPERSPECTIVA_H

#include <iostream>
using namespace std;
#include <eigen3/Eigen/Dense>
#include "DataMalla.h"
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Vector2d;
using Eigen::Matrix3d;

class GestorPerspectiva
{
    MatrixXd coordEjesModel;
    MatrixXd *dataMalla;
    VectorXd *vectTra;
    VectorXd *vectRot;
    MatrixXd matRotGen;
    MatrixXd matTrans;
    MatrixXd matProy;
    MatrixXd mallaProy3d;
    MatrixXd coordsZ3d;
    MatrixXd coordsZ2d;
    int numPuntos;

public:
    GestorPerspectiva();
    ~GestorPerspectiva();

    void setDataMalla(MatrixXd &dataMalla);
    void setVectTrasRot(VectorXd &tra, VectorXd &rot);
    void printCoordModel();
    void inicioModelo();
    void calcMatRotGen();
    void calcMatTransformacion();
    void calcMatProyeccion();
    MatrixXd getMallaProy3d();
    MatrixXd reproyeccion(MatrixXd &dMalla);
};

#endif // GESTORPERSPECTIVA_H
