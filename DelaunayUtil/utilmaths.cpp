#include "utilmaths.h"

UtilMaths::UtilMaths()
{

}


bool UtilMaths::orderCounterClockwise(Punto *p1,Punto *p2,Punto *p3){

    //double sigma = (p2->getY() - p1->getY())/(p2->getX()-p1->getX());
    //double tau = (p3->getY() - p2->getY())/(p3->getX()-p2->getX());
    double val = (p2->getY() - p1->getY()) * (p3->getX() - p2->getX()) -
                (p2->getX() - p1->getX()) * (p3->getY() - p2->getY());

    if(val > 0){
        //alerta de clockwise //cambiamos las posiciones del p1 al p2 y el p2 al p1
        double tempX = p1->getX();
        double tempY = p1->getY();
        //cambiando orientacion de p1
        p1->setX(p2->getX());
        p1->setY(p2->getY());
        //cambiando orientacion de p2
        p2->setX(tempX);
        p2->setY(tempY);
        return true;
    }else
        return false;

}

bool UtilMaths::testBelongCirc(Arista *base, Punto candAct, Punto candNext){
    Punto po = base->getPtoOrigen(); //P
    Punto pd = base->getPtoDestino(); //Q

    //mpq = [(P(1)+Q(1))/2 (P(2)+Q(2))/2];
    vector<double> mpq(2);
    double mpqx = (po.getX() + pd.getX())/2;
    double mpqy = (po.getY() + pd.getY())/2;
    mpq={mpqx,mpqy};

    //mqr = [(Q(1)+R(1))/2 (Q(2)+R(2))/2];
    vector<double> mqr(2);
    double mqrx = (pd.getX() + candAct.getX())/2;
    double mqry = (pd.getY() + candAct.getY())/2;
    mqr={mqrx,mqry};

    //Gradientes
    //gpq = (Q(2)-P(2))/(Q(1)-P(1));
    double gpq = (pd.getY() - po.getY())/(pd.getX()-po.getX());
    //gqr = (R(2)-Q(2))/(R(1)-Q(1));
    double gqr = (candAct.getY() - pd.getY())/(candAct.getX()-pd.getX());

    //actualizando cuando la grandiente escogida da cero al ser horizontal la linea
    //por lo cual se escoge la otra recta pr
    if(gpq == 0.0){
        gpq = (candAct.getY() - po.getY())/(candAct.getX()-po.getX());
        mpqx =(po.getX() + candAct.getX())/2;
        mpqy =(po.getY() + candAct.getY())/2;
        mpq={mpqx,mpqy};
    }else if(gqr==0.0){
        gqr = (candAct.getY() - po.getY())/(candAct.getX()-po.getX());
    }

    //gl1 = -(gpq^-1);
    double gl1 = -pow(gpq,-1);
    //gl2 = -(gqr^-1);
    double gl2 = -pow(gqr,-1);

    Matrix2f A;
    Vector2f b;

    A << -gl1,1,-gl2,1;
    b << mpq[1] - gl1*(mpq[0]), mqr[1]-gl2*(mqr[0]);
    //cout << "Here is the matrix A:\n" << A << endl;
    //cout << "Here is the vector b:\n" << b << endl;
    Vector2f x = A.colPivHouseholderQr().solve(b);
    //cout << "The solution is:\n" << x << endl;

    //radio de la circunferencia
    //dpc = sqrt((P(1)-C(1))^2 + (P(2)-C(2))^2);
    double dpc = pow(po.getX()-x(0),2) + pow(po.getY()-x(1),2);
    double ndpc;
    dpc *= 10000.0;
    modf(dpc, &ndpc);
    ndpc /= 10000.0;


    //probar ahora el punto
    double testRad = pow(candNext.getX()-x(0),2) + pow(candNext.getY()-x(1),2);
    double ntestRad;
    testRad *= 10000.0;
    modf(testRad, &ntestRad);
    ntestRad /= 10000.0;

    if(ntestRad == ndpc){
        //el punto esta en el borde de la circuferencia
        return true;
    }
    else{
        //el punto no pertenece al borde de la circuferencia
        return false;
    }

}

bool UtilMaths::testCoCircularIn(Arista *base,Punto candAct, Punto candNext){
    Punto ptoOrig = base->getPtoOrigen();
    Punto ptoDest = base->getPtoDestino();

    //ordenar los puntos contrario al reloj
    bool cambioOrden = orderCounterClockwise(&ptoOrig,&ptoDest,&candAct);

    //El orden de los puntos no se altera a pesar de que varie el subgrafo elegido
    double p1 = pow(ptoOrig.getX(),2) + pow(ptoOrig.getY(),2);
    double q1 = pow(ptoDest.getX(),2) + pow(ptoDest.getY(),2);

    //Punto candidato
    double r1 = pow(candAct.getX(),2) + pow(candAct.getY(),2);

    //Siguiente punto candidatos a evaluar
    double d1 = pow(candNext.getX(),2) + pow(candNext.getY(),2);

    //medimos las distancias de los puntos evaluados hacia la linea base
    //en caso de ser iguales entonces estan justo en el borde de la circumferencia
    //formando un cuadrilatero

    //esto siempre se hara para todas las evaluaciones
    //bool passEqualDiam = testEqualDiametro(base, candAct, candNext);


    //en caso de no cumplir la restruiccion cuadrilatero
    //se evalua normalmente la cocircularidad
    MatrixXd cocircular;

    cocircular.resize(4,4);

    cocircular <<   ptoOrig.getX(), ptoOrig.getY(), p1, 1,
            ptoDest.getX(), ptoDest.getY(), q1, 1,
            candAct.getX(), candAct.getY(), r1, 1,
            candNext.getX(), candNext.getY(), d1, 1;

    if(cocircular.determinant()>0)
        //FALLO debido a que el next pto candidato se encontro al interior del cocircular
        return false;
    else
        //APROBO debido a que el next pto candidato no se encuentra al interior del cocircular
        return true;

}

bool UtilMaths::testCoCircularOut(Arista *base,Punto candAct, Punto candNext){
    Punto ptoOrig = base->getPtoOrigen();
    Punto ptoDest = base->getPtoDestino();

    //ordenar los puntos contrario al reloj
    bool cambioOrden = orderCounterClockwise(&ptoOrig,&ptoDest,&candAct);

    //El orden de los puntos no se altera a pesar de que varie el subgrafo elegido
    double p1 = pow(ptoOrig.getX(),2) + pow(ptoOrig.getY(),2);
    double q1 = pow(ptoDest.getX(),2) + pow(ptoDest.getY(),2);

    //Punto candidato
    double r1 = pow(candAct.getX(),2) + pow(candAct.getY(),2);

    //Siguiente punto candidatos a evaluar
    double d1 = pow(candNext.getX(),2) + pow(candNext.getY(),2);

    //medimos las distancias de los puntos evaluados hacia la linea base
    //en caso de ser iguales entonces estan justo en el borde de la circumferencia
    //formando un cuadrilatero

    //esto siempre se hara para todas las evaluaciones
    bool estaBordeCirc = testBelongCirc(base, candAct, candNext);

    if(!estaBordeCirc){
        //en caso de no cumplir la restruiccion cuadrilatero
        //se evalua normalmente la cocircularidad
        MatrixXd cocircular;

        cocircular.resize(4,4);

        cocircular << ptoOrig.getX(), ptoOrig.getY(), p1, 1,
                ptoDest.getX(), ptoDest.getY(), q1, 1,
                candAct.getX(), candAct.getY(), r1, 1,
                candNext.getX(), candNext.getY(), d1, 1;

        if(cocircular.determinant()>0)
            //FALLO debido a que el next pto candidato se encontro al interior del cocircular
            return false;
        else
            //APROBO debido a que el next pto candidato no se encuentra al interior del cocircular
            return true;
    }else{
        //forzar a que el punto candnext este adentro de la circuferencia
        return false;
    }


}

double UtilMaths::calcAngulo(Arista *base,Arista *candidata,bool esIzq){
    //calculo del angulo
    vector<double> a = calcPendiente(base, esIzq);
    vector<double> b = calcPendiente(candidata, true);

    double modA = sqrt(pow(a[0],2)+pow(a[1],2));
    double modB = sqrt(pow(b[0],2)+pow(b[1],2));

    vector<double> unitA = {a[0]/modA,a[1]/modA};
    vector<double> unitB = {b[0]/modB,b[1]/modB};

    /*double num = abs(a[0]*b[0]+a[1]*b[1]);
    double den = sqrt(pow(a[0],2)+pow(a[1],2))*sqrt(pow(b[0],2)+pow(b[1],2));
    double res = num/den;*/
    double ang_rad = atan2(unitB[1],unitB[0]) - atan2(unitA[1],unitA[0]);

    if(ang_rad > M_PI){
        ang_rad -= 2*M_PI;
    }else if(ang_rad < -M_PI){
        ang_rad += 2*M_PI;
    }
    /*double ang_rad = acos(res);*/
    double new_angulo = ang_rad*180/M_PI;
    //en caso de ser negativo
    if(new_angulo < 0)
        new_angulo = -new_angulo;
    return new_angulo;
}

vector<double> UtilMaths::calcPendiente(Arista *base,bool esIzq){
    //calcular el vector de la arista
    Punto a,b;
    if(!esIzq){
        a = base->getPtoDestino();
        b = base->getPtoOrigen();
    }else{
        a = base->getPtoOrigen();
        b = base->getPtoDestino();
    }

    //vector es la diferencia de cada coordenada
    vector <double> vectDir = {b.getX()-a.getX(),b.getY()-a.getY()};
    return vectDir;
}


