#include "perspecdata.h"

PerspecData::PerspecData()
{
    vtraslacion.resize(3);
    vrotacion.resize(3);
}

//Vector Traslacion
void PerspecData::setVtraslacion(float x, float y, float z){
    vtraslacion << x,y,z;
}
void PerspecData::setTx(float val){
    vtraslacion(0) = val;
}

void PerspecData::setTy(float val){
    vtraslacion(1) = val;
}

void PerspecData::setTz(float val){
    vtraslacion(2) = val;
}

VectorXd& PerspecData::getVtraslacion(){
    return vtraslacion;
}


//Vector Rotacion
void PerspecData::setVrotacion(float x, float y, float z){
    vrotacion << x,y,z;
}
void PerspecData::setRx(float val){
    vrotacion(0) = val;
}

void PerspecData::setRy(float val){
    vrotacion(1) = val;
}

void PerspecData::setRz(float val){
    vrotacion(2) = val;
}

VectorXd& PerspecData::getVrotacion(){
    return vrotacion;
}
