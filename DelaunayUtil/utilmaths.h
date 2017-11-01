#ifndef UTILMATHS_H
#define UTILMATHS_H
#include <vector>
#include "arista.h"
#include <numeric>
#include "subgrafo.h"
#include <eigen3/Eigen/Dense>
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Matrix2f;
using Eigen::Vector2f;
using namespace std;


class UtilMaths
{
public:
    //template < class RAIter, class Compare >
    //vector<size_t> argSort(RAIter first,RAIter last, Compare comp);
    UtilMaths();
    double calcAngulo(Arista *base,Arista *candidata,bool esIzq);
    vector<double> calcPendiente(Arista *base,bool esIzq);
    //evaluacion cocircular de 3 puntos (baseI,baseD,candAct) con el candidato siguiente
    bool testCoCircularIn(Arista *base,Punto candAct, Punto candNext);
    bool testCoCircularOut(Arista *base,Punto candAct, Punto candNext);
    //ordena los puntos en direccion contraria al reloj
    bool orderCounterClockwise(Punto *o,Punto *d,Punto *c);
    bool testBelongCirc(Arista *base, Punto candAct, Punto candNext);



};

#endif // UTILMATHS_H
