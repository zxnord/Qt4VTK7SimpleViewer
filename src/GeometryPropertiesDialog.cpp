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

#include "Geometry.h"

//------------------------------------------------------------------------------

GeometryPropertiesDialog::GeometryPropertiesDialog(QWidget *parent) :
  QDialog(parent),
  m_ui(new Ui::GeometryPropertiesDialog)
{
  m_ui->setupUi(this);
}

//------------------------------------------------------------------------------

GeometryPropertiesDialog::~GeometryPropertiesDialog()
{
  delete m_ui;
}

//------------------------------------------------------------------------------

void GeometryPropertiesDialog::setCurrentGeometryList(
  std::vector<std::weak_ptr<GeometryRepresentation>>& geometries)
{
  if( geometries.empty() )
  {
    return;
  }

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

}

//------------------------------------------------------------------------------
