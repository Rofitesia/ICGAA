#include "ConvexHull.h"

int main(int argc,char* argv[]){
    int pointsNumber=0;
    if(argc>1){
        pointsNumber=std::atoi(argv[1]);
    }
    pointsNumber=100;
    ConvexHull convexHull(pointsNumber);
    convexHull.init();
    convexHull.generateVertices();
    convexHull.display();
}