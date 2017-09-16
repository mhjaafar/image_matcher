/*
chapter09_robustMatching.hpp

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
#ifndef CHAPTER09_ROBUSTMATCHING_HPP
#define CHAPTER09_ROBUSTMATCHING_HPP

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "headers/matcher.hpp"
#include <workspace.h>
#include <workspace_if.h>
#include <QString>

bool robustMatcher(MWorkspaceIF* pWs, cv::Mat img_1, cv::Mat img_2);

class RobustMatcher{
	private:
		Matcher m_matcher;
		cv::Mat m_F_mat;
		cv::Mat m_img_1;
		cv::Mat m_img_1_circlesAndLines;
		cv::Mat m_img_2;
		cv::Mat m_img_2_circlesAndLines;
		std::vector<cv::DMatch> m_matches;
		std::vector<cv::KeyPoint> m_keypoints_1, m_keypoints_2;
		std::vector<cv::Point2f> m_points_1, m_points_2;
		cv::Mat m_descriptors_1, m_descriptors_2;
		std::vector<cv::DMatch> m_symMatches;

		// Optional: Resize the images to be smaller, to speed up processing
		bool m_bResizeInputImages;
		// Optional: Downscale the image to this percentage
		int m_iResizePercentage;
		/*
		@brief: Convert keypoints into Point2f.
		*/
		void convertKeypointsAndDrawCircles();

		bool initImage(cv::Mat imgSrc
			,cv::Mat& imgClone
			,std::vector<cv::KeyPoint>& keypoints
			,cv::Mat& descriptors
			,int flag
			);

	public:
		RobustMatcher(MWorkspaceIF* pWs);
		~RobustMatcher();

		bool m_bImgSampleOkay;
		bool m_bImgTargetOkay;

		size_t getMatchesCount(){ return m_matches.size(); }

		Matcher getMatcher(){ return m_matcher; }

		/*
			@brief: Set the first image - the sample.
			@param imgSample: The first image or the sample image.
		*/
		void setImageSample(cv::Mat imgSample);
		/*
		@brief: Initialize the first image - the sample, running detection and extraction.
		@param imgSample: The first image or the sample image.
		*/
		bool initImageSample(cv::Mat imgSample);

		/*
			@brief: Set the second image - the target to be tested.
			@param imgTarget: The image to be matched with the sample image.
		*/
		void setImageTarget(cv::Mat imgTarget);
		/*
		@brief: Initialize the second image - the target, running detection and extraction.
		@param imgTarget: The image to be matched with the sample image.
		*/
		bool initImageTarget(cv::Mat imgTarget);

		/*
			@brief Get the calculated fundamental matrix.
			@return: m_F_mat
		*/
		cv::Mat getFundamentalMatrix(){
			return m_F_mat;
		}

		/*
			@brief: Draw the matches
			@param imgDst: The image containing image 1 and 2 with the matches.
		*/
		bool drawMatches(cv::Mat& imgDst);

		/*
			@brief: Prepare the matcher.
			@param pWs: Pointer to the workspace interface.
		*/
		void initMatcher(MWorkspaceIF* pWs);

		/*
			@brief: Draw the circles on keypoints and corresponding epipolar lines.
		*/
		bool drawCirclesAndLines(cv::Mat& imgDst_1, cv::Mat& imgDst_2);

		/*
		@brief: Match the two images
		@return: The success of the matching
		*/
		bool runTwoImagesMatching();
};

#endif