#include <cmath>
#include "data.h"
#include "debug.h"

Point &Point::operator*(const Point &other)
{
    int nx = other.x*y - x*other.y;
    int ny = other.y*x - y*other.x;
    x = nx;
    y = ny;
    return *this;
}

void Point::operator +=(const Point &other)
{
    x += other.x;
    y += other.y;
}

bool Point::operator ==(const double value)
{
    return (x==value && y==value);
}

bool Point::operator ==(const Point &other)
{
    return (x==other.x) && (y==other.y);
}

bool Point::infinite()
{
    return (x == std::numeric_limits<double>::infinity()) || (y == std::numeric_limits<double>::infinity());
}

Point Point::operator*(double value) const
{
    return Point(x*value, y*value);
}

Point Point::operator/(double value) const
{
    return Point(x/value, y/value);
}

void Point::operator=(const Point &other)
{
//    qDebug() << x << y << "=" << other.x << other.y;
    x = other.x;
    y = other.y;
}


void Circle::setR(double r)
{
//    Q_ASSERT(r);
    this->r = r;
    double s = 0.25 * r;
    if (r>1) v = 1./s;
    else v = 0;
}

bool Circle::operator<(const Circle &second)
{
    return this->r < second.r;
}

bool Circle::operator()(const Circle &first, const Circle &second)
{
    return first.r < second.r;
}

bool lineCircleIntersect(const Line &line, const Circle &circle)
{
    double x1 = line.begin.x - circle.pos.x;
    double y1 = line.begin.y - circle.pos.y;
    double x2 = line.end.x - circle.pos.x;
    double y2 = line.end.y - circle.pos.y;

    double dx = x2 - x1;
    double dy = y2 - y1;

    //составляем коэффициенты квадратного уравнения на пересечение прямой и окружности.
    //если на отрезке [0..1] есть отрицательные значения, значит отрезок пересекает окружность
    double a = dx*dx + dy*dy;
    double b = 2.*(x1*dx + y1*dy);
    double c = x1*x1 + y1*y1 - circle.r*circle.r;

    //а теперь проверяем, есть ли на отрезке [0..1] решения
    if (-b < 0)
        return (c < 0);
    if (-b < (2.*a))
        return ((4.*a*c - b*b) < 0);

    return (a+b+c < 0);
}

double dist(std::vector<Circle>::const_iterator one, std::vector<Circle>::const_iterator two)
{
    return dist(one->pos, two->pos)-one->r-two->r;
}

double dist(const Circle &one, const Circle &two)
{
    return dist(one.pos, two.pos)-one.r-two.r;
}

double dist(const Point &one, const Point &two)
{
    return sqrt((one.x-two.x)*(one.x-two.x) + (one.y-two.y)*(one.y-two.y));
}


Point operator+(const Point &one, const Point &two)
{
    Point  r;
    r.x = one.x+two.x;
    r.y = one.y+two.y;
    return r;
}


Point operator-(const Point &one, const Point &two)
{
    Point  r;
    r.x = one.x-two.x;
    r.y = one.y-two.y;
    return r;
}

double len(const Point &point)
{
    return sqrt(point.x*point.x+point.y*point.y);
}

void setLen(Point &p, double l){
     p = p * (l/len(p));
}


double sqdist(const Point &one, const Point &two)
{
    return (one.x-two.x)*(one.x-two.x) + (one.y-two.y)*(one.y-two.y);
}

Point pointToLineDistance(const Point &point, const Line &line){
    double d0t = dot(line.end-line.begin, point-line.begin);
//    debug() << "dot" << d0t << point.x << point.y << sqdist(line.end,line.begin);
    if (d0t < 0) return line.begin;
    else {
        if (d0t < sqdist(line.end,line.begin)) return linePerpendicular(line, point);
        else return line.end;
    }
}


double dot(const Point &one, const Point &two)
{
    return one.x*two.x + one.y*two.y;
}


Point linePerpendicular(const Line &line, const Point &point)
{
    return line.begin + (line.end-line.begin)*dot(line.end-line.begin, point-line.begin)/sqdist(line.end, line.begin);
}

const double EPS = 1E-9;
inline double min(double a, double b)
{
    return a < b ? a : b;
}

inline double max(double a, double b)
{
    return a < b ? b : a;
}

inline void swap(double a, double b)
{
    double c = a;
    a = b;
    b = c;
}

inline double det (double a, double b, double c, double d) {
    return a * d - b * c;
}

inline bool between (double a, double b, double c) {
    return min(a,b) <= c + EPS && c <= max(a,b) + EPS;
}

inline bool intersect_1 (double a, double b, double c, double d) {
    if (a > b)  swap (a, b);
    if (c > d)  swap (c, d);
    return max(a,c) <= min(b,d);
}

bool lineIntersect(const Line &one, const Line &two)
{
    double A1 = one.begin.y-one.end.y,  B1 = one.end.x-one.begin.x,  C1 = -A1*one.begin.x - B1*one.begin.y;
    double A2 = two.begin.y-two.end.y,  B2 = two.end.x-two.begin.x,  C2 = -A2*two.begin.x - B2*two.begin.y;
    double zn = det (A1, B1, A2, B2);
    if (zn != 0) {
        double x = - det (C1, B1, C2, B2) * 1. / zn;
        double y = - det (A1, C1, A2, C2) * 1. / zn;
        return between (one.begin.x, one.end.x, x) && between (one.begin.y, one.end.y, y)
                && between (two.begin.x, two.end.x, x) && between (two.begin.y, two.end.y, y);
    }
    else
        return det (A1, C1, A2, C2) == 0 && det (B1, C1, B2, C2) == 0
                && intersect_1 (one.begin.x, one.end.x, two.begin.x, two.end.x)
                && intersect_1 (one.begin.y, one.end.y, two.begin.y, two.end.y);
}

Point lineIntersectPoint(const Line &one, const Line &two)
{
    Point p(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
    double A1 = one.begin.y-one.end.y,  B1 = one.end.x-one.begin.x,  C1 = -A1*one.begin.x - B1*one.begin.y;
    double A2 = two.begin.y-two.end.y,  B2 = two.end.x-two.begin.x,  C2 = -A2*two.begin.x - B2*two.begin.y;
    double zn = det (A1, B1, A2, B2);
    if (zn != 0) {
        p.x = - det (C1, B1, C2, B2) * 1. / zn;
        p.y = - det (A1, C1, A2, C2) * 1. / zn;
    }
    return p;
}


double anglecos(const Point &one, const Point &two)
{
    return dot(one,two)/(len(one)*len(two));
}


double sqdist(std::vector<Circle>::const_iterator one, std::vector<Circle>::const_iterator two)
{
    return sqdist(one->pos, two->pos);
}

double sqdist(std::vector<Circle>::iterator one, std::vector<Circle>::iterator two)
{
    return sqdist(one->pos, two->pos);
}


std::ostream &operator <<(std::ostream &o, const Point &p)
{
    o.write(reinterpret_cast<const char*>(&p.x), sizeof(p.x));
    o.write(reinterpret_cast<const char*>(&p.y), sizeof(p.y));
    return o;
}

std::ostream &operator <<(std::ostream &o, const Line &l)
{
    o << l.begin << l.end;
    return o;
}

std::ostream &operator <<(std::ostream &o, const Circle &c)
{
    o << c.pos << c.dir;// << c.r << c.v;
    o.write(reinterpret_cast<const char*>(&c.r), sizeof(c.r));
    o.write(reinterpret_cast<const  char*>(&c.v), sizeof(c.v));
    return o;
}

std::istream &operator >>(std::istream &i, Point &p)
{
    i.read(reinterpret_cast<char*>(&p.x), sizeof(p.x));
    i.read(reinterpret_cast<char*>(&p.y), sizeof(p.y));
    return i;
}


std::istream &operator >>(std::istream &i, Line &l)
{
    i >> l.begin >> l.end;
    return i;
}


std::istream &operator >>(std::istream &i, Circle &c)
{
    i >> c.pos >> c.dir;// >> c.r >> c.v;
    i.read(reinterpret_cast<char*>(&c.r), sizeof(c.r));
    i.read(reinterpret_cast<char*>(&c.v), sizeof(c.v));
    return i;
}

double qwerty(const std::vector<Point> &p)
{
    return 0;
}

double sqdist(const Circle &one, const Circle &two)
{
    return (one.pos.x-two.pos.x)*(one.pos.x-two.pos.x) + (one.pos.y-two.pos.y)*(one.pos.y-two.pos.y);
}
