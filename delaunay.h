#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <eigen3/Eigen/Dense>
#include <vector>
#include <iostream>
#include "DelaunayUtil/grafo.h"

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;

class Delaunay
{
private:
    MatrixXd nubePuntos;
    vector<vector<double>> nubePtos;
    vector<vector<vector<double>>> groupBig;
    Grafo *grafoDelaunay;
    int nroPuntos;
public:
    //constructor con puntos ya definidos
    Delaunay();
    //constructor con puntos de una silueta
    Delaunay(MatrixXd &silueta);
    void nubeMatrix2nubeVect();
    void ordenarNubePtos();
    void dividirNubePtos();
    void dividirRecNubePtos(int divisor,vector<vector<double>>);
    void estructurarInGrafo();
    Grafo * getGrafoDelaunay();

    long getNroPuntosNube();
    vector<vector<double>> getNubeVector();
    MatrixXd getNubeMatrix();
    void printNubePuntos();
    void printNubeVect();
    void printGroupBig();
};

#endif // DELAUNAY_H
