/*
robustMatcher.cpp

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
#include <opencv2/opencv.hpp>
#include <workspace.h>
#include <workspace_if.h>
#include <QString>
#include <vector>
#include "headers/robustMatcher.hpp"
#include "headers/image_matcher_constants.hpp"

RobustMatcher::RobustMatcher(MWorkspaceIF* pWs){

    //pWs->settingsIF()->getItem(ocvt::PluginName + "/" + ocvt::Chapter09_RobustMatching + "/" + ocvt::ResizeInputImageEnabled, m_bResizeInputImages, false);
    //pWs->settingsIF()->getItem(ocvt::PluginName + "/" + ocvt::Chapter09_RobustMatching + "/" + ocvt::ResizeInputImagePercentage, m_iResizePercentage, 75);

    m_bImgSampleOkay = false;
    m_bImgTargetOkay = false;
}

RobustMatcher::~RobustMatcher(){
}

void RobustMatcher::initMatcher(MWorkspaceIF* pWs){

    //double dMinDistToEpipolar;
    //double dConfidenceLevel;
    //double dRatio;
    //bool bRefineFMatrix;
    //int iSFD_octaves;

    //pWs->settingsIF()->getItem(ocvt::PluginName + "/" + ocvt::Chapter09_RobustMatching + "/" + ocvt::MinDistanceToEpipolar, dMinDistToEpipolar, 3.0);
    //pWs->settingsIF()->getItem(ocvt::PluginName + "/" + ocvt::Chapter09_RobustMatching + "/" + ocvt::ConfidenceLevel, dConfidenceLevel, 99.0);
    //pWs->settingsIF()->getItem(ocvt::PluginName + "/" + ocvt::Chapter09_RobustMatching + "/" + ocvt::RatioNN1to2, dRatio, 0.65);
    //pWs->settingsIF()->getItem(ocvt::PluginName + "/" + ocvt::Chapter09_RobustMatching + "/" + ocvt::RefineFMatrixEnabled, bRefineFMatrix, true);
    //pWs->settingsIF()->getItem(ocvt::PluginName + "/" + ocvt::Chapter09_RobustMatching + "/" + ocvt::SurfFeatureDetector
    //	+ "/" + ocvt::SurfFeatureOctaves, iSFD_octaves, 10);
    //dConfidenceLevel /= 100.0;

    //m_matcher.setConfidenceLevel(dConfidenceLevel);
    //m_matcher.setRatio(float(dRatio));
    //m_matcher.setMinDistanceToEpipolar(dMinDistToEpipolar);
    //m_matcher.refineFundamental(bRefineFMatrix);
    //
    //cv::Ptr<cv::FeatureDetector> pfd = new cv::SurfFeatureDetector(iSFD_octaves);

    //m_matcher.setFeatureDetector(pfd);
} // END: void RobustMatcher::initMatcher()

/*
    @brief: Match the two images
    @return: The success of the matching
*/
bool RobustMatcher::runTwoImagesMatching(){
    if (!m_bImgSampleOkay || !m_bImgTargetOkay) return false;

    std::vector<std::vector<cv::DMatch>> matches_1to2, matches_2to1;

    m_matcher.executeBFMatching(m_descriptors_1,
        m_descriptors_2,
        matches_1to2,
        matches_2to1);

    if (m_symMatches.size() != 0) m_symMatches.clear();

    m_matcher.executeRatioAndSymmetryTest(matches_1to2,
        matches_2to1,
        m_symMatches);

    if (m_symMatches.size() == 0) return false;

    m_F_mat = m_matcher.executeRansacTest(m_symMatches,
        m_keypoints_1,
        m_keypoints_2,
        m_matches);

    if (m_matches.size() == 0) return false;

    return true;
}

/*
    @brief: Set image 1 or the image sample.
    @param imgSample: Input image 1
*/
void RobustMatcher::setImageSample(cv::Mat imgSample){

    // TODO: remove this method

    if (imgSample.channels() > 1){
        cv::cvtColor(imgSample, m_img_1, CV_RGB2GRAY);
    }
    else{
        if (m_bResizeInputImages){
            cv::Size newSize = cv::Size(imgSample.cols*m_iResizePercentage / 100
                , imgSample.rows*m_iResizePercentage / 100
                );
            cv::resize(imgSample, m_img_1, newSize);
        }
        else m_img_1 = imgSample.clone();
    }

}// END: void RobustMatcher::setImageSample()

bool RobustMatcher::initImage(
    cv::Mat imgSrc
    , cv::Mat& imgClone
    , std::vector<cv::KeyPoint>& keypoints
    , cv::Mat& descriptors
    , int flag
    ) {
    if (imgSrc.channels() > 1){
        cv::cvtColor(imgSrc, imgClone, CV_RGB2GRAY);
    }
    else{
        if (m_bResizeInputImages){
        cv::Size newSize = cv::Size(imgSrc.cols*m_iResizePercentage / 100,
            imgSrc.rows*m_iResizePercentage / 100);
        cv::resize(imgSrc, imgClone, newSize);
        }
        else imgClone = imgSrc.clone();
    }

    m_matcher.runImageDetection(imgClone, keypoints, flag);
    if (keypoints.size() == 0) return false;

    m_matcher.runImageExtraction(imgClone, keypoints, descriptors, flag);

    return true;
}// END: bool RobustMatcher::initImage()

bool RobustMatcher::initImageSample(cv::Mat imgSample){

    m_bImgSampleOkay = initImage(imgSample
        ,m_img_1
        ,m_keypoints_1
        ,m_descriptors_1
        ,m_matcher.IMG_SAMPLE
    );

    return m_bImgSampleOkay;

}// END: bool RobustMatcher::initImageSample(cv::Mat imgSample)

bool RobustMatcher::initImageTarget(cv::Mat imgTarget){

    m_bImgTargetOkay = initImage(imgTarget
        ,m_img_2
        ,m_keypoints_2
        ,m_descriptors_2
        ,m_matcher.IMG_TARGET
        );

    return m_bImgTargetOkay;

}// END:  bool RobustMatcher::initImageTarget(cv::Mat imgTarget)

/*
@brief: Set the second image - the target to be tested.
@param imgTarget: The image to be matched with the sample image.
*/
void RobustMatcher::setImageTarget(cv::Mat imgTarget){

    // TODO: remove this method

    if (imgTarget.channels() > 1){
        cv::cvtColor(imgTarget, m_img_2, CV_RGB2GRAY);
    }
    else{
        if (m_bResizeInputImages){
        cv::Size newSize = cv::Size(imgTarget.cols*m_iResizePercentage / 100,
            imgTarget.rows*m_iResizePercentage / 100);
        cv::resize(imgTarget, m_img_2, newSize);
        }
        else m_img_2 = imgTarget.clone();
    }

}// END: void RobustMatcher::setImageTarget()

/*
    @brief: Convert keypoints and draw circles of the match points.
*/
void RobustMatcher::convertKeypointsAndDrawCircles(){

    for (std::vector<cv::DMatch>::const_iterator it = m_matches.begin();
        it != m_matches.end(); ++it){

        // Get the position of left keypoints
        float x_1 = m_keypoints_1[it->queryIdx].pt.x;
        float y_1 = m_keypoints_1[it->queryIdx].pt.y;
        m_points_1.push_back(cv::Point2f(x_1, y_1));
        cv::circle(m_img_1_circlesAndLines, cv::Point(x_1, y_1), 3, cv::Scalar(255, 255, 255), 3);

        // Get the position of right keypoints
        float x_2 = m_keypoints_2[it->trainIdx].pt.x;
        float y_2 = m_keypoints_2[it->trainIdx].pt.y;
        m_points_2.push_back(cv::Point2f(x_2, y_2));
        cv::circle(m_img_2_circlesAndLines, cv::Point(x_2, y_2), 3, cv::Scalar(255, 255, 255), 3);
    }

}// END: void RobustMatcher::convertKeypoints()

/*
    @brief: Draw the circles on keypoints and corresponding epipolar lines.
*/
bool RobustMatcher::drawCirclesAndLines(cv::Mat& imgDst_1, cv::Mat& imgDst_2){

    if (m_img_1.empty() || m_img_2.empty()){
        return false;
    }

    if (m_img_1.rows != m_img_2.rows || m_img_1.cols != m_img_2.cols){
        return false;
    }

    m_img_1_circlesAndLines = m_img_1.clone();
    m_img_2_circlesAndLines = m_img_2.clone();

    convertKeypointsAndDrawCircles();

    // Draw lines
    std::vector<cv::Vec3f> lines_1;
    cv::computeCorrespondEpilines(cv::Mat(m_points_1), 1, m_F_mat, lines_1);

    for (std::vector<cv::Vec3f>::const_iterator it = lines_1.begin();
        it != lines_1.end(); ++it){
        cv::line(m_img_2_circlesAndLines, cv::Point(0, -(*it)[2] / (*it)[1]),
        cv::Point(m_img_2_circlesAndLines.cols, -((*it)[2] + (*it)[0] * m_img_2_circlesAndLines.cols) / (*it)[1]),
        cv::Scalar(255, 255, 255));
    }

    std::vector<cv::Vec3f> lines_2;
    cv::computeCorrespondEpilines(cv::Mat(m_points_2), 2, m_F_mat, lines_2);

    for (std::vector<cv::Vec3f>::const_iterator it = lines_2.begin();
        it != lines_2.end(); ++it){

        cv::line(m_img_1_circlesAndLines, cv::Point(0, -(*it)[2] / (*it)[1]),
            cv::Point(m_img_1_circlesAndLines.cols, -((*it)[2] + (*it)[0] * m_img_1_circlesAndLines.cols) / (*it)[1]),
            cv::Scalar(255, 255, 255));
    }

    imgDst_1 = m_img_1_circlesAndLines.clone();
    imgDst_2 = m_img_2_circlesAndLines.clone();

    return true;
}// END: bool RobustMatcher::drawEpipolarLines()

/*
    @brief: Draw the matches
    @param imgDst: The image containing image 1 and 2 with the matches.
*/
bool RobustMatcher::drawMatches(cv::Mat& imgDst){

    if (m_matches.size() == 0){
        return false;
    }

    cv::drawMatches(m_img_1, m_keypoints_1, // 1st image and its keypoints
        m_img_2, m_keypoints_2,
        m_matches,
        imgDst,
        cv::Scalar(255, 255, 255));

    return true;

}// END: void RobustMatcher::drawMatches()