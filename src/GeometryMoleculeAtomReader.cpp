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
#include "GeometryMoleculeAtomReader.h"

#include "Geometry.h"
#include "GeometryPartMolecule.h"

#include <QFile>
#include <QSettings>
#include <QStringList>
#include <QVariant>
#include <QVector>

#include <vtkAppendPolyData.h>
#include <vtkGlyph3D.h>
#include <vtkLineSource.h>
#include <vtkMolecule.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkTubeFilter.h>

//------------------------------------------------------------------------------

std::unique_ptr<GeometryPart> readAtomicMolPart(
  const QSettings& geomFile,
  int regionId)
{
  QString fileRegionId = QString("Region_%1").arg(regionId);

  QVariant partNameVariant     = geomFile.value(fileRegionId+"/name", "");
  QVariant nofParticlesVariant = geomFile.value(fileRegionId+"/nofParticles", 0);
  QVariant nofBondsVariant     = geomFile.value(fileRegionId+"/nofBonds", 0);

  bool numOk = false;

  QString partName = partNameVariant.toString();
  int nofParticles = nofParticlesVariant.toInt(&numOk);

  if( !numOk )
  {
    return std::unique_ptr<GeometryPart>();
  }

  int nofBonds = nofBondsVariant.toInt(&numOk);

  if( !numOk )
  {
    return std::unique_ptr<GeometryPart>();
  }

  QList<vtkAtom> atoms;
  vtkSmartPointer<vtkMolecule> molecule =
    vtkSmartPointer<vtkMolecule>::New();

  std::unique_ptr<GeometryPart> atomicPart =
    std::unique_ptr<GeometryPartMolecule>(new GeometryPartMolecule(partName));

  for( int i = 0; i < nofParticles; ++i )
  {
    QVector<double> pointCoords(3, 0.0);
    QStringList pointRepresentation;

    QVariant currentPointValueVariant = geomFile.value(
      fileRegionId+QString("/points/p%1").arg(i), "0, 0, 0");

    pointRepresentation = currentPointValueVariant.toStringList();

    for(int j = 0; j < 3; ++j)
    {
      pointCoords[j] = pointRepresentation[j].toDouble();
    }

    atoms <<
      molecule->AppendAtom(10, pointCoords[0], pointCoords[1], pointCoords[2]);
  }

  for( int i = 0; i < nofBonds; ++i )
  {
    QVector<int> bondIds(2, 0);
    QStringList pointRepresentation;

    QVariant currentPointValueVariant = geomFile.value(
      fileRegionId+QString("/bonds/b%1").arg(i), "0, 0");

    pointRepresentation = currentPointValueVariant.toStringList();

    for(int j = 0; j < 2; ++j)
    {
      bondIds[j] = pointRepresentation[j].toInt();
    }

    molecule->AppendBond(atoms[bondIds[0]], atoms[bondIds[1]]);
  }

  if( GeometryPartMolecule* molPart =
    static_cast<GeometryPartMolecule*>( atomicPart.get() ) )
  {
    molPart->setMoleculeData(molecule);
  }

  return std::move( atomicPart );
}

//------------------------------------------------------------------------------

std::unique_ptr<GeometryPart> readAtomicPolyPart(
  const QSettings& geomFile,
  int regionId,
  QList<QVector<double>>& atoms)
{
  QString fileRegionId = QString("Region_%1").arg(regionId);

  QVariant partNameVariant     = geomFile.value(fileRegionId+"/name", "");
  QVariant nofParticlesVariant = geomFile.value(fileRegionId+"/nofParticles", 0);

  bool numOk = false;

  QString partName = partNameVariant.toString();
  int nofParticles = nofParticlesVariant.toInt(&numOk);

  if( !numOk )
  {
    return std::unique_ptr<GeometryPart>();
  }

  vtkSmartPointer<vtkPoints> partPoints =
    vtkSmartPointer<vtkPoints>::New();
  partPoints->Resize(nofParticles);

  std::unique_ptr<GeometryPart> atomicPart =
    std::unique_ptr<GeometryPart>(new GeometryPart(partName));

  for( int i = 0; i < nofParticles; ++i )
  {
    QVector<double> pointCoords(3, 0.0);
    QStringList pointRepresentation;

    QVariant currentPointValueVariant = geomFile.value(
      fileRegionId+QString("/points/p%1").arg(i), "0, 0, 0");

    pointRepresentation = currentPointValueVariant.toStringList();

    for(int j = 0; j < 3; ++j)
    {
      pointCoords[j] = pointRepresentation[j].toDouble();
    }

    partPoints->InsertNextPoint(
      pointCoords[0], pointCoords[1], pointCoords[2]);

    atoms << pointCoords;
  }

  vtkSmartPointer<vtkPolyData> data = vtkSmartPointer<vtkPolyData>::New();
  data->SetPoints(partPoints);

  vtkSmartPointer<vtkSphereSource> spheres =
    vtkSmartPointer<vtkSphereSource>::New();
  spheres->SetCenter(0, 0, 0);
  spheres->SetRadius(0.5);

  vtkSmartPointer<vtkGlyph3D> glyph =
    vtkSmartPointer<vtkGlyph3D>::New();

  glyph->SetInputData(data);
  glyph->SetSourceConnection(spheres->GetOutputPort());
  glyph->Update();

  atomicPart->setGeometryData( glyph->GetOutput() );

  return std::move( atomicPart );
}

//------------------------------------------------------------------------------

std::unique_ptr<GeometryPart> readAtomicPolyPart_Bonds(
  const QSettings& geomFile,
  int regionId,
  QList<QVector<double>>& atoms)
{
  QString fileRegionId = QString("Region_%1").arg(regionId);

  QVariant partNameVariant     = geomFile.value(fileRegionId+"/name", "");
  QVariant nofBondsVariant     = geomFile.value(fileRegionId+"/nofBonds", 0);

  bool numOk = false;

  QString partName = partNameVariant.toString() + QString("_bonds");

  int nofBonds = nofBondsVariant.toInt(&numOk);

  if( !numOk )
  {
    return std::unique_ptr<GeometryPart>();
  }

  std::unique_ptr<GeometryPart> atomicPart =
    std::unique_ptr<GeometryPart>(new GeometryPart(partName));

  vtkSmartPointer<vtkAppendPolyData> finalData =
    vtkSmartPointer<vtkAppendPolyData>::New();

  for( int i = 0; i < nofBonds; ++i )
  {
    QVector<int> bondData(2, 0);
    QStringList pointRepresentation;

    QVariant currentPointValueVariant = geomFile.value(
      fileRegionId+QString("/bonds/b%1").arg(i), "0, 0");

    pointRepresentation = currentPointValueVariant.toStringList();

    for(int j = 0; j < 2; ++j)
    {
      bondData[j] = pointRepresentation[j].toInt();
    }

    vtkSmartPointer<vtkLineSource> line =
      vtkSmartPointer<vtkLineSource>::New();
    line->SetPoint1(atoms[ bondData[0] ].data());
    line->SetPoint2(atoms[ bondData[1] ].data());

    vtkSmartPointer<vtkTubeFilter> tube =
      vtkSmartPointer<vtkTubeFilter>::New();
    tube->SetNumberOfSides(12);
    tube->SetRadius(0.2);
    tube->SetInputConnection(line->GetOutputPort());

    finalData->AddInputConnection(tube->GetOutputPort());
  }

  if( finalData->GetTotalNumberOfInputConnections() <= 0 )
  {
    return std::unique_ptr<GeometryPart>();
  }

  finalData->Update();

  atomicPart->setGeometryData( finalData->GetOutput() );

  return std::move( atomicPart );
}

//------------------------------------------------------------------------------

std::unique_ptr<Geometry> readAtomicMolGeometry(const QSettings& geomFile)
{
  QVariant geomNameVariant   = geomFile.value("Geometry/name", "");
  QVariant nofRegionsVariant = geomFile.value("Geometry/nofRegions", 0);

  bool numOk = false;

  QString geomName = geomNameVariant.toString();
  int nofRegions = nofRegionsVariant.toInt(&numOk);

  if( !numOk )
  {
    return std::unique_ptr<Geometry>();
  }

  std::unique_ptr<Geometry> atomicGeom =
    std::unique_ptr<Geometry>( new Geometry(geomName) );

  for( int i = 0; i < nofRegions; ++i )
  {
    std::unique_ptr<GeometryPart> moleculePart = readAtomicMolPart(geomFile, i);
    atomicGeom->addPart( moleculePart );
  }

  return std::move( atomicGeom );
}

//------------------------------------------------------------------------------

std::unique_ptr<Geometry> readAtomicPolyGeometry(const QSettings& geomFile)
{
  QVariant geomNameVariant   = geomFile.value("Geometry/name", "");
  QVariant nofRegionsVariant = geomFile.value("Geometry/nofRegions", 0);

  bool numOk = false;

  QString geomName = geomNameVariant.toString();
  int nofRegions = nofRegionsVariant.toInt(&numOk);

  if( !numOk )
  {
    return std::unique_ptr<Geometry>();
  }

  std::unique_ptr<Geometry> atomicGeom =
    std::unique_ptr<Geometry>( new Geometry(geomName) );

  for( int i = 0; i < nofRegions; ++i )
  {
    QList<QVector<double>> atoms;
    std::unique_ptr<GeometryPart> moleculePart = readAtomicPolyPart(
      geomFile,
      i,
      atoms);

    atomicGeom->addPart( moleculePart );

    std::unique_ptr<GeometryPart> bondsPart = readAtomicPolyPart_Bonds(
      geomFile,
      i,
      atoms);

    atomicGeom->addPart( bondsPart );
  }

  return std::move( atomicGeom );
}

//------------------------------------------------------------------------------

std::unique_ptr<Geometry> GeometryMoleculeAtomReader::FromMolFile(
  const QString& fileName)
{
  QFile testFile(fileName);

  if( !testFile.exists() )
  {
    return std::unique_ptr<Geometry>();
  }

  QSettings geometryFile(fileName, QSettings::IniFormat);

  return readAtomicMolGeometry(geometryFile);
}

//------------------------------------------------------------------------------

std::unique_ptr<Geometry> GeometryMoleculeAtomReader::FromPolyFile(
  const QString& fileName)
{
  QFile testFile(fileName);

  if( !testFile.exists() )
  {
    return std::unique_ptr<Geometry>();
  }

  QSettings geometryFile(fileName, QSettings::IniFormat);

  return readAtomicPolyGeometry(geometryFile);
}

//------------------------------------------------------------------------------
