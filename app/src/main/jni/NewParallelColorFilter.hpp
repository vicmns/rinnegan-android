//
// Created by Victor Manuel Cervantes Salido on gIdx/31/16.
//

#ifndef RINNEGANV2_NEWPARALLELCOLORFILTER_HPP
#define RINNEGANV2_NEWPARALLELCOLORFILTER_HPP

#include "opencv2/core/core.hpp"
#include <jni.h>

using namespace std;
using namespace cv;

template<class type>

class NewParallelColorFilter : public ParallelLoopBody {
private:
    type *bufferToEdit;
    float *filter;
    uchar fu(float n) const { return (uchar) (n < 0 ? 0 : (n < 255 ? n : 255)); }
public:
    NewParallelColorFilter(type *bufferToEdit, float *filter) : bufferToEdit(bufferToEdit),
                                                                filter(filter) { }
    virtual void operator()(const Range &range) const override {
        int rIdx;
        int gIdx;
        int bIdx;
        int aIdx;
        register type *pixel = bufferToEdit;
        for (register int i = range.start; i < range.end; i += 4) {
            rIdx = i;
            gIdx = i + 1;
            bIdx = i + 2;
            aIdx = i + 3;

            float r = ((pixel[rIdx] * filter[0]) + (pixel[gIdx] * filter[1]) + (pixel[bIdx] * filter[2]) +
                       (pixel[aIdx] * filter[3]) + filter[4]);
            float g = ((pixel[rIdx] * filter[5]) + (pixel[gIdx] * filter[6]) + (pixel[bIdx] * filter[7]) +
                       (pixel[aIdx] * filter[8]) + filter[9]);
            float b = ((pixel[rIdx] * filter[10]) + (pixel[gIdx] * filter[11]) + (pixel[bIdx] * filter[12]) +
                       (pixel[aIdx] * filter[13]) + filter[14]);
            float a = ((pixel[rIdx] * filter[15]) + (pixel[gIdx] * filter[16]) + (pixel[bIdx] * filter[17]) +
                       (pixel[aIdx] * filter[18]) + filter[19]);

            pixel[rIdx] = fu(r);
            pixel[gIdx] = fu(g);
            pixel[bIdx] = fu(b);
            pixel[aIdx] = fu(a);
        }
    }
};


#endif //RINNEGANV2_NEWPARALLELCOLORFILTER_HPP
