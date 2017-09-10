#ifndef ARISTA_H
#define ARISTA_H


class Arista
{
public:
    Arista();
    void construirArista(int idPuntoA,int idePuntoB);
    void getP_MoreLeft();
    void getP_MoreRight();
    void getOpossedPoint(int idPunto);
private:
    static int idArista;
    Punto puntoA;
    Punto puntoB;
    int distancia;
};

#endif // ARISTA_H
