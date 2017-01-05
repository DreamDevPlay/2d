#include "model.h"
#include "ai.h"
#include "physics.h"
#include <cmath>

#define DATA_SIZE 4000;

Model::Model(): mapSizeX(1900), mapSizeY(1000),
    mainCircle(50, 50, 20)
{
    mainCircle.v = 2;
    const double d = 10.5;
//    for (int i = 0; i < 200; ++i)
//        otherCircles.push_back(Circle(rand()%int(mapSizeX-2*d) + d, rand()%int(mapSizeY-2*d) + d, double(rand()%5)+1.+10./double(rand())));

//    for (int i = 0; i < 100; ++i)
//        otherCircles.push_back(Circle(rand()%int(mapSizeX-2*d) + d, rand()%int(mapSizeY-2*d) + d, 0.5));

    lines.push_back(Line(Point(d,d), Point(mapSizeX-d,d)));
    lines.push_back(Line(lines.back(), Point(mapSizeX-d,mapSizeY-d)));
    lines.push_back(Line(lines.back(), Point(d,mapSizeY-d)));
    lines.push_back(Line(lines.back(), Point(d,d)));

    lines.push_back(Line(Point(100,100), Point(100,200)));
    lines.push_back(Line(Point(100,200), Point(200,200)));

//    for (int i = 0; i < 40; ++i)
//        otherCircles.push_back(Circle(rand()%mapSizeX + 10, rand()%mapSizeY + 10, 4));

//    otherCircles.push_back(Circle(rand()%mapSizeX + 10, rand()%mapSizeY + 10, 3));
//    otherCircles.push_back(Circle(rand()%mapSizeX + 10, rand()%mapSizeY + 10, 2));
//    otherCircles.push_back(Circle(rand()%mapSizeX + 10, rand()%mapSizeY + 10, 1));
}

void Model::moveMain(double dx, double dy)
{
    Point np = Point(dx, dy) + mainCircle.pos;
    if (np.x > 0 && np.x < mapSizeX) mainCircle.pos.x = np.x;
    if (np.y > 0 && np.y < mapSizeY) mainCircle.pos.y = np.y;
    return;
}

void Model::moveMainToPoint(double x, double y)
{
    Point v = Point(x,y) - mainCircle.pos;
    setLen(v, mainCircle.v);
    moveMain(v.x, v.y);
}

void Model::nextStep(char* data)
{
//    int i = 0;
    AI::nextStep(mainCircle, otherCircles, lines);
    Physics::calc(mainCircle, otherCircles, lines);

//    if (rand()%100 < 10){
//        if (rand()%100 < 5) otherCircles.push_back(Circle(rand()%mapSizeX + 10, rand()%mapSizeY + 10, double(rand()%3)+1.+10./double(rand())));
//        Circle c = Circle(rand()%mapSizeX + 10, rand()%mapSizeY + 10, 1);
//        if (!lineCircleIntersect())
//        otherCircles.push_back(Circle(rand()%mapSizeX + 10, rand()%mapSizeY + 10, 1));
//    }
//    i = 0;
    for (std::vector<Circle>::iterator it = otherCircles.begin();
         it < otherCircles.end(); ) {
//        double dist = sqrt((it->pos.x-mainCircle.pos.x)*(it->pos.x-mainCircle.pos.x) + (it->pos.y-mainCircle.pos.y)*(it->pos.y-mainCircle.pos.y));
        if (mainCircle.r > it->r && dist(it->pos, mainCircle.pos) < fabs((it->r-mainCircle.r)))
        {
            if (*it < mainCircle){
                double bs = M_PI * mainCircle.r * mainCircle.r;
                bs += M_PI * it->r * it->r;
                mainCircle.r = sqrt(bs/M_PI);
                it = otherCircles.erase(it);
            }
            else if (mainCircle < *it){
               init();
            }
        }
        else ++it;
    }
    for (auto it1 = otherCircles.begin(); it1 < otherCircles.end(); ++it1){
            for (std::vector<Circle>::iterator it2 = it1+1;
                 it2 < otherCircles.end(); ){
//                double dist = sqrt((it1->pos.x-it2->pos.x)*(it1->pos.x-it2->pos.x) + (it1->pos.y-it2->pos.y)*(it1->pos.y-it2->pos.y));
                if (dist(it1->pos, it2->pos) < fabs((it1->r-it2->r)))
                {
                    int ret = twoCircles(*it1, *it2);
                    if (ret == 2){
                        it1 = otherCircles.erase(it1);
                        it2 = it1+1;
                    }
                    else if (ret == 1){
                        it2 = otherCircles.erase(it2);
                    }
                    else ++it2;
                }
                else ++it2;
            }
        }

#ifndef Q_OS_WIN
    if (!data) return;

    int f = 0;
    data[0] = sizeof(Point::x);
    data[1] = sizeof(Circle::r);
    data[2] = sizeof(std::size_t);
    f = 3;
    f = valueToData(otherCircles.size()+1, data, f);
    f = valueToData(sizeof(Point::x)+sizeof(Point::y)+sizeof(Circle::r), data, f);
    for (auto c : otherCircles){
        f = valueToData(c.pos.x, data, f);
        f = valueToData(c.pos.y, data, f);
        f = valueToData(c.r, data, f);
    }
    f = valueToData(mainCircle.pos.x, data, f);
    f = valueToData(mainCircle.pos.y, data, f);
    f = valueToData(mainCircle.r, data, f);
    f = valueToData(lines.size()+1, data, f);
    f = valueToData(sizeof(Point::x)+sizeof(Point::y)+sizeof(Point::x)+sizeof(Point::y), data, f);
    for (auto l : lines){
        f = valueToData(l.begin.x, data, f);
        f = valueToData(l.begin.y, data, f);
        f = valueToData(l.end.x, data, f);
        f = valueToData(l.end.y, data, f);
    }
#endif
}

int Model::twoCircles(Circle &one, Circle &two)
{
    if (one < two){
        double bs = M_PI * two.r * two.r;
        bs += M_PI * one.r * one.r;
        two.setR(sqrt(bs/M_PI));
        return 2;
    }
    else if (two < one){
        double bs = M_PI * one.r * one.r;
        bs += M_PI * two.r * two.r;
        one.setR(sqrt(bs/M_PI));
        return 1;
    }

    return 0;
}

void Model::init()
{
    mainCircle = Circle(mapSizeX/2, mapSizeY/2, 5);
    otherCircles.clear();
    for (int i = 0; i < 100; ++i)
        otherCircles.push_back(Circle(rand()%mapSizeX + 10, rand()%mapSizeY + 10, rand()%10+1));
}


void Model::save(std::string filename)
{
    std::ofstream f;
    f.open(filename, std::ios::out | std::ios::trunc | std::ios::binary);

    if (!f.is_open()) return;

    f << mainCircle << otherCircles << lines << mapSizeX << mapSizeY;

    f.close();
}

void Model::load(std::string filename)
{
    std::ifstream f;
    f.open(filename,  std::ifstream::binary | std::ifstream::in);

    if (!f.is_open()) return;

    f >> mainCircle >> otherCircles >> lines >> mapSizeX >> mapSizeY;

    f.close();
}
