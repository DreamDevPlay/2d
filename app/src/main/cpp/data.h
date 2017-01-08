#ifndef DATA_H
#define DATA_H


#include "vector"
#include "string"
#include "limits"
#include "stdlib.h"
#include "iostream"
#include "fstream"

#define M_PI		3.14159265358979323846

struct Point
{
    double x, y;
    Point():x(0),y(0){}
    Point(double x, double y): x(x), y(y){}
    void  operator=(const Point& other);
    Point operator*(double value) const;
    Point operator/(double value) const;
    Point &operator*(const Point& other);
    void operator +=(const Point& other);
    bool operator ==(const double value);
    bool operator ==(const Point &other);
    bool infinite();
};

struct Line
{
    Point begin, end;
    Line(){}
    Line(const Point &begin, const Point &end): begin(begin), end(end){}
    Line(const Line &other, const Point &end): begin(other.end), end(end){}
};

double len(const Point& point);
double dot(const Point& one, const Point& two);
Point operator+(const Point& one, const Point& two);
Point operator-(const Point& one, const Point& two);

void setLen(Point &p, double l);

struct Circle
{
    Point pos;
    Point dir;
    double r;
    double v;
    double db;
    Point near_big_pos;
    double near_big_r;

    Circle(){}
    Circle(int x, int y, int r): pos(x,y)
    {
        setR(r);
    }
    void setR(double r);
    bool operator<(const Circle& second);
    bool operator()(const Circle& first, const Circle& second);
};

bool lineCircleIntersect(const Line &line, const Circle &circle);
Point linePerpendicular(const Line &line, const Point &point);
bool lineIntersect(const Line &one, const Line &two);
Point lineIntersectPoint(const Line &one, const Line &two);
Point pointToLineDistance(const Point &point, const Line &line);
//double dist(std::vector<Circle>::const_iterator one, std::vector<Circle>::const_iterator two);
double dist(const Circle& one, const Circle& two);
double dist(const Point &one, const Point &two);
//double sqdist(std::vector<Circle>::const_iterator one, std::vector<Circle>::const_iterator two);
//double sqdist(std::vector<Circle>::iterator one, std::vector<Circle>::iterator two);
double qwerty(const std::vector<Point> &p);
double sqdist(const Point &one, const Point &two);
double sqdist(const Circle &one, const Circle &two);
double anglecos(const Point &one, const Point &two);


std::ostream &operator <<(std::ostream &o, const Point &p);
std::ostream &operator <<(std::ostream &o, const Line &l);
std::ostream &operator <<(std::ostream &o, const Circle &c);
std::istream &operator >>(std::istream &i, Point &p);
std::istream &operator >>(std::istream &i, Line &l);
std::istream &operator >>(std::istream &i, Circle &c);
template<typename T> std::ostream &operator <<(std::ostream &o, const std::vector<T> &v)
{
    auto s = v.size();
    o.write(reinterpret_cast<char*>(&s), sizeof(s));
    for (auto t : v) o << t;
    return o;
}
template<typename T> std::istream &operator >>(std::istream &i, std::vector<T> &v)
{
    typename std::vector<T>::size_type s;
    i.read(reinterpret_cast<char*>(&s), sizeof(s));
    v.clear();
    v.reserve(s);
    for(typename std::vector<T>::size_type n = 0;  n < s; n++){
        T t;
        i >> t;
        v.push_back(std::move(t));
    }
    return i;
}


#endif // DATA_H
