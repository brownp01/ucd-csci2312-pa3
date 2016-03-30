//
// Created by Paul Brown on 2/16/16.
//

#include "Cluster.h"
#include "Exceptions.h"
#include <cassert>
#include <sstream>

using Clustering::Point;

namespace Clustering {

    unsigned int Cluster::__idGenerator = 0;
    const char Cluster::POINT_CLUSTER_ID_DELIM = ':';


    Cluster::Cluster(const Cluster &cluster) :
            __dimensionality(cluster.__dimensionality),
            __size(0), __points(nullptr), centroid(__dimensionality, *this){

        __id = ++__idGenerator;

        if (cluster.__points == nullptr){
            assert (cluster.__size == 0);
            __size = 0;
            __points = nullptr;

        }

        else
            __cpy(cluster.__points);

        __id = cluster.__id;

    }

    Cluster &Cluster::operator=(const Cluster &cluster) {

        if (this == &cluster)
            return *this;

        if (__dimensionality != cluster.__dimensionality)
            throw DimensionalityMismatchEx(__dimensionality, cluster.__dimensionality);

        __del();
        __cpy(cluster.__points);

        __id = cluster.__id;

        return *this;
    }

    Cluster::~Cluster() {

        __del();
        assert (__size == 0);

    }

    void Cluster::add(const Point &point) {

        if (__dimensionality != point.getDims())
            throw DimensionalityMismatchEx(__dimensionality, point.getDims());

        LNodePtr a = new LNode(point, nullptr);

        if (__points == nullptr) {
            __points = a;
            centroid.setValid(false);
        }

        else {
            if (contains(point))
                return;

            centroid.setValid(false);

            LNodePtr curr = __points, nxt = __points->next;
            while (true) {
                if (nxt == nullptr || nxt->point > point) {
                    curr->next = a;
                    a->next = nxt;
                    break;
                }
                else if (__points->point > point) {
                    a->next = curr;
                    curr = a;

                }

                else {
                    curr = nxt;
                    nxt = nxt->next;
                }
            }
        }

        __size++;
    }

    const Point &Cluster::remove(const Point &point) {

        if (__dimensionality != point.getDims())
            throw DimensionalityMismatchEx(__dimensionality, point.getDims());


        LNodePtr prev = nullptr, del = nullptr;

        if (__points->point == point) {
            del = __points;
            __points = __points->next;
            delete del;
            __size--;

        }
        else {
            prev = __points;
            del = __points->next;

            while (del != nullptr) {

                if (del->point == point) {
                    prev->next = del->next;
                    delete del;
                    __size--;

                    break;
                }

                prev = del;
                del = del->next;
            }
        }


        if (!this->contains(point))
            this->centroid.setValid(false);


        return point;
    }

    std::ostream &operator<<(std::ostream &out, const Cluster &cluster) {


        LNodePtr curr = cluster.__points;

        while (curr != nullptr) {

            out << curr->point;
            out << " ";
            out << cluster.POINT_CLUSTER_ID_DELIM;
            out << " ";
            out << cluster.__id << std::endl;

            curr = curr->next;
        }

        return out;
    }

    std::istream &operator>>(std::istream &istream, Cluster &cluster) {

        std::string line;
        while (getline(istream,line)) {
            int d = (int) std::count(line.begin(),
                                     line.end(),
                                     Point::POINT_VALUE_DELIM);
            Point p(d + 1);
            std::stringstream lineStream(line);

            // call to Point::operator>>
            try {
                lineStream >> p;
                cluster.add(p);
            }
            catch (DimensionalityMismatchEx &ex) {
                std::cout << "Caught an exception of mismatched dimensionality: " << ex << std::endl;
                p.rewindIdGen();
            }

        }

        return istream;
    }


    bool operator==(const Cluster &lhs, const Cluster &rhs){

        LNodePtr left = lhs.__points;
        LNodePtr right = rhs.__points;

        bool equal = true;

        while (true){

            if (left == nullptr && right == nullptr)
                break;

            else if (left == nullptr || right == nullptr || left->point != right->point){
                equal = false;
                break;
            }

            else{
                left = left->next;
                right = right->next;
            }
        }

        return equal;
    }

    bool operator!=(const Cluster &lhs, const Cluster &rhs){

        if (lhs.__dimensionality != rhs.__dimensionality)
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.__dimensionality);

        return ! (lhs == rhs);
    }

    Cluster &Cluster::operator+=(const Cluster &rhs) {

        for (int i = 0; i < rhs.__size; i++){

            if (!contains(rhs[i]))
                add(rhs[i]);
        }

        return *this;
    }

    Cluster &Cluster::operator-=(const Cluster &rhs) {

        for (int i = 0; i < rhs.__size; i++){

            if (contains(rhs[i]))
                remove(rhs[i]);
        }

        return *this;
    }

    Cluster &Cluster::operator+=(const Point &rhs) {

        if (__dimensionality != rhs.getDims())
            throw DimensionalityMismatchEx(__dimensionality, rhs.getDims());

        add(Point(rhs));

        return *this;
    }

    Cluster &Cluster::operator-=(const Point &rhs) {

        if (__dimensionality != rhs.getDims())
            throw DimensionalityMismatchEx(__dimensionality, rhs.getDims());

        remove(Point(rhs));

        return *this;
    }

    const Cluster operator+(const Cluster &lhs, const Cluster &rhs) {

        if (lhs.__dimensionality != rhs.__dimensionality)
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.__dimensionality);

        Cluster c1(lhs);

        return c1 += rhs;
    }

    const Cluster operator-(const Cluster &lhs, const Cluster &rhs) {

        Cluster c1(lhs);

        return c1 -= rhs;
    }

    const Cluster operator+(const Cluster &lhs, Point const &rhs) {

        if (lhs.__dimensionality != rhs.getDims())
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.getDims());

        Cluster c1(lhs);

        return c1 += rhs;
    }

    const Cluster operator-(const Cluster &lhs, Point const &rhs) {

        if (lhs.__dimensionality != rhs.getDims())
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.getDims());

        Cluster c1(lhs);

        return c1 -= rhs;
    }

    void Cluster::__del() {

        while (__points != nullptr) {

            LNodePtr curr = __points;
            __points = __points->next;

            delete curr;
            __size--;
        }

    }

    void Cluster::__cpy(LNodePtr pts) {

        if (pts == nullptr)
            return;

        LNodePtr reached = pts;
        LNodePtr curr = new LNode(reached->point, nullptr);
        __points = curr;
        LNodePtr prev = curr;
        reached = reached->next;
        __size++;

        for (; reached != nullptr; reached = reached->next) {
            curr = new LNode(reached->point, nullptr);
            prev->next = curr;
            prev = curr;
            __size++;
        }

    }

    unsigned int Cluster::getSize() const {
        return __size;
    }

    const Point &Cluster::operator[](unsigned int index) const {

        if (__size == 0)
            throw EmptyClusterEx();

        if (__size <= index || index < 0)
            throw OutOfBoundsEx(index, __size);

        LNodePtr curr = __points;

        for (int i = 0; i < index; i++)
            curr = curr->next;

        return curr->point;
    }

    bool Cluster::contains(const Point &point) const {

        LNodePtr curr = __points;

    if (this->__dimensionality != point.getDims())
        throw DimensionalityMismatchEx(curr->point.getDims(), point.getDims());

        while (curr != nullptr){

            if (curr->point == point)
                return true;

            else
                curr = curr->next;

        }

        return false;
    }

    Cluster::Cluster(unsigned int d) :
             __dimensionality(d),
             __size(0),
             __points(nullptr),
             centroid(__dimensionality, *this) {

        __id = ++__idGenerator;
    }

    Cluster::Centroid::Centroid(unsigned int d, const Cluster &c) :
            __dimensions (d), __p(d), __c(c) {

        if (__c.__size == 0) {
            __valid = false;
            toInfinity();
        }


    }

    unsigned int Cluster::getDimensionality() const {

        return __dimensionality;
    }

    unsigned int Cluster::getId() const {

        return __id;
    }

    const Point Cluster::Centroid::get() const {

        return __p;
    }

    void Cluster::Centroid::compute() {

        if (__c.__points == nullptr){
            assert(__c.__size == 0);
            toInfinity();
            __valid = true;
            return;
        }

        LNodePtr curr = __c.__points;
        Point p(__c.__dimensionality);
        unsigned int sizeCheck = 0;

        while( curr != nullptr){
            p += curr->point / __c.getSize();
            curr = curr->next;
            sizeCheck++;
        }
        assert(sizeCheck == __c.getSize());
        set(p);
    }


    //}
    Cluster::Move::Move(const Point &p, Cluster &from, Cluster &to) :
            __p(p), __from(from), __to(to) {

    }

    void Cluster::Move::perform() {

        __to.add(__from.remove(__p));

        __to.centroid.setValid(false);
        __from.centroid.setValid(false);
    }

    bool Cluster::Centroid::isValid() const {

        return __valid;
    }

    void Cluster::Centroid::setValid(bool valid) {

        __valid = valid;
    }

    void Cluster::Centroid::set(const Point &p) {

        __valid = true;
        __p = p;

    }

    bool Cluster::Centroid::equal(const Point &point) const {

        for (unsigned int i = 0; i < __dimensions; i++){
            if (__p[i] != point[i]) {

                return false;
                break;
            }
        }

        return true;

    }

    void Cluster::Centroid::toInfinity() {

        for (int i = 0; i < __dimensions; i++)
            __p[i] = std::numeric_limits<double>::max();

    }

    void Cluster::pickCentroids(unsigned int k, Point **pointArray) {

        if (k == __size){

            for (int i = 0; i < __size; i++)
                *(pointArray[i]) = (*this)[i];
        }
    }
}


