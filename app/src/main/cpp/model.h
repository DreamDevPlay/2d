#ifndef MODEL_H
#define MODEL_H

#include "data.h"

class Model
{
    int mapSizeX, mapSizeY;
public:
    Circle mainCircle;
    std::vector<Circle> otherCircles;
    std::vector<Line> lines;
    Model();
    void moveMain(double dx, double dy);
    void moveMainToPoint(double x, double y);
    void nextStep(char* data=0);
    int  twoCircles(Circle& one, Circle& two);
    void init();
    void save(std::string filename);
    void load(std::string filename);
};

template <typename  T>
int valueToData(const T value, char *data, const int &f)
{
    memcpy(data+f, &value, sizeof(T));

    return f + sizeof(T);
}

#endif // MODEL_H
