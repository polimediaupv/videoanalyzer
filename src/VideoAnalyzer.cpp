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
#include <VideoAnalyzer.hpp>

VideoAnalyzer::VideoAnalyzer()
	:_analyzerCallback(NULL),
	_frameStep(25),
	_detectionStep(250),
	_currentFrameIndex(0),
	_nextFrameIndex(0),
	_changeDetected(false),
	_analyzedFrames(0),
	_frameRate(0.0),
	_videoCapture(NULL)
{
	
}

bool VideoAnalyzer::loadVideo(const std::string & path) {
	_videoCapture = new cv::VideoCapture(path);
	if (_videoCapture->isOpened()) {
		_frameCount = _videoCapture->get(cv::CAP_PROP_FRAME_COUNT);
        _frameRate = _videoCapture->get(cv::CAP_PROP_FPS);
		_duration = _frameCount * 1/_frameRate;
		if (_frameCount<=0 || _frameRate<=0) {
			releaseVideoCapture();
			return false;
		}
		return true;
	}
	else {
		releaseVideoCapture();
		return false;
	}
}

void VideoAnalyzer::addFrameCallback(FrameCallback * callback) {
	_frameCallbacks.push_back(callback);
	callback->setAnalyzer(this);
}

void VideoAnalyzer::setAnalyzerCallback(AnalyzerCallback * callback) {
	releaseAnalyzerCallback();
	_analyzerCallback = callback;
	_analyzerCallback->setAnalyzer(this);
}

bool VideoAnalyzer::nextFrame() {
	if (_videoCapture==NULL) return false;
	if (_currentFrameIndex>=_frameCount) return false;
	
    _videoCapture->set(cv::CAP_PROP_POS_FRAMES, _nextFrameIndex);
	(*_videoCapture) >> _currentFrame;
	
	if (_analyzedFrames%2==0) {
		_evenFrame.release();
		_evenFrame = _currentFrame.clone();
	}
	else {
		_oddFrame.release();
		_oddFrame = _currentFrame.clone();
	}
	
	if (_currentFrameIndex==0) {
		analysisStarted();
	}
	else if (_analyzerCallback && _analyzerCallback->analyze(_evenFrame, _oddFrame)) {
		callFrameCallback();
	}

	++_analyzedFrames;
	_nextFrameIndex += _frameStep;
	_currentFrameIndex = _nextFrameIndex;
	
	if (_nextFrameIndex>=_frameCount) {
		analysisFinished();
	}
	
	return true;
}

void VideoAnalyzer::release() {
	releaseFrameCallback();
	releaseAnalyzerCallback();
	releaseFrames();
	releaseVideoCapture();
}

void VideoAnalyzer::analysisStarted() {
	std::vector<FrameCallback*>::iterator it;
	for (it=_frameCallbacks.begin(); it!=_frameCallbacks.end(); ++it) {
		FrameCallback * callback = *it;
		unsigned int time = static_cast<unsigned int>(_currentFrameIndex / _frameRate);
		callback->analysisStarted();
		callback->frameDetected(_currentFrame, time, _currentFrameIndex);
	}
}

void VideoAnalyzer::callFrameCallback() {
	std::vector<FrameCallback*>::iterator it;
	for (it=_frameCallbacks.begin(); it!=_frameCallbacks.end(); ++it) {
		FrameCallback * callback = *it;
		unsigned int time = static_cast<unsigned int>(_currentFrameIndex / _frameRate);
		callback->frameDetected(_currentFrame, time, _currentFrameIndex);
		if (_nextFrameIndex>=_frameCount) {
			callback->analysisFinished();
		}
	}
}

void VideoAnalyzer::analysisFinished() {
	std::vector<FrameCallback*>::iterator it;
	for (it=_frameCallbacks.begin(); it!=_frameCallbacks.end(); ++it) {
		(*it)->analysisFinished();
	}
}

void VideoAnalyzer::releaseFrameCallback() {
	std::vector<FrameCallback*>::iterator it;
	for (it=_frameCallbacks.begin(); it!=_frameCallbacks.end(); ++it) {
		FrameCallback * callback = *it;
		delete callback;
	}
	_frameCallbacks.clear();
}

void VideoAnalyzer::releaseAnalyzerCallback() {
	if (_analyzerCallback) {
		delete _analyzerCallback;
		_analyzerCallback = NULL;
	}
}

void VideoAnalyzer::releaseVideoCapture() {
	if (_videoCapture) {
		_videoCapture->release();
		delete _videoCapture;
		_videoCapture = NULL;
	}
}

void VideoAnalyzer::releaseFrames() {
	_currentFrameIndex = 0;
	_nextFrameIndex = 0;
	_frameCount = 0;
	_changeDetected = false;
	_analyzedFrames = 0;
	_evenFrame.release();
	_oddFrame.release();
	_currentFrame.release();
	_frameRate = 0.0;
}