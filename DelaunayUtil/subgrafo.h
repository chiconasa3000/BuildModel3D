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
#include "utilmaths.h"

class SubGrafo
{
public:
    SubGrafo();
    SubGrafo(vector<vector<double>> listapuntos);
    void insertPunto(Punto &p);
    void doConnectionPoints();

    //conseguir los candidadtos del actual subgrafo
    void saveCandidates(Arista * lBase, Punto ptoInicio,bool esIzq);
    //ordenar aristas candidatas
    void orderAristCand(vector<double> angulosAristas);
    //eliminar la linea base como candidato
    void eraseLineaBaseFromCandidatos(Arista *lbase,bool esIzq);


    //conseguir el menor punto de la lista de puntos del subgrafo
    void calcMenorPointInYcoord();
    Punto getMenorPtoy();

    //insertar puntos temporalmente ya que posteriormente seran reemplazados
    void insertPointsTemp(SubGrafo *s,bool esSubIzq);
    void insertArista(int idOrigen, int idDestino);

    void printSubgrafo();
    void printAristaSubGra();
    void drawSubgrafo(vtkMutableUndirectedGraph *g,vtkPoints *points);
    void createIdsPuntosSubgrafo(vtkSmartPointer<vtkMutableUndirectedGraph> g);
    void printIdes();
    vector<Punto> getGroupPuntos();
    vector<Punto>* getRefGroupPuntos();
    vector<Arista> getListArisCand();
    int getNroPtos();
    void setNroPtos(int nroPtos);
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

    //grupo de puntos del subgrafo
    vector<Punto> group_puntos;

    //lista de aristas candidatas del subgrafo
    vector<Arista> listArisCand;

    //Menor punto en coordenada y
    Punto minPtoy;

    //Final Candidato
    Punto finalCandidato;

    //Nro de puntos por subgrafo
    int nroPtos;
};

#endif // SUBGRAFO_H
