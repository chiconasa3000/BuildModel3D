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
    vector<double> getVectDir();
    //en punto
    Punto getPtoOrigen();
    Punto getPtoDestino();
private:
    //static int idArista;

    //Arista muy especial q posee los puntos y no los ides
    Punto *ptoOrigen;
    Punto *ptoDestino;

    //Arista normal que solo posee los ides de los puntos
    int idOrigen = 0;
    int idDestino = 0;

    vector<double> vectDir;
};

#endif // ARISTA_H
