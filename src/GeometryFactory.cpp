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
#include "GeometryMoleculeAtomReader.h"
#include "GeometryPart.h"
#include "GeometryPartMolecule.h"

#include <QStringList>

#include <vtkAlgorithmOutput.h>
#include <vtkAppendPolyData.h>
#include <vtkCubeSource.h>
#include <vtkDoubleArray.h>
#include <vtkGlyph3D.h>
#include <vtkLineSource.h>
#include <vtkMolecule.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkTubeFilter.h>

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
    std::unique_ptr<Geometry>(new Geometry("Basic_Cube"));

  std::unique_ptr<GeometryPart> part =
    std::unique_ptr<GeometryPart>(new GeometryPart("Cube_part"));

  part->setGeometryData(data);

  geom->addPart( part );
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

std::unique_ptr<Geometry> createCubeAtomGeometry()
{
  double datasetPoints[] {
    0.0, 1.0, 1.0, 0.0,
    1.0, 0.0, 0.0, 1.0,
    0.0, 1.0, 1.0, 0.0,
    1.0, 0.0, 0.0, 1.0,
    0.0, 1.0, 1.0, 0.0,
    1.0, 0.0, 0.0, 1.0,
  };

  // Create cube points
  vtkSmartPointer<vtkPoints> cubePoints =
    vtkSmartPointer<vtkPoints>::New();

  cubePoints->InsertNextPoint(0.5, 0.5, 0.5);
  cubePoints->InsertNextPoint(-0.5, 0.5, 0.5);
  cubePoints->InsertNextPoint(-0.5, -0.5, 0.5);
  cubePoints->InsertNextPoint(0.5, -0.5, 0.5);
  cubePoints->InsertNextPoint(0.5, 0.5, -0.5);
  cubePoints->InsertNextPoint(-0.5, 0.5, -0.5);
  cubePoints->InsertNextPoint(-0.5, -0.5, -0.5);
  cubePoints->InsertNextPoint(0.5, -0.5, -0.5);

  vtkSmartPointer<vtkDoubleArray> dataset =
      vtkSmartPointer<vtkDoubleArray>::New();
  dataset->SetName("TestField");
  dataset->SetNumberOfTuples(24);

  for(vtkIdType i = 0; i < 24; ++i)
  {
    dataset->SetTuple1(i, datasetPoints[i]);
  }

  // Create a sphere
  vtkSmartPointer<vtkSphereSource> sphereSource =
    vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->SetRadius(0.1);
  sphereSource->SetPhiResolution(20);
  sphereSource->SetThetaResolution(20);

  vtkSmartPointer<vtkGlyph3D> glyph =
    vtkSmartPointer<vtkGlyph3D>::New();

  vtkSmartPointer<vtkPolyData> data =
    vtkSmartPointer<vtkPolyData>::New();

  data->SetPoints(cubePoints);
  data->GetPointData()->AddArray(dataset);
  glyph->SetInputData( data );
  glyph->SetSourceConnection(sphereSource->GetOutputPort());

//------------------------------------------------------------------------------

  vtkSmartPointer<vtkAppendPolyData> bondsData1 =
    vtkSmartPointer<vtkAppendPolyData>::New();

  vtkSmartPointer<vtkLineSource> line11 =
    vtkSmartPointer<vtkLineSource>::New();
  line11->SetPoint1(0.5, 0.5, 0.5);
  line11->SetPoint2(-0.5, 0.5, 0.5);

  vtkSmartPointer<vtkTubeFilter> tubeFilter11 =
    vtkSmartPointer<vtkTubeFilter>::New();
  tubeFilter11->SetRadius(0.05);
  tubeFilter11->SetNumberOfSides(12);

  tubeFilter11->SetInputConnection( line11->GetOutputPort() );

  vtkSmartPointer<vtkLineSource> line12 =
    vtkSmartPointer<vtkLineSource>::New();
  line12->SetPoint1(0.5, 0.5, 0.5);
  line12->SetPoint2(0.5, -0.5, 0.5);

  vtkSmartPointer<vtkTubeFilter> tubeFilter12 =
    vtkSmartPointer<vtkTubeFilter>::New();
  tubeFilter12->SetRadius(0.05);
  tubeFilter12->SetNumberOfSides(12);

  tubeFilter12->SetInputConnection( line12->GetOutputPort() );

  vtkSmartPointer<vtkLineSource> line13 =
    vtkSmartPointer<vtkLineSource>::New();
  line13->SetPoint1(0.5, 0.5, 0.5);
  line13->SetPoint2(0.5, 0.5, -0.5);

  vtkSmartPointer<vtkTubeFilter> tubeFilter13 =
    vtkSmartPointer<vtkTubeFilter>::New();
  tubeFilter13->SetRadius(0.05);
  tubeFilter13->SetNumberOfSides(12);

  tubeFilter13->SetInputConnection( line13->GetOutputPort() );

  bondsData1->AddInputConnection( tubeFilter11->GetOutputPort() );
  bondsData1->AddInputConnection( tubeFilter12->GetOutputPort() );
  bondsData1->AddInputConnection( tubeFilter13->GetOutputPort() );

//------------------------------------------------------------------------------

  vtkSmartPointer<vtkAppendPolyData> bondsData2 =
    vtkSmartPointer<vtkAppendPolyData>::New();

  vtkSmartPointer<vtkLineSource> line21 =
    vtkSmartPointer<vtkLineSource>::New();
  line21->SetPoint1(-0.5, -0.5, -0.5);
  line21->SetPoint2(-0.5, -0.5, 0.5);

  vtkSmartPointer<vtkTubeFilter> tubeFilter21 =
    vtkSmartPointer<vtkTubeFilter>::New();
  tubeFilter21->SetRadius(0.05);
  tubeFilter21->SetNumberOfSides(12);

  tubeFilter21->SetInputConnection( line21->GetOutputPort() );

  vtkSmartPointer<vtkLineSource> line22 =
    vtkSmartPointer<vtkLineSource>::New();
  line22->SetPoint1(-0.5, -0.5, -0.5);
  line22->SetPoint2(0.5, -0.5, -0.5);

  vtkSmartPointer<vtkTubeFilter> tubeFilter22 =
    vtkSmartPointer<vtkTubeFilter>::New();
  tubeFilter22->SetRadius(0.05);
  tubeFilter22->SetNumberOfSides(12);

  tubeFilter22->SetInputConnection( line22->GetOutputPort() );

  vtkSmartPointer<vtkLineSource> line23 =
    vtkSmartPointer<vtkLineSource>::New();
  line23->SetPoint1(-0.5, -0.5, -0.5);
  line23->SetPoint2(-0.5, 0.5, -0.5);

  vtkSmartPointer<vtkTubeFilter> tubeFilter23 =
    vtkSmartPointer<vtkTubeFilter>::New();
  tubeFilter23->SetRadius(0.05);
  tubeFilter23->SetNumberOfSides(12);

  tubeFilter23->SetInputConnection( line23->GetOutputPort() );

  bondsData2->AddInputConnection( tubeFilter21->GetOutputPort() );
  bondsData2->AddInputConnection( tubeFilter22->GetOutputPort() );
  bondsData2->AddInputConnection( tubeFilter23->GetOutputPort() );

//------------------------------------------------------------------------------

  std::unique_ptr<Geometry> geom =
    std::unique_ptr<Geometry>(new Geometry("Basic_Cube"));

  std::unique_ptr<GeometryPart> part =
    std::unique_ptr<GeometryPart>(new GeometryPart("Cube_edges"));

  std::unique_ptr<GeometryPart> part2 =
    std::unique_ptr<GeometryPart>(new GeometryPart("Cube_bonds_1"));

  std::unique_ptr<GeometryPart> part3 =
    std::unique_ptr<GeometryPart>(new GeometryPart("Cube_bonds_2"));

  part->setGeometryConnection( glyph->GetOutputPort() );
  part2->setGeometryConnection( bondsData1->GetOutputPort() );
  part3->setGeometryConnection( bondsData2->GetOutputPort() );

  geom->addPart( part );
  geom->addPart( part2 );
  geom->addPart( part3 );
  return geom;
}

//------------------------------------------------------------------------------

std::unique_ptr<Geometry> createMoleculeGeometry()
{
  vtkSmartPointer<vtkMolecule> molecule =
    vtkSmartPointer<vtkMolecule>::New();

  molecule->Initialize();
  vtkAtom carbon = molecule->AppendAtom(6, 0.5, 0.5, 0.5);
  vtkAtom oxygen = molecule->AppendAtom(8, -0.5, 0.5, 0.5);
  vtkAtom hydrogen1 = molecule->AppendAtom(1, 0.5, -0.5, 0.5);
  vtkAtom hydrogen2 = molecule->AppendAtom(1, 0.5, 0.5, -0.5);

  molecule->AppendBond(carbon, oxygen, 2);
  molecule->AppendBond(carbon, hydrogen1, 1);
  molecule->AppendBond(carbon, hydrogen2, 1);

  std::unique_ptr<GeometryPart> part =
    std::unique_ptr<GeometryPartMolecule>(new GeometryPartMolecule("Molecule_1"));

//  part->setMoleculeData(molecule);
  static_cast<GeometryPartMolecule*>(part.get())->setMoleculeData(molecule);

  molecule->Initialize();
  carbon = molecule->AppendAtom(6, -0.5, -0.5, -0.5);
  oxygen = molecule->AppendAtom(8, 0.5, -0.5, -0.5);
  hydrogen1 = molecule->AppendAtom(1, -0.5, -0.5, 0.5);
  hydrogen2 = molecule->AppendAtom(1, -0.5, 0.5, -0.5);

  molecule->AppendBond(carbon, oxygen, 2);
  molecule->AppendBond(carbon, hydrogen1, 1);
  molecule->AppendBond(carbon, hydrogen2, 1);

  std::unique_ptr<GeometryPart> part2 =
    std::unique_ptr<GeometryPartMolecule>(new GeometryPartMolecule("Molecule_2"));

  static_cast<GeometryPartMolecule*>(part2.get())->setMoleculeData(molecule);

  std::unique_ptr<Geometry> geom =
    std::unique_ptr<Geometry>(new Geometry("Molecule_Cube"));

//  geom->addPart( std::static_pointer_cast<GeometryPart>(part) );
//  geom->addPart( std::static_pointer_cast<GeometryPart>(part2) );
//  geom->addPart( std::move(part) );
//  geom->addPart( std::move(part2) );
  geom->addPart( part );
  geom->addPart( part2 );

  return geom;
}

//------------------------------------------------------------------------------

std::unique_ptr<Geometry> GeometryFactory::CreateBasicGeometry(
  BasicGeometries type)
{
  switch (type)
  {
    case CUBE_GEOMETRY:
      return createCubeGeometry();

    case CUBE_ATOM_GEOMETRY:
      return createCubeAtomGeometry();

    case MOLECULE_GEOMETRY:
      return createMoleculeGeometry();

    case SPHERE_GEOMETRY: // Not implemented yet
    default:
      return std::unique_ptr<Geometry>();
  }
}

//------------------------------------------------------------------------------

std::unique_ptr<Geometry> GeometryFactory::CreateGeometryFromFile(QString fileName)
{
  QString ext = fileName.split('.').last();

  if( ext.toUpper() == "MOL" )
  {
    return GeometryMoleculeAtomReader::FromMolFile(fileName);
  }

  return std::unique_ptr<Geometry>();
}

//------------------------------------------------------------------------------
