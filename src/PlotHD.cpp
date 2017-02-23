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

#include "PlotHD.h"

#include <vtkActor.h>
#include <vtkActorCollection.h>
#include <vtkAssignAttribute.h>
#include <vtkAutoInit.h>
#include <vtkBandedPolyDataContourFilter.h>
#include <vtkContourFilter.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkInteractorStyle.h>
#include <vtkTDxInteractorStyleCamera.h>
#include <vtkTDxInteractorStyleSettings.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderingFreeTypeModule.h>
#include <vtkRenderingFreeTypeObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>

#include <QVTKWidget2.h>

#include <QApplication>
#include <QMouseEvent>
#include <QVBoxLayout>

#include "Geometry.h"
#include "GeometryPart.h"
#include "GeometryPartRepresentation.h"
#include "GeometrySettings.h"
#include "MainWindow.h"

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingFreeType)

//------------------------------------------------------------------------------

PlotHD::PlotHD(QWidget *parent)
  :
  QWidget(parent),
  m_selected(false)
{
  QVBoxLayout* lay = new QVBoxLayout(this);
  m_renderWidget = new QVTKWidget2(this);
  m_renderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_renderWidget->installEventFilter(
    new SelectPlotEventFilter(this));

  m_renderer = vtkSmartPointer<vtkRenderer>::New();
  m_renderer->SetBackground(1, .714, .757); // Background color green ffb6c1

  lay->addWidget(m_renderWidget);
  lay->setContentsMargins(0, 0, 0, 0);

  m_renderWidget->GetRenderWindow()->AddRenderer(m_renderer);
}

//------------------------------------------------------------------------------

PlotHD::~PlotHD()
{
}

//------------------------------------------------------------------------------

void PlotHD::addGeometry(const std::unique_ptr<Geometry>& geom)
{
  std::shared_ptr<GeometrySettings> geomRep =
    std::make_shared<GeometrySettings>( geom );
//    std::make_shared<GeometrySettings>( GeometrySettings(geom) );
  geomRep->registerToUpdate(this);

  geomRep->addGeometryActorsToRenderer(m_renderer);

  m_geomSettings.push_back( std::move(geomRep) );
}

//------------------------------------------------------------------------------

bool PlotHD::checkPlotDeletion()
{
//  unsigned int expiredGeoms = 0;

//  for( auto& rep : m_representations )
//  {
//    expiredGeoms += rep->m_geometry.expired()? 1 : 0;
//  }

//  if( m_representations.size() == expiredGeoms )
//  {
//    delete this;
//    //    this->deleteLater();
//    return true;
//  }

  return false;
}

//------------------------------------------------------------------------------

bool PlotHD::isSelected() const
{
  return m_selected;
}

//------------------------------------------------------------------------------

void PlotHD::selectPlot()
{
  if( m_selected )
  {
    return;
  }


  if( !m_selectedPlotActor )
  {
    m_selectedPlotActor = vtkSmartPointer<vtkTextActor>::New();
    m_selectedPlotActor->SetInput( "A" );
    m_selectedPlotActor->SetPosition2(15, 10);
    m_selectedPlotActor->GetTextProperty()->SetFontSize(24);
    m_selectedPlotActor->GetTextProperty()->BoldOn();
    m_selectedPlotActor->GetTextProperty()->SetColor(0.0, 0.0, 0.0);
    m_renderer->AddActor2D(m_selectedPlotActor);
  }

  m_selected = true;
  m_selectedPlotActor->VisibilityOn();

  emit unSelectAllPlotsBut(this);

  m_renderWidget->update();
}

//------------------------------------------------------------------------------

void PlotHD::unSelectPlot()
{
  if( !m_selected )
  {
    return;
  }

  m_selected = false;

  if( m_selectedPlotActor )
  {
    m_selectedPlotActor->VisibilityOff();
  }

  m_renderWidget->update();
}

//------------------------------------------------------------------------------

std::vector<std::weak_ptr<GeometrySettings> > PlotHD::getRepresentations() const
{
  std::vector<std::weak_ptr<GeometrySettings>> weakRepresentations;

  for( auto& rep : m_geomSettings )
  {
    if( rep )
    {
      weakRepresentations.push_back(rep);
    }
  }

  return weakRepresentations;
}

//------------------------------------------------------------------------------

void PlotHD::customEvent(QEvent* ev)
{
  if( ev->type() == GeometrySettings::GeometryUpdated )
  {
    m_renderWidget->update();
    update();
    return;
  }

  return QObject::customEvent(ev);
}

//------------------------------------------------------------------------------
