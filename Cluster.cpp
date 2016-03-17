//
// Created by Paul Brown on 2/16/16.
//

#include "Cluster.h"
#include <cassert>
#include <sstream>

using Clustering::Point;

namespace Clustering {


    Cluster::Cluster(const Cluster &cluster) {

        __size = 0;

        if (cluster.__points == nullptr){
            assert (cluster.__size == 0);
            __size = 0;
            __points = nullptr;

        }

        else
            __cpy(cluster.__points);

    }

    Cluster &Cluster::operator=(const Cluster &cluster) {

        if (this == &cluster)
            return *this;

        __del();
        __cpy(cluster.__points);

        return *this;
    }

    Cluster::~Cluster() {

        __del();
        assert (__size == 0);

    }

    void Cluster::add(const Point &point) {

        LNodePtr a = new LNode(point, nullptr);

        if (__points == nullptr) {
            __points = a;
        }
        else if (__points->point > point) {
            a->next = __points;
            __points = a;
        }
        else {
            LNodePtr curr = __points, nxt = __points->next;
            while (true) {
                if (nxt == nullptr || nxt->point > point) {
                    curr->next = a;
                    a->next = nxt;
                    break;
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

        return point;
    }

    std::ostream &operator<<(std::ostream &out, const Cluster &cluster) {


        LNodePtr curr = cluster.__points;

        while (curr != nullptr) {

            out << curr->point << std::endl;

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
            lineStream >> p;

            cluster.add(p);
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

        add(Point(rhs));

        return *this;
    }

    Cluster &Cluster::operator-=(const Point &rhs) {

        remove(Point(rhs));

        return *this;
    }

    const Cluster operator+(const Cluster &lhs, const Cluster &rhs) {

        Cluster c1(lhs);

        return c1 += rhs;
    }

    const Cluster operator-(const Cluster &lhs, const Cluster &rhs) {

        Cluster c1(lhs);

        return c1 -= rhs;
    }

    const Cluster operator+(const Cluster &lhs, Point const &rhs) {

        Cluster c1(lhs);

        return c1 += rhs;
    }

    const Cluster operator-(const Cluster &lhs, Point const &rhs) {

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

        LNodePtr curr = __points;

        for (int i = 0; i < index; i++)
            curr = curr->next;

        return curr->point;
    }

    bool Cluster::contains(const Point &point) const {

        LNodePtr curr = __points;


        while (curr != nullptr){

            if (curr->point == point)
                return true;

            else
                curr = curr->next;

        }

        return false;
    }
}


