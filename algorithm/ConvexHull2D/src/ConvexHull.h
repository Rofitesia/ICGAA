#pragma once
#include <iostream>
#include <vector>
struct PointCoordinate {
  double _x = 0;
  double _y = 0;
};

struct LineInfo {
  size_t pointIndex1;
  size_t pointIndex2;
};

class ConvexHull {
 public:
  ConvexHull(int pointsNumber=100) : _pointsNumber(pointsNumber) {}
  void generateVertices();
  void display();
  void init();

 private:
  double getRandomDouble(double lowBound = -50.0, double topBound = 50.0);
  bool checkTurnRight(const PointCoordinate& p1, const PointCoordinate& p2,
                      const PointCoordinate& p3);
  std::vector<PointCoordinate> getRandomPointsCoordinate(int n);


  std::vector<PointCoordinate> _points;
  std::vector<PointCoordinate> _vertices;
  int _pointsNumber{0};
};