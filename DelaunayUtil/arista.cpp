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

    ptoOrigen->copiar(ptoO);
    //ptoOrigen->setX(ptoO.getX());
    //ptoOrigen->setY(ptoO.getY());

    ptoDestino->copiar(ptoD);
    //ptoDestino->setX(ptoD.getX());
    //ptoDestino->setY(ptoD.getY());

    //solo para inicializar aunque nunca los tendra
    idOrigen = ptoO.getIdVert();
    idDestino = ptoD.getIdVert();

    anguloWithLineb = -1;
}


Arista::Arista(Punto ptoO, Punto ptoD,int difA,int difB){
    //creando alojamiento de memoria
    ptoOrigen = new Punto();
    ptoDestino = new Punto();

    ptoOrigen->copiar(ptoO);
    ptoDestino->copiar(ptoD);

    //el idvert general convertir a un indice vertice local
    idOrigen = ptoO.getIdVert()-difA;
    //el idvert general convertir a un indice vertice local
    idDestino = ptoD.getIdVert()-difB;

    anguloWithLineb = -1;
}

void Arista::copiar(const Arista &a){
    idOrigen = a.idOrigen;
    idDestino = a.idDestino;

    //creando el espacio de almacenamiento
    ptoOrigen = new Punto();
    ptoDestino = new Punto();

    ptoOrigen = a.ptoOrigen;
    ptoDestino = a.ptoDestino;

    anguloWithLineb = a.anguloWithLineb;
}

Arista& Arista::operator=(const Arista &rhs) {

    //si son diferentes
    if (this != &rhs) {
        this->idOrigen = -1;
        this->idDestino = -1;
        this->ptoOrigen = NULL;
        this->ptoDestino = NULL;
        this->anguloWithLineb = -1;
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
        this->anguloWithLineb = -1;
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

void Arista::setIdOrigen(int id){
    idOrigen = id;
}
void Arista::setIdDestino(int id){
    idDestino = id;
}

Punto Arista::getPtoOrigen(){
    return *ptoOrigen;
}

Punto Arista::getPtoDestino(){
    return *ptoDestino;
}

void Arista::setPtoOrigen(Punto o){
    ptoOrigen->setX(o.getX());
    ptoOrigen->setY(o.getY());
    ptoOrigen->setIdVert(o.getIdVert());
}

void Arista::setPtoDestino(Punto d){
    ptoDestino->setX(d.getX());
    ptoDestino->setY(d.getY());
    ptoDestino->setIdVert(d.getIdVert());
}


vector<double> Arista::getVectDir(){
    return vectDir;
}

double Arista::getAnguloLb(){
    return anguloWithLineb;
}

void Arista::setAnguloLb(double angulo){
    anguloWithLineb = angulo;
}


