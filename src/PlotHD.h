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

#ifndef PLOTHD_H
#define PLOTHD_H

#include <QWidget>

#include <vtkSmartPointer.h>

#include <memory>

class Geometry;

class QVTKWidget2;
class vtkRenderer;

class GeometryPartRepresentation;

struct GeometryRepresentation
{
  std::weak_ptr<Geometry>                                  m_geometry;
  std::vector<std::unique_ptr<GeometryPartRepresentation>> m_geometryParts;
};

class PlotHD : public QWidget
{
  Q_OBJECT
public:
  explicit PlotHD(QWidget *parent = 0);
  virtual ~PlotHD();

  void addGeometry(std::weak_ptr<Geometry> geom);

  bool checkPlotDeletion();

signals:

public slots:

protected:
  std::vector<std::unique_ptr<GeometryRepresentation>> m_representations;

  QVTKWidget2* m_renderWidget;
  vtkSmartPointer<vtkRenderer> m_renderer;
};

#endif // PLOTHD_H
