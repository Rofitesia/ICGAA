#include "ConvexHull.h"

#include <math.h>
#include <vtkActor.h>
#include <vtkGlyph3D.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkLine.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>

#include <algorithm>
#include <cfloat>
#include <cstdlib>
#include <list>
#include <random>

void ConvexHull::init() {
  _points = std::move(getRandomPointsCoordinate(_pointsNumber));
}

void ConvexHull::generateVertices() {
  std::sort(std::begin(_points), std::end(_points),
            [](const PointCoordinate& lhs, const PointCoordinate& rhs) {
              if (std::abs(lhs._x - rhs._x) < DBL_EPSILON) {
                return std::abs(lhs._y - rhs._y) < DBL_EPSILON || lhs._y < rhs._y;
              } else
                return lhs._x < rhs._x;
            });
  std::list<PointCoordinate> upperList;
  std::list<PointCoordinate> lowerList;
  upperList.push_back(_points[0]);
  upperList.push_back(_points[1]);
  for (size_t i = 2; i < _points.size(); ++i) {
    upperList.push_back(_points[i]);
    while (upperList.size() > 2) {
      auto p3 = std::prev(std::end(upperList));
      auto p2 = std::prev(p3);
      auto p1 = std::prev(p2);
      if (checkTurnRight(*p1, *p2, *p3)) {
        break;
      } else {
        std::cout<<"[erase in upperlist]: "<<p2->_x<<", "<<p2->_y<<std::endl;
        upperList.erase(p2);
      }
    }
  }
  std::cout<<"====================="<<std::endl;
  for (auto& point : upperList) {
    std::cout << "upperlist point: " << point._x << ", " << point._y << std::endl;
  }
  lowerList.push_back(_points[_points.size() - 1]);
  lowerList.push_back(_points[_points.size() - 2]);
  for (int i = _points.size() -3; i >= 0; --i) {
    lowerList.push_back(_points[i]);
    while (lowerList.size() > 2) {
      auto p3 = std::prev(std::end(lowerList));
      auto p2 = std::prev(p3);
      auto p1 = std::prev(p2);
      if (checkTurnRight(*p1, *p2, *p3)) {
        break;
      } else {
        std::cout<<"[erase in lowerlist]: "<<p2->_x<<", "<<p2->_y<<std::endl;
        lowerList.erase(p2);
      }
    }
  }
  lowerList.pop_back();
  lowerList.pop_front();
  std::cout<<"====================="<<std::endl;
  for (auto& point : lowerList) {
    std::cout << "lowerList point: " << point._x << ", " << point._y << std::endl;
  }

  _vertices.reserve(lowerList.size() + upperList.size());
  _vertices.assign(std::make_move_iterator(std::begin(upperList)),
                   std::make_move_iterator(std::end(upperList)));
  _vertices.insert(_vertices.end(),std::make_move_iterator(std::begin(lowerList)),
            std::make_move_iterator(std::end(lowerList)));
  // std::copy(std::make_move_iterator(std::begin(lowerList)),
            // std::make_move_iterator(std::end(lowerList)), _vertices.end());
}

void ConvexHull::display() {
  vtkNew<vtkPoints> points;
  vtkNew<vtkPoints> vertices;
  // create random points coordinates
  std::cout<<"====================="<<std::endl;
  for (auto& corrdinate : _points) {
    std::cout << "point: " << corrdinate._x << ", " << corrdinate._y << std::endl;
    points->InsertNextPoint(corrdinate._x, corrdinate._y, 0);
  }
  std::cout<<"====================="<<std::endl;
  for (auto& vertex : _vertices) {
    std::cout << "vertex: " << vertex._x << ", " << vertex._y << std::endl;

    vertices->InsertNextPoint(vertex._x, vertex._y, 0);
  }
  // create line source
  vtkNew<vtkCellArray> lines;
  for (size_t i = 0; i < _vertices.size(); ++i) {
    vtkNew<vtkLine> line;
    line->GetPointIds()->SetId(0, i);
    line->GetPointIds()->SetId(1, (i + 1) % _vertices.size());
    lines->InsertNextCell(std::move(line));
  }
  vtkNew<vtkLine> line;
  // create poly data for points and lines
  vtkNew<vtkPolyData> pointsPolyData;
  vtkNew<vtkPolyData> linesPolyData;
  pointsPolyData->SetPoints(points);
  linesPolyData->SetPoints(vertices);
  linesPolyData->SetLines(lines);

  vtkNew<vtkSphereSource> sphereSource;
  sphereSource->SetRadius(1);
  vtkNew<vtkGlyph3D> glyph;
  glyph->SetInputData(pointsPolyData);
  glyph->SetSourceConnection(sphereSource->GetOutputPort());

  vtkNew<vtkPolyDataMapper> pointsMapper;
  pointsMapper->SetInputConnection(glyph->GetOutputPort());

  vtkNew<vtkPolyDataMapper> linesMapper;
  linesMapper->SetInputData(linesPolyData);

  vtkNew<vtkActor> pointsActor;
  pointsActor->SetMapper(pointsMapper);

  vtkNew<vtkActor> linesActor;
  linesActor->SetMapper(linesMapper);

  vtkNew<vtkRenderer> renderer;
  renderer->AddActor(pointsActor);
  renderer->AddActor(linesActor);

  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  renderWindow->SetSize(1800, 1000);

  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renderWindow);

  vtkNew<vtkInteractorStyleTrackballCamera> style;
  iren->SetInteractorStyle(style);

  iren->Initialize();
  iren->Start();
}

double ConvexHull::getRandomDouble(double lowBound, double topBound) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  if (std::abs(topBound - lowBound) < DBL_EPSILON || topBound < lowBound) {
    topBound = lowBound + 100;
  }
  std::uniform_real_distribution<double> dis(lowBound, topBound);
  return dis(gen);
}

std::vector<PointCoordinate> ConvexHull::getRandomPointsCoordinate(int n) {
  std::vector<PointCoordinate> result;
  result.reserve(n);
  for (int i = 0; i < n; ++i) {
    result.push_back({getRandomDouble(), getRandomDouble()});
  }
  return result;
}

bool ConvexHull::checkTurnRight(const PointCoordinate& p1, const PointCoordinate& p2,
                                const PointCoordinate& p3) {
  double deltaX1 = p2._x - p1._x;
  double deltaX2 = p3._x - p2._x;
  double deltaY1 = p2._y - p1._y;
  double deltaY2 = p3._y - p2._y;
  // if (std::abs(deltaX1) < DBL_EPSILON) deltaX1 = 0;
  // if (std::abs(deltaX2) < DBL_EPSILON) deltaX2 = 0;
  // if (std::abs(deltaY1) < DBL_EPSILON) deltaY1 = 0;
  // if (std::abs(deltaY2) < DBL_EPSILON) deltaY2 = 0;
  double slop1 = deltaY1 / deltaX1;
  double slop2 = deltaY2 / deltaX2;
  return !(std::abs(slop1 - slop2) < DBL_EPSILON || slop1 < slop2);
}