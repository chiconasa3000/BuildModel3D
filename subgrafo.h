#ifndef SUBGRAFO_H
#define SUBGRAFO_H

#include <list>
#include <arista.h>

class SubGrafo
{
public:
    SubGrafo();
    bool doFirstCond();
    void anguloEnAristas(int idAristaA, int idAristaB);
    bool doSecondCond();
    bool pointInCircle();
    void completarDelaunay();
    //retorna id de aristas(
    std::list<int> buscarAristasConPuntoOrigen(int idPunto);
    void agregarArista();
    void eliminarArista();
    //retorna id de punto
    int getCommonPointEnAristas(int idAristaA, int idAristaB);
private:
    std::list<Arista> group_aristas;
    Arista lineaBase;
};

#endif // SUBGRAFO_H
