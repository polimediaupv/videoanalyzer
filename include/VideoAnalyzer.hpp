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
#ifndef _VIDEOANALYZER_HPP_
#define _VIDEOANALYZER_HPP_

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class VideoAnalyzer;

class VideoAnalyzerCallback {
public:
	void setAnalyzer(VideoAnalyzer * analyzer) { _analyzer = analyzer; }

protected:
	VideoAnalyzer * _analyzer;
};

class AnalyzerCallback : public VideoAnalyzerCallback {
public:
	virtual bool analyze(const cv::Mat & frameA, const cv::Mat & frameB) = 0;
	virtual ~AnalyzerCallback() {}
};

class FrameCallback : public VideoAnalyzerCallback {
public:
	virtual void analysisStarted() = 0;
	virtual void frameDetected(const cv::Mat & frame, unsigned int time, int frameIndex) = 0;
	virtual void analysisFinished() = 0;

	virtual ~FrameCallback() {}
};

class VideoAnalyzer {
public:
	VideoAnalyzer();

	bool loadVideo(const std::string & path);

	void addFrameCallback(FrameCallback * callback);
	void setAnalyzerCallback(AnalyzerCallback * callback);

	void setFrameStep(int step) { _frameStep = step; }
	int getFrameStep() const { return _frameStep; }

	// Returns true if there are more frames to process
	bool nextFrame();

	const cv::Mat & getCurrentFrame() const { return _currentFrame; }
	bool frameDetected() const { return _changeDetected; }
	int getCurrentFrameIndex() const { return _currentFrameIndex; }
	int getFrameCount() const { return _frameCount; }
	int getDuration() const { return _duration; }
	double getFrameRate() const { return _frameRate; }

	void release();
	
protected:
	std::vector<FrameCallback*> _frameCallbacks;
	AnalyzerCallback * _analyzerCallback;
	int _frameStep;
	int _detectionStep;
	
	cv::Mat _currentFrame;
	int _currentFrameIndex;
	int _nextFrameIndex;
	bool _changeDetected;
	int _analyzedFrames;

	cv::Mat _oddFrame;
	cv::Mat _evenFrame;
	cv::VideoCapture * _videoCapture;
	int _frameCount;
	int _duration;
	double _frameRate;
	
	void analysisStarted();
	void callFrameCallback();
	void analysisFinished();
	void releaseFrameCallback();
	void releaseAnalyzerCallback();
	void releaseVideoCapture();
	void releaseFrames();
};

#endif
