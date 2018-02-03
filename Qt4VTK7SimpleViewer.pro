#-------------------------------------------------------------------------------
#
# Project created by QtCreator 2017-01-09T12:30:07
#
# Copyright 2017 Edson Contreras
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.

#-------------------------------------------------------------------------------

QT       += core gui opengl widgets

TARGET = QtVTKViewer
TEMPLATE = app

#Note: Even though the program has the c++11 flag forced,
#      it is not necessary to build Qt4 with c++11 support
#QMAKE_CXXFLAGS += -std=c++11
#QMAKE_CXXFLAGS += /std:c++14


SOURCES += \
  ./src/main.cpp\
  ./src/MainWindow.cpp \
  ./src/PlotHD.cpp \
  ./src/Geometry.cpp \
  ./src/MyVTKApplication.cpp \
  ./src/AboutDialog.cpp \
  ./src/GeometryPartRepresentation.cpp \
  ./src/GeometryPart.cpp \
  ./src/GeometryFactory.cpp

HEADERS  += \
  ./src/MainWindow.h \
  ./src/PlotHD.h \
  ./src/Geometry.h \
  ./src/MyVTKApplication.h \
  ./src/AboutDialog.h \
  ./src/GeometryPartRepresentation.h \
  ./src/GeometryPart.h \
  ./src/GeometryFactory.h

FORMS    += \
  ./src/ui/MainWindow.ui \
  ./src/ui/AboutDialog.ui

INCLUDEPATH += \
  $$(VTK_8_INCLUDE_PATH)

LIBS += \
  -L$$(VTK_8_LIBRARY_PATH) \
  -lvtkCommonColor-8.1 \
  -lvtkCommonCore-8.1 \
  -lvtkCommonComputationalGeometry-8.1 \
  -lvtkCommonDataModel-8.1 \
  -lvtkCommonExecutionModel-8.1 \
  -lvtkCommonMath-8.1 \
  -lvtkCommonMisc-8.1 \
  -lvtkCommonSystem-8.1 \
  -lvtkCommonTransforms-8.1 \
  -lvtkDICOMParser-8.1 \
  -lvtkFiltersCore-8.1 \
  -lvtkFiltersGeneral-8.1 \
  -lvtkFiltersGeometry-8.1 \
  -lvtkFiltersModeling-8.1 \
#  -lvtkFiltersExtraction-8.1 \
  -lvtkFiltersSources-8.1 \
  -lvtkFiltersStatistics-8.1 \
  -lvtkGUISupportQt-8.1 \
  -lvtkGUISupportQtOpenGL-8.1 \
  -lvtkIOImage-8.1 \
  -lvtkImagingCore-8.1 \
  -lvtkImagingFourier-8.1 \
  -lvtkInteractionStyle-8.1 \
  -lvtkRenderingCore-8.1 \
  -lvtkRenderingOpenGL2-8.1 \
  -lvtkRenderingQt-8.1 \
  -lvtkalglib-8.1 \
  -lvtkglew-8.1 \
  -lvtkjpeg-8.1 \
  -lvtkmetaio-8.1 \
  -lvtkpng-8.1 \
  -lvtksys-8.1 \
  -lvtktiff-8.1 \
  -lvtkzlib-8.1
