#ifndef ARISTA_H
#define ARISTA_H
#include <iostream>
#include "punto.h"
using namespace std;

//advertencia: arista usa punto y punto usa arista
class Punto;

class Arista
{
public:
    Arista();
    //Arista(Punto ptoO, Punto ptoD);
    Arista(int idOrigen, int idDestino);
//    void construirArista(int idPuntoA,int idePuntoB);
//    void getP_MoreLeft();
//    void getP_MoreRight();
//    void getOpossedPoint(int idPunto);
    void printArista();
    int getIdOrigen();
    int getIdDestino();
private:
    //static int idArista;
    //Punto *ptoOrigen;
    //Punto *ptoDestino;
    int idOrigen = 0;
    int idDestino = 0;
    int distancia;
};

#endif // ARISTA_H
