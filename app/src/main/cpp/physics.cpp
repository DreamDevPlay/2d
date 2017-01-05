#include "physics.h"

#include "debug.h"

Physics::Physics()
{

}

void error()
{

}

static const double e = 0.0;
void Physics::calc(Circle &player, std::vector<Circle> &phys, const std::vector<Line> &lines)
{
    for(std::vector<Circle>::iterator iph = phys.begin();
        iph < phys.end(); iph++){
        int i = 0;
        for (auto l = lines.cbegin(); l < lines.cend(); ++l)
            if (lineCircleIntersect(*l, *iph))
            {
                Point p = pointToLineDistance(iph->pos, *l);
                Point v = iph->pos - p;
                setLen(v, iph->r+e);
                // вычисляем новую позицию
                iph->pos = v + p;


//                if (dist(l->begin, iph->pos) < iph->r)
//                {
//                    iph->pos = l->begin + (iph->pos - l->begin)*((iph->r+e)/dist(l->begin, iph->pos));
//                    l = lines.cbegin();
//                    if (i < 100) l = lines.cbegin();
//                    else error();
//                    i++;
//                }
//                else if (dist(l->end, iph->pos) < iph->r)
//                {
//                    iph->pos = l->end + (iph->pos - l->end)*((iph->r+e)/dist(l->end, iph->pos));
//                    if (i < 100) l = lines.cbegin();
//                    else error();
//                    i++;
//                }
//                else
//                {
//                    // вычисляем координаты перпендикуляра из новой позиции к линии
//                    Point p = linePerpendicular(*l, iph->pos);
//                    // вектор перепендикуляра
//                    Point v = iph->pos - p;
//                    // делаем длину вектора равной радиусу круга
//                    setLen(v, iph->r);
//                    // вычисляем новую позицию
//                    iph->pos = v + p;
//                }
            }
    }
    int i = 0;
    for (auto l = lines.cbegin(); l < lines.cend(); ++l)
        if (lineCircleIntersect(*l, Circle(player.pos.x, player.pos.y, player.r)))
        {
            Point p = pointToLineDistance(player.pos, *l);
            Point v = player.pos - p;
            setLen(v, player.r+e);
            // вычисляем новую позицию
            player.pos = v + p;

//            if (dist(l->begin, player.pos) < player.r)
//            {
//                player.pos = l->begin + (player.pos - l->begin)*((player.r+e)/dist(l->begin, player.pos));
//                l = lines.cbegin();
//                if (i < 100) l = lines.cbegin();
//                else error();
//                i++;
//            }
//            else if (dist(l->end, player.pos) < player.r)
//            {
//                player.pos = l->end + (player.pos - l->end)*((player.r+e)/dist(l->end, player.pos));
//                if (i < 100) l = lines.cbegin();
//                else error();
//                i++;
//            }
//            else
//            {
//                // вычисляем координаты перпендикуляра из новой позиции к линии
//                Point p = linePerpendicular(*l, player.pos);
//                // вектор перепендикуляра
//                Point v = player.pos - p;
//                // делаем длину вектора равной радиусу круга
//                setLen(v, player.r);
//                // вычисляем новую позицию
//                player.pos = v + p;
//            }
        }
}

