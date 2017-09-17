#ifndef INTERACTORSTYLE_H
#define INTERACTORSTYLE_H

#include <vtk-7.1/vtkVersion.h>
#include <vtk-7.1/vtkSmartPointer.h>
#include <vtk-7.1/vtkPointData.h>
#include <vtk-7.1/vtkIdTypeArray.h>
#include <vtk-7.1/vtkDataSetSurfaceFilter.h>
#include <vtk-7.1/vtkRendererCollection.h>
#include <vtk-7.1/vtkProperty.h>
#include <vtk-7.1/vtkPlanes.h>
#include <vtk-7.1/vtkObjectFactory.h>
#include <vtk-7.1/vtkPolyDataMapper.h>
#include <vtk-7.1/vtkActor.h>
#include <vtk-7.1/vtkRenderWindow.h>
#include <vtk-7.1/vtkRenderer.h>
#include <vtk-7.1/vtkRenderWindowInteractor.h>
#include <vtk-7.1/vtkPolyData.h>
#include <vtk-7.1/vtkPointSource.h>
#include <vtk-7.1/vtkInteractorStyleRubberBandPick.h>
#include <vtk-7.1/vtkAreaPicker.h>
#include <vtk-7.1/vtkExtractGeometry.h>
#include <vtk-7.1/vtkDataSetMapper.h>
#include <vtk-7.1/vtkUnstructuredGrid.h>
#include <vtk-7.1/vtkVertexGlyphFilter.h>
#include <vtk-7.1/vtkIdFilter.h>

// Define interaction style
class InteractorStyle : public vtkInteractorStyleRubberBandPick
{
  public:
    static InteractorStyle* New();
    vtkTypeMacro(InteractorStyle,vtkInteractorStyleRubberBandPick);

    InteractorStyle()
    {
      this->SelectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
      this->SelectedActor = vtkSmartPointer<vtkActor>::New();
      this->SelectedActor->SetMapper(SelectedMapper);
    }

    virtual void OnLeftButtonUp()
    {
      // Forward events
      vtkInteractorStyleRubberBandPick::OnLeftButtonUp();

      vtkPlanes* frustum = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();

      vtkSmartPointer<vtkExtractGeometry> extractGeometry =
        vtkSmartPointer<vtkExtractGeometry>::New();
      extractGeometry->SetImplicitFunction(frustum);
#if VTK_MAJOR_VERSION <= 5
      extractGeometry->SetInput(this->Points);
#else
      extractGeometry->SetInputData(this->Points);
#endif
      extractGeometry->Update();

      vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter =
        vtkSmartPointer<vtkVertexGlyphFilter>::New();
      glyphFilter->SetInputConnection(extractGeometry->GetOutputPort());
      glyphFilter->Update();

      vtkPolyData* selected = glyphFilter->GetOutput();
      std::cout << "Seleccionado " << selected->GetNumberOfPoints() << " puntos." << std::endl;
      std::cout << "Seleccionado " << selected->GetNumberOfCells() << " celdas." << std::endl;
#if VTK_MAJOR_VERSION <= 5
      this->SelectedMapper->SetInput(selected);
#else
      this->SelectedMapper->SetInputData(selected);
#endif
      this->SelectedMapper->ScalarVisibilityOff();

      vtkIdTypeArray* ids = vtkIdTypeArray::SafeDownCast(selected->GetPointData()->GetArray("OriginalIds"));
      for(vtkIdType i = 0; i < ids->GetNumberOfTuples(); i++)
        {
            std::cout << "Ide " << i << " : " << ids->GetValue(i) << std::endl;
        }

      this->SelectedActor->GetProperty()->SetColor(1.0, 0.0, 0.0); //(R,G,B)
      this->SelectedActor->GetProperty()->SetPointSize(5);

      this->CurrentRenderer->AddActor(SelectedActor);
      this->GetInteractor()->GetRenderWindow()->Render();
      this->HighlightProp(NULL);
    }

    void SetPoints(vtkSmartPointer<vtkPolyData> points) {this->Points = points;}
  private:
    vtkSmartPointer<vtkPolyData> Points;
    vtkSmartPointer<vtkActor> SelectedActor;
    vtkSmartPointer<vtkDataSetMapper> SelectedMapper;

};



#endif // INTERACTORSTYLE_H
