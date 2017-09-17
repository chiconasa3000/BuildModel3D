#include "delaunay.h"

struct PointListCompare{
public:
    PointListCompare(int column, int column2) : m_column(column), m_column2(column2) {}

    bool operator()(const vector<double>& lhs, const vector<double>& rhs)
    {
        //comparar la coordenada del eje x
        if (lhs[m_column] == rhs[m_column]){
            return lhs[m_column2] < rhs[m_column2];
        }
        //comparar la coordena del eje y
        else{
            return lhs[m_column] < rhs[m_column];
        }
    }
private:
    int m_column;
    int m_column2;
} toolComparePoints(0,1);

Delaunay::Delaunay()
{
    //cargar de una matriz de puntos del modelo
    MatrixXd nubePuntos(10,2);
    nubePuntos << 2,2,0,0,2,-2,2,4,4,0,6,4,10,-2,8.5,2,10,0,10,4;
    this->nubePuntos = nubePuntos; //iniciando nube matriz
    this->nroPuntos = 10; //iniciando nro de puntos
}

//convertir la nube de puntos de matriz a vector
void Delaunay::nubeMatrix2nubeVect(){
    for(int i=0; i<nroPuntos;i++){
        std::vector<double> puntoTemp;
        for(int j=0; j<2; j++){
            VectorXd ptoActual = nubePuntos.row(i);
            double coordTemp = ptoActual(j);
            puntoTemp.push_back(coordTemp);
        }
        nubePtos.push_back(puntoTemp);
    }
}

//ordena los puntos por coordenada x y despues por coord y
void Delaunay::ordenarNubePtos(){
    sort(nubePtos.begin(), nubePtos.end(), toolComparePoints);
}


void Delaunay::dividirNubePtos(){
    //comienza con el nro de puntos
    //le daremos como base la lista de nro de puntos vector
    dividirRecNubePtos(nroPuntos,nubePtos);
    //printGroupBig();

}

void Delaunay::dividirRecNubePtos(int divisor,vector<vector<double>> groupTiny){
    //capturamos el grupo
    if(divisor <= 2 || divisor <= 3){
        //agregamos el minigrupo al grupo general
        groupBig.push_back(groupTiny);

    }
    //llamamos a sus respectivos minigrupos
    else{
        int cutA = ceil(divisor/2.0);
        int cutB = floor(divisor/2.0);

       vector<vector<double>> groupA(groupTiny.begin(), groupTiny.begin() + cutA);
       vector<vector<double>> groupB(groupTiny.begin()+cutA, groupTiny.end());

       dividirRecNubePtos(cutA,groupA);
       dividirRecNubePtos(cutB,groupB);

    }
}


void Delaunay::estructurarInGrafo(){
    //construyendo el grafo a partir del vector ya recolectado "groupBig"
    grafoDelaunay = new Grafo(groupBig);
}

/************************ Funciones Adicionales **************************/


//imprime la nube de puntos de forma matriz
void Delaunay::printNubePuntos(){
    cout<<"NubePuntos:"<<endl;
    cout<<nubePuntos<<endl;
}

//imprime la nube de puntos en forma de vector
void Delaunay::printNubeVect(){
    for(int i=0; i<nubePtos.size();i++){
        for(int j=0; j<nubePtos[i].size(); j++){
            cout<<nubePtos[i][j]<<" ";
        }
        cout<<endl;
    }
}

//imprime la nube de puntos en forma de vector
void Delaunay::printGroupBig(){
    //recorriendo el grupo general
    for(int i=0; i<groupBig.size();i++){
        //imprimiendo los grupos pequeños
        for(int j=0; j<groupBig[i].size(); j++){
            //imprimiendo los puntos de cada grupo pequeño
            for(int k=0; k<groupBig[i][j].size(); k++){
                cout<<groupBig[i][j][k]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
    }
}



//devuelve el numero de puntos de la entrada para el delaunay
long Delaunay::getNroPuntosNube(){
    return nroPuntos;
}

//devuelve la estructura en vector de la nube de puntos
vector<vector<double>> Delaunay::getNubeVector(){
    return nubePtos;
}

//devuelve en estructura matriz la nube de puntos
MatrixXd Delaunay::getNubeMatrix(){
    return nubePuntos;
}

//Pasa el grafo para q pueda ser manejado externamente
Grafo * Delaunay::getGrafoDelaunay(){
    return grafoDelaunay;
}
