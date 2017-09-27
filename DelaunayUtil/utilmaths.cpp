#include "utilmaths.h"

UtilMaths::UtilMaths()
{

}

bool UtilMaths::testCoCircular(Arista *base,Punto candAct, Punto candNext){
    Punto ptoOrig = base->getPtoOrigen();
    Punto ptoDest = base->getPtoDestino();

    //El orden de los puntos no se altera a pesar de que varie el subgrafo elegido
    double p1 = pow(ptoOrig.getX(),2) + pow(ptoOrig.getY(),2);
    double q1 = pow(ptoDest.getX(),2) + pow(ptoDest.getY(),2);
    //Punto candidato
    double r1 = pow(candAct.getX(),2) + pow(candAct.getY(),2);

    //Siguiente punto candidatos a evaluar
    double d1 = pow(candNext.getX(),2) + pow(candNext.getY(),2);

    MatrixXd cocircular;

    cocircular.resize(4,4);

    cocircular <<   ptoOrig.getX(), ptoOrig.getY(), p1, 1,
                    ptoDest.getX(), ptoDest.getY(), q1, 1,
                    candAct.getX(), candAct.getY(), r1, 1,
                    candNext.getX(), candNext.getY(), d1, 1;

    if(cocircular.determinant()>0)
        //FALLO debido a que el next pto candidato se encontro al interior del cocircular
        return false;
    else
        //APROBO debido a que el next pto candidato no se encuentra al interior del cocircular
        return true;
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


