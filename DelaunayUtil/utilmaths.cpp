#include "utilmaths.h"

UtilMaths::UtilMaths()
{

}

double UtilMaths::calcAngulo(Arista *base,Arista *candidata){
    //calculo del angulo
    vector<double> a = calcPendiente(base);
    vector<double> b = calcPendiente(candidata);

    double num = abs(a[0]*b[0]+a[1]*b[1]);
    double den = sqrt(pow(a[0],2)+pow(a[1],2))*sqrt(pow(b[0],2)+pow(b[1],2));
    double res = num/den;
    double ang_rad = acos(res);
    double new_angulo = ang_rad*180/M_PI;
    return new_angulo;
}

vector<double> UtilMaths::calcPendiente(Arista *base){
    //calcular el vector de la arista
    Punto a = base->getPtoOrigen();
    Punto b = base->getPtoDestino();

    //vector es la diferencia de cada coordenada
    vector <double> vectDir = {a.getX() - b.getX(),a.getY()-b.getY()};
    return vectDir;
}


