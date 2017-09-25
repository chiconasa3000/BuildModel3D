#include "arista.h"

//ojo los puntos origen y destino su lista de aristas debe ser vacia

Arista::Arista(){
    ptoOrigen = NULL;
    ptoDestino = NULL;
}

/*Arista::Arista(Punto *ptoO, Punto *ptoD){
    //creando espacio de memoria
    ptoOrigen = new Punto();
    ptoDestino = new Punto();

    ptoOrigen = ptoO;
    ptoDestino = ptoD;
    //solo para inicializar aunque nunca los tendra
    idOrigen = -1;
    idDestino = -1;
}*/

Arista::Arista(Punto ptoO, Punto ptoD){
    //creando alojamiento de memoria
    ptoOrigen = new Punto();
    ptoDestino = new Punto();

    ptoOrigen = &ptoO;
    ptoDestino = &ptoD;
    //solo para inicializar aunque nunca los tendra
    idOrigen = -1;
    idDestino = -1;
}


void Arista::copiar(const Arista &a){
    idOrigen = a.idOrigen;
    idDestino = a.idDestino;

    //creando el espacio de almacenamiento
    ptoOrigen = new Punto();
    ptoDestino = new Punto();

    ptoOrigen = a.ptoOrigen;
    ptoDestino = a.ptoDestino;
}

Arista& Arista::operator=(const Arista &rhs) {

    //si son diferentes
    if (this != &rhs) {
        this->idOrigen = -1;
        this->idDestino = -1;
        this->ptoOrigen = NULL;
        this->ptoDestino = NULL;
        copiar(rhs);
    }

    return *this;
}

Arista* Arista::operator=(const Arista *rhs) {

    //si son diferentes
    if (this != rhs) {
        this->idOrigen = -1;
        this->idDestino = -1;
        this->ptoOrigen = NULL;
        this->ptoDestino = NULL;
        copiar(*rhs);
    }

    return this;
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

Punto Arista::getPtoOrigen(){
    return *ptoOrigen;
}

Punto Arista::getPtoDestino(){
    return *ptoDestino;
}

vector<double> Arista::getVectDir(){
    return vectDir;
}


