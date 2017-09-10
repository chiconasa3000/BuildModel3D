#ifndef PUNTO_H
#define PUNTO_H

#include <list>

class Punto
{
public:
    Punto();
    void getLessPoint(std::list<int> idPuntos);
private:
    int idePunto;
    double coor_x;
    double coor_y;
};

#endif // PUNTO_H
