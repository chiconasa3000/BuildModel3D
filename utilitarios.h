#ifndef UTILITARIOS_H
#define UTILITARIOS_H

#include <eigen3/Eigen/Dense>
using Eigen::MatrixXd;
using Eigen::VectorXd;


class utilitarios
{
public:
    utilitarios();
    //funcion recursiva de segmentacion de puntos
    MatrixXd segmPoints(int div, MatrixXd kernel);
};

#endif // UTILITARIOS_H
