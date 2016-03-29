//
// Created by Paul Brown on 3/17/16.
//
#include "Exceptions.h"

namespace Clustering {

    Clustering::OutOfBoundsEx::OutOfBoundsEx(unsigned int c, int r) {

        __name = "OutOfBoundsEx";
        __current = c;
        __rhs = r;

    }

    unsigned int OutOfBoundsEx::getCurrent() const {

        return __current;
    }

    int OutOfBoundsEx::getRhs() const {

        return __rhs;
    }

    std::string OutOfBoundsEx::getName() const {

        return __name;
    }

    std::ostream &Clustering::operator<<(std::ostream &os, const OutOfBoundsEx &ex) {

        os << "Current size of " << ex.__current << " is out of bounds of size " << ex.__rhs << std::endl;

        return os;
    }

    DimensionalityMismatchEx::DimensionalityMismatchEx(unsigned int c, unsigned int r) {

        __name = "DimensionalityMismatchEx";
        __current = c;
        __rhs = r;

    }

    unsigned int DimensionalityMismatchEx::getCurrent() const {

        return __current;
    }

    unsigned int DimensionalityMismatchEx::getRhs() const {

        return __rhs;
    }

    std::string DimensionalityMismatchEx::getName() const {

        return __name;
    }

    std::ostream &Clustering::operator<<(std::ostream &os, const DimensionalityMismatchEx &ex) {

        os << "The current size of " << ex.__current << " does not match the other size of " << ex.__rhs << std::endl;

        return os;
    }

    ZeroClustersEx::ZeroClustersEx() {

        __name = "ZeroClustersEx";

    }

    std::string ZeroClustersEx::getName() const {

        return __name;
    }

    std::ostream &Clustering::operator<<(std::ostream &os, const ZeroClustersEx &ex) {

        os << "there are zero clusters" << std::endl;

        return os;
    }

    DataFileOpenEx::DataFileOpenEx(std::string filename) {

        __filename = filename;
        __name = "DataFileOpenEx";

    }

    std::string DataFileOpenEx::getFilename() const {

        return __filename;
    }

    std::string DataFileOpenEx::getName() const {

        return __name;
    }

    std::ostream &Clustering::operator<<(std::ostream &os, const DataFileOpenEx &ex) {

        os << "The file " << ex.__filename << "did not open" << std::endl;

        return os;
    }

    ZeroDimensionsEx::ZeroDimensionsEx() {

        __name = "ZeroDimensionsEx";

    }

    std::string ZeroDimensionsEx::getName() const {

        return __name;
    }

    std::ostream &Clustering::operator<<(std::ostream &os, const ZeroDimensionsEx &ex) {

        os << "the dimensions equal zero" << std::endl;

        return os;
    }
}
