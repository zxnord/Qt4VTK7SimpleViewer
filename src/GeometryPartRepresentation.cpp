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

#include "GeometryPartRepresentation.h"

#include "GeometryPart.h"
#include "MyVTKApplication.h"

#include <vtkAssignAttribute.h>
#include <vtkBandedPolyDataContourFilter.h>
#include <vtkCellData.h>
#include <vtkGeometryFilter.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkUnstructuredGrid.h>

//------------------------------------------------------------------------------

GeometryPartRepresentation::GeometryPartRepresentation(
  std::weak_ptr<GeometryPart> geomPart,
  vtkWeakPointer<vtkRenderer> ren,
  QObject* parent)
  :
  QObject(parent),
  m_renderer(ren),
  m_geomPart(geomPart),
  m_solidColor(QColor(Qt::gray)),
  m_contoursColor(QColor(Qt::black)),
  m_nofBands(10),
  m_oldVisibility{false, false, false},
  m_showSolid(true),
  m_showDataset(false),
  m_showDatasetLines(false),
  m_modified(false),
  m_solidActor(vtkSmartPointer<vtkActor>::New()),
  m_datasetActor(vtkSmartPointer<vtkActor>::New()),
  m_datasetLinesActor(vtkSmartPointer<vtkActor>::New())
{
  m_solidActor->VisibilityOff();
  m_datasetActor->VisibilityOff();
  m_datasetLinesActor->VisibilityOff();

  if( m_renderer )
  {
    m_renderer->AddActor(m_solidActor);
    m_renderer->AddActor(m_datasetActor);
    m_renderer->AddActor(m_datasetLinesActor);
  }

  updateSolidPartActor();
}

//------------------------------------------------------------------------------

void GeometryPartRepresentation::updateSolidPartActor()
{
  if( m_geomPart.expired() )
  {
    return;
  }

  m_datasetActor->VisibilityOff();

  vtkSmartPointer<vtkGeometryFilter> gFilter =
    vtkSmartPointer<vtkGeometryFilter>::New();

  gFilter->SetInputConnection(m_geomPart.lock()->getGeometryPort());

  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();

  mapper->SetInputConnection(gFilter->GetOutputPort());

  m_solidActor->SetMapper(mapper);
  m_solidActor->GetProperty()->SetColor(
    m_solidColor.redF(),
    m_solidColor.greenF(),
    m_solidColor.blueF() );

  m_solidActor->VisibilityOn();

  m_oldVisibility[0] = true;
  m_oldVisibility[1] = false;
  m_oldVisibility[2] = false;
}

//------------------------------------------------------------------------------

void GeometryPartRepresentation::updateDatasetPartActor()
{
  if( m_geomPart.expired() )
  {
    return;
  }

  auto validPart = m_geomPart.lock();

  if( m_showDataset )
  {
    vtkSmartPointer<vtkGeometryFilter> gFilter;

    if( vtkUnstructuredGrid::SafeDownCast(validPart->getGeometryData()) )
    {
      gFilter =
        vtkSmartPointer<vtkGeometryFilter>::New();

      gFilter->SetInputConnection(validPart->getGeometryPort());
    }


    vtkSmartPointer<vtkAssignAttribute> assigner =
      vtkSmartPointer<vtkAssignAttribute>::New();

    if( validPart->getGeometryData()->GetPointData()
      ->HasArray(qPrintable(m_datasetInfo.first)) )
    {
      if( gFilter )
      {
        assigner->SetInputConnection(gFilter->GetOutputPort());
      }
      else
      {
        assigner->SetInputConnection(validPart->getGeometryPort());
      }

      assigner->Assign(
        qPrintable(m_datasetInfo.first),
        vtkDataSetAttributes::SCALARS,
        vtkAssignAttribute::POINT_DATA);
    }
    else if( validPart->getGeometryData()->GetCellData()
      ->HasArray(qPrintable(m_datasetInfo.first)) )
    {
      if( gFilter )
      {
        assigner->SetInputConnection(gFilter->GetOutputPort());
      }
      else
      {
        assigner->SetInputConnection(validPart->getGeometryPort());
      }

      assigner->Assign(
        qPrintable(m_datasetInfo.first),
        vtkDataSetAttributes::SCALARS,
        vtkAssignAttribute::CELL_DATA);
    }
    else
    {
      updateSolidPartActor();
    }


    vtkSmartPointer<vtkBandedPolyDataContourFilter> contours =
      vtkSmartPointer<vtkBandedPolyDataContourFilter>::New();

    contours->SetInputConnection( assigner->GetOutputPort() );
    contours->GenerateValues(
      m_nofBands,
      m_datasetInfo.second[0],
      m_datasetInfo.second[1]);
    contours->ClippingOff();
    contours->SetClipTolerance(0.0);
    contours->SetScalarModeToValue();
    contours->GenerateContourEdgesOn();

    vtkSmartPointer<vtkPolyDataMapper> mapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();

    mapper->SetInputConnection(contours->GetOutputPort());
    mapper->SetScalarRange(m_datasetInfo.second[0], m_datasetInfo.second[1]);
    mapper->SetScalarModeToUseCellData();

    m_datasetActor->SetMapper(mapper);

    if( m_showDatasetLines )
    {
      vtkSmartPointer<vtkPolyDataMapper> mapper2 =
        vtkSmartPointer<vtkPolyDataMapper>::New();

      mapper2->SetInputData(contours->GetContourEdgesOutput());

      m_datasetLinesActor->SetMapper(mapper2);
      m_datasetLinesActor->GetProperty()->SetColor(
        m_contoursColor.redF(),
        m_contoursColor.greenF(),
        m_contoursColor.blueF());

      m_datasetLinesActor->VisibilityOn();
      m_oldVisibility[2] = true;
    }
    else
    {
      m_datasetLinesActor->VisibilityOff();
      m_oldVisibility[2] = false;
    }

    m_solidActor->VisibilityOff();
    m_datasetActor->VisibilityOn();

    m_oldVisibility[0] = false;
    m_oldVisibility[1] = true;
  }
  else
  {
    m_datasetActor->VisibilityOff();
    m_datasetLinesActor->VisibilityOff();

    m_oldVisibility[1] = false;
    m_oldVisibility[2] = false;
  }



//  m_datasetActor->GetProperty()->EdgeVisibilityOn();
//  m_datasetActor->GetProperty()->SetEdgeColor(0.0, 0.0, 0.0);

}

//------------------------------------------------------------------------------


const QPair<QString, double*>& GeometryPartRepresentation::getDatasetInfo() const
{
  return m_datasetInfo;
}

//------------------------------------------------------------------------------


int GeometryPartRepresentation::getNofBands() const
{
  return m_nofBands;
}

//------------------------------------------------------------------------------

const QColor& GeometryPartRepresentation::getSolidColor() const
{
  return m_solidColor;
}

//------------------------------------------------------------------------------

const QColor& GeometryPartRepresentation::getContoursColor() const
{
  return m_contoursColor;
}

//------------------------------------------------------------------------------

bool GeometryPartRepresentation::isShowSolid() const
{
  return m_showSolid;
}

//------------------------------------------------------------------------------

bool GeometryPartRepresentation::isShowDataset() const
{
  return m_showDataset;
}

//------------------------------------------------------------------------------

bool GeometryPartRepresentation::isShowDatasetLines() const
{
  return m_showDatasetLines;
}

//------------------------------------------------------------------------------

void GeometryPartRepresentation::setDatasetInfo(
  const QPair<QString, double*>& info)
{
  if( m_datasetInfo != info )
  {
    m_datasetInfo = info;
    m_modified = true;

    qApp->postEvent(this, new QEvent(RedrawEvent));
  }
}

//------------------------------------------------------------------------------

void GeometryPartRepresentation::setNofBands(int nofBands)
{
  if( m_nofBands != nofBands )
  {
    m_nofBands = nofBands;
    m_modified = true;

    qApp->postEvent(this, new QEvent(RedrawEvent));
  }
}

//------------------------------------------------------------------------------

void GeometryPartRepresentation::setSolidColor(const QColor& color)
{
  if( m_solidColor != color )
  {
    m_solidColor = color;
    m_modified = true;

    qApp->postEvent(this, new QEvent(RedrawEvent));
  }
}

//------------------------------------------------------------------------------

void GeometryPartRepresentation::setContoursColor(const QColor& color)
{
  if( m_contoursColor != color )
  {
    m_contoursColor = color;
    m_modified = true;

    qApp->postEvent(this, new QEvent(RedrawEvent));
  }
}

//------------------------------------------------------------------------------

void GeometryPartRepresentation::setShowSolid(bool on)
{
  m_showSolid = on;
  m_showDataset = !on;
  m_modified = true;

  qApp->postEvent(this, new QEvent(RedrawEvent));
}

//------------------------------------------------------------------------------

void GeometryPartRepresentation::setShowDataset(bool on)
{
  m_showDataset = on;
  m_showSolid = !on;
  m_modified = true;

  qApp->postEvent(this, new QEvent(RedrawEvent));
}

//------------------------------------------------------------------------------

void GeometryPartRepresentation::setShowDatasetLines(bool on)
{
  m_showDatasetLines = on;
  m_modified = true;

  qApp->postEvent(this, new QEvent(RedrawEvent));
}

//------------------------------------------------------------------------------

void GeometryPartRepresentation::customEvent(QEvent* ev)
{
  if( (ev->type() == RedrawEvent) && m_modified )
  {
    m_modified = false;

    if( m_showSolid )
    {
      updateSolidPartActor();
    }
    else if( m_showDataset )
    {
      updateDatasetPartActor();
    }
  }

  QObject::customEvent(ev);
}

//------------------------------------------------------------------------------

