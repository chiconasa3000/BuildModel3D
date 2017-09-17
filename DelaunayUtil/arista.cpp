#include "arista.h"

//ojo los puntos origen y destino su lista de aristas debe ser vacia

Arista::Arista(){
    //ptoOrigen = NULL;
    //ptoDestino = NULL;
}

Arista::Arista(int idPtoO, int idPtoD)
{

    //ptoOrigen = new Punto(ptoO);
    //ptoDestino = new Punto(ptoD);

    //los puntos deben estar relacionados al vector de puntos original
    idOrigen = idPtoO;
    idDestino = idPtoD;

    //ptoOrigen->copiar(ptoO);
    //ptoDestino->copiar(ptoD);
    //ptoOrigen = &ptoO;
    //ptoDestino = &ptoD;

}

void Arista::printArista(){
    cout<<idOrigen<<" "<<idDestino<<endl;
    //ptoOrigen->printPoint();
    //ptoDestino->printPoint();
}

int Arista::getIdOrigen(){
    return idOrigen;
}
int Arista::getIdDestino(){
    return idDestino;
}
