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
#include <QAction>
#include <QFileDialog>
#include <QHBoxLayout>

//VTK Includes
#include <vtkDebugLeaks.h>

//Project Includes
#include "AboutDialog.h"
#include "Geometry.h"
#include "GeometryPropertiesDialog.h"
#include "GeometryFactory.h"
#include "PlotHD.h"

//------------------------------------------------------------------------------

std::unique_ptr<MainWindow> MainWindow::m_winInstance =
  std::unique_ptr<MainWindow>();

//------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget* parent) :
  QMainWindow(parent),
  m_ui(new Ui::MainWindow),
  m_geomPropertiesDialog( std::unique_ptr<GeometryPropertiesDialog>(new
    GeometryPropertiesDialog()) )
{
  m_ui->setupUi(this);

  new QHBoxLayout(m_ui->m_plotsWidget);

  connect(
    m_ui->actionNew_Plot, SIGNAL(triggered(bool)),
    this,                 SLOT(addPlot()));

  connect(
    m_ui->actionFrom_File, SIGNAL(triggered(bool)),
    this,                  SLOT(addGeometryFromFile()) );

//  connect(
//    m_ui->m_removeBtn, SIGNAL(pressed()),
//    this,              SLOT(removePlot()));

  connect(
    m_ui->actionCube, SIGNAL(triggered(bool)),
    this,             SLOT(addBasicGeometry()));

  connect(
    m_ui->actionCube_Atom, SIGNAL(triggered(bool)),
    this,                  SLOT(addBasicGeometry()));

  connect(
    m_ui->actionMolecule, SIGNAL(triggered(bool)),
    this,                 SLOT(addBasicGeometry()));

//  connect(
//    m_ui->m_removeGeomBtn, SIGNAL(pressed()),
//    this,                  SLOT(removeGeometry()));

  connect(
    m_ui->action_About, SIGNAL(triggered(bool)),
    this,               SLOT(showAboutDialog()));

  connect(
    m_ui->m_propertiesBtn, SIGNAL(pressed()),
    this,                  SLOT(showPropertiesDialog()));

  addPlot();
}

//------------------------------------------------------------------------------

MainWindow::~MainWindow()
{
  delete m_ui;

//  qDeleteAll(m_plotList);
}

//------------------------------------------------------------------------------

void MainWindow::addPlot()
{
  m_plotList.append(std::shared_ptr<PlotHD>( new PlotHD() ));

  m_ui->m_plotsWidget->layout()->addWidget(m_plotList.last().get());

  if( m_geomList.empty() )
  {
    m_geomList.push_back(
      GeometryFactory::CreateBasicGeometry(GeometryFactory::CUBE_GEOMETRY) );
  }

  connect(
    m_plotList.last().get(), SIGNAL(unSelectAllPlotsBut(PlotHD*)),
    this,                    SLOT(updateActivePlot(PlotHD*)) );

  m_plotList.last()->addGeometry(m_geomList.back());
  m_plotList.last()->selectPlot();

//  vtkDebugLeaks::PrintCurrentLeaks();
}

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

void MainWindow::addBasicGeometry()
{
  QObject* obj = sender();

  GeometryFactory::BasicGeometries geom =
    GeometryFactory::CUBE_GEOMETRY;

  if( obj == m_ui->actionCube_Atom )
  {
    geom = GeometryFactory::CUBE_ATOM_GEOMETRY;
  }
  else if( obj == m_ui->actionMolecule )
  {
    geom = GeometryFactory::MOLECULE_GEOMETRY;
  }

  m_geomList.push_back(
    GeometryFactory::CreateBasicGeometry( geom ) );
}

//------------------------------------------------------------------------------

void MainWindow::removeGeometry()
{
  if( m_geomList.empty() )
  {
    std::cout << "No more geometries left!!!" << std::endl;
    return;
  }

  m_geomList.back().reset();
  m_geomList.pop_back();

  QVector<std::shared_ptr<PlotHD>> newPlotList;

  for( std::shared_ptr<PlotHD>& plt : m_plotList )
  {
    if( !plt->checkPlotDeletion() )
    {
      newPlotList << plt;
    }
    else
    {
      plt.reset();
    }
  }

  m_plotList = newPlotList;

//  vtkDebugLeaks::PrintCurrentLeaks();
}

//------------------------------------------------------------------------------

void MainWindow::customEvent(QEvent* ev)
{
  if( ev->type() == QEvent::User )
  {
    std::cout << "Event post Plot deletion." << std::endl;
    //    vtkDebugLeaks::PrintCurrentLeaks();
  }
}

//------------------------------------------------------------------------------

std::unique_ptr<MainWindow>& MainWindow::GetWindowInstance()
{
  if( !m_winInstance )
  {
    m_winInstance = std::unique_ptr<MainWindow>( new MainWindow() );
  }

  return m_winInstance;
}

//------------------------------------------------------------------------------

void MainWindow::removeAllPlots()
{
  for( std::shared_ptr<PlotHD>& plt : m_plotList )
  {
    plt.reset();
  }
  m_plotList.clear();
}

//------------------------------------------------------------------------------

void MainWindow::removeAllGeometries()
{
  for( auto& geom : m_geomList )
  {
    geom.reset();
  }

  m_geomList.clear();
}

//------------------------------------------------------------------------------

void MainWindow::showAboutDialog()
{
  AboutDialog* dialog = new AboutDialog();
  dialog->setModal(true);
  dialog->show();
}

//------------------------------------------------------------------------------

void MainWindow::showPropertiesDialog()
{
  if( !m_geomPropertiesDialog )
  {
    m_geomPropertiesDialog = std::unique_ptr<GeometryPropertiesDialog>(
      new GeometryPropertiesDialog());
  }

  m_geomPropertiesDialog->show();
}

//------------------------------------------------------------------------------

void MainWindow::updateActivePlot(PlotHD* activePlt)
{
  for( std::shared_ptr<PlotHD>& plt : m_plotList )
  {
    if( plt.get() != activePlt )
    {
      plt->unSelectPlot();
    }
    else if( m_geomPropertiesDialog )
    {
      m_geomPropertiesDialog->setCurrentGeometryList(
        plt->getRepresentations());
    }
  }
}

//------------------------------------------------------------------------------

void MainWindow::addGeometryFromFile()
{
  QString filename = QFileDialog::getOpenFileName(
    0,
    "Select Geometry File",
    "./",
    "*.mol");

  m_geomList.push_back(
    GeometryFactory::CreateGeometryFromFile( filename ) );

  addPlot();
}

//------------------------------------------------------------------------------
