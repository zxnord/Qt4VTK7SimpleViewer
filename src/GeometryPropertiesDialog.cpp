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
#include "GeometrySettings.h"

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

  connect(
    m_ui->m_geometryPartsCombo, SIGNAL(activated(QString)),
    this,                       SLOT(onGeometryPartComboBoxChanged(QString)));
}

//------------------------------------------------------------------------------

void GeometryPropertiesDialog::setCurrentGeometryList(
  std::vector<std::weak_ptr<GeometrySettings> > geometries)
{
  if( geometries.empty() )
  {
    return;
  }

  m_ui->m_geometriesCombo->clear();

  m_geometriesSettings = geometries;
  std::weak_ptr<GeometrySettings> mainGeom =
    m_geometriesSettings[0];

  for( auto weakGeomSett : m_geometriesSettings )
  {
    if( auto validGeomSett = weakGeomSett.lock() )
    {
      if( m_ui->m_geometriesCombo->count() == 0 )
      {
        m_activeGeometry = validGeomSett->getGeometryName();
        m_ui->m_geometriesCombo->addItem(m_activeGeometry);
      }
      else
      {
        m_ui->m_geometriesCombo->addItem(validGeomSett->getGeometryName());
      }
    }
  }

  if( auto validGeomSett = mainGeom.lock() )
  {
    updateUI(std::move(validGeomSett));
  }
}

//------------------------------------------------------------------------------

void GeometryPropertiesDialog::updateUI(std::shared_ptr<GeometrySettings>&& geom)
{
  QStringList geomPartNames = geom->getGeometryPartsNames();

  if( geomPartNames.size() <= 0 )
  {
    return;
  }

  m_ui->m_geometryPartsCombo->clear();

  for( auto& geomPart : geomPartNames )
  {
    if( m_ui->m_geometryPartsCombo->count() == 0 )
    {
      m_activePart = geomPart;
      m_ui->m_geometryPartsCombo->addItem(m_activePart);
    }
    else
    {
      m_ui->m_geometryPartsCombo->addItem(geomPart);
    }
  }

//  updateDatasetsCombo( validGeom );

  updateSolidColorUI();
}

//------------------------------------------------------------------------------

void GeometryPropertiesDialog::updateSolidColorUI()
{
  QColor solidColor;

  if( auto&& gs = getActiveGeometry() )
  {
    if( !gs->getGeometryPartSolidColor(m_activePart, solidColor) )
    {
      return;
    }
  }

  QPixmap pixmap(30, 30);
  QPainter painter(&pixmap);
  painter.setBrush(Qt::SolidPattern);
  painter.setPen(Qt::black);
  painter.drawRect(0, 0, 29, 29);
  painter.fillRect(2, 2, 26, 26, solidColor);
  painter.end();

  m_ui->m_colorLabel->setPixmap(pixmap);
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
  std::shared_ptr<GeometrySettings> activeGeom =
    getActiveGeometry();

  if( !activeGeom )
  {
    return;
  }

  QColor oldSolidColor;

  if( !activeGeom->getGeometryPartSolidColor(m_activePart, oldSolidColor) )
  {
    return;
  }

  QColor solidColor = QColorDialog::getColor(
    oldSolidColor,
    nullptr,
    "Select Solid Color");

  activeGeom->setGeometryPartSolidColor(m_activePart, solidColor);
}

//------------------------------------------------------------------------------

void GeometryPropertiesDialog::onGeometryPartComboBoxChanged(QString partName)
{
  m_activePart = partName;

//  updateDatasetsCombo();

  updateSolidColorUI();
}

//------------------------------------------------------------------------------

std::shared_ptr<GeometrySettings>
  GeometryPropertiesDialog::getActiveGeometry() const
{
  std::shared_ptr<GeometrySettings> invalid;

  if( m_geometriesSettings.empty() )
  {
    return invalid;
  }

  for( auto weakRep : m_geometriesSettings )
  {
    if( auto validSettings = weakRep.lock() )
    {
      if( validSettings->getGeometryName() == m_activeGeometry )
      {
        return validSettings;
      }
    }
  }

  return invalid;
}

//------------------------------------------------------------------------------
