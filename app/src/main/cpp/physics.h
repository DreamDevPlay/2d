#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>
#include <map>

#include "data.h"

class Physics
{
public:
    Physics();
    static void calc(Circle& player, std::vector<Circle>& phys, const std::vector<Line> &lines);
};

#endif // PHYSICS_H
