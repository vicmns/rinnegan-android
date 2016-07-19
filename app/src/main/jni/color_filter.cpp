//
// Created by Victor Manuel Cervantes Salido on 1/11/16.
//
#include "opencv2/core/core.hpp"
#include "ParallelColorFilter.hpp"
#include "NewParallelColorFilter.hpp"

using namespace std;
using namespace cv;

extern "C" {

static const int DEUTERANOPIA_FILTER_TYPE = 0;
static const int PROTONOPIA_FILTER_TYPE = 1;
static const int TRITANOPIA_FILTER_TYPE = 2;
static float deutoronopia[] = {0.625, 0.375, 0, 0, 0, 0.7, 0.3, 0, 0, 0, 0, 0.3, 0.7, 0, 0, 0,
                                     0, 0, 1, 0, 0, 0, 0, 0, 1};
static float protonopia[] = {0.567, 0.433, 0, 0, 0, 0.558, 0.442, 0, 0, 0, 0, 0.242, 0.758, 0,
                                   0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1};
static float tritanopia[] = {0.95, 0.05, 0, 0, 0, 0, 0.433, 0.567, 0, 0, 0, 0.475, 0.525, 0,
                                   0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1};

vector<vector<float>> getDeuteranopiaFilter() {
    vector<float> rColor = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    vector<float> gColor = {0.5f, 0, 0.5f, 0, 0, 0, 0, 0, 0, 0};
    vector<float> bColor = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
    vector<vector<float>> filter = {rColor, gColor, bColor};
    return filter;
}
vector<vector<float>> getProtanopiaFilter() {
    vector<float> rColor = {0, 0.5f, 0.5f, 0, 0, 0, 0, 0, 0, 0};
    vector<float> gColor = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    vector<float> bColor = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
    vector<vector<float>> filter = {rColor, gColor, bColor};
    return filter;
}
vector<vector<float>> getTritanopiaFilter() {
    vector<float> rColor = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    vector<float> gColor = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    vector<float> bColor = {0.5f, 0.5f, 0, 0, 0, 0, 0, 0, 0, 0};
    vector<vector<float>> filter = {rColor, gColor, bColor};
    return filter;
}

vector<vector<uchar>> getDeuteranopiaFilterUchar() {
    vector<uchar> rColor = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    vector<uchar> gColor = {static_cast<unsigned char>(0.5f), 0,
                            static_cast<unsigned char>(0.5f), 0, 0, 0, 0, 0, 0, 0};
    vector<uchar> bColor = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
    vector<vector<uchar>> filter = {rColor, gColor, bColor};
    return filter;
}
vector<vector<uchar>> getProtanopiaFilterUchar() {
    vector<uchar> rColor = {0, static_cast<unsigned char>(0.5f), static_cast<unsigned char>(0.5f),
                            0, 0, 0, 0, 0, 0, 0};
    vector<uchar> gColor = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    vector<uchar> bColor = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
    vector<vector<uchar>> filter = {rColor, gColor, bColor};
    return filter;
}
vector<vector<uchar>> getTritanopiaFilterUchar() {
    vector<uchar> rColor = {1 * 255, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    vector<uchar> gColor = {0, 1 * 255, 0, 0, 0, 0, 0, 0, 0, 0};
    vector<uchar> bColor = {static_cast<unsigned char>(0.5f), static_cast<unsigned char>(0.5f), 0,
                            0, 0, 0, 0, 0, 0, 0};
    vector<vector<uchar>> filter = {rColor, gColor, bColor};
    return filter;
}

uchar fu(float n) { return (uchar) (n < 0 ? 0 : (n < 255 ? n : 255)); }

vector<uchar> getColorMatrix(uchar matrix[], float filter[]) {

    float r = ((matrix[0] * filter[0]) + (matrix[1] * filter[1]) + (matrix[2] * filter[2]) +
               (matrix[3] * filter[3]) + filter[4]);
    float g = ((matrix[0] * filter[5]) + (matrix[1] * filter[6]) + (matrix[2] * filter[7]) +
               (matrix[3] * filter[8]) + filter[9]);
    float b = ((matrix[0] * filter[10]) + (matrix[1] * filter[11]) + (matrix[2] * filter[12]) +
               (matrix[3] * filter[13]) + filter[14]);
    float a = ((matrix[0] * filter[15]) + (matrix[1] * filter[16]) + (matrix[2] * filter[17]) +
               (matrix[3] * filter[18]) + filter[19]);

    vector<uchar> outFilter = {fu(r), fu(g), fu(b), fu(a)};
    return outFilter;
}

void otherSlowMethod(Mat mRgb);
void testMethod(Mat mRgb);

JNIEXPORT void JNICALL Java_com_lonelystudios_rinneganv2_MainActivity_ApplyFilter(JNIEnv *, jobject,
                                                                                  jlong addrRgba,
                                                                                  jint typeOfFilter);

JNIEXPORT void JNICALL Java_com_lonelystudios_rinneganv2_MainActivity_ApplyFilter(JNIEnv *, jobject,
                                                                                  jlong addrRgba,
                                                                                  jint typeOfFilter) {
    Mat &mRgb = *(Mat *) addrRgba;
    vector<vector<float>> filter;
    int channels = mRgb.channels();
    int nRows = mRgb.rows;
    int nCols = mRgb.cols * channels;
    if (mRgb.isContinuous()) {
        nCols *= nRows;
        nRows = 1;
    }

    switch (typeOfFilter) {
        case DEUTERANOPIA_FILTER_TYPE:
            filter = getDeuteranopiaFilter();
            break;
        case PROTONOPIA_FILTER_TYPE:
            filter = getProtanopiaFilter();
            break;
        case TRITANOPIA_FILTER_TYPE:
            filter = getTritanopiaFilter();
            break;
        default:
            filter = getDeuteranopiaFilter();
    }

    for (int i = 0; i < nRows; i++) {
        /*parallel_for_(Range(0, nCols),
                      ParallelColorFilter<uchar>(mRgb.ptr<uchar>(i), filter, channels));*/
        parallel_for_(Range(0, nCols),
                      NewParallelColorFilter<uchar>(mRgb.ptr<uchar>(i), deutoronopia));
    }
}

void slowMethodLogic(Mat mRgb) {
    Mat filterFrame(mRgb.rows, mRgb.cols, CV_32FC4);
    mRgb.convertTo(filterFrame, CV_32FC4);
    int rIdx = 0;
    int gIdx = 1;
    int bIdx = 2;
    vector<vector<float>> filter = getDeuteranopiaFilter();
    MatIterator_<Vec3f> it, end;
    for (it = filterFrame.begin<Vec3f>(), end = filterFrame.end<Vec3f>(); it != end; ++it) {

        //Apply filter
        (*it)[0] = (((*it)[rIdx]) * filter[0][0] + ((*it)[gIdx]) * filter[0][1] +
                    ((*it)[bIdx]) * filter[0][2] +
                    ((*it)[rIdx]) * ((*it)[rIdx]) * filter[0][3] +
                    ((*it)[gIdx]) * ((*it)[gIdx]) * filter[0][4] +
                    ((*it)[bIdx]) * ((*it)[bIdx]) * filter[0][5] +
                    ((*it)[rIdx]) * ((*it)[gIdx]) * filter[0][6] +
                    ((*it)[gIdx]) * ((*it)[bIdx]) * filter[0][7] +
                    ((*it)[bIdx]) * ((*it)[rIdx]) * filter[0][8] + filter[0][9]); //Red
        (*it)[1] = (((*it)[rIdx]) * filter[1][0] + ((*it)[gIdx]) * filter[1][1] +
                    ((*it)[bIdx]) * filter[1][2] +
                    ((*it)[rIdx]) * ((*it)[rIdx]) * filter[1][3] +
                    ((*it)[gIdx]) * ((*it)[gIdx]) * filter[1][4] +
                    ((*it)[bIdx]) * ((*it)[bIdx]) * filter[1][5] +
                    ((*it)[rIdx]) * ((*it)[gIdx]) * filter[1][6] +
                    ((*it)[gIdx]) * ((*it)[bIdx]) * filter[1][7] +
                    ((*it)[bIdx]) * ((*it)[rIdx]) * filter[1][8] + filter[1][9]); // Green
        (*it)[2] = (((*it)[rIdx]) * filter[2][0] + ((*it)[gIdx]) * filter[2][1] +
                    ((*it)[bIdx]) * filter[2][2] +
                    ((*it)[rIdx]) * ((*it)[rIdx]) * filter[2][3] +
                    ((*it)[gIdx]) * ((*it)[gIdx]) * filter[2][4] +
                    ((*it)[bIdx]) * ((*it)[bIdx]) * filter[2][5] +
                    ((*it)[rIdx]) * ((*it)[gIdx]) * filter[2][6] +
                    ((*it)[gIdx]) * ((*it)[bIdx]) * filter[2][7] +
                    ((*it)[bIdx]) * ((*it)[rIdx]) * filter[2][8] + filter[2][9]); //Blue
    }

    filterFrame.convertTo(mRgb, CV_8UC(mRgb.channels()));
    filterFrame.release();
    filter.clear();
}

void otherSlowMethod(Mat mRgb) {
    MatIterator_<Vec3b> it, end;
    int rIdx = 0;
    int gIdx = 1;
    int bIdx = 2;
    vector<vector<float>> filter = getDeuteranopiaFilter();
    for (it = mRgb.begin<Vec3b>(), end = mRgb.end<Vec3b>(); it != end; ++it) {

        //Apply filter
        (*it)[0] = (((*it)[rIdx]) * filter[0][0] + ((*it)[gIdx]) * filter[0][1] +
                    ((*it)[bIdx]) * filter[0][2] +
                    ((*it)[rIdx]) * ((*it)[rIdx]) * filter[0][3]); //Red
        (*it)[1] = (((*it)[rIdx]) * filter[1][0] + ((*it)[gIdx]) * filter[1][1] +
                    ((*it)[bIdx]) * filter[1][2] +
                    ((*it)[rIdx]) * ((*it)[rIdx]) * filter[1][3]); // Green
        (*it)[2] = (((*it)[rIdx]) * filter[2][0] + ((*it)[gIdx]) * filter[2][1] +
                    ((*it)[bIdx]) * filter[2][2] +
                    ((*it)[rIdx]) * ((*it)[rIdx]) * filter[2][3]); //Blue
    }
}
}

void testMethod(Mat mRGB) {
    int rIdx = 0;
    int gIdx = 1;
    int bIdx = 2;
    vector<vector<float>> filter = getDeuteranopiaFilter();
    for (int i = 0; i < mRGB.rows; i++) {
        Vec3b *pixel = mRGB.ptr<Vec3b>(i);
        uchar nRValue, nGValue, nBValue;
        for (int j = 0; j < mRGB.cols; j++) {
            nRValue =
                    pixel[j][rIdx] * filter[0][0] +
                    pixel[j][gIdx] * filter[0][1] +
                    pixel[j][bIdx] * filter[0][2] +
                    pixel[j][rIdx] * pixel[j][rIdx] * filter[0][3];//Red
            nGValue =
                    pixel[j][rIdx] * filter[1][0] +
                    pixel[j][gIdx] * filter[1][1] +
                    pixel[j][bIdx] * filter[1][2] +
                    pixel[j][rIdx] * pixel[j][rIdx] * filter[1][3]; // Green
            nBValue =
                    pixel[j][rIdx] * filter[2][0] +
                    pixel[j][gIdx] * filter[2][1] +
                    pixel[j][bIdx] * filter[2][2] +
                    pixel[j][rIdx] * pixel[j][rIdx] * filter[2][3]; //Blue

            pixel[j][rIdx] = nRValue;
            pixel[j][gIdx] = nGValue;
            pixel[j][bIdx] = nBValue;
        }
    }
}