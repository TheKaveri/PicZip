
#include "stats.h"

stats::stats(PNG &im) {
    for (unsigned int y = 0; y < im.height(); y++) {
        vector<long> sumRedLevel;
        vector<long> sumGreenLevel;
        vector<long> sumBlueLevel;
        vector<long> sumsqRedLevel;
        vector<long> sumsqGreenLevel;
        vector<long> sumsqBlueLevel;
        long red = 0;
        long green = 0;
        long blue = 0;
        long redsq = 0;
        long greensq = 0;
        long bluesq = 0;
        for (unsigned int x = 0; x < im.width(); x++) {
            RGBAPixel *p = im.getPixel(x, y);
            red += p->r;
            green += p->g;
            blue += p->b;
            redsq += (p->r) * (p->r);
            greensq += (p->g) * (p->g);
            bluesq += (p->b) * (p->b);
            sumRedLevel.push_back(red);
            sumGreenLevel.push_back(green);
            sumBlueLevel.push_back(blue);
            sumsqRedLevel.push_back(redsq);
            sumsqGreenLevel.push_back(greensq);
            sumsqBlueLevel.push_back(bluesq);
        }
        this->sumRed.push_back(sumRedLevel);
        this->sumGreen.push_back(sumGreenLevel);
        this->sumBlue.push_back(sumBlueLevel);
        this->sumsqRed.push_back(sumsqRedLevel);
        this->sumsqGreen.push_back(sumsqGreenLevel);
        this->sumsqBlue.push_back(sumsqBlueLevel);
    }
}

long stats::getSum(char channel, pair<int, int> ul, int dim) {
    int sideLength = static_cast<int>(std::pow(2, dim));
    long sum = 0;
    int startIdx = ul.first - 1;
    int endIdx = ul.first + sideLength - 1;
    for (int y = ul.second; y < ul.second + sideLength; y++) {
        if (channel == 'r') {
            if (startIdx == -1) {
                sum += sumRed[y][endIdx];
            } else {
                sum += sumRed[y][endIdx] - sumRed[y][startIdx];
            }
        } else if (channel == 'g') {
            if (startIdx == -1) {
                sum += sumGreen[y][endIdx];
            } else {
                sum += sumGreen[y][endIdx] - sumGreen[y][startIdx];
            }
        } else if (channel == 'b') {
            if (startIdx == -1) {
                sum += sumBlue[y][endIdx];
            } else {
                sum += sumBlue[y][endIdx] - sumBlue[y][startIdx];
            }
        }
    }
    return sum;
}

long stats::getSumSq(char channel, pair<int, int> ul, int dim) {
    int sideLength = static_cast<int>(std::pow(2, dim));
    long sumsq = 0;
    int startIdx = ul.first - 1;
    int endIdx = ul.first + sideLength - 1;
    for (int y = ul.second; y < ul.second + sideLength; y++) {
        if (channel == 'r') {
            if (startIdx == -1) {
                sumsq += sumsqRed[y][endIdx];
            } else {
                sumsq += sumsqRed[y][endIdx] - sumsqRed[y][startIdx];
            }
        } else if (channel == 'g') {
            if (startIdx == -1) {
                sumsq += sumsqGreen[y][endIdx];
            } else {
                sumsq += sumsqGreen[y][endIdx] - sumsqGreen[y][startIdx];
            }
        } else if (channel == 'b') {
            if (startIdx == -1) {
                sumsq += sumsqBlue[y][endIdx];
            } else {
                sumsq += sumsqBlue[y][endIdx] - sumsqBlue[y][startIdx];
            }
        }
    }
    return sumsq;
}

long stats::rectArea(int dim) {
    return static_cast<int>(std::pow(2, dim)) * static_cast<int>(std::pow(2, dim));;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int, int> ul, int dim) {
    double area = static_cast<double >( rectArea(dim));
    double variabilityRed = static_cast<double >(getSumSq('r', ul, dim)) - pow(getSum('r', ul, dim), 2) / area;
    double variabilityGreen =
            static_cast<double >(getSumSq('g', ul, dim)) - pow(getSum('g', ul, dim), 2) / area;
    double variabilityBlue =
            static_cast<double >(getSumSq('b', ul, dim)) - pow(getSum('b', ul, dim), 2) / area;

    double variabilityTotal = variabilityRed + variabilityGreen + variabilityBlue;
    return variabilityTotal;
}

RGBAPixel stats::getAvg(pair<int, int> ul, int dim) {
    int red = getSum('r', ul, dim) / rectArea(dim);
    int green = getSum('g', ul, dim) / rectArea(dim);
    int blue = getSum('b', ul, dim) / rectArea(dim);

    RGBAPixel pixel(red, green, blue);
    return pixel;
}
