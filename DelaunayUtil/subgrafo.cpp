#include "subgrafo.h"

//
SubGrafo::SubGrafo(){

}

SubGrafo::SubGrafo(vector<vector<double>> listapuntos)
{

}

void SubGrafo::insertPunto(Punto &p){
    group_puntos.push_back(p);
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
