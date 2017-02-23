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
#include "GeometrySettings.h"

#include <vtkRenderer.h>

#include "Geometry.h"
#include "GeometryPartRepresentation.h"
#include "MyVTKApplication.h"

//------------------------------------------------------------------------------

const QString InvalidGeomName = "InvalidGeometryName";
std::unique_ptr<GeometryPartRepresentation> InvalidPartRepresentation;

//------------------------------------------------------------------------------

GeometrySettings::GeometrySettings(
  const std::unique_ptr<Geometry>& geom,
  QObject* parent)
  :
  QObject(parent),
  m_geom(geom)
{
  createPartRepresentations();
}

//------------------------------------------------------------------------------

GeometrySettings::GeometrySettings(GeometrySettings&& settings)
  :
  QObject(settings.parent()),
  m_geom(settings.m_geom),
  m_geometryParts(std::move(settings.m_geometryParts))
{
}

//------------------------------------------------------------------------------

GeometrySettings::~GeometrySettings()
{
}

//------------------------------------------------------------------------------

void GeometrySettings::createPartRepresentations()
{
  for( auto& part : m_geom.get()->getParts() )
  {
//    auto geomPartRep = std::unique_ptr<GeometryPartRepresentation>(
//      new GeometryPartRepresentation( part ));

//        QPair<QString, double*> info;
//        info.first = "TestField";
//        info.second = new double[2] {0.0, 0.0};

//        info.second[0] = geom->getPointDatasetsInfo()[info.first][0];
//        info.second[1] = geom->getPointDatasetsInfo()[info.first][1];

//        geomPartRep->setDatasetInfo(info);

////        geomPartRep->setSolidColor(QColor(Qt::red));

////        geomPartRep->setNofBands(5);

//        geomPartRep->setShowDataset(true);
//        geomPartRep->setShowDatasetLines(true);

////        qApp->processEvents();

//    m_geometryParts.push_back(std::move(geomPartRep));
    m_geometryParts.push_back( std::unique_ptr<GeometryPartRepresentation>(
      new GeometryPartRepresentation( part )) );
  }
}

//------------------------------------------------------------------------------

void GeometrySettings::addGeometryActorsToRenderer(vtkSmartPointer<vtkRenderer> ren)
{
  for( auto& geomRep : m_geometryParts )
  {
    ren->AddActor(geomRep->getSolidActor());
    ren->AddActor(geomRep->getDatasetActor());
    ren->AddActor(geomRep->getDatasetLinesActor());

    if( geomRep->isModified() )
    {
      qApp->postEvent(
        geomRep.get(),
        new QEvent( GeometryPartRepresentation::UpdateActorsEvent) );
    }
  }
}

//------------------------------------------------------------------------------

void GeometrySettings::registerToUpdate(QObject* obj)
{
  if( !obj )
  {
    return;
  }

  m_toUpdate.push_back(obj);
}

//------------------------------------------------------------------------------

void GeometrySettings::registerToDelete(QObject* obj)
{
  if( !obj )
  {
    return;
  }

  m_toDelete.push_back(obj);
}

//------------------------------------------------------------------------------

const QString& GeometrySettings::getGeometryName() const
{
  return m_geom.get()->getName();
}

//------------------------------------------------------------------------------

QStringList GeometrySettings::getGeometryPartsNames() const
{
  QStringList partsNames;

  for( auto& partRep : m_geometryParts )
  {
    partsNames << partRep->getPartName();
  }

  return partsNames;
}

//------------------------------------------------------------------------------

std::unique_ptr<GeometryPartRepresentation>&
  GeometrySettings::getGeometryPartByName(const QString& name)
{
  for( auto& geomPart : m_geometryParts )
  {
    if( geomPart->getPartName() == name )
    {
      return geomPart;
    }
  }

  return InvalidPartRepresentation;
}

//------------------------------------------------------------------------------

bool GeometrySettings::getGeometryPartSolidColor(
  const QString& name,
  QColor& color)
{
  std::unique_ptr<GeometryPartRepresentation>& geomPart =
    getGeometryPartByName(name);

  if( !geomPart )
  {
    color = QColor();
    return false;
  }

  color = geomPart->getSolidColor();
  return true;
}

//------------------------------------------------------------------------------

bool GeometrySettings::getGeometryPartDatasetLinesColor(
  const QString& name,
  QColor& color)
{
  std::unique_ptr<GeometryPartRepresentation>& geomPart =
    getGeometryPartByName(name);

  if( !geomPart )
  {
    color = QColor();
    return false;
  }

  color = geomPart->getContoursColor();
  return true;
}

//------------------------------------------------------------------------------

bool GeometrySettings::getGeometryPartShowSolid(const QString& name, bool& show)
{
  std::unique_ptr<GeometryPartRepresentation>& geomPart =
    getGeometryPartByName(name);

  if( !geomPart )
  {
    show = false;
    return false;
  }

  show = geomPart->isShowSolid();
  return true;
}

//------------------------------------------------------------------------------

bool GeometrySettings::getGeometryPartShowDataset(
  const QString& name,
  bool& show)
{
  std::unique_ptr<GeometryPartRepresentation>& geomPart =
    getGeometryPartByName(name);

  if( !geomPart )
  {
    show = false;
    return false;
  }

  show = geomPart->isShowDataset();
  return true;
}

//------------------------------------------------------------------------------

bool GeometrySettings::getGeometryPartShowDatasetLines(
  const QString& name,
  bool& show)
{
  std::unique_ptr<GeometryPartRepresentation>& geomPart =
    getGeometryPartByName(name);

  if( !geomPart )
  {
    show = false;
    return false;
  }

  show = geomPart->isShowDatasetLines();
  return true;
}

//------------------------------------------------------------------------------

bool GeometrySettings::getGeometryPartNofDatasetLines(
  const QString& name,
  int& nofContours)
{
  std::unique_ptr<GeometryPartRepresentation>& geomPart =
    getGeometryPartByName(name);

  if( !geomPart )
  {
    nofContours = -1;
    return false;
  }

  nofContours = geomPart->getNofBands();
  return true;
}

//------------------------------------------------------------------------------

bool GeometrySettings::setGeometryPartSolidColor(
  const QString& name,
  const QColor& color)
{
  std::unique_ptr<GeometryPartRepresentation>& geomPart =
    getGeometryPartByName(name);

  if( !geomPart )
  {
    return false;
  }

  geomPart->setSolidColor(color);

  if( geomPart->isModified() )
  {
    QEvent ev(GeometryPartRepresentation::UpdateActorsEvent);
    MyVTKApplication::instance()->sendEvent(
     geomPart.get(),
     &ev );

    MyVTKApplication::instance()->postEvent(
     this,
     new QEvent(GeometryUpdated),
     1000);

    return true;
  }

  return false;
}

//------------------------------------------------------------------------------

bool GeometrySettings::setGeometryPartDatasetLinesColor(
  const QString& name,
  const QColor& color)
{
  std::unique_ptr<GeometryPartRepresentation>& geomPart =
    getGeometryPartByName(name);

  if( !geomPart )
  {
    return false;
  }

  geomPart->setContoursColor(color);

  if( geomPart->isModified() )
  {
    MyVTKApplication::instance()->postEvent(
     this,
     new QEvent(GeometryUpdated) );

    return true;
  }

  return false;
}

//------------------------------------------------------------------------------

bool GeometrySettings::setGeometryPartShowSolid(
  const QString& name,
  bool show)
{
  std::unique_ptr<GeometryPartRepresentation>& geomPart =
    getGeometryPartByName(name);

  if( !geomPart )
  {
    return false;
  }

  geomPart->setShowSolid(show);

  if( geomPart->isModified() )
  {
    MyVTKApplication::instance()->postEvent(
     this,
     new QEvent(GeometryUpdated) );

    return true;
  }

  return false;
}

//------------------------------------------------------------------------------

bool GeometrySettings::setGeometryPartShowDataset(
  const QString& name,
  bool show)
{
  std::unique_ptr<GeometryPartRepresentation>& geomPart =
    getGeometryPartByName(name);

  if( !geomPart )
  {
    return false;
  }

  geomPart->setShowDataset(show);

  if( geomPart->isModified() )
  {
    MyVTKApplication::instance()->postEvent(
     this,
     new QEvent(GeometryUpdated) );

    return true;
  }

  return false;
}

//------------------------------------------------------------------------------

bool GeometrySettings::setGeometryPartShowDatasetLines(
  const QString& name,
  bool show)
{
  std::unique_ptr<GeometryPartRepresentation>& geomPart =
    getGeometryPartByName(name);

  if( !geomPart )
  {
    return false;
  }

  geomPart->setShowDatasetLines(show);

  if( geomPart->isModified() )
  {
    MyVTKApplication::instance()->postEvent(
     this,
     new QEvent(GeometryUpdated) );

    return true;
  }

  return false;
}

//------------------------------------------------------------------------------

bool GeometrySettings::setGeometryPartNofDatasetLines(
  const QString& name,
  int nofLines)
{
  std::unique_ptr<GeometryPartRepresentation>& geomPart =
    getGeometryPartByName(name);

  if( !geomPart )
  {
    return false;
  }

  geomPart->setNofBands(nofLines);

  if( geomPart->isModified() )
  {
    MyVTKApplication::instance()->postEvent(
     this,
     new QEvent(GeometryUpdated) );

    return true;
  }

  return false;
}

//------------------------------------------------------------------------------

void GeometrySettings::customEvent(QEvent* ev)
{
  if( ev->type() == GeometryUpdated )
  {
    for( auto obj : m_toUpdate )
    {
      MyVTKApplication::instance()->postEvent(
        obj,
        new QEvent(GeometryUpdated));
    }
  }

  return QObject::customEvent(ev);
}

//------------------------------------------------------------------------------
