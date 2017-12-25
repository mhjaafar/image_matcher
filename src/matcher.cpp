/*
matcher.cpp

Milan Multi-Platform Imaging Language, an open source image processing library

Copyright (C) 2017 Mohamad Hafiz Jaafar

h_da University of Applied Sciences
Faculty of Mathematics and Science
Dept. Computer Vision and Photonics

Schoefferstrasse 3
D-64295 Darmstadt
Germany

This program is free software; you can redistribute it and/or modify it under the terms of the
GNU General Public License as published by the Free Software Foundation version 2 of the License

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write
to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110, USA

Dieses Programm ist freie Software. Sie koennen es unter den Bedingungen der GNU General Public License, wie
von der Free Software Foundation veroeffentlicht, weitergeben und/oder modifizieren, gemaess Version 2 der Lizenz.

Die Veroeffentlichung dieses Programms erfolgt in der Hoffnung, dass es Ihnen von Nutzen sein wird, aber OHNE
IRGENDEINE GARANTIE, sogar ohne die implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT FUER EINEN BESTIMMTEN
ZWECK. Details finden Sie in der GNU General Public License.

Sie sollten ein Exemplar der GNU General Public License zusammen mit diesem Programm erhalten haben. Falls nicht,
schreiben Sie an die Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110, USA.
*/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp> // SURF features, for OpenCV 2.4 and above.
#include <opencv2/opencv.hpp>
#include <opencv2/legacy/legacy.hpp>
#include "headers/matcher.hpp"

Matcher::Matcher(): m_fRatio(0.65f)
, m_bRefineF(true)
, m_dConfidence(0.99)
, m_dDistance(3.0)
{
    m_detector = new cv::SurfFeatureDetector();
    m_extractor = new cv::SurfDescriptorExtractor();
    m_matchPointsCount.x = 0;
    m_matchPointsCount.y = 0;
    m_dSymMatchCount = 0.;

    m_dTimeRansac = 0.;
}

Matcher::~Matcher(){
    // Deconstructor
}

int Matcher::ratioTest(std::vector<std::vector<cv::DMatch>>& matches){

    int removed = 0;

    // for all matches
    for (std::vector<std::vector<cv::DMatch>>::iterator matchIterator = matches.begin();
        matchIterator != matches.end(); ++matchIterator){
        // if 2 NN has been identified
        if (matchIterator->size() > 1){
            // check distance ratio
            if ((*matchIterator)[0].distance / (*matchIterator)[1].distance > m_fRatio){
                // remove match
                matchIterator->clear();
                removed++;
            }
        }
        else{ // does not have 2 neighbors
            // remove match
            matchIterator->clear();
            removed++;
        }
    }

    return removed;
} // END: int Matcher::ratioTest()

void Matcher::symmetryTest(const std::vector <std::vector<cv::DMatch>>& matches_1
    , const std::vector<std::vector<cv::DMatch>>& matches_2
    , std::vector<cv::DMatch>& symMatches){

    // For all matches image 1 -> image 2
    for (std::vector<std::vector<cv::DMatch>>::const_iterator matchIterator_1 = matches_1.begin();
        matchIterator_1 != matches_1.end(); ++matchIterator_1){

        if (matchIterator_1->size() < 2) // ignore deleted matches
            continue;

        // For all matches image 2 -> image 1
        for (std::vector<std::vector<cv::DMatch>>::const_iterator matchIterator_2 = matches_2.begin();
            matchIterator_2 != matches_2.end(); ++matchIterator_2){

            if (matchIterator_2->size() < 2) // ignore deleted matches
                continue; 

            // Match symmetry test
            if ((*matchIterator_1)[0].queryIdx == (*matchIterator_2)[0].trainIdx
                && (*matchIterator_2)[0].queryIdx == (*matchIterator_1)[0].trainIdx){
                // add symmetrical match
                symMatches.push_back(cv::DMatch((*matchIterator_1)[0].queryIdx
                    , (*matchIterator_1)[0].trainIdx
                    , (*matchIterator_1)[0].distance)
                );
                break; // next match in image 1 -> image 2
            }
        }
    }
}// END: void Matcher::symmetryTest()

/*
    @return F_mat
*/
cv::Mat Matcher::ransacTest(const std::vector<cv::DMatch>& symMatches
    , const std::vector<cv::KeyPoint>& keypoints_1
    , const std::vector<cv::KeyPoint>& keypoints_2
    , std::vector<cv::DMatch>& outMatches){

    // Convert keypoints into type Point2f
    std::vector<cv::Point2f> points_1, points_2;

    cv::TickMeter tmRansac;
    tmRansac.start();

    for (std::vector<cv::DMatch>::const_iterator it = symMatches.begin();
        it != symMatches.end(); ++it){

        // Get the position of left keypoints
        float x = keypoints_1[it->queryIdx].pt.x;
        float y = keypoints_1[it->queryIdx].pt.y;
        points_1.push_back(cv::Point2f(x, y));
        // Get the position of right keypoints
        x = keypoints_2[it->trainIdx].pt.x;
        y = keypoints_2[it->trainIdx].pt.y;
        points_2.push_back(cv::Point2f(x, y));
    }

    // Compute the F matrix using RANSAC
    std::vector<uchar> inliers(points_1.size(), 0);
    cv::Mat F_mat = cv::findFundamentalMat(
        cv::Mat(points_1), cv::Mat(points_2), // matching points
        inliers, // match status (inlier and outlier)
        CV_FM_RANSAC, // flag: RANSAC method
        m_dDistance, // distance to epipolar line
        m_dConfidence); // confidence probability

    // extract the filtered (inliers) matches
    std::vector<uchar>::const_iterator itIn = inliers.begin();
    std::vector<cv::DMatch>::const_iterator itM = symMatches.begin();

    // for all matches
    for (; itIn != inliers.end(); ++itIn, ++itM){
        // Valid match
        if (*itIn){
            outMatches.push_back(*itM);
        }
    }

    if (m_bRefineF){
        // The F-matrix will be recomputed with all accepted matches
        // Convert keypoints into Point2f for final F computation
        points_1.clear();
        points_2.clear();

        for (std::vector<cv::DMatch>::const_iterator it = outMatches.begin();
            it != outMatches.end(); ++it){
            // Get the position of left keypoints
            float x = keypoints_1[it->queryIdx].pt.x;
            float y = keypoints_1[it->queryIdx].pt.y;
            points_1.push_back(cv::Point2f(x, y));
            // Get the position of right keypoints
            x = keypoints_2[it->trainIdx].pt.x;
            y = keypoints_2[it->trainIdx].pt.y;
            points_2.push_back(cv::Point2f(x, y));
        }

        // Compute 8-point F from all accepted matches
        F_mat = cv::findFundamentalMat(
            cv::Mat(points_1), cv::Mat(points_2), // matching points
            CV_FM_8POINT); // flag, 8-point method
    } // END: f (m_bRefineF)

    tmRansac.stop();
    m_dTimeRansac = tmRansac.getTimeMilli();

    return F_mat;

}// END: cv::Mat Matcher::ransacTest()

void Matcher::runImageDetection(
    cv::Mat img
    , std::vector<cv::KeyPoint>& keypoints
    , int imageFlag
    ){

    cv::TickMeter tmDetect;
    tmDetect.start();

    m_detector->detect(img, keypoints);

    tmDetect.stop();
    if (imageFlag == IMG_SAMPLE){
        m_dTimeSURF_imgSample.x = tmDetect.getTimeMilli();
    }
    else m_dTimeSURF_imgTarget.x = tmDetect.getTimeMilli();
}// END void Matcher::runImageDetection()

void Matcher::runImageExtraction(
    cv::Mat img
    ,std::vector<cv::KeyPoint> keypoints
    ,cv::Mat& descriptors
    ,int imageFlag
    ){

    cv::TickMeter tmExtract;
    tmExtract.start();

    m_extractor->compute(img, keypoints, descriptors);

    tmExtract.stop();

    if (imageFlag == IMG_SAMPLE){
        m_dTimeSURF_imgSample.y = tmExtract.getTimeMilli();
    }
    else m_dTimeSURF_imgTarget.y = tmExtract.getTimeMilli();
}// END: void Matcher::runImageExtraction()

void Matcher::executeBFMatching(
    cv::Mat descriptorSample
    , cv::Mat descriptorTarget
    , std::vector<std::vector<cv::DMatch>>& matches_1to2
    , std::vector<std::vector<cv::DMatch>>& matches_2to1
    ){

    //  Construction of the matcher
    cv::BruteForceMatcher<cv::L2<float>> bfMatcher; // Using OpenCV Legacy.

    cv::TickMeter tmBFM;
    tmBFM.start();

    bfMatcher.knnMatch(descriptorSample, descriptorTarget,
        matches_1to2, // vector of matches (up to 2 per entry)
        2); // return 2 nearest neighbors

    tmBFM.stop();
    m_dTimeBFM.x = tmBFM.getTimeMilli();
    tmBFM.reset();

    tmBFM.start();

    bfMatcher.knnMatch(descriptorTarget, descriptorSample,
        matches_2to1, // vector of matches (up to 2 per entry)
        2); // return 2 nearest neighbors

    tmBFM.stop();
    m_dTimeBFM.y = tmBFM.getTimeMilli();
}

void Matcher::executeRatioAndSymmetryTest(std::vector<std::vector<cv::DMatch>>& matches_1to2
    , std::vector<std::vector<cv::DMatch>>& matches_2to1
    , std::vector<cv::DMatch>& symMatches){

    cv::TickMeter tmSymTest;

    // 3. Remove matches for which NN ratio > threshold
    // 3.a Clean image 1 -> image 2 matches
    tmSymTest.start();
    int removed_1to2 = ratioTest(matches_1to2);
    int removed_2to1 = ratioTest(matches_2to1);

    tmSymTest.stop();
    m_dTimeRatioAndSymTest.x = tmSymTest.getTimeMilli();
    tmSymTest.reset();

    m_matchPointsCount.x = (int) matches_1to2.size() - removed_1to2;
    m_matchPointsCount.y = (int) matches_2to1.size() - removed_2to1;

    // 4. Remove non-symmetrical matches
    tmSymTest.start();
    symmetryTest(matches_1to2, matches_2to1, symMatches);

    tmSymTest.stop();
    m_dTimeRatioAndSymTest.y = tmSymTest.getTimeMilli();

    m_dSymMatchCount = double(symMatches.size());
}// END: void Matcher::executeRatioAndSymmetryTest()

cv::Mat Matcher::executeRansacTest(std::vector<cv::DMatch>& symMatches
    , std::vector<cv::KeyPoint>& keypoints_1
    , std::vector<cv::KeyPoint>& keypoints_2
    , std::vector<cv::DMatch>& matchResult){

    // RANSAC
    cv::TickMeter tmRansac;
    tmRansac.start();
    m_Fmat = ransacTest(symMatches, keypoints_1, keypoints_2, matchResult);
    tmRansac.stop();
    m_dTimeRansac = tmRansac.getTimeMilli();

    return m_Fmat;
}// END: void Matcher::executeRansacTest()