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
#include "./src/GeometryPropertiesDialog.h"
#include "ui_GeometryPropertiesDialog.h"

#include <QApplication>
#include <QColorDialog>
#include <QPainter>
#include <QPixmap>

#include "Geometry.h"
#include "GeometryPartRepresentation.h"

//------------------------------------------------------------------------------

GeometryPropertiesDialog::GeometryPropertiesDialog(QWidget* parent) :
  QDialog(parent),
  m_ui(new Ui::GeometryPropertiesDialog)
{
  m_ui->setupUi(this);

  initConnections();
}

//------------------------------------------------------------------------------

GeometryPropertiesDialog::~GeometryPropertiesDialog()
{
  delete m_ui;
}

//------------------------------------------------------------------------------

void GeometryPropertiesDialog::initConnections()
{
  connect(
    m_ui->m_changeColorButton, SIGNAL(pressed()),
    this,                      SLOT(onColorButtonPressed()));
}

//------------------------------------------------------------------------------

void GeometryPropertiesDialog::setCurrentGeometryList(
  std::vector<std::weak_ptr<GeometryRepresentation>> geometries)
{
  if( geometries.empty() )
  {
    return;
  }

  m_ui->m_geometriesCombo->clear();

  m_geometriesRepresentation = geometries;
  std::weak_ptr<GeometryRepresentation> mainGeom =
    m_geometriesRepresentation[0];

  for( auto weakGeomRep : m_geometriesRepresentation )
  {
    if( auto validGeomRep = weakGeomRep.lock() )
    {
      if( auto validGeom = validGeomRep->m_geometry.lock() )
      {
        m_ui->m_geometriesCombo->addItem(validGeom->getName());
      }
    }
  }

  if( auto validGeomRep = mainGeom.lock() )
  {
    if( validGeomRep->m_geometry.lock() )
    {
      updateUI(validGeomRep);
    }
  }
}

//------------------------------------------------------------------------------

void GeometryPropertiesDialog::updateUI(
  std::shared_ptr<GeometryRepresentation>& geom)
{
  if( (geom->m_geometryParts.size() <= 0) || geom->m_geometry.expired() )
  {
    return;
  }

  m_ui->m_geometryPartsCombo->clear();

  for( auto& geomPart : geom->m_geometryParts )
  {
    m_ui->m_geometryPartsCombo->addItem(geomPart->getPartName());
  }

  if( auto validGeom = geom->m_geometry.lock() )
  {
    updateDatasetsCombo( validGeom );
  }

  updateSolidColorUI( geom->m_geometryParts.at(0) );
}

//------------------------------------------------------------------------------

void GeometryPropertiesDialog::updateSolidColorUI(
  const std::unique_ptr<GeometryPartRepresentation>& geomPartRep)
{
  QPixmap pixmap(30, 30);
  QPainter painter(&pixmap);
  painter.setBrush(Qt::SolidPattern);
  painter.setPen(Qt::black);
  painter.drawRect(0, 0, 29, 29);
  painter.fillRect(2, 2, 26, 26, geomPartRep->getSolidColor());
  painter.end();

  m_ui->m_colorLabel->setPixmap(pixmap);

  m_activePart = geomPartRep->getPartName();
}

//------------------------------------------------------------------------------

void GeometryPropertiesDialog::updateDatasetsCombo(std::shared_ptr<Geometry>& geom)
{
  m_ui->m_datasetNamesCombo->clear();

  for( auto& dataName : geom->getPointDatasetsInfo().keys() )
  {
    m_ui->m_datasetNamesCombo->addItem(dataName);
  }

  for( auto& dataName : geom->getCellDatasetsInfo().keys() )
  {
    m_ui->m_datasetNamesCombo->addItem(dataName);
  }
}

//------------------------------------------------------------------------------

void GeometryPropertiesDialog::onColorButtonPressed()
{
  std::shared_ptr<GeometryRepresentation>&& activeGeom =
    getActiveGeometry();

  if( !activeGeom )
  {
    return;
  }

  for( auto& geomPartRep : activeGeom->m_geometryParts )
  {
    if( geomPartRep->getPartName() == m_ui->m_geometryPartsCombo->currentText() )
    {
      QColor solidColor = QColorDialog::getColor(
        geomPartRep->getSolidColor(),
        nullptr,
        "Select Solid Color");

      geomPartRep->setSolidColor(solidColor);
      updateSolidColorUI(geomPartRep);
    }
  }

//  if()
}

//------------------------------------------------------------------------------

std::shared_ptr<GeometryRepresentation>&&
  GeometryPropertiesDialog::getActiveGeometry() const
{
  std::shared_ptr<GeometryRepresentation> invalid;

  if( m_geometriesRepresentation.empty() )
  {
    return std::move( invalid );
  }

  for( auto weakRep : m_geometriesRepresentation )
  {
    if( auto validRepresentation = weakRep.lock() )
    {
      if( auto validGeometry = validRepresentation->m_geometry.lock() )
      {
        if( validGeometry->getName() == m_ui->m_geometriesCombo->currentText() )
        {
          return std::move( validRepresentation );
        }
      }
    }
  }

  return std::move( invalid );
}

//------------------------------------------------------------------------------
