#ifndef MOUSEINTERACTORSTYLE_H
#define MOUSEINTERACTORSTYLE_H

#include <vtk-7.1/vtkVersion.h>
#include <vtk-7.1/vtkSmartPointer.h>
#include <vtk-7.1/vtkRendererCollection.h>
#include <vtk-7.1/vtkDataSetMapper.h>
#include <vtk-7.1/vtkUnstructuredGrid.h>
#include <vtk-7.1/vtkIdTypeArray.h>
#include <vtk-7.1/vtkTriangleFilter.h>
#include <vtk-7.1/vtkPolyDataMapper.h>
#include <vtk-7.1/vtkActor.h>
#include <vtk-7.1/vtkCommand.h>
#include <vtk-7.1/vtkRenderWindow.h>
#include <vtk-7.1/vtkRenderer.h>
#include <vtk-7.1/vtkRenderWindowInteractor.h>
#include <vtk-7.1/vtkPolyData.h>
#include <vtk-7.1/vtkPoints.h>
#include <vtk-7.1/vtkCellArray.h>
#include <vtk-7.1/vtkPlaneSource.h>
#include <vtk-7.1/vtkCellPicker.h>
#include <vtk-7.1/vtkInteractorStyleTrackballCamera.h>
#include <vtk-7.1/vtkProperty.h>
#include <vtk-7.1/vtkSelectionNode.h>
#include <vtk-7.1/vtkSelection.h>
#include <vtk-7.1/vtkExtractSelection.h>
#include <vtk-7.1/vtkObjectFactory.h>

// eventos para coger el mouse
class MouseInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    //devuelve un puntero de la clase
    static MouseInteractorStyle* New();

    MouseInteractorStyle()
    {
        //el actor y el maper para el puntero del mouse
        selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
        selectedActor = vtkSmartPointer<vtkActor>::New();
    }


    //evento para el click izquierdo sin modo r
    virtual void OnLeftButtonDown()
    {
        //Conseguir la ubicacion del click (en coordenadas el mundo)
        int* pos = this->GetInteractor()->GetEventPosition();

        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        picker->SetTolerance(0.0005);

        // Coger desde una ubicacion por defecto
        picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

        double* worldPosition = picker->GetPickPosition();
        std::cout << "El ide de la celda es: " << picker->GetCellId() << std::endl;

        if(picker->GetCellId() != -1)
        {

            std::cout << "Position del click: " << worldPosition[0] << " " << worldPosition[1]
                      << " " << worldPosition[2] << endl;

            vtkSmartPointer<vtkIdTypeArray> ids = vtkSmartPointer<vtkIdTypeArray>::New();
            ids->SetNumberOfComponents(1);
            ids->InsertNextValue(picker->GetCellId());

            vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
            selectionNode->SetFieldType(vtkSelectionNode::CELL);
            selectionNode->SetContentType(vtkSelectionNode::INDICES);
            selectionNode->SetSelectionList(ids);

            vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
            selection->AddNode(selectionNode);

            vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();

#if VTK_MAJOR_VERSION <= 5
            extractSelection->SetInput(0, this->Data);
            extractSelection->SetInput(1, selection);
#else
            extractSelection->SetInputData(0, this->Data);
            extractSelection->SetInputData(1, selection);
#endif
            extractSelection->Update();

            // In selection
            vtkSmartPointer<vtkUnstructuredGrid> selected = vtkSmartPointer<vtkUnstructuredGrid>::New();
            selected->ShallowCopy(extractSelection->GetOutput());

            std::cout << "Hay " << selected->GetNumberOfPoints()
                      << " puntos en la seleccion." << std::endl;
            std::cout << "Hay " << selected->GetNumberOfCells()
                      << " celdas en la seleccion." << std::endl;


#if VTK_MAJOR_VERSION <= 5
            selectedMapper->SetInputConnection(selected->GetProducerPort());
#else
            selectedMapper->SetInputData(selected);
#endif
            selectedActor->SetMapper(selectedMapper);
            selectedActor->GetProperty()->EdgeVisibilityOn();
            selectedActor->GetProperty()->SetEdgeColor(1,0,0);
            selectedActor->GetProperty()->SetLineWidth(3);

            this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(selectedActor);

        }
        // Eventos hacias adelante
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }

    //el maper, el actor y la data correspondiente a la clase
    vtkSmartPointer<vtkPolyData> Data;
    vtkSmartPointer<vtkDataSetMapper> selectedMapper;
    vtkSmartPointer<vtkActor> selectedActor;

};


#endif // MOUSEINTERACTORSTYLE_H
