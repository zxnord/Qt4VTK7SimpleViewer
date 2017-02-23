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
#include "GeometryPart.h"

#include <vtkAlgorithmOutput.h>
#include <vtkPassThrough.h>
#include <vtkPolyData.h>
#include <vtkUnstructuredGrid.h>

//------------------------------------------------------------------------------

GeometryPart::GeometryPart(const QString& name)
  :
  m_partName(name),
  m_inputFilter( vtkSmartPointer<vtkPassThrough>::New() )
{
}

//------------------------------------------------------------------------------

GeometryPart::~GeometryPart()
{
}

//------------------------------------------------------------------------------

vtkAlgorithmOutput* GeometryPart::getGeometryPort()
{
  return m_inputFilter->GetOutputPort();
}

//------------------------------------------------------------------------------

vtkDataSet* GeometryPart::getGeometryData()
{
  return vtkDataSet::SafeDownCast( m_inputFilter->GetOutput() );
}

//------------------------------------------------------------------------------

void GeometryPart::setGeometryData(vtkDataSet* data)
{
  if( data )
  {
    if( auto uGrid = vtkUnstructuredGrid::SafeDownCast(data) )
    {
      m_data = vtkSmartPointer<vtkUnstructuredGrid>::New();

      m_data->ShallowCopy(uGrid);
    }
    else if( auto pData = vtkPolyData::SafeDownCast(data) )
    {
      m_data = vtkSmartPointer<vtkPolyData>::New();

      m_data->ShallowCopy(pData);
    }

    updateData();
  }
}

//------------------------------------------------------------------------------

void GeometryPart::setGeometryConnection(vtkAlgorithmOutput* port)
{
  if( m_data )
  {
    m_data = vtkSmartPointer<vtkDataSet>();
  }

  m_inputFilter->SetInputConnection(port);
  m_inputFilter->Update();
}

//------------------------------------------------------------------------------

void GeometryPart::updateData()
{
  if( m_data )
  {
    m_inputFilter->SetInputData(m_data);
    m_inputFilter->Update();
  }
}

//------------------------------------------------------------------------------

const QString& GeometryPart::getPartName() const
{
  return m_partName;
}

//------------------------------------------------------------------------------
