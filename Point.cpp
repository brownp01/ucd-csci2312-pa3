//
// Created by Paul Brown on 2/16/16.
//

#include "Point.h"
#include "Exceptions.h"
#include <cmath>
#include <sstream>


namespace Clustering {


    unsigned int Point::__idGen = 0;
    const char Point::POINT_VALUE_DELIM = ',';

    Point::Point(unsigned int initDim) {

        if (initDim == 0)
            throw ZeroDimensionsEx();

        __id = ++__idGen;
        __dim = initDim;
        __values = new double[__dim];


        for (int i = 0; i < __dim; i++)
            __values[i] = 0;

    }

    Point::Point(unsigned int initDim, double *initValues) : Point(initDim) {

        for (int i = 0; i < initDim; i++)
            __values[i] = initValues[i];

    }

    Point::Point(const Point &point) {

        __id = point.__id;
        __dim = point.__dim;
        __values = new double[__dim];

        for (int i = 0; i < __dim; i++)
            __values[i] = point.__values[i];

    }

    Point &Point::operator=(const Point &point) {

        if (__dim != point.__dim)
            throw DimensionalityMismatchEx(__dim, point.__dim);

        else if (this == &point)
            return *this;

        else {
            delete[] __values;
            __id = point.__id;
            __dim = point.__dim;
            __values = new double[__dim];

            for (int i = 0; i < __dim; i++)
                __values[i] = point.__values[i];

            return *this;
        }
    }

    Point::~Point() {

        delete[] __values;

    }

    void Point::setValue(unsigned int dim, double newValues) {

        if (__dim <= dim || dim < 0)
            throw OutOfBoundsEx(dim, __dim);

        __values[dim] = newValues;
    }

    double Point::getValue(unsigned int dim) const {

        if (__dim <= dim || dim < 0)
            throw OutOfBoundsEx(dim, __dim);

        return __values[dim];
    }

    double Point::distanceTo(const Point &point) const {

        double sum = 0;

        if (__dim != point.__dim)
            throw DimensionalityMismatchEx(__dim, point.__dim);

        for (int i = 0; i < __dim; i++)
            sum += pow(__values[i] - point.__values[i], 2);

        return sqrt(sum);
    }

    Point &Point::operator*=(double d) {

        for (int i = 0; i < __dim; i++)
            __values[i] *= d;

        return *this;
    }

    Point &Point::operator/=(double d) {

        for (int i = 0; i < __dim; i++)
            __values[i] /= d;

        return *this;
    }

    const Point Point::operator*(double d) const {

        for (int i = 0; i < __dim; i++)
            __values[i] *= d;

        return *this;
    }

    const Point Point::operator/(double d) const {

        if (d == 0)
            return *this;

        else {
            for (int i = 0; i < __dim; i++)
                __values[i] /= d;

            return *this;
        }
    }

    Point &operator+=(Point &point, const Point &point1) {

        if (point.__dim != point1.__dim)
            throw DimensionalityMismatchEx(point.__dim, point1.__dim);

        for (int i = 0; i < point.__dim; i++)
            point.__values[i] += point1.__values[i];

        return point;
    }

    Point &operator-=(Point &point, const Point &point1) {

        if (point.__dim != point1.__dim)
            throw DimensionalityMismatchEx(point.__dim, point1.__dim);

        for (int i = 0; i < point.__dim; i++)
            point.__values[i] -= point1.__values[i];

        return point;
    }

    const Point operator+(const Point &point, const Point &point1) {

        Point(p) = point;
        p += point1;

        return p;
    }

    const Point operator-(const Point &point, const Point &point1) {

        Point(p) = point;
        p -= point1;

        return p;
    }

    bool operator==(const Point &point, const Point &point1) {

        if (point.__id != point1.__id)
            return false;

        for (int i = 0; i < point.__dim; i++) {
            if (point.__values[i] != point1.__values[i])
                return false;
        }
        return true;
    }

    bool operator!=(const Point &point, const Point &point1) {

        if (point.__dim != point1.__dim)
            throw DimensionalityMismatchEx(point.__dim, point1.__dim);

        return ! (point == point1);
    }

    bool operator<(const Point &point, const Point &point1) {

        if (point.__dim != point1.__dim)
            throw DimensionalityMismatchEx(point.__dim, point1.__dim);

        for (int i = 0; i < point.__dim; i++) {
            if (point.__values[i] < point1.__values[i])
                return true;
        }
        return false;
    }

    bool operator>(const Point &point, const Point &point1) {

        if (point.__dim != point1.__dim)
            throw DimensionalityMismatchEx(point.__dim, point1.__dim);

        for (int i = 0; i < point.__dim; i++) {
            if (point.__values[i] > point1.__values[i])
                return true;
        }
        return false;
    }

    bool operator<=(const Point &point, const Point &point1) {

        if (point.__dim != point1.__dim)
            throw DimensionalityMismatchEx(point.__dim, point1.__dim);

        for (int i = 0; i < point.__dim; i++) {
            if (point.__values[i] <= point1.__values[i])
                return true;
        }
        return false;
    }

    bool operator>=(const Point &point, const Point &point1) {

        if (point.__dim != point1.__dim)
            throw DimensionalityMismatchEx(point.__dim, point1.__dim);

        for (int i = 0; i < point.__dim; i++) {
            if (point.__values[i] >= point1.__values[i])
                return true;
        }
        return false;
    }

    std::ostream &operator<<(std::ostream &out, const Point &point) {

        int i = 0;
        for ( ; i < point.__dim - 1; i++)
            out << point.__values[i] << ", ";
        out << point.__values[i];

        return out;
    }

    std::istream &operator>>(std::istream &istream, Point &point) {

        int i = 0;
        std::string value;
        int count = 0;

        while (getline(istream, value, point.POINT_VALUE_DELIM)) {
            double d;
            d = stod(value);
            std::cout << "Value: " << d << std::endl;

            if (i < point.__dim)
                point.setValue(i++, d);

            count++;
        }

        if (count != point.__dim)
            throw DimensionalityMismatchEx(i, point.__dim);

        std::cout << "Point: " << point << std::endl;

        return istream;
    }

    double &Point::operator[](unsigned int index) {

        if (__dim <= index || index < 0)
            throw OutOfBoundsEx(index, __dim);
        return __values[index];
    }

    int Point::getId() const {
        return __id;
    }

    unsigned int Point::getDims() const {
        return __dim;
    }
}



