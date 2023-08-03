#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include "qtcount.h"
#include "qtvar.h"

using namespace cs221util;
using namespace std;

int main() {
    // read in image
    PNG origIm1;
    origIm1.readFromFile("images/orig/KJVFbig.png");
    PNG origIm2;
    origIm2.readFromFile("images/orig/adasquare.png");
    PNG origIm3;
    origIm3.readFromFile("images/orig/stanley-totem-poles.png");

    // use it to build a quadtree
    qtcount t1(origIm1);
    qtcount tCopy1(t1);
    qtcount t2(origIm2);
    qtcount tCopy2(t2);
    qtcount t3(origIm3);
    qtcount tCopy3(t3);

    // prune the quadtree
    tCopy1.prune(10000);
    tCopy2.prune(10000);
    tCopy3.prune(3000);

    // render the quadtree
    PNG ppic1 = tCopy1.render();
    PNG ppic2 = tCopy2.render();
    PNG ppic3 = tCopy3.render();

    ppic1.writeToFile("images/out/output-prunedKJVFbig.png");
    ppic2.writeToFile("images/out/output-prunedadasquare.png");
    ppic3.writeToFile("images/out/output-prunedstanley-totem-poles.png");

    // use it to build a quadtree
    qtvar v1(origIm1);
    qtvar vCopy1(v1);
    qtvar v2(origIm2);
    qtvar vCopy2(v2);
    qtvar v3(origIm3);
    qtvar vCopy3(v3);

    // prune the quadtree
    vCopy1.prune(10000);
    vCopy2.prune(10000);
    vCopy3.prune(3000);

    // render the quadtree
    PNG vpic1 = vCopy1.render();
    PNG vpic2 = vCopy2.render();
    PNG vpic3 = vCopy3.render();

    vpic1.writeToFile("images/out/output-prunedKJVFbig-var.png");
    vpic2.writeToFile("images/out/output-prunedadasquare-var.png");
    vpic3.writeToFile("images/out/output-prunedstanley-totem-poles-var.png");

    // comparisons

    qtcount countcomp(origIm3);
    qtvar varcomp(origIm3);

    countcomp.prune(countcomp.idealPrune(10000));
    varcomp.prune(varcomp.idealPrune(10000));

    PNG countpng = countcomp.render();
    PNG varpng = varcomp.render();

    countpng.writeToFile("images/out/output-comp-count-stanley.png");
    varpng.writeToFile("images/out/output-comp-var-stanley.png");

    return 0;
}
