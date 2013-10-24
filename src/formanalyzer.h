/*
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef FORMANALYZER_H
#define FORMANALYZER_H

#include <QObject>
#include "datatypes.h"

// How many buckets are used per shape analyzation.
#define BUCKETSET_SIZE 20
#define ANALYZER_SHOW_DEBUG_INFO

/*!
  \class FormAnalyzer
  \brief Compares two dot-sets and return their similarity
*/
class FormAnalyzer : public QObject
{
public:
    /*!
      Constructor
    */
    FormAnalyzer(QObject *parent = 0);
    virtual ~FormAnalyzer();

    /*!
      Compares two shapes and returns the similarity, 1 means
      completely similar.
    */
    float compareShapes( AS::Shape &shape1, AS::Shape &shape2 );

    /*!
      Calculates the travelled distance and the bounding box for the shape.
    */
    void preAnalyzeShape( AS::Shape &shape );

protected:
    /*!
      A structure for internal analyzing use.
    */
    struct SAnalyzerBucket
    {
        float pos[3];               // resampled position from the shape.
        float relPos[3];            // movement from the previous point
        float normRelPos[2];        // Normalized relpos
    };

    /*!
      Goes through a shape with any number of points and creates a new bucketset
      out of it with desired amount of targetpoints. All of the returned
      points have static distance between it's neighbours.
    */
    SAnalyzerBucket *createBuckets( AS::Shape &shape, int bucketCount );

    /*!
      Takes two bucketsets (both sets must have the same amount of points)
      and compares their similarity. returns -1 if bucketsets are completely
      opposite, and 1 if they are completely similar.
    */
    float compareBuckets( SAnalyzerBucket *b1, SAnalyzerBucket *b2 );
};

#endif // FORMANALYZER_H
