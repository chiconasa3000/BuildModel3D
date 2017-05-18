#ifndef PERSPECDATA_H
#define PERSPECDATA_H

#include <iostream>
using namespace std;
#include <eigen3/Eigen/Dense>
using Eigen::VectorXd;

class PerspecData
{
private:
    VectorXd vtraslacion;
    VectorXd vrotacion;
public:
    PerspecData();
    ~PerspecData();
    void setVtraslacion(float x, float y, float z);
    void setTx(float val);
    void setTy(float val);
    void setTz(float val);
    VectorXd& getVtraslacion();
    void setVrotacion(float x, float y, float z);
    void setRx(float val);
    void setRy(float val);
    void setRz(float val);
    VectorXd& getVrotacion();
};

#endif // PERSPECDATA_H
