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
#ifndef _ANALYZER_CALLBACK_HPP_
#define	_ANALYZER_CALLBACK_HPP_

#include <VideoAnalyzer.hpp>

class HistogramAnalyzer : public AnalyzerCallback {
public:
	HistogramAnalyzer() :_detectionTresshold(96.0f), _debug(false) {}

	bool detectionTresshold() const { return _detectionTresshold; }
	void setDetectionTresshold(float value) { _detectionTresshold = value; }
	bool debug() const { return _debug; }
	void setDebug(bool value) { _debug = value; }

	virtual bool analyze(const cv::Mat & frameA, const cv::Mat & frameB);
	virtual ~HistogramAnalyzer() {}

protected:
	float _detectionTresshold;
	bool _debug;
};

#endif
