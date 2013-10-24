#include <QDebug>
#include <math.h>
#include "formanalyzer.h"

using namespace AS;


FormAnalyzer::FormAnalyzer( QObject *parent )
    : QObject(parent)
{
}


FormAnalyzer::~FormAnalyzer()
{
}


void FormAnalyzer::preAnalyzeShape( Shape &shape )
{
    // Do not preanalyze if its already done.
    if (shape.shapePreAnalyzed == true) {
        return;
    }

    float minx = 1000.0f;
    float maxx = -1000.0f;

    float miny = 1000.0f;
    float maxy = -1000.0f;

    float dx, dy, len;
    int index = 0;
    qreal travDistance = 0.0f;
    Coordinate *prevCoordinate = 0;

    Shape::Coordinates::Iterator i;
    for (i = shape.coordinates.begin(); i!= shape.coordinates.end(); i++) {
        Coordinate *c = &(*i);
        if (prevCoordinate == 0) {
            len = 0.0f;
        }
        else {
            dx = c->pos[0] - prevCoordinate->pos[0];
            dy = c->pos[1] - prevCoordinate->pos[1];
            len = sqrtf(dx * dx + dy * dy);
        }

        travDistance += len;
        c->travelledDistance = travDistance;

        // calculate bounds
        if (index==0 || c->pos[0]>maxx)
            maxx = c->pos[0];
        if (index==0 || c->pos[0]<minx)
            minx = c->pos[0];
        if (index==0 || c->pos[1]>maxy)
            maxy = c->pos[1];
        if (index==0 || c->pos[1]<miny)
            miny = c->pos[1];

        prevCoordinate = c;
        index++;
    }

    shape.minx = minx;
    shape.maxx = maxx;
    shape.miny = miny;
    shape.maxy = maxy;
    shape.totalDistanceTravelled = travDistance;
    shape.shapePreAnalyzed = true;

#ifdef ANALYZER_SHOW_DEBUG_INFO
    qDebug() << "Shape total distance: "
             << shape.totalDistanceTravelled
             << "Bounds: " << minx << ","
             << maxx <<" : " << miny << ","
             << maxy;
#endif

}


FormAnalyzer::SAnalyzerBucket *FormAnalyzer::createBuckets( Shape &shape, int bucketCount )
{
#ifdef ANALYZER_SHOW_DEBUG_INFO
    qDebug() << "createBuckets:" << shape.coordinates.count();
#endif

    SAnalyzerBucket *buckets = new SAnalyzerBucket[bucketCount];

    Shape::Coordinates::Iterator i = shape.coordinates.begin();

    float shapeWidth = shape.maxx - shape.minx;
    if (shapeWidth == 0.0f) {
        shapeWidth = 0.00001f;
    }

    float shapeHeight = shape.maxy - shape.miny;
    if (shapeHeight == 0.0f) {
        shapeHeight = 0.00001f;
    }

    float ftemp;
    Coordinate *prevCoord = 0;
    Coordinate *coord;

    // scan through each bucket and see the result
    for (int bi=0; bi < bucketCount; bi++) {
        float bucketPos =
                ((float)bi + 0.5f) / (float)bucketCount
                * shape.totalDistanceTravelled;

        coord = &(*i);
        while (coord->travelledDistance < bucketPos) {
            if (i == shape.coordinates.end()) {
                qDebug() << "ERROR: Analyzer::createBuckets: out of coordinates.";
            }
            else {
                i++;
            }

            prevCoord = coord;
            coord = &(*i);
        }

        float interpolateLength = (coord->travelledDistance - prevCoord->travelledDistance);
        float coordMul = (bucketPos - prevCoord->travelledDistance) / interpolateLength;
        float prevCoordMul = (1.0f - coordMul);

        // Fill the bucket
        buckets[bi].pos[0] = prevCoord->pos[0] * prevCoordMul +
                coord->pos[0] * coordMul;
        buckets[bi].pos[1] = prevCoord->pos[1] * prevCoordMul +
                coord->pos[1] * coordMul;
        buckets[bi].pos[2] = prevCoord->pos[2] * prevCoordMul +
                coord->pos[2] * coordMul;

        if (bi>0) {
            buckets[bi].relPos[0] = buckets[bi].pos[0] - buckets[bi-1].pos[0];
            buckets[bi].relPos[1] = buckets[bi].pos[1] - buckets[bi-1].pos[1];
            buckets[bi].relPos[2] = buckets[bi].pos[2] - buckets[bi-1].pos[2];
        }
        else {
            buckets[bi].relPos[0] = 0.0f;
            buckets[bi].relPos[1] = 0.0f;
            buckets[bi].relPos[2] = 0.0f;
        }

        // normalize relpos
        ftemp = buckets[bi].relPos[0] * buckets[bi].relPos[0] +
                buckets[bi].relPos[1] * buckets[bi].relPos[1];

        ftemp = sqrtf(ftemp);

        if (ftemp == 0.0f) {
            ftemp = 0.00001f;
        }

        buckets[bi].normRelPos[0] = buckets[bi].relPos[0] / ftemp;
        buckets[bi].normRelPos[1] = buckets[bi].relPos[1] / ftemp;
    }

    return buckets;
}


float FormAnalyzer::compareBuckets( SAnalyzerBucket *b1, SAnalyzerBucket *b2 )
{
    float similarity = 0.0f;

    // Total sigma of dotproducts between all the vectors. Used as a "checksum" multiplier
    // for error.
    float b1total_dot = 0.0f;
    float b2total_dot = 0.0f;

    for (int b=1; b<BUCKETSET_SIZE; b++) {
        b1total_dot += b1[b].normRelPos[0] * b1[b-1].normRelPos[0] +
                       b1[b].normRelPos[1] * b1[b-1].normRelPos[1];

        b2total_dot += b2[b].normRelPos[0] * b2[b-1].normRelPos[0] +
                       b2[b].normRelPos[1] * b2[b-1].normRelPos[1];

        similarity += b1[b].normRelPos[0] * b2[b].normRelPos[0] +
                      b1[b].normRelPos[1] * b2[b].normRelPos[1];
    }

    b1total_dot /= (float)(BUCKETSET_SIZE - 1);
    b2total_dot /= (float)(BUCKETSET_SIZE - 1);
    similarity /= (float)(BUCKETSET_SIZE - 1);

    qDebug() << "Total similarity: " << similarity;

    float totaldotdifference = fabsf( b1total_dot - b2total_dot );
    float dotdifferencemul = 1.1f - totaldotdifference;

    if (dotdifferencemul > 1.0f) {
        dotdifferencemul = 1.0f;
    }

    if (dotdifferencemul < 0.0f) {
        dotdifferencemul = 0.0f;
    }

    similarity *= dotdifferencemul;

    qDebug() << "b1totaldot: " << b1total_dot << " b2totaldot:" << b2total_dot;
    qDebug() << "dotdifferencemul:" << dotdifferencemul;
    qDebug() << "Totaldotdifference_fixed similarity: " << similarity;

    return 1.0f - similarity;
}


float FormAnalyzer::compareShapes( Shape &shape1, Shape &shape2 )
{
    SAnalyzerBucket *shape1_buckets = createBuckets( shape1, BUCKETSET_SIZE );
    SAnalyzerBucket *shape2_buckets = createBuckets( shape2, BUCKETSET_SIZE );

    float error = compareBuckets( shape1_buckets, shape2_buckets );

    qDebug() << "compare-error: " << error;

    if (shape1_buckets) {
        delete [] shape1_buckets;
    }

    if (shape2_buckets) {
        delete [] shape2_buckets;
    }

    float similarity = 1.0f - error;
    if (similarity < 0.0f) {
        similarity = 0.0f;
    }

    if (similarity > 1.0f) {
        similarity = 1.0f;
    }

    qDebug() << "Similarity: " << similarity;

    return similarity;
}
