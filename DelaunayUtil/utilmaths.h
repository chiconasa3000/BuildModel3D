#ifndef UTILMATHS_H
#define UTILMATHS_H
#include <vector>
#include "arista.h"
#include <numeric>


using namespace std;


class UtilMaths
{
public:
    //template < class RAIter, class Compare >
    //vector<size_t> argSort(RAIter first,RAIter last, Compare comp);
    UtilMaths();
    double calcAngulo(Arista *base,Arista *candidata);
    vector<double> calcPendiente(Arista *base);


};

#endif // UTILMATHS_H
