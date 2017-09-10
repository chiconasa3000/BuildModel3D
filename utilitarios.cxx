#include "utilitarios.h"
#include <math.h>

utilitarios::utilitarios()
{

}

MatrixXd utilitarios::segmPoints(int div, MatrixXd kernel){
    MatrixXd kernelMayor;
    MatrixXd kernelA;
    MatrixXd kernelB;
    if(div <= 2 || div<=3)
        kernelMayor = kernel,-1,-1;
    else{
        int diva = ceil(div/2);
        int divb = floor(div/2);
        /*kernelA = kernel(1:divA,:);
        kernelB = kernel(divA+1:end,:);
        kernels =[segPoints(divA,kernelA);segPoints(divB,kernelB)];*/
    }

    return kernelMayor;
}


