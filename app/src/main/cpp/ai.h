#ifndef AI_H
#define AI_H

#include <vector>
#include <map>
#include "data.h"

class AI
{
public:
    AI();
    static void nextStep(const Circle &player, std::vector<Circle> &ai, /*const*/ std::vector<Line> &lines);
};

#endif // AI_H
