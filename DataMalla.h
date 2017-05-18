#ifndef DATAMALLA_H
#define DATAMALLA_H

#include <iostream>
#include <unordered_map>
#include <list>
#include <eigen3/Eigen/Dense>
#include <vtk-7.1/vtkSmartPointer.h>
#include <vtk-7.1/vtkPolyData.h>
#include <vtk-7.1/vtkTransform.h>
#include <vtk-7.1/vtkTransformPolyDataFilter.h>
using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;

class DataMalla{
private:
    MatrixXd matrix;
    double bounds_data[6];
    double center_data[3];
    double bounds_contour[6];
    double center_contour[3];
    vtkSmartPointer<vtkPolyData> mypolyd;
    //std::unordered_map<double*,double*> *mapEdgesContornoMalla;
    std::unordered_map<double*,double**> *mapEdgesContornoMalla;
    std::list<double*> orderContourMalla;
public:
    DataMalla();
    DataMalla(int fil, int col);
    ~DataMalla();
    void buildDataMallaWithPolyData(vtkSmartPointer<vtkPolyData> *polyd);
    MatrixXd& getMatrizDataMalla();
    vtkSmartPointer<vtkPolyData> convMatrix2PolyData(MatrixXd & nuevoMalla);
    MatrixXd convPolyData2Matrix(vtkSmartPointer<vtkPolyData> * polyd);
    void setBoundsData(vtkSmartPointer<vtkPolyData> *data3d);
    void setCenterData(vtkSmartPointer<vtkPolyData> *data3d);
    void setBoundsCont(vtkSmartPointer<vtkPolyData> *data2d);
    void setCenterCont(vtkSmartPointer<vtkPolyData> *data2d);
    double* calcRatio();
    double* calcTraslacion();
    vtkSmartPointer<vtkPolyData> setAlignEscala(vtkSmartPointer<vtkPolyData> *polydata);
    vtkSmartPointer<vtkPolyData> setAlignPosicion(vtkSmartPointer<vtkPolyData> *polydata);
    void printDataMalla();
    void setPolyData(vtkSmartPointer<vtkPolyData> mypolyd);
    vtkSmartPointer<vtkPolyData> getPolyData();
    void setMapEdges(std::unordered_map<double*,double**> &mapEdgesContornoMalla);
    std::unordered_map<double*,double**> & getMapEdges();
    void ordenarContornos();
    vtkSmartPointer<vtkPolyData> build_mesh_grid(double s1,double s2,double s3);
    std::list<double> calcPtosControl(double salto,double xa,double xb);

};

#endif // DATAMALLA_H


