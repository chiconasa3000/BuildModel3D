#ifndef PUNTO_H
#define PUNTO_H

#include "arista.h"
#include <list>
#include <vector>
#include <vtk-7.1/vtkSmartPointer.h>
#include <vtk-7.1/vtkMutableUndirectedGraph.h>
#include <map>
using namespace std;

class Arista;

class Punto
{
public:
    Punto();
    Punto(double x, double y);
    Punto(const Punto &punto);
    void copiar(const Punto &p);
    Punto& operator=(const Punto &rhs);
    Punto* operator=(const Punto *rhs);
    void insertArista(Arista a);
    double getX();
    double getY();
    void setX(double coor_x);
    void setY(double coor_y);
    map<int,Arista,less<int>>* getGroupAristas();
    void printPoint();
    void printAristas();
    vtkIdType getIdVert();
    void setIdVert(vtkIdType idVert);
    void drawAristas(vtkSmartPointer<vtkMutableUndirectedGraph> g,vector<Punto> group_puntos);
    void createIdPunto(vtkSmartPointer<vtkMutableUndirectedGraph> g);
private:
    //en caso de preguntar por el ide del punto
    //asumo que ya el vector le otorgara este
    //int idePunto;

    //informacion de coords del punto
    double coor_x;
    double coor_y;

    //un ide para el punto
    //este no sera iniciado aun
    vtkIdType idVert;

    //grupo de aristas del punto
    //vector<Arista> group_aristas;
    map<int, Arista, less<int>> group_aristas;
    int iterActGA = 0;
    //vector<Arista> group_aristas;
};

#endif // PUNTO_H
