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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

#include <memory>

class Geometry;
class GeometryPropertiesDialog;
class PlotHD;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  static std::unique_ptr<MainWindow>& GetWindowInstance();
  virtual ~MainWindow();

  void removeAllPlots();
  void removeAllGeometries();

protected slots:
  void addPlot();
  void removePlot();
  void addGeometry();
  void removeGeometry();
  void showAboutDialog();
  void updateActivePlot(PlotHD*);
  void showPropertiesDialog();

protected:
  MainWindow(QWidget* parent = 0);
  virtual void customEvent(QEvent*);

private:
  static std::unique_ptr<MainWindow> m_winInstance;

  Ui::MainWindow*                    m_ui;
  QVector<std::shared_ptr<Geometry>> m_geomList;
  QVector<std::shared_ptr<PlotHD>>   m_plotList;

  std::unique_ptr<GeometryPropertiesDialog> m_geomPropertiesDialog;
};

#endif // MAINWINDOW_H
