#ifndef UTILMATHS_H
#define UTILMATHS_H
#include <vector>
#include "arista.h"
#include <numeric>
#include <eigen3/Eigen/Dense>
using Eigen::MatrixXd;
using Eigen::VectorXd;
using namespace std;


class UtilMaths
{
public:
    //template < class RAIter, class Compare >
    //vector<size_t> argSort(RAIter first,RAIter last, Compare comp);
    UtilMaths();
    double calcAngulo(Arista *base,Arista *candidata);
    vector<double> calcPendiente(Arista *base);
    //evaluacion cocircular de 3 puntos (baseI,baseD,candAct) con el candidato siguiente
    bool testCoCircular(Arista *base,Punto candAct, Punto candNext);


};

#endif // UTILMATHS_H
