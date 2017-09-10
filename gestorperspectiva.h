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
    MatrixXd mallaProy2d;
    MatrixXd coordsZ3d;
    MatrixXd coordsZ3dr;
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
    void aplicarBasicTranf();
    void recalcularZ3d(MatrixXd &silueta);
    MatrixXd getCoordZ3d();
    MatrixXd getCoordZ3dr();
    MatrixXd getMallaProy3d();
    MatrixXd reproyeccion(MatrixXd &dMalla,MatrixXd &z3d);
};

#endif // GESTORPERSPECTIVA_H
