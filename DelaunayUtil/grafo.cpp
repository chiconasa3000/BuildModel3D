#include "grafo.h"

Grafo::Grafo(){
    //el vector ya esta iniciado en null
}

Grafo::Grafo(vector<vector<vector<double>>> listSubgrafo)
{
    int contNroVert= 0;
    //recorriendo el grupo general
    for(int i=0; i<listSubgrafo.size();i++){
        //imprimiendo los grupos pequeños

        //creamos un subgrafo temporal
        SubGrafo subgrafTemp;


        int nroPuntos = listSubgrafo[i].size();
        //acumulador de nro de vertices
        contNroVert += nroPuntos;

        for(int j=0; j<nroPuntos; j++){

            //creamos un punto temporal (en el universo :D)
            double x = listSubgrafo[i][j][0];
            double y = listSubgrafo[i][j][1];
            Punto ptoTemp(x,y);

            //guardamos el punto temporal en el subgrafo
            subgrafTemp.insertPunto(ptoTemp);

        }
        //subgrafTemp.printSubgrafo();
        //guardamos el subgrafo temporal al grafo general
        group_subgrafos.push_back(subgrafTemp);
    }

    //iniciamos el nro de vertices total del grafo
    nroVertGrafo = contNroVert;

}

void Grafo::doTriangBase(){
    //debemos formar aristas de los puntos de cada subgrafo
    for(int i=0; i<group_subgrafos.size();i++){
        //obtenemos cada subgrafo
        group_subgrafos[i].doConnectionPoints();
    }
}

void Grafo::drawGrafo(vtkSmartPointer<vtkRenderWindow> renderWindow){
    //crear un grafo
    grafoVtk = vtkSmartPointer<vtkMutableUndirectedGraph>::New();

    //almacenador de los puntos
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    //recorremos cada subgrafo
    for(int i=0; i<group_subgrafos.size();i++){
        group_subgrafos[i].createIdsPuntosSubgrafo(grafoVtk);
    }

    //recorremos cada subgrafo
    for(int i=0; i<group_subgrafos.size();i++){
        group_subgrafos[i].drawSubgrafo(grafoVtk.Get(),points.Get());
    }

    cout<<"Puntos::"<<endl;
    for(int i=0;i<points->GetNumberOfPoints();i++){
        double *p = points->GetPoint(i);
        cout<<p[0]<<" "<<p[1]<<endl;
    }

    //conversion del grafo a un polydata
    grafo2PolyData();

    //renderizar el grafo
    doRender(renderWindow);

}

void Grafo::doRender(vtkSmartPointer<vtkRenderWindow> renderWindow){
    // Create a mapper and actor
      vtkSmartPointer<vtkPolyDataMapper> mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
      mapper->SetInputConnection(grafoPolyData->GetOutputPort());

      vtkSmartPointer<vtkActor> actor =
        vtkSmartPointer<vtkActor>::New();
      actor->SetMapper(mapper);

      // Create a renderer, render window, and interactor
      vtkSmartPointer<vtkRenderer> renderer =
        vtkSmartPointer<vtkRenderer>::New();

      // Add the actor to the scene
      renderer->AddActor(actor);
      renderer->SetBackground(.3, .6, .3); // Background color green

      // VTK/Qt wedded
      renderWindow->AddRenderer(renderer);
}


vtkSmartPointer<vtkGraphToPolyData> Grafo::getGrafoPolydata(){
    return grafoPolyData;
}

void Grafo::grafo2PolyData(){
    grafoPolyData = vtkSmartPointer<vtkGraphToPolyData>::New();
    grafoPolyData->SetInputData(grafoVtk);
    grafoPolyData->Update();
}


/*********** Funciones Adicionales del Grafo *******************/

void Grafo::printDataGrafo(){
    for(int i=0; i<group_subgrafos.size();i++){
        group_subgrafos[i].printSubgrafo();
    }
}

void Grafo::printAristasGrafo(){
    for(int i=0; i<group_subgrafos.size();i++){
        group_subgrafos[i].printAristaSubGra();
    }
}
