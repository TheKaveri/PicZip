#include "qtvar.h"

bool qtvar::prunable(Node *node, const int tol) const {
    /* Your code here! */
    if (node != nullptr) {
        return node->var < tol;
    }
    return false;
}
