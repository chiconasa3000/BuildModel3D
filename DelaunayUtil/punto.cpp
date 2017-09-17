#include "punto.h"

Punto::Punto(){
    coor_x = NULL;
    coor_y = NULL;
}

Punto::Punto(double x, double y)
{
    //iniciando ambas coordenadas x,y
    coor_x = x;
    coor_y = y;
}

void Punto::copiar(const Punto &p){
    coor_x = p.coor_x;
    coor_y = p.coor_y;
    idVert = p.idVert;
}

Punto& Punto::operator=(const Punto &rhs) {

    //si son diferentes
    if (this != &rhs) {
        this->coor_x = NULL;
        this->coor_y = NULL;
        this->idVert = 0;
        copiar(rhs);
    }

    return *this;
}

Punto::Punto(const Punto &p){
    coor_x = NULL;
    coor_y = NULL;
    idVert = 0;
    copiar(p);
}

void Punto::insertArista(Arista a){
    //insertando una nueva arista para el punto actual
    group_aristas.push_back(a);
}

//Funciones Utilitarias Punto

double Punto::getX(){
    return coor_x;
}

double Punto::getY(){
    return coor_y;
}

//impresion del punto
void Punto::printPoint(){
    cout<<coor_x<<" "<<coor_y<<endl;
}

//impresion de la lista de aristas de un punto
void Punto::printAristas(){
    for(int i=0;i<group_aristas.size();i++){
        //imprimiendo la arista actual
        group_aristas[i].printArista();
    }
}

void Punto::drawAristas(vtkSmartPointer<vtkMutableUndirectedGraph> g,vector<Punto> group_puntos){
    //vtkIdType idPunto = this->idVert;

    //debemos buscar los puntos y obtener sus respectivos ides vertices


    for(int i=0;i<group_aristas.size();i++){

        //ides puntos de la actual arista
        int idDestino = group_aristas[i].getIdDestino();
        int idOrigen = group_aristas[i].getIdOrigen();

        //buscamos en la tabla de puntos
        vtkIdType a = group_puntos[idDestino].getIdVert();
        vtkIdType b = group_puntos[idOrigen].getIdVert();

        //conseguimos sus ides respectivos

        //seran tantas aristas hubiero en el vector de vertices
        g->AddEdge(b,a);
    }
}

void Punto::createIdPunto(vtkSmartPointer<vtkMutableUndirectedGraph> g){
    idVert = g->AddVertex();
}

vtkIdType Punto::getIdVert(){
    return idVert;
}