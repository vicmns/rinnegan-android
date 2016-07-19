//
// Created by Victor Manuel Cervantes Salido on 1/18/16.
//

#ifndef RINNEGANV2_PARALLELCOLORFILTER_HPP
#define RINNEGANV2_PARALLELCOLORFILTER_HPP

#include "opencv2/core/core.hpp"
#include <jni.h>

using namespace std;
using namespace cv;

template<class type>

class ParallelColorFilter : public ParallelLoopBody {
private:
    type *bufferToEdit;
    vector<vector<float>> filter;
    int mChannels;
public:
    ParallelColorFilter(type *bufferToEdit, const vector<vector<float>> &filter, const int &channels) : bufferToEdit(
            bufferToEdit), filter(filter), mChannels(channels) { }

    virtual void operator()(const Range &range) const override {
        register type *pixel = bufferToEdit;
        int rIdx;
        int gIdx;
        int bIdx;
        int aIdx;
        for (register int i = range.start; i < range.end; i += mChannels) {
            rIdx = i;
            gIdx = i + 1;
            bIdx = i + 2;
            uchar nRValue, nGValue, nBValue;
            nRValue =
                    pixel[rIdx] * filter[0][0] +
                    pixel[gIdx] * filter[0][1] +
                    pixel[bIdx] * filter[0][2];//Red
            nGValue =
                    pixel[rIdx] * filter[1][0] +
                    pixel[gIdx] * filter[1][1] +
                    pixel[bIdx] * filter[1][2]; // Green
            nBValue =
                    pixel[rIdx] * filter[2][0] +
                    pixel[gIdx] * filter[2][1] +
                    pixel[bIdx] * filter[2][2]; //Blue

            pixel[rIdx] = nRValue;
            pixel[gIdx] = nGValue;
            pixel[bIdx] = nBValue;
        }

    }
};


#endif //RINNEGANV2_PARALLELCOLORFILTER_HPP
