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


Arista Grafo::doLineaBase(SubGrafo *s1,SubGrafo *s2){
    //encontramos los puntos mas bajos de cada subgrafo
    //los cuales estan al principio de la lista
    Punto ps1 = s1->getMenorPtoy();
    Punto ps2 = s2->getMenorPtoy();

    //debemos hacer que esta arista copie todos los datos a la hora de retornar
    Arista aristaBase(ps1,ps2);

    return aristaBase;
}

//funcion general del grafo para guardar candidatos de 2 subgrafos
void Grafo::saveCandidatos(Arista *lbase, SubGrafo *s1,SubGrafo *s2){

    //conseguir los menores puntos en coord y
    Punto ps1 = s1->getMenorPtoy();
    Punto ps2 = s2->getMenorPtoy();

    //con el punto menor guardamos los candidatos de aristas de cada subgrafo
    s1->saveCandidates(*lbase,ps1); //candidatos del subgrafo 1
    s2->saveCandidates(*lbase,ps2); //candidatos del subgrafo 2
}



/*void Grafo::checkSubgrafo(vector<Arista> a, vector<Arista> b, Arista *lineabase){

}*/

void Grafo::mergeGrafo(){

    //recorremos solo 2 subgrafos para poder hacer las pruebas
    //for(int i=0; i<2; i++){

        SubGrafo *s1 = &group_subgrafos[0];
        SubGrafo *s2 = &group_subgrafos[1];

        //capturamos el menor punto en y de cada subgrafo
        s1->calcMenorPointInYcoord();
        s2->calcMenorPointInYcoord();

        //establecemos la linea base para ambos subgrafos
        Arista lineaBase = doLineaBase(s1,s2);
        saveCandidatos(&lineaBase,s1,s2); //guarda los candidatos para ambos subgrafos

        vector<Arista> listCandS1 = s1->getListArisCand();
        vector<Arista> listCandS2 = s2->getListArisCand();

        //checkSubgrafo(listCandS1,listCandS2,lineaBase);
    //}
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
