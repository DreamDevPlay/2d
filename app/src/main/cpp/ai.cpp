#include "ai.h"

#include <algorithm>

#include "debug.h"

AI::AI()
{

}

bool linesIntersect(const Line &line, const std::vector<Line>& lines)
{
    for (auto l: lines) {
        if (lineIntersect(line, l)) return true;
    }
    return false;
}

void AI::nextStep(const Circle &player, std::vector<Circle> &ai, /*const*/ std::vector<Line>& lines)
{
    std::sort(ai.begin(), ai.end(), player);
//    Line l1(Point(1,1), Point(4,1));
//    Line l2(Point(1,-1), Point(4,-1));
//    bool intersect;
//    Point p = lineIntersect(l1,l2,intersect);
//    debug() << intersect << p.x << p.y << p.infinite();

//    qDebug() << ai.size();
    int i = 0;
    for(std::vector<Circle>::iterator it1 = ai.begin();
        it1 < ai.end(); it1++){
        i++;
        if (it1->v > 0){
            Point frombig(0,0);
            const double distance = 100, sqdistance = distance * distance;
            int n = 0;
            double p = -distance, em = 0., dn;
            it1->db = distance;
            //            std::map<double, std::vector<Circle>::const_iterator> bigs;
            for (std::vector<Circle>::const_iterator itb = it1+1;
                 itb < ai.end(); itb++){
                if (it1->r < itb->r && sqdist(*it1, *itb) < sqdistance && !linesIntersect(Line(it1->pos, itb->pos), lines)){
                    n++;
                    dn = dist(*it1, *itb);
//                    if (dn < 0) qDebug() << dn;
                    if (dn < it1->db) {
                        it1->db = dn;
                        it1->near_big_pos = itb->pos;
                        it1->near_big_r = itb->r;
                    }
//                    double m = (distance-dn)*(distance-dn)/(1.+itb->r-it1->r);
//                    double m = (distance-dn)*(distance-dn);
                    double m = pow(distance-dn, 20.0);
                    frombig += itb->pos*m;
                    em += m;
                    if ( -dn > p) p =  -dn;
                }
            }

            if (n > 0){  // если больших нет то линии не учитываются
                for (auto l: lines) {
                    Point pp = linePerpendicular(l, it1->pos);
                    double ll = dist(l.begin, l.end);
                    ll = ll * ll;
                    if (sqdist(pp, l.begin) < ll && sqdist(pp, l.end) < ll &&
                            sqdist(it1->pos, pp) < sqdistance /*&& !linesIntersect(Line(it1->pos, pp), lines)*/)
                    {
                        n++;
                        dn = dist(it1->pos, pp);
                        //                    qDebug() << dn;
                        //                    double m = (distance-dn)*(distance-dn)/(1.+itb->r-it1->r);
                        //                    double m = (distance-dn)*(distance-dn);
                        double m = pow(distance-dn, 10.0);
                        frombig += pp*m;
                        em += m;
                        if ( -dn > p) p =  -dn;
                    }
                }
            }

            double toplayer = dist(*it1, player);
            if (toplayer < distance){
                n++;
//                frombig = frombig + ((it1->pos - player.pos)/(toplayer*toplayer));
                frombig += player.pos*pow(distance-toplayer, 2.0);
            }
            if (em > 0) frombig = frombig/em;
            if (n) {
                frombig = (it1->pos-frombig)*pow(1.5,p)/sqdist(it1->pos, frombig);
            }

            // поиск ближайшего маленького - в какую ссторону двигаться
            std::vector<Circle>::const_iterator nearsmall=it1;
            double d = 1000000;
            for (std::vector<Circle>::iterator its = ai.begin();
                 its < it1; its++){
                if (its->near_big_r == it1->r &&
                    its->near_big_pos == it1->pos)
                { // если текущий является ближайшим большим у малекого то поиск окончен
                    nearsmall = its;
//                    its->b = ai.end();
                    its->near_big_r = -1.0;
                    its = it1;
//                    qDebug() << 333;
                }
                else if (it1->r > its->r){
                    dn = sqdist(*it1, *its);
                    if (dn < d && its->db < sqrt(dn)  && !linesIntersect(Line(it1->pos, its->pos), lines)){
                        d = dn;
                        nearsmall = its;
                    }
                }
            }

            Circle nsc = *nearsmall;
            Point tosmall(0,0);
            if (nearsmall != it1 && nearsmall->r < it1->r){
                tosmall = nsc.pos - it1->pos;
                double tsl = len(tosmall);
                if (tsl == 0) tsl = 1.;
                tosmall = tosmall/(tsl*tsl);
                d = dist(*it1, *nearsmall);
            }
//            Point dir;
            Point dir = frombig + tosmall;
//            qDebug() << n << pow(1.5,p) << frombig.x << frombig.y << tosmall.x << tosmall.y << it1->dir.x << it1->dir.y;
//            double dl = len(it1->dir);
            if (it1->v){
                if (!(dir == 0))
                {
                    setLen(dir, it1->v);
                    //складываем вектор движения и вектор движения от линии
                    it1->dir = it1->dir + dir;
                    setLen(it1->dir, it1->v);
//                    qDebug() << frombig.x << frombig.y << tosmall.x << tosmall.y;
                }
                else if (it1->dir == 0)
                {
                    it1->dir = Point(1-rand()%3,1-rand()%3);
                    setLen(it1->dir, it1->v);
//                    qDebug() << 321;
                }
                Point newpos = it1->pos+it1->dir;
                Point point(0,0);
                double toLineDist = 1e50;
                for (auto l : lines)
                    if (lineCircleIntersect(l, Circle(newpos.x, newpos.y, it1->r))){
                        Point p = pointToLineDistance(newpos, l);
                        double d = sqdist(newpos, p);
                        if (d < toLineDist){
                            point = p;
                            toLineDist = d;
                        }
                    }

                // TODO круги без скорости не пересекались с линиями
                // TODO пересечение траектории и линий (маленькие круги проходят сквозь линии т.к. у них большая скорость)
                // TODO обход концов линий
                if (toLineDist >= 1e50)
                {
                    it1->pos = newpos;

                    debug()  << 321;
                }
                else {
                    // вектор перепендикуляра
                    Point v = it1->pos - point;
                    // длина равна длине вектора движения
                    double dl = len(it1->dir);
                    setLen(v, dl);
                    //складываем вектор движения и вектор движения от линии
                    it1->dir = it1->dir + v;
                    setLen(it1->dir, dl);

                    newpos = it1->pos + it1->dir;
                    //                        if (l != lines.cbegin()) l = lines.cbegin();

                    lines.push_back(Line(point, newpos));

                    debug()  << 123;

                }

//                if (ls.size() == 2 && lineCircleIntersect(ls.at(1), Circle(newpos.x, newpos.y, it1->r)))
//                {
//                    qDebug() << 321 << it1->r << it1->dir.x << it1->dir.y;
//                    // вычисляем координаты перпендикуляра из новой позиции к линии
//                    Point p = linePerpendicular(ls.at(0), newpos);
//                    // вектор перепендикуляра
//                    Point v = newpos - p;
//                    // делаем длину вектора равной радиусу круга
//                    v = v*((it1->r)/len(v));
//                    // вычисляем новую позицию
//                    newpos = v + p;
//                }
                it1->pos = newpos;
            }
        }
    }

    Point newpos = player.pos+player.dir;
    Point point(0,0);
    double toLineDist = 1e50;
    for (auto l : lines)
        if (lineCircleIntersect(l, Circle(newpos.x, newpos.y, player.r))){
            Point p = pointToLineDistance(newpos, l);
            double d = sqdist(newpos, p);
            if (d < toLineDist){
                point = p;
                toLineDist = d;
            }
        }

    //            qDebug() << i << it1->r << prev_pos.x << prev_pos.y << it1->pos.x << it1->pos.y;
}

