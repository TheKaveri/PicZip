#include "qtcount.h"
#include <queue>

bool qtcount::prunable(Node *node, const int tol) const {
    /* Your code here */
    // BFS
    std::queue<Node *> myQueue;
    myQueue.push(node);
    while (!myQueue.empty()) {
        Node *t = myQueue.front();
        myQueue.pop();
        if (t != nullptr) {
            if (t->NW == nullptr && t->NE == nullptr && t->SW == nullptr && t->SE == nullptr) {
                int colourDistance = (t->avg.r - node->avg.r) * (t->avg.r - node->avg.r) +
                                     (t->avg.g - node->avg.g) * (t->avg.g - node->avg.g) +
                                     (t->avg.b - node->avg.b) * (t->avg.b - node->avg.b);
                if (colourDistance <= tol) {
                    //pass
                } else {
                    return false;
                }
            }
            myQueue.push(t->NW);
            myQueue.push(t->NE);
            myQueue.push(t->SW);
            myQueue.push(t->SE);
        }
    }
    return true;
}
