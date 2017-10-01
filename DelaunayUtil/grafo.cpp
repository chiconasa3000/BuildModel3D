#include "grafo.h"

Grafo::Grafo(){
    //el vector ya esta iniciado en null
}

Grafo::Grafo(vector<vector<vector<double>>> listSubgrafo)
{
    //crear un grafo (adevertencia: adelantandose al render del grafo)
    grafoVtk = vtkSmartPointer<vtkMutableUndirectedGraph>::New();

    int contNroVertGraf= 0;
    //recorriendo el grupo general
    for(int i=0; i<listSubgrafo.size();i++){

        //creamos un subgrafo temporal
        SubGrafo subgrafTemp;


        int nroPuntos = listSubgrafo[i].size();
        subgrafTemp.setNroPtos(nroPuntos);
        //acumulador de nro de vertices
        contNroVertGraf += nroPuntos;

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

    //Creacion de indices usando la clase render del grafo
    //despues de haber hecho la construccion inicial
    for(int i=0; i<group_subgrafos.size();i++){
        group_subgrafos[i].createIdsPuntosSubgrafo(grafoVtk);
    }

    //iniciamos el nro de vertices total del grafo
    nroVertGrafo = contNroVertGraf;

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
                //ptoCand->setX(ptosS1[aristCandTemp.getIdDestino()].getX());
                //ptoCand->setY(ptosS1[aristCandTemp.getIdDestino()].getY());
                //ptoCand->setIdVert(ptosS1[aristCandTemp.getIdDestino()].getIdVert());
                ptoCand->copiar(ptosS1[aristCandTemp.getIdDestino()]);
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

    //Ojo: si no entra al primer for: testCand sera false

    //para todos los casos
    return testCand;
}

Punto Grafo::testCandidatos(SubGrafo *s1,SubGrafo *s2,vector<Arista> listCandS1, vector<Arista>listCandS2, Arista *lineabase, bool &esSubIzq){
    //manejo de operaciones con aristas y puntos.
    UtilMaths util;
    //Arista *lineabase_2 = doLineaBase(s1,s2);
    //creacion de almacenadores para los puntos candidatos
    Punto *ptoCandRight;
    Punto *ptoCandLeft;
    //se comprobara por cada subgrafo los criterios de aceptacion
    bool finalCandLeft = testFinalCandidato(s1,listCandS1, lineabase, ptoCandLeft);
    bool finalCandRight = testFinalCandidato(s2,listCandS2, lineabase, ptoCandRight);

    //En caso de que ambos candidatos existan tanto de s1 y s2
    if(finalCandLeft && finalCandRight){
        bool testMixCoCircular = util.testCoCircular(lineabase,*ptoCandLeft,*ptoCandRight);

        //cumple el punto candidato izquierda
        if(testMixCoCircular == true){
            esSubIzq = true;
            return *ptoCandLeft;
        }else{
            return *ptoCandRight;
        }
    }else if(finalCandLeft == true && finalCandRight == false){
        esSubIzq  = true;
        return *ptoCandLeft;
    }else if(finalCandLeft == false && finalCandRight == true){
        return *ptoCandRight;
    }else if(finalCandLeft == false && finalCandRight == false){
        //por defecto se mandara un punto vacio
        //y la bandera estara en false al no haber ningun candidato
        Punto *ptoCand = new Punto();
        return *ptoCand;
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
    //el punto origen sera para el subgrafo s1 (izquierdo)
    //el punto destino sera para el subgrafo s2 (derecho)
    s1->saveCandidates(lbase,s1->getGroupPuntos()[lbase->getIdOrigen()],true); //candidatos del subgrafo 1
    s2->saveCandidates(lbase,s2->getGroupPuntos()[lbase->getIdDestino()],false); //candidatos del subgrafo 2
}

void Grafo::saveIdPairCand(Punto ptoCand,Arista *lineBase, vector<vector<int>> *idsCandL,vector<vector<int>> *idsCandR,bool esSubIzq){
    //primero guardamos la linea base actual
    vector<int> idPair;
    idPair.push_back(lineBase->getIdOrigen());
    idPair.push_back(lineBase->getIdDestino());
    //push los ides de la linea base
    idsCandL->push_back(idPair);


    vector<int> idPairPto;
    //el orden de insercion depende de que subgrafo pertenece el pto Candidato
    if(esSubIzq == true){
        //ahora guardamos los id del pto candidato
        idPairPto.push_back(ptoCand.getIdVert());       //por ser del subgrafo izquierdo
        idPairPto.push_back(lineBase->getIdDestino());

        //actualizando linea base
        lineBase->setPtoOrigen(ptoCand);

    }else{
        //ahora guardamos los id del pto candidato
        idPairPto.push_back(lineBase->getIdOrigen());
        idPairPto.push_back(ptoCand.getIdVert());       //por ser del subgrafo derecho

        //actualizando linea base
        lineBase->setPtoDestino(ptoCand);
    }
    idsCandL->push_back(idPairPto);
}

void Grafo::mergeGrafo(){

    //solo se lee el un solo grafo el 1ero siempre quedara fija
    //al realizar el merge
    vector<vector<int>> *idesPairCandL = new vector<vector<int>>();
    vector<vector<int>> *idesPairCandR = new vector<vector<int>>();
    SubGrafo *s1 = &group_subgrafos[0];
    //este subgrafo es el que cambiara constantemente
    SubGrafo *s2 = &group_subgrafos[1];
    //capturamos el menor punto en y de cada subgrafo
    s1->calcMenorPointInYcoord();
    s2->calcMenorPointInYcoord();
    //establecemos la linea base para ambos subgrafos
    Arista *lineaBase = doLineaBase(s1,s2);

    bool esSubIzq = false;

    //insertamos los nuevos puntos sobre cada subgrafo
    //estos seran leidos o consultados durante la prueba de candidatos
    //pero no deberan ser recorridos es decir no iterar el subgrafo
    //del inicio al final

    s1->insertPointsTemp(s2,true);
    s2->insertPointsTemp(s1,false);

    //for(int i=1; i<group_subgrafos.size(); i++){
    int cont = 1;
    while(true){
        //guarda los candidatos para ambos subgrafos
        saveCandidatos(lineaBase,s1,s2);

        //obtenemos los candidatos
        vector<Arista> listCandS1 = s1->getListArisCand();
        vector<Arista> listCandS2 = s2->getListArisCand();

        //Obteniendo el punto final candidato de ambos subgrafos
        Punto ptoFinalCand = testCandidatos(s1,s2,listCandS1,listCandS2,lineaBase,esSubIzq);

        //guardamos los ides de las nuevas aristas y actualizamos la linea base
        //saveIdPairCand(ptoFinalCand, lineaBase, idesPairCandL,idesPairCandR,esSubIzq);

        int difPuntosS1 = s1->getNroPtos();
        int difPuntosS2 = s2->getNroPtos();

        //insertar la linea base
        s1->insertArista(lineaBase->getIdOrigen(),lineaBase->getIdDestino());
        s2->insertArista(lineaBase->getIdDestino(),lineaBase->getIdOrigen());

        //insertamos la arista que contiene el punto candidato y un punto de la linea base
        if(esSubIzq){
            s1->insertArista(ptoFinalCand.getIdVert(),lineaBase->getIdDestino());
            s2->insertArista(lineaBase->getIdDestino(),ptoFinalCand.getIdVert());
            //actualizamos la linea base
            lineaBase->setPtoOrigen(ptoFinalCand);
            lineaBase->setIdOrigen(ptoFinalCand.getIdVert());
        }else{
            s2->insertArista(ptoFinalCand.getIdVert(),lineaBase->getIdOrigen());
            s1->insertArista(lineaBase->getIdOrigen(),ptoFinalCand.getIdVert());
            //actualizamos la linea base
            lineaBase->setPtoDestino(ptoFinalCand);
            lineaBase->setIdDestino(ptoFinalCand.getIdVert());
        }
        cont++;


    }
}




void Grafo::doTriangBase(){
    //debemos formar aristas de los puntos de cada subgrafo
    for(int i=0; i<group_subgrafos.size();i++){
        //obtenemos cada subgrafo
        group_subgrafos[i].doConnectionPoints();
    }
}

void Grafo::drawGrafo(vtkSmartPointer<vtkRenderWindow> renderWindow){


    //almacenador de los puntos
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    //recorremos cada subgrafo
    /*for(int i=0; i<group_subgrafos.size();i++){
        group_subgrafos[i].createIdsPuntosSubgrafo(grafoVtk);
    }*/

    //recorremos cada subgrafo
    for(int i=0; i<group_subgrafos.size();i++){
        group_subgrafos[i].drawSubgrafo(grafoVtk.Get(),points.Get());
    }

    /*cout<<"Puntos::"<<endl;
    for(int i=0;i<points->GetNumberOfPoints();i++){
        double *p = points->GetPoint(i);
        //cout<<p[0]<<" "<<p[1]<<endl;
    }*/

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
