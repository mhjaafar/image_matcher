/*
matcher.hpp

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
#ifndef MATCHER_HPP
#define MATCHER_HPP

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

class Matcher{
	private:
		// Pointer to the feature point detector object
		cv::Ptr<cv::FeatureDetector> m_detector;
		// Pointer to the feature descriptor extractor object
		cv::Ptr<cv::DescriptorExtractor> m_extractor;
		// Max. ratio between the 1st and 2nd NN
		float m_fRatio;
		// To refine the F-matrix if true
		bool m_bRefineF;
		// Min. distance to epipolar
		double m_dDistance;
		// Confidence level (probability)
		double m_dConfidence;
		// No. of matched points:
		cv::Point2i m_matchPointsCount;
		// No. of symmetrical match points
		double m_dSymMatchCount;
		// Elapsed time BruteForceMatcher
		cv::Point2d m_dTimeBFM;
		// Elapsed time for RANSAC test
		double m_dTimeRansac;
		// Elapsed time for ratio and symmetry test
		cv::Point2d m_dTimeRatioAndSymTest;
		// Elapsed time for SURF features detection and extraction
		cv::Point2d m_dTimeSURF_imgSample;
		// Elapsed time for SURF features detection and extraction
		cv::Point2d m_dTimeSURF_imgTarget;
		// The calculated fundamental matrix from the RANSAC test
		cv::Mat m_Fmat;

	public:
		Matcher();
		~Matcher();

		enum MatcherFlag
		{
			IMG_SAMPLE,
			IMG_TARGET
		};

		cv::Point2i getMatchPointsCount() { return m_matchPointsCount; }
		double getSymmetryMatchesCount() { return m_dSymMatchCount; }
		cv::Point2d getTimeBFM() { return m_dTimeBFM; }
		cv::Point2d getTimeRatioAndSymTest() { return m_dTimeRatioAndSymTest; }
		double getTimeRansac() { return m_dTimeRansac; }
		cv::Point2d getTimeSURF_imgSample() { return m_dTimeSURF_imgSample; }
		cv::Point2d getTimeSURF_imgTarget() { return m_dTimeSURF_imgTarget; }
		cv::Mat getFmatrix() { return m_Fmat; }

		void runImageDetection(cv::Mat img, std::vector<cv::KeyPoint>& keypoints, int imageFlag=IMG_SAMPLE);
		void runImageExtraction(cv::Mat img, std::vector<cv::KeyPoint> keypoints, cv::Mat& descriptors, int imageFlag=IMG_SAMPLE);
		/*
			@brief: Uses BruteForceMatcher to match sample image to target image, and vice-versa.
		*/
		void executeBFMatching(cv::Mat descriptorSample,
			cv::Mat descriptorTarget,
			std::vector<std::vector<cv::DMatch>>& matches_1to2,
			std::vector<std::vector<cv::DMatch>>& matches_2to1);
		
		void executeRatioAndSymmetryTest(std::vector<std::vector<cv::DMatch>>& matches_1to2,
			std::vector<std::vector<cv::DMatch>>& matches_2to1,
			std::vector<cv::DMatch>& symMatches);

		cv::Mat executeRansacTest(std::vector<cv::DMatch>& symMatches,
			std::vector<cv::KeyPoint>& keypoints_1,
			std::vector<cv::KeyPoint>& keypoints_2,
			std::vector<cv::DMatch>& matchResult);

		void setFeatureDetector(cv::Ptr<cv::FeatureDetector>& detect){
			m_detector = detect;
		}

		void setDescriptorExtractor(cv::Ptr<cv::DescriptorExtractor>& desc){
			m_extractor = desc;
		}

		void setMinDistanceToEpipolar(double dMinDistToEpipolar){
			m_dDistance = dMinDistToEpipolar;
		}

		void setConfidenceLevel(double dConfidenceLevel){
			m_dConfidence = dConfidenceLevel;
		}

		void setRatio(float fRatio){
			m_fRatio = fRatio;
		}

		/*
			@brief: If the F-matrix needs to be recalculated.
		*/
		void refineFundamental(bool bFlag){
			m_bRefineF = bFlag;
		}

		/*
		 @brief: Clear matches for which NN ratio is > than threshold
		 @return removed: The number of removed points.
		*/
		int ratioTest(std::vector<std::vector<cv::DMatch>>& matches);

		/*
			@brief: Insert symmetrical matches in symMatches vector
		*/
		void symmetryTest(const std::vector <std::vector<cv::DMatch>>& matches_1,
			const std::vector<std::vector<cv::DMatch>>& matches_2,
			std::vector<cv::DMatch>& symMatches);

		/*
			@brief: Identify good matches using RANSAC.
			@return fundamentalMatrix
		*/
		cv::Mat ransacTest(const std::vector<cv::DMatch>& matches,
			const std::vector<cv::KeyPoint>& keypoints_1,
			const std::vector<cv::KeyPoint>& keypoints_2,
			std::vector<cv::DMatch>& outMatches);

		/*
			@brief: Match feature points using symmetry test and RANSAC
			@return: F_mat Fundamental matrix
		*/
		cv::Mat match(cv::Mat& image_1, cv::Mat& image_2, // input images
			std::vector<cv::DMatch>& matches, // output matches and keypoints
			std::vector<cv::KeyPoint>& keypoints_1, std::vector<cv::KeyPoint>& keypoints_2);
};


#endif