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
    void drawGrafo(vtkSmartPointer<vtkRenderWindow> renderWindow);
    void doRender(vtkSmartPointer<vtkRenderWindow> renderWindow);
    void grafo2PolyData();
    void printDataGrafo();
    void printAristasGrafo();
private:
    vector<SubGrafo> group_subgrafos;
    vtkSmartPointer<vtkMutableUndirectedGraph> grafoVtk;
    vtkSmartPointer<vtkGraphToPolyData> grafoPolyData;
    int nroVertGrafo=0;
};

#endif // GRAFO_H
