
/**
 * quadtree.cpp
 */

#include "quadtree.h"
#include <queue>

using namespace std;

// Node constructor, given.
quadtree::Node::Node(pair<int, int> ul, int d, RGBAPixel a, double v)
        : upLeft(ul), dim(d), avg(a), var(v), NW(nullptr), NE(nullptr), SE(nullptr), SW(nullptr) {}


// quadtree destructor, given.
quadtree::~quadtree() {
    clear();
}

// quadtree copy constructor, given.
quadtree::quadtree(const quadtree &other) {
    copy(other);
}

// quadtree assignment operator, given.
quadtree &quadtree::operator=(const quadtree &rhs) {
    if (this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}

quadtree::quadtree(PNG &imIn) {
    this->edge = imIn.width();
    stats s(imIn);
    pair<int, int> ul = make_pair(0, 0);
    int dim = static_cast<int> (::log2(this->edge));
    this->root = buildTree(s, ul, dim);
}

quadtree::Node *quadtree::buildTree(stats &s, pair<int, int> ul, int dim) {
    Node *tree = new Node(ul, dim, s.getAvg(ul, dim), s.getVar(ul, dim));
    if (dim != 0) {
        int sideLength = static_cast<int>( ::pow(2, dim - 1));
        tree->NW = buildTree(s, make_pair(ul.first, ul.second), dim - 1);
        tree->NE = buildTree(s, make_pair(ul.first + sideLength, ul.second), dim - 1);
        tree->SE = buildTree(s, make_pair(ul.first + sideLength, ul.second + sideLength), dim - 1);
        tree->SW = buildTree(s, make_pair(ul.first, ul.second + sideLength), dim - 1);
    }
    return tree;
}

PNG quadtree::render() const {
    //BFS
    std::queue<Node *> myQueue;
    PNG img(this->edge, this->edge);
    myQueue.push(this->root);
    while (!myQueue.empty()) {
        Node *t = myQueue.front();
        myQueue.pop();
        if (t != nullptr) {
            int sideLength = static_cast<int> (::pow(2, t->dim));
            for (int h = t->upLeft.second; h < sideLength + t->upLeft.second; h++) {
                for (int w = t->upLeft.first; w < sideLength + t->upLeft.first; w++) {
                    RGBAPixel *p = img.getPixel(w, h);
                    p->r = t->avg.r;
                    p->g = t->avg.g;
                    p->b = t->avg.b;
                }
            }
            myQueue.push(t->NW);
            myQueue.push(t->NE);
            myQueue.push(t->SW);
            myQueue.push(t->SE);
        }
    }
    return img;
}

int quadtree::idealPrune(const int leaves) const {
    /* Your code here! */
//    int min_tol = 0;
//    int max_tol = 1000000;
//    int tol = (max_tol + min_tol) / 2;
//    while (min_tol <= max_tol) {
//        int nodes = pruneSize(tol);
//        int nonLeaves = nodes - leaves;
//        if (nonLeaves <= 0) {
//            max_tol = tol - 1;
//        } else {
//            min_tol = tol + 1;
//        }
//        tol = (min_tol + max_tol) / 2;
//    }
//    return tol;

    int min_tol = 0;
    int max_tol = 1;
    while (pruneSize(max_tol) - leaves > 0) {
        min_tol = max_tol;
        max_tol *= 2;
    }
    while (min_tol < max_tol) {
        int tol = (min_tol + max_tol + 1) / 2;
        int nodes = pruneSize(tol);
        int nonLeaves = nodes - leaves;
        if (nonLeaves <= 0) {
            max_tol = tol - 1;
        } else {
            min_tol = tol;
        }
    }
    if (min_tol == 0) {
        return min_tol;
    }
    return min_tol + 1;
}

int quadtree::pruneSize(const int tol) const {
    return pruneSizeQuadTree(this->root, tol);
}

int quadtree::pruneSizeQuadTree(quadtree::Node *croot, const int tol) const {
    if (croot == nullptr) {
        return 0;
    }
    if ((croot->NW == nullptr && croot->NE == nullptr && croot->SW == nullptr && croot->SE == nullptr) ||
        prunable(croot, tol)) {
        // if leaf or if prunable node
        return 1;
    }

    int count = 0;
    count += pruneSizeQuadTree(croot->NW, tol);
    count += pruneSizeQuadTree(croot->NE, tol);
    count += pruneSizeQuadTree(croot->SW, tol);
    count += pruneSizeQuadTree(croot->SE, tol);

    return count;
}

void quadtree::prune(const int tol) {
    pruneQuadTree(this->root, tol);
}

void quadtree::pruneQuadTree(Node *croot, const int tol) {
    if (croot == nullptr) {
        return;
    }
    if (!(croot->NW == nullptr and croot->NE == nullptr and croot->SW == nullptr and croot->SE == nullptr)) {
        if (prunable(croot, tol)) { // is a prunable subtree
            // garbage collection
            clearQuadTree(croot->NW);
            clearQuadTree(croot->NE);
            clearQuadTree(croot->SE);
            clearQuadTree(croot->SW);

            // pruning
            croot->NW = nullptr;
            croot->NE = nullptr;
            croot->SW = nullptr;
            croot->SE = nullptr;
        } else {
            pruneQuadTree(croot->NW, tol);
            pruneQuadTree(croot->NE, tol);
            pruneQuadTree(croot->SW, tol);
            pruneQuadTree(croot->SE, tol);
        }
    }
}

void quadtree::clear() {
    clearQuadTree(this->root);
    this->edge = 0;
}

void quadtree::copy(const quadtree &orig) {
    this->root = copyQuadTree(orig.root);
    this->edge = orig.edge;
}

void quadtree::clearQuadTree(Node *croot) {
    if (croot != nullptr) {
        clearQuadTree(croot->NW);
        clearQuadTree(croot->NE);
        clearQuadTree(croot->SE);
        clearQuadTree(croot->SW);
        delete croot;
        croot = nullptr;
    }
}

quadtree::Node *quadtree::copyQuadTree(Node *croot) {
    Node *tree = nullptr;
    if (croot != nullptr) {
        tree = new Node(croot->upLeft, croot->dim, croot->avg, croot->var);
        tree->NW = copyQuadTree(croot->NW);
        tree->NE = copyQuadTree(croot->NE);
        tree->SE = copyQuadTree(croot->SE);
        tree->SW = copyQuadTree(croot->SW);
    }
    return tree;
}


