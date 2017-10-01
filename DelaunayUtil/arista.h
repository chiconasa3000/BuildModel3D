#ifndef ARISTA_H
#define ARISTA_H
#include <iostream>
#include <vector>
#include "punto.h"
#include "math.h"
using namespace std;

//advertencia: arista usa punto y punto usa arista
class Punto;

class Arista
{
public:
    Arista();
    //Arista(Punto *ptoO, Punto *ptoD);
    Arista(Punto ptoO, Punto ptoD);
    Arista(Punto ptoO, Punto ptoD,int difA,int difB);
    Arista(int idOrigen, int idDestino);

    Arista& operator=(const Arista &rhs);
    Arista* operator=(const Arista *rhs);
    void copiar(const Arista &a);
//    void construirArista(int idPuntoA,int idePuntoB);
//    void getP_MoreLeft();
//    void getP_MoreRight();
//    void getOpossedPoint(int idPunto);
    void printArista();
    int getIdOrigen();
    int getIdDestino();
    void setIdOrigen(int id);
    void setIdDestino(int id);
    vector<double> getVectDir();
    //recuperacion de puntos
    Punto getPtoOrigen();
    Punto getPtoDestino();
    void setPtoOrigen(Punto o);
    void setPtoDestino(Punto ds);

    //recuperacion del angulo de arista con linea base
    double getAnguloLb();
    void setAnguloLb(double angulo);
private:
    //static int idArista;

    //Arista muy especial q posee los puntos y no los ides
    Punto *ptoOrigen;
    Punto *ptoDestino;

    //Arista normal que solo posee los ides de los puntos
    int idOrigen = 0;
    int idDestino = 0;

    //angulo de arista con la linea base
    double anguloWithLineb = 0;

    vector<double> vectDir;
};

#endif // ARISTA_H
