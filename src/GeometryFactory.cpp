//------------------------------------------------------------------------------
// Copyright 2017 Edson Contreras

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//------------------------------------------------------------------------------
#include "GeometryFactory.h"

#include "Geometry.h"
#include "GeometryPart.h"

#include <vtkAlgorithmOutput.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkCubeSource.h>

//------------------------------------------------------------------------------

std::unique_ptr<Geometry> createCubeGeometry()
{
  double datasetPoints[] {
    0.0, 1.0, 1.0, 0.0,
    1.0, 0.0, 0.0, 1.0,
    0.0, 1.0, 1.0, 0.0,
    1.0, 0.0, 0.0, 1.0,
    0.0, 1.0, 1.0, 0.0,
    1.0, 0.0, 0.0, 1.0,
  };

  double center[] {0.0, 0.0, 0.0};

  // Create a cube
  vtkSmartPointer<vtkCubeSource> cubeSource =
      vtkSmartPointer<vtkCubeSource>::New();
  cubeSource->SetCenter(center);
  cubeSource->SetBounds(
    -0.5, 0.5,
    -0.5, 0.5,
    -0.5, 0.5);

  vtkSmartPointer<vtkDoubleArray> dataset =
      vtkSmartPointer<vtkDoubleArray>::New();
  dataset->SetName("TestField");
  dataset->SetNumberOfTuples(24);

  for(vtkIdType i = 0; i < 24; ++i)
  {
    dataset->SetTuple1(i, datasetPoints[i]);
  }

  cubeSource->Update();

  vtkSmartPointer<vtkPolyData> data =
    vtkSmartPointer<vtkPolyData>::New();

  data->ShallowCopy(cubeSource->GetOutput());
  data->GetPointData()->AddArray(dataset);

  std::unique_ptr<Geometry> geom =
    std::unique_ptr<Geometry>(new Geometry());

  std::unique_ptr<GeometryPart> part =
    std::unique_ptr<GeometryPart>(new GeometryPart());

  part->setPartName("Cube_part");
  part->setGeometryData(data);

  geom->addPart(std::move(part));
  return geom;
}

//------------------------------------------------------------------------------

//std::unique_ptr<Geometry> createSphereGeometry()
//{
//  double datasetPoints[] = {
//    0.0, 1.0, 1.0, 0.0,
//    1.0, 0.0, 0.0, 1.0,
//    0.0, 1.0, 1.0, 0.0,
//    1.0, 0.0, 0.0, 1.0,
//    0.0, 1.0, 1.0, 0.0,
//    1.0, 0.0, 0.0, 1.0,
//  };

//  //  double datasetPoints[] = {
//  //    0.0, 0.0, 1.0, 0.0,
//  //    1.0, 1.0, 0.0, 1.0,
//  //  };

//  // Create a cube
//  vtkSmartPointer<vtkCubeSource> cubeSource =
//      vtkSmartPointer<vtkCubeSource>::New();
//  cubeSource->SetCenter(center);
//  cubeSource->SetBounds(
//        -0.5, 0.5,
//        -0.5, 0.5,
//        -0.5, 0.5);

//  vtkSmartPointer<vtkDoubleArray> dataset =
//      vtkSmartPointer<vtkDoubleArray>::New();
//  dataset->SetName(g_fieldName);
//  dataset->SetNumberOfTuples(24);

//  for(vtkIdType i = 0; i < 24; ++i)
//  {
//    dataset->SetTuple1(i, datasetPoints[i]);
//  }

//  //  // Create a sphere
//  //  vtkSmartPointer<vtkCubeSource> cubeSource =
//  //    vtkSmartPointer<vtkCubeSource>::New();
//  //  sphereSource->SetRadius(3.0);
//  //  sphereSource->SetPhiResolution(20);
//  //  sphereSource->SetThetaResolution(20);

//  cubeSource->Update();

//  vtkSmartPointer<vtkPolyData> data =
//      vtkSmartPointer<vtkPolyData>::New();

//  data->ShallowCopy(cubeSource->GetOutput());
//  data->GetPointData()->AddArray(dataset);

//  data->GetPointData()->CopyAllOff();
//}

//------------------------------------------------------------------------------

std::unique_ptr<Geometry> GeometryFactory::CreateBasicGeometry(
  BasicGeometries type)
{
  switch (type)
  {
    case CUBE_GEOMETRY:
      return createCubeGeometry();

    case SPHERE_GEOMETRY: // Not implemented yet
    default:
      return std::unique_ptr<Geometry>();
  }
}

//------------------------------------------------------------------------------


