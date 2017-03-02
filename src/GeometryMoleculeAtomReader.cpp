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

#include <vtkMolecule.h>

//------------------------------------------------------------------------------

std::unique_ptr<GeometryPart> readPart(const QSettings& geomFile, int regionId)
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

std::unique_ptr<Geometry> readGeometry(const QSettings& geomFile)
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
    std::unique_ptr<GeometryPart> moleculePart = readPart(geomFile, i);
    atomicGeom->addPart( moleculePart );
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

  return readGeometry(geometryFile);
}

//------------------------------------------------------------------------------
