#ifndef GRAFO_H
#define GRAFO_H

#include "subgrafo.h"
#include <vtk-7.1/vtkActor.h>
#include <vtk-7.1/vtkRenderWindow.h>
#include <vtk-7.1/vtkRenderer.h>
#include <vtk-7.1/vtkPolyDataMapper.h>


class Grafo
{
public:
    //recibe un vector de puntos
    Grafo();
    Grafo(vector<vector<vector<double>>> listaSubgrafos);
    void doTriangBase();
    vtkSmartPointer<vtkGraphToPolyData> getGrafoPolydata();

    //unir los subgrafos
    void mergeGrafo();
    //establecer la linea base para un par de subgrafos
    Arista* doLineaBase(SubGrafo *s1,SubGrafo *s2);
    void saveCandidatos(Arista *lBase,SubGrafo *s1,SubGrafo *s2);
    //comprobacion de criterios para el final candidato
    Punto testCandidatos(SubGrafo *s1,SubGrafo *s2,vector<Arista> a, vector<Arista> b, Arista *lineabase);
    bool testFinalCandidato(SubGrafo *s, vector<Arista> listArisCand, Arista * lineabase, Punto * &ptoCand);
    //eliminar arista candidata del subgrafo
    void delArisFromSubgrafo(SubGrafo *s,Arista *aristCandTemp);
    void delArisFromPoint(Punto ptoCand, int idOrigenPto);

    void drawGrafo(vtkSmartPointer<vtkRenderWindow> renderWindow);
    void doRender(vtkSmartPointer<vtkRenderWindow> renderWindow);
    void grafo2PolyData();
    void printDataGrafo();
    void printAristasGrafo();
    void printIdPuntos();
private:
    vector<SubGrafo> group_subgrafos;
    vtkSmartPointer<vtkMutableUndirectedGraph> grafoVtk;
    vtkSmartPointer<vtkGraphToPolyData> grafoPolyData;
    int nroVertGrafo=0;
};

#endif // GRAFO_H
