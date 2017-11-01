#include "punto.h"

Punto::Punto(){
    coor_x = NULL;
    coor_y = NULL;
}

Punto::Punto(double x, double y)
{
    //initializa both coords x,y
    //iniciando ambas coordenadas x,y
    coor_x = x;
    coor_y = y;
}

void Punto::copiar(const Punto &p){
    coor_x = p.coor_x;
    coor_y = p.coor_y;
    idVert = p.idVert;
    //copy edges group
    //copia del grupo de aristas

    //warning: we only share the same reference or maybe
    //we do a full copy from lista edges
    //adevertencia: solo estamos cambiando la referencia
    //esto seria que ambos apuntan al mismo grupo de aristas
    group_aristas.clear();
    group_aristas = map<int,Arista,less<int>>(p.group_aristas.begin(),p.group_aristas.end());
    //group_aristas.(p.group_aristas.begin(), p.group_aristas.end());
    //group_aristas = new vector<Arista>(p.group_aristas->begin(), p.group_aristas->end() );
}

Punto& Punto::operator=(const Punto &rhs) {

    //in case they are differents
    //si son diferentes
    if (this != &rhs) {
        this->coor_x = NULL;
        this->coor_y = NULL;
        this->idVert = 0;
        copiar(rhs);
    }

    return *this;
}

Punto* Punto::operator=(const Punto *rhs) {

    //in case they are different
    //si son diferentes
    if (this != rhs) {
        this->coor_x = NULL;
        this->coor_y = NULL;
        this->idVert = 0;
        copiar(*rhs);
    }

    return this;
}


Punto::Punto(const Punto &p){
    coor_x = NULL;
    coor_y = NULL;
    idVert = 0;
    copiar(p);
}

void Punto::insertArista(Arista a){
    //insert a new edge to the actual point
    //insertando una nueva arista para el punto actual
    //group_aristas.push_back(a);
    int newId = group_aristas.size();

    group_aristas.insert (pair<int,Arista>(newId,a));
    //group_aristas.push_back(a);
}

//Aditional Function from Point
//Funciones Utilitarias Punto

double Punto::getX(){
    return coor_x;
}

double Punto::getY(){
    return coor_y;
}

void Punto::setX(double coor_x){
    this->coor_x = coor_x;
}

void Punto::setY(double coor_y){
    this->coor_y = coor_y;
}

//print the point
//impresion del punto
void Punto::printPoint(){
    cout<<"Id:"<<idVert<<" -> "<<coor_x<<" "<<coor_y<<endl;

}

//print point edges list
//impresion de la lista de aristas de un punto
void Punto::printAristas(){
    for(int i=0;i<group_aristas.size();i++){
        //imprimiendo la arista actual
        group_aristas[i].printArista();
    }
}

void Punto::drawAristas(vtkSmartPointer<vtkMutableUndirectedGraph> g,vector<Punto> group_puntos){
    //vtkIdType idPunto = this->idVert;

    //we should find the points and get his vertex ides
    //debemos buscar los puntos y obtener sus respectivos ides vertices


    for(int i=0;i<group_aristas.size();i++){
        //ides point from actual edge
        //ides puntos de la actual arista
        int idDestino = group_aristas[i].getIdDestino();
        int idOrigen = group_aristas[i].getIdOrigen();

        //we find in the point tables
        //buscamos en la tabla de puntos
        vtkIdType a = group_puntos[idDestino].getIdVert();
        vtkIdType b = group_puntos[idOrigen].getIdVert();

        //they will be as edges as array vectors are
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

void Punto::setIdVert(vtkIdType idVert){
    this->idVert = idVert;
}

map<int,Arista,less<int>>* Punto::getGroupAristas(){
    return &group_aristas;
}
