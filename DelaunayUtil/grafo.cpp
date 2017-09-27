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

void Grafo::delArisFromPoint(Punto ptoCand, int idOrigenArista){
    //Eliminar arista del punto de la linea base

    vector<Arista> *listArist = ptoCand.getGroupAristas();

    for(int i=0;i<listArist->size();i++){
        int idDestPto = listArist->at(i).getIdDestino();
        if( idDestPto == idOrigenArista){
            //ADEVERTENCIA: cuidado con el indice
            listArist->erase(listArist->begin()+i+1);
            break;
        }
    }
}

void Grafo::delArisFromSubgrafo(SubGrafo *s,Arista *aristCandTemp){
    //Vector de puntos del sungrafo actual
    vector<Punto> ptosSub = s->getGroupPuntos();
    //Eliminar arista del punto candidato

    Punto ptoCand = ptosSub[aristCandTemp->getIdDestino()]; //subgrafo
    Punto ptoCandlb = ptosSub[aristCandTemp->getIdOrigen()]; //lineaBase

    int idOrigen = aristCandTemp->getIdOrigen();

    //Eliminar ambas aristas tanto del origen como del destino
    delArisFromPoint(ptoCand,idOrigen);
    delArisFromPoint(ptoCandlb,idOrigen);
}

//comprobacion de los criterios
bool Grafo::testFinalCandidato(SubGrafo *s,vector<Arista> listCand, Arista * lineabase, Punto * &ptoCand){
    //Se asume que el subgrafo1 es de la izquierda y subgrafo2 es de la derecha

    //manejo de operaciones con aristas y puntos.
    UtilMaths util;

    //Bandera de Testing Final
    bool testCand = false;

    //recorriendo la lista de aristas candidatas
    for(int i=0;i<listCand.size();i++){
        Arista aristCandTemp = listCand[i];

        //1ER: CRITERIO: Angulo menor a 180 grados
        if(aristCandTemp.getAnguloLb() < 180){


            //2DO CRITERIO: puntosCocirculares
            //Obteniendo punto candidato
            vector<Punto> ptosS1 = s->getGroupPuntos();
            Punto currentPtoCand  = ptosS1[aristCandTemp.getIdDestino()];

            //debemos analizar los siguientes candidatos
            bool testCoCirc = true;

            //en caso de ser un solo candidato
            //cumple la cocircularidad ya que los 3 puntos (baseI, baseD,currentPtoCand)
            //siempre formaran un circulo

            if(listCand.size() > 1){
                for(int j=i+1; i<listCand.size();i++){
                    //obteniendo siguiente punto candidato
                    Arista nextArisCand = listCand[j];
                    Punto nextPtoCand = ptosS1[nextArisCand.getIdDestino()];
                    bool testCoCircAct = util.testCoCircular(lineabase,currentPtoCand,nextPtoCand);

                    //Si alguno no es cocircular entonces termina a evaluacion
                    if(testCoCircAct == false){
                        testCoCirc = false;
                        break;
                    }
                }
            }

            //EVALUACION FINAL DE 2DO CRITERIO
            if(testCoCirc == true){
                //En caso de ser finalmente cocircular
                //Se encontro un punto final candidato
                testCand = true;
                //Guardamos el punto final candidato
                ptoCand = new Punto();
                ptoCand->setX(ptosS1[aristCandTemp.getIdDestino()].getX());
                ptoCand->setY(ptosS1[aristCandTemp.getIdDestino()].getY());
            }else{
                //En caso de ser finalmente no cocircular
                //borramos la arista actual
                delArisFromSubgrafo(s,&aristCandTemp);
                //continuamos con el siguiente iteracion+
                continue;
            }



        }else{
            //como el primero es mas de 180 el resto de candidatos se obvia
            testCand = false;
            //inicializamo como un punto vacio
            ptoCand = new Punto();
            break;
        }
    }

    //para todos los casos
    return testCand;
}

Punto Grafo::testCandidatos(SubGrafo *s1,SubGrafo *s2,vector<Arista> listCandS1, vector<Arista>listCandS2, Arista *lineabase){
    //manejo de operaciones con aristas y puntos.
    UtilMaths util;
    //Arista *lineabase_2 = doLineaBase(s1,s2);
    //creacion de almacenadores para los puntos candidatos
    Punto *ptoCandRight;
    Punto *ptoCandLeft;
    //se comprobara por cada subgrafo los criterios de aceptacion
    bool finalCandRight = testFinalCandidato(s1,listCandS1, lineabase, ptoCandRight);
    bool finalCandLeft = testFinalCandidato(s2,listCandS2, lineabase, ptoCandLeft);

    //En caso de que ambos candidatos existan tanto de s1 y s2
    if(finalCandLeft && finalCandRight){
        bool testMixCoCircular = util.testCoCircular(lineabase,*ptoCandLeft,*ptoCandRight);

        //cumple el punto candidato izquierda
        if(testMixCoCircular == true){
            return *ptoCandLeft;
        }else{
            return *ptoCandRight;
        }
    }else if(finalCandLeft == true && finalCandRight == false){
        return *ptoCandLeft;
    }else{
        return *ptoCandRight;
    }

}

Arista* Grafo::doLineaBase(SubGrafo *s1,SubGrafo *s2){
    //encontramos los puntos mas bajos de cada subgrafo
    //los cuales estan al principio de la lista
    Punto ps1 = s1->getMenorPtoy();
    Punto ps2 = s2->getMenorPtoy();

    //debemos hacer que esta arista copie todos los datos a la hora de retornar
    Arista *aristaBase=new Arista(ps1,ps2);

    return aristaBase;
}

//funcion general del grafo para guardar candidatos de 2 subgrafos
void Grafo::saveCandidatos(Arista *lbase, SubGrafo *s1,SubGrafo *s2){
    Arista *lbase2 = lbase;
    //conseguir los menores puntos en coord y
    Punto ps1 = s1->getMenorPtoy();
    Punto ps2 = s2->getMenorPtoy();

    //con el punto menor guardamos los candidatos de aristas de cada subgrafo
    s1->saveCandidates(lbase2,ps1); //candidatos del subgrafo 1
    s2->saveCandidates(lbase2,ps2); //candidatos del subgrafo 2
}

void Grafo::mergeGrafo(){

    //solo se lee el un solo grafo el 1ero siempre quedara fija
    //al realizar el merge

    //for(int i=1; i<group_subgrafos.size(); i++){

        SubGrafo *s1 = &group_subgrafos[0];
        //este subgrafo es el que cambiara constantemente
        SubGrafo *s2 = &group_subgrafos[1];

        //capturamos el menor punto en y de cada subgrafo
        s1->calcMenorPointInYcoord();
        s2->calcMenorPointInYcoord();

        //establecemos la linea base para ambos subgrafos
        Arista *lineaBase = doLineaBase(s1,s2);
        saveCandidatos(lineaBase,s1,s2); //guarda los candidatos para ambos subgrafos

        vector<Arista> listCandS1 = s1->getListArisCand();
        vector<Arista> listCandS2 = s2->getListArisCand();


        //Obteniendo el punto final candidato de ambos subgrafos
        Punto ptoFinalCand = testCandidatos(s1,s2,listCandS1,listCandS2,lineaBase);
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

void Grafo::printIdPuntos(){
    for(int i=0; i<group_subgrafos.size();i++){
        group_subgrafos[i].printIdes();
    }
}
