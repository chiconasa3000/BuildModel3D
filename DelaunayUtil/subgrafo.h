#ifndef SUBGRAFO_H
#define SUBGRAFO_H

#include <list>
#include <arista.h>
#include <vector>
#include "punto.h"

//librerias de vtk
//para el menejo de punteros sobre el polydata
#include <vtk-7.1/vtkSmartPointer.h>
//para el manejo de puntos
#include <vtk-7.1/vtkPoints.h>
//para el grafo no dirigido
#include <vtk-7.1/vtkMutableUndirectedGraph.h>
//la conversion de grafo a polydata
#include <vtk-7.1/vtkGraphToPolyData.h>
//para el mapeo del polydata a renderizacion
#include <vtk-7.1/vtkPolyDataMapper.h>
//el actor pasado al maper
#include <vtk-7.1/vtkActor.h>
//para la renderizacion
#include <vtk-7.1/vtkRenderWindow.h>
#include <vtk-7.1/vtkRenderer.h>
#include <vtk-7.1/vtkRenderWindowInteractor.h>


class SubGrafo
{
public:
    SubGrafo();
    SubGrafo(vector<vector<double>> listapuntos);
    void insertPunto(Punto &p);
    void doConnectionPoints();
    void printSubgrafo();
    void printAristaSubGra();
    void drawSubgrafo(vtkMutableUndirectedGraph *g,vtkPoints *points);
    void createIdsPuntosSubgrafo(vtkSmartPointer<vtkMutableUndirectedGraph> g);
    void printIdes();
//    bool doFirstCond();
//    void anguloEnAristas(int idAristaA, int idAristaB);
//    bool doSecondCond();
//    bool pointInCircle();
//    void completarDelaunay();
//    //retorna id de aristas(
//    std::list<int> buscarAristasConPuntoOrigen(int idPunto);
//    void agregarArista();
//    void eliminarArista();
//    //retorna id de punto
//    int getCommonPointEnAristas(int idAristaA, int idAristaB);
private:
      vector<Punto> group_puntos;
//    Arista lineaBase;
};

#endif // SUBGRAFO_H
