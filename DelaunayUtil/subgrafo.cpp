#include "subgrafo.h"

//Funcion Externa


template <class RAIter, class Compare>
vector<size_t> argSort(RAIter first, RAIter last, Compare comp){
    //creamos el vector que contendra los indices
    vector<size_t> idx(last-first);
    //para llenar idx con el rango de inicio y final comennzando de cero (c++ 11)
    iota(idx.begin(), idx.end(),0);

    //gran uso de una funcion lambda (*_*)
    //hay captura de variables de first y comp y los usa en una funcion
    //q recibe los indices actuales y compara sus valores
    auto idxComp = [&first, comp](size_t i1,size_t i2){
        return comp(first[i1], first[i2]);
    };

    //ordenamos los indices de acuerdo a la comparacion
    //del valor asociado (vector de angulos)
    sort(idx.begin(), idx.end(), idxComp);

    return idx;
}


SubGrafo::SubGrafo(){

}

SubGrafo::SubGrafo(vector<vector<double>> listapuntos)
{

}

void SubGrafo::insertPunto(Punto &p){
    group_puntos.push_back(p);
}


//guardar los candidatos dado el punto base
void SubGrafo::saveCandidates(Arista lBase, Punto ptoInicio){

    //conseguir la lista de aristas del punto base
    vector<Arista> *tmp = ptoInicio.getGroupAristas();
    this->listArisCand = vector<Arista>(tmp->begin(),tmp->end());

    //manejo de operaciones con aristas.
    UtilMaths util;


    vector<double> angulosAristas;
    //debemos ordenar las aristas de acuerdo al angulo formado
    //con la linea base
    double minAng = -1.0;

    //recorremos la lista de aristas
    for(int i=0; i<listArisCand.size();i++){
        Punto a = group_puntos[listArisCand[i].getIdOrigen()];
        Punto b = group_puntos[listArisCand[i].getIdDestino()];

        Arista aristaTmp(a,b);
        //calculamos el angulo actual entre linea base y la arista actual
        double angTemp = util.calcAngulo(&lBase,&aristaTmp);
        //insertamos el angulo a la lista
        angulosAristas.push_back(angTemp);
    }

    orderAristCand(angulosAristas);
}

//ordenar aristas candidatas
void SubGrafo::orderAristCand(vector<double> angulosAristas){
    //funcion que usaremos para comparar (ascendente)
    auto compAng = [](const double &angA, const double &angB){
        return angA < angB;
    };

    //ordenando los angulos de la lista de angulos
    const auto& beginIt = angulosAristas.begin();
    vector<size_t> indAng = argSort(beginIt, angulosAristas.end(),compAng);

    vector<Arista> listAristTemp;

    for(int i=0;i<indAng.size();i++){
        listAristTemp.push_back(listArisCand[indAng[i]]);
    }

    //borramos su contenido y copiamos de la listAristTemp
    listArisCand.clear();
    listArisCand = vector<Arista>(listAristTemp.begin(),listAristTemp.end());
}

//todo punto solo poseera arista de salida
//ademas desde cualquier punto se debera acceder a sus
//respectivas aristas conectadas.
void SubGrafo::doConnectionPoints(){

    //verificamos el numero de puntos del subgrafo
    int numPuntos = group_puntos.size();

    //las combinaciones posibles de ides para formar un triangulo
    vector<vector<int>> ides={{1,2},{0,2},{0,1}};

    if(numPuntos>=3){
        //en caso de poder formar un triangulo
        for(int i=0;i<numPuntos; i++){
            //creamos las aristas para el punto 0
            Arista aristaA(i,ides[i][0]);
            Arista aristaB(i,ides[i][1]);

            //guardamos ambas aristas del punto actual
            group_puntos[i].insertArista(aristaA);
            group_puntos[i].insertArista(aristaB);
        }
    }else if(numPuntos == 2){
        //en caso de forma solo una linea

        //creamos las aristas para el punto 0
        Arista aristaA(0,1);
        Arista aristaB(1,0);

        //guardamos ambas aristas del punto actual
        group_puntos[0].insertArista(aristaA);
        group_puntos[1].insertArista(aristaB);

    }
}

void SubGrafo::drawSubgrafo(vtkMutableUndirectedGraph *g,vtkPoints *points){

    //estamos iterando sobre un subgrafo

    //verificamos el numero de puntos del subgrafo
    int numPuntos = group_puntos.size();

    //creamos los ides con orden de recorrido del grafo
    //createIdsPuntosSubgrafo(g);

    //debe gestionar cualquier numero de vertices
    for(int i=0;i<numPuntos; i++){
        //leemos las aristas de cada punto
        group_puntos[i].drawAristas(g,group_puntos);
    }

    //asignar coordenadas de cada punto del actual subgrafo
    for(int i=0; i<group_puntos.size();i++){
        double x = group_puntos[i].getX();
        double y = group_puntos[i].getY();
        points->InsertNextPoint(x,y,0);
    }

    //agregamos los puntos al grafo
    g->SetPoints(points);
}

void SubGrafo::printSubgrafo(){
    for(int i=0; i<group_puntos.size();i++){
        group_puntos[i].printPoint();
        cout<<endl;
    }
}

void SubGrafo::printAristaSubGra(){
    for(int i=0; i<group_puntos.size();i++){
        cout<<"Punto"<<endl;
        group_puntos[i].printPoint();
        cout<<"Arista"<<endl;
        group_puntos[i].printAristas();
        cout<<endl;
    }
}

void SubGrafo::createIdsPuntosSubgrafo(vtkSmartPointer<vtkMutableUndirectedGraph> g){
    for(int i=0; i<group_puntos.size();i++){
        group_puntos[i].createIdPunto(g);
    }
}

void SubGrafo::printIdes(){
    cout<<"Subgrafo:"<<endl;
    for(int i=0; i<group_puntos.size();i++){
        cout<<group_puntos[i].getIdVert();
    }
}

vector<Punto> SubGrafo::getGroupPuntos(){
    return group_puntos;
}

vector<Arista> SubGrafo::getListArisCand(){
    //adevertencia para del valor de la lista de aristas
    return listArisCand;
}

void SubGrafo::calcMenorPointInYcoord(){
    //Advertencia: uso de orden BURBUJA por ser
    //una cantidad menor de puntos al inicio del subgrafo

    //esto sera solo una vez despues el minimo solo sera
    //actualizado

    Punto pMin = group_puntos[0];
    //inicializacion del primer menor y
    double minTempy = pMin.getY();

    //recorremos la lista de puntos del subgrafo
    for(int i=1; i<group_puntos.size(); i++){
        Punto pTemp = group_puntos[i];
        //comprobramos si existe un y inferior al actual
        if(pTemp.getY() < minTempy){
            //actualizamos el minimo
            minTempy = pTemp.getY();
            //guardamos el punto
            pMin = pTemp;
        }
    }

    //al final minTempy tendra el mas inferior coord en y
    minPtoy = pMin;
}

Punto SubGrafo::getMenorPtoy(){
    return minPtoy;
}
