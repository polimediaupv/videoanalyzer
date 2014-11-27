/*
 Video Analyzer
 Copyright (C) 2012  Fernando Serrano Carpena - Universidad Politécnica de Valencia
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 */
#include <HistogramAnalyzer.hpp>
#include <iomanip>

bool HistogramAnalyzer::analyze(const cv::Mat & frameA, const cv::Mat & frameB) {
	if (frameA.rows!=frameB.rows || frameA.cols!=frameB.cols) {
		return false;
	}
	cv::Mat diffImage = frameA - frameB;
	
	int histSize = 4;
	float range[] = { 0, 255};
	const float * histRange = { range };
	bool uniform = true;
	bool accumulate = false;
	int channels = 0;
	cv::Mat histogram;
	
	cv::calcHist(&diffImage, 1, &channels, cv::Mat(), histogram, 1, &histSize, &histRange, uniform, accumulate);
	
	float blackValue = histogram.at<float>(0);
	float darkGrayValue = histogram.at<float>(1);
	float lightGrayValue = histogram.at<float>(2);
	float whiteValue = histogram.at<float>(3);
	float total = blackValue + darkGrayValue + lightGrayValue + whiteValue;

	float blackPercent = blackValue / total * 100;
	//float darkGrayPercent = darkGrayValue / total * 100;
	//float lightGrayPercent = lightGrayValue / total * 100;
	//float whitePercent = whiteValue / total * 100;
	
	
	if (blackPercent<=_detectionTresshold) {
		if (_debug) std::cout << "Frame change detected with tresshold: " << blackPercent << std::endl;
		return true;
	}
	else {
		return false;
	}
}