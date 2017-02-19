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
#ifndef GEOMETRYPROPERTIESDIALOG_H
#define GEOMETRYPROPERTIESDIALOG_H

#include <QDialog>

#include "PlotHD.h"

//------------------------------------------------------------------------------

namespace Ui {
  class GeometryPropertiesDialog;
}

//------------------------------------------------------------------------------

class GeometryPropertiesDialog : public QDialog
{
  Q_OBJECT

public:
  explicit GeometryPropertiesDialog(QWidget* parent = 0);
  ~GeometryPropertiesDialog();

  void setCurrentGeometryList(std::vector<std::weak_ptr<GeometryRepresentation>>);

protected slots:
  void onColorButtonPressed();
//  void resetToDefault();

private:
  void initConnections();
  void updateUI(std::shared_ptr<GeometryRepresentation>&);
  void updateSolidColorUI(const std::unique_ptr<GeometryPartRepresentation>&);
  void updateDatasetsCombo(std::shared_ptr<Geometry>&);

  std::shared_ptr<GeometryRepresentation>&& getActiveGeometry() const;

  Ui::GeometryPropertiesDialog* m_ui;
  std::vector<std::weak_ptr<GeometryRepresentation>> m_geometriesRepresentation;
  QString m_activeGeometry;
  QString m_activePart;
};

//------------------------------------------------------------------------------

#endif // GEOMETRYPROPERTIESDIALOG_H
