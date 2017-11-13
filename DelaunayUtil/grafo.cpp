#include "grafo.h"

Grafo::Grafo(){
    //the vector has already initialize in null value
    //el vector ya esta iniciado en null
}

Grafo::Grafo(vector<vector<vector<double>>> listSubgrafo)
{
    //create a graph (attention: forward to graph render)
    //crear un grafo (adevertencia: adelantandose al render del grafo)
    grafoVtk = vtkSmartPointer<vtkMutableUndirectedGraph>::New();

    int contNroVertGraf= 0;
    //atraversing general group
    //recorriendo el grupo general
    for(int i=0; i<listSubgrafo.size();i++){

        //create a temporal graph
        //creamos un subgrafo temporal
        SubGrafo subgrafTemp;


        int nroPuntos = listSubgrafo[i].size();
        subgrafTemp.setNroPtos(nroPuntos);
        //acumulator of number of vertex
        //acumulador de nro de vertices
        contNroVertGraf += nroPuntos;

        for(int j=0; j<nroPuntos; j++){

            //create a temporal point  (in the universe :D)
            //creamos un punto temporal (en el universo :D)
            double x = listSubgrafo[i][j][0];
            double y = listSubgrafo[i][j][1];
            Punto ptoTemp(x,y);

            //save a temporal point in the subgraph
            //guardamos el punto temporal en el subgrafo
            subgrafTemp.insertPunto(ptoTemp);

        }
        //subgrafTemp.printSubgrafo();
        //save temporal subgraph into general graph
        //guardamos el subgrafo temporal al grafo general
        group_subgrafos.push_back(subgrafTemp);
    }

    //Create index using graph render class after doing initial build
    //Creacion de indices usando la clase render del grafo
    //despues de haber hecho la construccion inicial
    for(int i=0; i<group_subgrafos.size();i++){
        group_subgrafos[i].createIdsPuntosSubgrafo(grafoVtk);
    }

    //initializa total number o vertex from graph
    //iniciamos el nro de vertices total del grafo
    nroVertGrafo = contNroVertGraf;

}

void Grafo::delArisFromPoint(Punto ptoCand, int idOrigenArista){
    //erase edge from point from base line
    //Eliminar arista del punto de la linea base

    map<int,Arista,less<int>> *listArist = ptoCand.getGroupAristas();

    for(int i=0;i<listArist->size();i++){
        int idDestPto = listArist->at(i).getIdDestino();
        if( idDestPto == idOrigenArista){
            //CORRECCION el indice comienza desde cero
            //La KEY tambien comenzara desde cero
            listArist->erase(i);
            break;
        }
    }
}

void Grafo::delArisFromSubgrafo(SubGrafo *s,Arista *aristCandTemp){
    //point vector from general subgraph
    //Vector de puntos del sungrafo actual
    vector<Punto> ptosSub = s->getGroupPuntos();
    //erase edge from candidate point
    //Eliminar arista del punto candidato

    Punto ptoCand = ptosSub[aristCandTemp->getIdDestino()]; //subgrafo
    Punto ptoCandlb = ptosSub[aristCandTemp->getIdOrigen()]; //lineaBase

    //CORRECCION el borrado debe ser tanto del origen
    //como del destino
    int idOrigen = aristCandTemp->getIdOrigen();
    int idDestino = aristCandTemp->getIdDestino();

    //erase both edges from origen and destiny
    //Eliminar ambas aristas tanto del origen como del destino
    delArisFromPoint(ptoCand,idOrigen);
    delArisFromPoint(ptoCandlb,idDestino);
}

//test conditionals
//comprobacion de los criterios
bool Grafo::testFinalCandidato(SubGrafo *s,vector<Arista> listCand, Arista * lineabase, Punto * &ptoCand){
    //subgraph1 is left subgraph  subgraph2 is right subgraph
    //Se asume que el subgrafo1 es de la izquierda y subgrafo2 es de la derecha

    //manage operations with edges and points
    //manejo de operaciones con aristas y puntos.
    UtilMaths util;

    //flag of final test
    //Bandera de Testing Final
    bool testCand = false;

    //traversing list from candidate edges
    //recorriendo la lista de aristas candidatas
    for(int i=0;i<listCand.size();i++){
        Arista aristCandTemp = listCand[i];

        //FIRST CONDITIONAL: less angle of 180 grades
        //1ER: CRITERIO: Angulo menor a 180 grados
        double angAristaCand = aristCandTemp.getAnguloLb();
        if(angAristaCand < 180 && angAristaCand > -180){


            //Second CONDITIONAL: points in circumference
            //2DO CRITERIO: puntosCocirculares
            //get the candidate point
            //Obteniendo punto candidato
            vector<Punto> ptosS1 = s->getGroupPuntos();
            Punto currentPtoCand  = ptosS1[aristCandTemp.getIdDestino()];

            //we should analize the next candidates
            //debemos analizar los siguientes candidatos
            bool testCoCirc = true;

            //in case of be only one candidate
            //en caso de ser un solo candidato
            //it pass circumference test, because the 3 points form a circle
            //cumple la cocircularidad ya que los 3 puntos (baseI, baseD,currentPtoCand)
            //siempre formaran un circulo

            if(listCand.size() > 1){
                for(int j=i+1; j<listCand.size();i++,j++){
                    //get the next candidate point
                    //obteniendo siguiente punto candidato
                    Arista nextArisCand = listCand[j];
                    Punto nextPtoCand = ptosS1[nextArisCand.getIdDestino()];
                    bool testCoCircAct = util.testCoCircularIn(lineabase,currentPtoCand,nextPtoCand);

                    //in case they are not cocircle so testing finish
                    //Si alguno no es cocircular entonces termina a evaluacion
                    if(testCoCircAct == false){
                        testCoCirc = false;
                        break;
                    }
                }
            }

            //FINAL TEST OF SECOND CONDITIONAL
            //EVALUACION FINAL DE 2DO CRITERIO
            if(testCoCirc == true){
                //In case of being cocircle
                //En caso de ser finalmente cocircular
                //it means that we find the final point candidate
                //Se encontro un punto final candidato
                testCand = true;
                //save the final point
                //Guardamos el punto final candidato
                ptoCand = new Punto();
                //ptoCand->setX(ptosS1[aristCandTemp.getIdDestino()].getX());
                //ptoCand->setY(ptosS1[aristCandTemp.getIdDestino()].getY());
                //ptoCand->setIdVert(ptosS1[aristCandTemp.getIdDestino()].getIdVert());
                ptoCand->copiar(ptosS1[aristCandTemp.getIdDestino()]);
            }else{
                //In case of not being cocircle
                //En caso de ser finalmente no cocircular
                //erase the actual edge
                //borramos la arista actual
                delArisFromSubgrafo(s,&aristCandTemp);
                //next with the next iteration
                //continuamos con el siguiente iteracion+
                continue;
            }



        }else{
            //how the first is more than 180 from the rest of candidates,
            //we cannot test the next candidates
            //como el primero es mas de 180 el resto de candidatos se obvia
            testCand = false;
            //intialize like empty point
            //inicializamo como un punto vacio
            ptoCand = new Punto();
            break;
        }
    }

    //remember: if it doesn't enter into for statement, testCand will be false
    //Ojo: si no entra al primer for: testCand sera false

    //For all cases
    //para todos los casos
    return testCand;
}

Punto Grafo::testCandidatos(SubGrafo *s1,SubGrafo *s2,vector<Arista> listCandS1, vector<Arista>listCandS2, Arista *lineabase, bool &esSubIzq,bool &endMerge){
    //manage operations with edges and points
    //manejo de operaciones con aristas y puntos.
    UtilMaths util;
    //Arista *lineabase_2 = doLineaBase(s1,s2);
    //creation of buckets for candidates points
    //creacion de almacenadores para los puntos candidatos
    Punto *ptoCandRight;
    Punto *ptoCandLeft;
    //it test for every subgraph the passing conditionals
    //se comprobara por cada subgrafo los criterios de aceptacion
    bool finalCandLeft = testFinalCandidato(s1,listCandS1, lineabase, ptoCandLeft);
    bool finalCandRight = testFinalCandidato(s2,listCandS2, lineabase, ptoCandRight);

    //lista de todos los ptos del subgrafo
    vector<Punto> lptosS1 = s1->getGroupPuntos();
    vector<Punto> lptosS2 = s2->getGroupPuntos();

    //in case of both candidates exist (s1, s2)
    //En caso de que ambos candidatos existan tanto de s1 y s2
    if(finalCandLeft && finalCandRight){
        bool testMixCoCircularR = false;
        bool testMixCoCircularL = false;
        //en caso de que el candidato next sea el punto candidato left
        //entonces no solo el si no todos los candidatos del subgrafo left
        for(int i=0;i<listCandS1.size();i++){
            Arista aristaCandAct = listCandS1[i];
            //ahora capturamos el punto
            Punto ptoAct = lptosS1[aristaCandAct.getIdDestino()];
            //vemos si no coincide con el punto candidato left
            if(!(ptoCandLeft->getIdVert() == ptoAct.getIdVert())){
                //realizamos la prueba cocircular
                testMixCoCircularR = util.testCoCircularOut(lineabase,*ptoCandRight,ptoAct);
            }else{
                testMixCoCircularR = util.testCoCircularOut(lineabase,*ptoCandRight,*ptoCandLeft);
            }

            //en caso de que algun punto no pase la prueba de cocircular
            if(testMixCoCircularR == false){
                break;
            }else{
                continue;
            }
        }

        //hacemos la prueba de cocircular con el opuesto punto candidato

        if(testMixCoCircularR == false){
            //en caso de no cumplir realizamos con el punto candidato right como el nextcandidato
            //es decir mantenemos como pivote ahora el punto candidato left
            for(int j=0;j<listCandS2.size();j++){
                Arista aristaCandAct = listCandS2[j];
                //ahora capturamos el punto
                Punto ptoAct = lptosS2[aristaCandAct.getIdDestino()];
                //vemos si no coincide con el punto candidato left
                if(!(ptoCandRight->getIdVert() == ptoAct.getIdVert())){
                    //realizamos la prueba cocircular
                    testMixCoCircularL = util.testCoCircularOut(lineabase,*ptoCandLeft,ptoAct);
                }else{
                    testMixCoCircularL = util.testCoCircularOut(lineabase,*ptoCandLeft,*ptoCandRight);
                }

                //en caso de que algun punto no pase la prueba de cocircular
                if(testMixCoCircularL == false){
                    break;
                }else{
                    continue;
                }
            }
        }
        //candidate right point pass the test
        //cumple el punto candidato derecha
        if(testMixCoCircularR == true && testMixCoCircularL == false){
            esSubIzq = false;
            return *ptoCandRight;
        }else if(testMixCoCircularR == false && testMixCoCircularL == true){
            esSubIzq = true;
            return *ptoCandLeft;
        }else if(testMixCoCircularR == false && testMixCoCircularL == false){
            //ninguno cumple como candidato final
            //por lo tanto termina el merge ha finalido
            endMerge = true;
        }


    }else if(finalCandLeft == true && finalCandRight == false){
        esSubIzq  = true;
        return *ptoCandLeft;
    }else if(finalCandLeft == false && finalCandRight == true){
        esSubIzq = false;
        return *ptoCandRight;
    }else if(finalCandLeft == false && finalCandRight == false){
        //by default it create an empty point
        //por defecto se mandara un punto vacio
        //flag will be in false because there aren't any candidate
        //y la bandera estara en false al no haber ningun candidato
        endMerge = false;
        Punto *ptoCand = new Punto();
        return *ptoCand;
    }

}

Arista* Grafo::doLineaBase(SubGrafo *s1,SubGrafo *s2){
    //we find the low points from every subgraph
    //encontramos los puntos mas bajos de cada subgrafo
    //it will be in the begining from the list
    //los cuales estan al principio de la lista
    Punto ps1 = s1->getMenorPtoy();
    Punto ps2 = s2->getMenorPtoy();

    //we should the full copy from the actual edge
    //debemos hacer que esta arista copie todos los datos a la hora de retornar
    Arista *aristaBase=new Arista(ps1,ps2);

    return aristaBase;
}

//general function in order to save candidates from every subgraph
//funcion general del grafo para guardar candidatos de 2 subgrafos
void Grafo::saveCandidatos(Arista *lbase, SubGrafo *s1,SubGrafo *s2){
    //origin point will be for s1 subgraph (LEFT)
    //el punto origen sera para el subgrafo s1 (izquierdo)
    //destiny point will be for s2 subgraph s2 (RIGHT)
    //el punto destino sera para el subgrafo s2 (derecho)
    s1->saveCandidates(lbase,s1->getGroupPuntos()[lbase->getIdOrigen()],true); //candidatos del subgrafo 1
    s2->saveCandidates(lbase,s2->getGroupPuntos()[lbase->getIdDestino()],false); //candidatos del subgrafo 2
}

void Grafo::saveIdPairCand(Punto ptoCand,Arista *lineBase, vector<vector<int>> *idsCandL,vector<vector<int>> *idsCandR,bool esSubIzq){
    //first save the actual line base
    //primero guardamos la linea base actual
    vector<int> idPair;
    idPair.push_back(lineBase->getIdOrigen());
    idPair.push_back(lineBase->getIdDestino());
    //push ides from the base line
    //push los ides de la linea base
    idsCandL->push_back(idPair);


    vector<int> idPairPto;
    //insertion order depends from every subgraph belongs  to candidate point
    //el orden de insercion depende de que subgrafo pertenece el pto Candidato
    if(esSubIzq == true){
        //now saving ids from candidate point
        //ahora guardamos los id del pto candidato
        idPairPto.push_back(ptoCand.getIdVert());       //por ser del subgrafo izquierdo
        idPairPto.push_back(lineBase->getIdDestino());

        //updaitng line base
        //actualizando linea base
        lineBase->setPtoOrigen(ptoCand);

    }else{
        //now save ids from candidate point
        //ahora guardamos los id del pto candidato
        idPairPto.push_back(lineBase->getIdOrigen());
        idPairPto.push_back(ptoCand.getIdVert());       //por ser del subgrafo derecho

        //updating base line
        //actualizando linea base
        lineBase->setPtoDestino(ptoCand);
    }
    idsCandL->push_back(idPairPto);
}

void Grafo::mergeGrafo(){

    //for the moment it read only the first subgraph
    //solo se lee el un solo grafo el 1ero siempre quedara fija
    //al realizar el merge

    vector<vector<int>> aristasNuevas;

    int sizeGraphs =  group_subgrafos.size();
    vector<SubGrafo> mergeGraphs;
    while(sizeGraphs > 1){

        //las banderas tambien se ponen otra vez a su estado inicial
        bool esSubIzq = false;
        bool endMerge = false;

        while(!endMerge){
            int itEvalNext = 1;
            SubGrafo *s1 = &group_subgrafos[itEvalNext - 1];
            //este subgrafo es el que cambiara constantemente
            SubGrafo *s2 = &group_subgrafos[itEvalNext];

            //ides de aristas nuevas del lado opuesto al subgrafo 1
            int idOrigenNewArista = -1;
            int idDestinoNewArista = -1;

            //al ser una nueva evaluacion se debe establecer la linea base
            //get the less point in every subgraph
            //capturamos el menor punto en y de cada subgrafo
            s1->calcMenorPointInYcoord();
            s2->calcMenorPointInYcoord();

            //establecemos la linea base para ambos subgrafos
            Arista *lineaBase = doLineaBase(s1,s2);


            //insert the new points in every subgraph
            //insertamos los nuevos puntos sobre cada subgrafo
            //they will be read or searching during the candidate test
            //estos seran leidos o consultados durante la prueba de candidatos
            //but they must not being read by in traversing subgraph
            //pero no deberan ser recorridos es decir no iterar el subgrafo
            //del inicio al final

            //en cada nueva evaluacion
            s1->insertPointsTemp(s2,true);
            s2->insertPointsTemp(s1,false);

            //for(int i=1; i<group_subgrafos.size(); i++){

            //at first it inserts base line like new edge
            //inicialmente se inserta la linea base como nueva arista
            //in every iteration a new baseline will be finding
            //en las iteracion la linea base sera encontrada insertada y actualzada

            //insert a the first bsae line
            //insertar la linea base //esto solo se hace una vez
            s1->insertArista(lineaBase->getIdOrigen(),lineaBase->getIdDestino());
            s2->insertArista(lineaBase->getIdDestino(),lineaBase->getIdOrigen());

            int cont = 1;
            while(!endMerge){
                if(cont == 4){
                    cout<<"hola"<<endl;
                }
                //save candidate for both subgraphs
                //guarda los candidatos para ambos subgrafos
                saveCandidatos(lineaBase,s1,s2);

                //get candidates
                //obtenemos los candidatos
                vector<Arista> listCandS1 = s1->getListArisCand();
                vector<Arista> listCandS2 = s2->getListArisCand();

                //get the final candidate for both subgraph
                //Obteniendo el punto final candidato de ambos subgrafos
                Punto ptoFinalCand = testCandidatos(s1,s2,listCandS1,listCandS2,lineaBase,esSubIzq,endMerge);

                //guardamos los ides de las nuevas aristas y actualizamos la linea base
                //saveIdPairCand(ptoFinalCand, lineaBase, idesPairCandL,idesPairCandR,esSubIzq);

                int difPuntosS1 = s1->getNroPtos();
                int difPuntosS2 = s2->getNroPtos();

                //evaluamos siempre la bandera de finalizacion
                if(!endMerge){
                    //insert a new edge which contains candidate point and point from line base
                    //insertamos la arista que contiene el punto candidato y un punto de la linea base
                    if(esSubIzq){
                        s1->insertArista(ptoFinalCand.getIdVert(),lineaBase->getIdDestino());
                        //s1->insertArista(lineaBase->getIdDestino(),ptoFinalCand.getIdVert());
                        idOrigenNewArista = lineaBase->getIdDestino();
                        idDestinoNewArista = ptoFinalCand.getIdVert();

                        s2->insertArista(lineaBase->getIdDestino(),ptoFinalCand.getIdVert());
                        //updating the base line
                        //actualizamos la linea base
                        lineaBase->setPtoOrigen(ptoFinalCand);
                        lineaBase->setIdOrigen(ptoFinalCand.getIdVert());
                    }else{
                        s2->insertArista(ptoFinalCand.getIdVert(),lineaBase->getIdOrigen());
                        //s2->insertArista(lineaBase->getIdOrigen(),ptoFinalCand.getIdVert());
                        idOrigenNewArista = ptoFinalCand.getIdVert();
                        idDestinoNewArista = lineaBase->getIdOrigen();
                        s1->insertArista(lineaBase->getIdOrigen(),ptoFinalCand.getIdVert());
                        //updating the base line
                        //actualizamos la linea base
                        lineaBase->setPtoDestino(ptoFinalCand);
                        lineaBase->setIdDestino(ptoFinalCand.getIdVert());
                    }
                    //acumular los ides en la lista de nuevas aristas
                    vector<int> pairAristas;
                    pairAristas.push_back(idOrigenNewArista);
                    pairAristas.push_back(idDestinoNewArista);
                    aristasNuevas.push_back(pairAristas);
                    cont++;
                }else{
                    for(int ina = 0; ina < aristasNuevas.size(); ina++){
                        s1->insertArista(aristasNuevas[ina][0],aristasNuevas[ina][1]);
                    }
                    //almacenar el subgrafo unido
                    s1->setNroPtos(group_subgrafos[itEvalNext - 1].getNroPtosReal());
                    mergeGraphs.push_back(group_subgrafos[itEvalNext - 1]);
                    //eliminar el subgrafo 2 y pasar de la lista los 2 subgrafos siguientes
                    //recorriendo todos los subgrafos restantes a la izquierda
                    //group_subgrafos.erase(group_subgrafos.begin() + itEvalNext);
                    group_subgrafos.erase(group_subgrafos.begin(), group_subgrafos.begin() + itEvalNext+1);
                    genNuevoIdsGraphGen();//para actualizar los ides de los subgrafos de group_subgrafos
                    sizeGraphs =  group_subgrafos.size();
                    if(sizeGraphs == 0){
                        group_subgrafos = vector<SubGrafo>(mergeGraphs.begin(),mergeGraphs.end());
                        sizeGraphs =  group_subgrafos.size();
                        genNuevoIdsGraphGen();//para actualizar los ides de los subgrafos de mergegraph
                        printDataGrafo();
                        break;
                    }
                }
            }
        }
        //actualizamos la lista de groupsubgrafos
        //con la lista de reserva de mergeGraphs

    }
}




void Grafo::doTriangBase(){
    //making edges from points of every subgraph
    //debemos formar aristas de los puntos de cada subgrafo
    for(int i=0; i<group_subgrafos.size();i++){
        //obtenemos cada subgrafo
        group_subgrafos[i].doConnectionPoints();
    }
}

void Grafo::drawGrafo(vtkSmartPointer<vtkRenderWindow> renderWindow){

    //saving points
    //almacenador de los puntos
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    //traversig every subgraph
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

    //cast graph to polydata
    //conversion del grafo a un polydata
    grafo2PolyData();

    //render the graph
    //renderizar el grafo
    doRender(renderWindow);

}

void Grafo::doRender(vtkSmartPointer<vtkRenderWindow> renderWindow){
    //create a mapper and actor
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

/*********** Aditional Functions from Graph ********************/
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

void Grafo::limpiarGrafoVtk(){
    grafoVtk = vtkSmartPointer<vtkMutableUndirectedGraph>::New();
}

void Grafo::genNuevoIdsGraphGen(){
    limpiarGrafoVtk();
    //Create index using graph render class after doing initial build
    //Creacion de indices usando la clase render del grafo
    //despues de haber hecho la construccion inicial
    for(int i=0; i<group_subgrafos.size();i++){
        group_subgrafos[i].createIdsPuntosSubgrafo(grafoVtk);
    }
}
