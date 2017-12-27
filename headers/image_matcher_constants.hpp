/*
image_matcher_constants.hpp

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
#ifndef IMAGE_MATCHER_CONSTANTS_HPP
#define IMAGE_MATCHER_CONSTANTS_HPP

#include <string>
#include <QString>

namespace ImgMatcher{
    const std::string PluginName = "Image_Matcher";
    const std::string PluginNameInCommand = "Image Matcher";
    const std::string QToolBoxes = "QToolBoxes";
    const std::string ConfidenceLevel = "ConfidenceLevel";
    const std::string RatioNN1to2 = "RatioNN1to2";
    const std::string RefineFMatrixEnabled = "RefineFMatrix";
    const std::string MinDistanceToEpipolar = "MinDistToEpipolar";
    const std::string MatchesDrawOption = "MatchesDrawOption";
    const std::string ResizeInputImageEnabled = "ResizeInputImg";
    const std::string ResizeInputImagePercentage = "ResizeInputImgPercentage";
    const std::string PathToImgSample = "PathImgSample";
    const std::string PathToImgTarget = "PathImgTarget";
    const std::string SurfFeatureDetector = "SurfFeatureDetector";
    const std::string SurfFeatureOctaves = "Octaves";
    const std::string PluginTasks = "Tasks";
    const std::string FanartPurger = "FanartPurger";
    const std::string FacialRecognition = "FacialRecognition";
    const std::string TaskSamples = "Samples";
    const std::string TaskTargets = "Targets";
    const std::string TargetCount = "TargetCount";
    const std::string SampleCount = "SampleCount";
    const std::string MatchingParameters = "MatchingParameters";

    const QString CmdToolBox("ImageMatcherToolBox");
    const QString CmdDummy("ImageMatcherDummy");
    const QString CmdPrintCurrentPickedTask("ImageMatcherPrintCurrentPickedTask");
    const QString CmdRestorePreviousImage("ImageMatcherRestorePreviousImage");
    const QString CmdInitDocViews("ImageMatcherInitDocViews");

    const QString DocNameImgSampleCurrent("Current Sample Image");
    const QString DocNameImgSampleNext("Next Sample Image");
    const QString DocNameImgTargetCurrent("Current Target Image");
    const QString DocNameImgTargetNext("Next Target Image");

    const QString IniFileName("plugin_ImageMatcher.ini");
    const QString XmlFileName("plugin_ImageMatcher.xml");
    const QString XmlFileOutputName("plugin_ImageMatcher_Output.xml");
}// END: namespace ImgMatcher

#endif