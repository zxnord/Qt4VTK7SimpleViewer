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

#include "MainWindow.h"
#include "ui_MainWindow.h"

//C++ Includes
#include <iostream>

//Qt Includes
#include <QHBoxLayout>

//VTK Includes
#include <vtkDebugLeaks.h>

//Project Includes
#include "AboutDialog.h"
#include "Geometry.h"
#include "GeometryFactory.h"
#include "PlotHD.h"

MainWindow* MainWindow::m_winInstance = nullptr;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);

  new QHBoxLayout(m_ui->m_plotsWidget);

  connect(
    m_ui->m_addBtn, SIGNAL(pressed()),
    this,           SLOT(addPlot()));

  connect(
    m_ui->m_removeBtn, SIGNAL(pressed()),
    this,              SLOT(removePlot()));

  connect(
    m_ui->m_addGeomBtn, SIGNAL(pressed()),
    this,               SLOT(addGeometry()));

  connect(
    m_ui->m_removeGeomBtn, SIGNAL(pressed()),
    this,                  SLOT(removeGeometry()));

  connect(
    m_ui->action_About, SIGNAL(triggered(bool)),
    this,               SLOT(showAboutDialog()));

  addGeometry();

  addPlot();
}

MainWindow::~MainWindow()
{
  delete m_ui;

//  qDeleteAll(m_plotList);
}

void MainWindow::addPlot()
{
  m_plotList.append(new PlotHD(m_ui->m_plotsWidget));

  m_ui->m_plotsWidget->layout()->addWidget(m_plotList.last());

  if( m_geomList.isEmpty() )
  {
    addGeometry();
  }

  m_plotList.last()->addGeometry(m_geomList.last());

//  vtkDebugLeaks::PrintCurrentLeaks();
}

void MainWindow::removePlot()
{
  if( m_plotList.isEmpty() )
  {
    std::cout << "No more plots left!!!" << std::endl;
    return;
  }

  m_plotList.last()->deleteLater();
  m_plotList.pop_back();

//  vtkDebugLeaks::PrintCurrentLeaks();
}

void MainWindow::addGeometry()
{
  std::unique_ptr<Geometry> geom =
    GeometryFactory::CreateBasicGeometry(GeometryFactory::CUBE_GEOMETRY);

  m_geomList.append(std::shared_ptr<Geometry>(std::move(geom)));

//  vtkDebugLeaks::PrintCurrentLeaks();
}

void MainWindow::removeGeometry()
{
  if( m_geomList.isEmpty() )
  {
    std::cout << "No more geometries left!!!" << std::endl;
    return;
  }

  m_geomList.last().reset();
  m_geomList.pop_back();

  QVector<PlotHD*> newPlotList;

  for( PlotHD* plt : m_plotList )
  {
    if( !plt->checkPlotDeletion() )
    {
      newPlotList << plt;
    }
  }

  m_plotList = newPlotList;

//  vtkDebugLeaks::PrintCurrentLeaks();
}

void MainWindow::customEvent(QEvent* ev)
{
  if( ev->type() == QEvent::User )
  {
    std::cout << "Event post Plot deletion." << std::endl;
    //    vtkDebugLeaks::PrintCurrentLeaks();
  }
}

MainWindow& MainWindow::GetWindowInstance()
{
  if( !m_winInstance )
  {
    m_winInstance = new MainWindow();
  }

  return *m_winInstance;
}

void MainWindow::removeAllPlots()
{
  for( PlotHD* plt : m_plotList )
  {
    delete plt;
  }
  m_plotList.clear();
}

void MainWindow::removeAllGeometries()
{
  for( auto geom : m_geomList )
  {
    geom.reset();
  }

  m_geomList.clear();
}

void MainWindow::showAboutDialog()
{
  AboutDialog* dialog = new AboutDialog();
  dialog->setModal(true);
  dialog->show();
}
