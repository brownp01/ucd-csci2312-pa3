//
// Created by Paul Brown on 3/17/16.
//

#include <iosfwd>
#include "KMeans.h"

namespace Clustering {

    Clustering::KMeans::KMeans(unsigned int dim, unsigned int k, std::string filename, unsigned int maxIter) {

        __dimensionality = dim;
        __k = k;
        __iFileName = filename;
        __maxIter = maxIter;



    }

    KMeans::~KMeans() {

    }


    unsigned int KMeans::getMaxIter() {

        return __maxIter;
    }

    unsigned int KMeans::getNumIters() {

        return __numIter;
    }

    unsigned int KMeans::getNumNonemptyClusters() {

        return __numNonempty;
    }

    unsigned int KMeans::getNumMovesLastIter() {

        return __numMovesLastIter;
    }

    Cluster &KMeans::operator[](unsigned int u) {

    }

    const Cluster &KMeans::operator[](unsigned int u) const {

    }

    std::ostream &Clustering::operator<<(std::ostream &os, const KMeans &kmeans) {

    }

    void KMeans::run() {

    }
}
