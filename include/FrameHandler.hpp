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
#ifndef __FRAME_HANDLER_HPP__
#define __FRAME_HANDLER_HPP__

#include <VideoAnalyzer.hpp>
#include <fstream>


class FrameHandler : public FrameCallback {
public:
	std::string & outputDir() { return _outputDir; }
	std::string & imagePrefix() { return _imagePrefix; }
	std::string & imageFormat() { return _imageFormat; }
	std::string & thumbDir() { return _thumbDir; }
	std::string & thumbPrefix() { return _thumbPrefix; }
	std::string & thumbFormat() { return _thumbFormat; }
	std::string & sequenceFileName() { return _sequenceFileName; }
	int & thumbWidth() { return _thumbWidth; }
	bool & generateImages() { return _generateImages; }
	bool & generateThumbnails() { return _generateThumbnails; }
	bool & generateSequence() { return _generateSequence; }
	bool & mpeg7Output() { return _mpeg7Output; }
	std::string & mediaUri() { return _mediaUri; }
	
	void setOutputDir(const std::string & value) { _outputDir = value; }
	void setImagePrefix(const std::string & value) { _imagePrefix = value; }
	void setImageFormat(const std::string & value) { _imageFormat = value; }
	void setThumbDir(const std::string & value) { _thumbDir = value; }
	void setThumbPrefix(const std::string & value) { _thumbPrefix = value; }
	void setThumbFormat(const std::string & value) { _thumbFormat = value; }
	void setSequenceFileName(const std::string & value) { _sequenceFileName = value; }
	void setThumbWidth(int value) { _thumbWidth = value; }
	void setGenerateImages(bool value) { _generateImages = value; }
	void setGenerateThumbnails(bool value) { _generateThumbnails = value; }
	void setGenerateSequence(bool value) { _generateSequence = value; }
	void setMpeg7Output(bool value) { _mpeg7Output = value; }
	void setMediaUri(const std::string & value) { _mediaUri = value; }
	
	FrameHandler()
		:_outputDir("")
		,_imagePrefix("frame")
		,_imageFormat("jpg")
		,_thumbDir("")
		,_thumbPrefix("thumb")
		,_thumbFormat("jpg")
		,_sequenceFileName("data.xml")
		,_thumbWidth(200)
		,_generateImages(true)
		,_generateThumbnails(false)
		,_generateSequence(true)
		,_mpeg7Output(false)
		,_mediaUri("")
	{
    }
	
	
	virtual void analysisStarted();
	virtual void frameDetected(const cv::Mat & frame, unsigned int time, int frameIndex);
	virtual void analysisFinished();
	
	virtual ~FrameHandler() {}

protected:
	std::string _outputDir;
	std::string _imagePrefix;
	std::string _imageFormat;
	std::string _thumbDir;
	std::string _thumbPrefix;
	std::string _thumbFormat;
	std::string _sequenceFileName;
	int _thumbWidth;
	bool _generateImages;
	bool _generateThumbnails;
	bool _generateSequence;
	bool _mpeg7Output;
	std::string _mediaUri;
	
	std::string getImageName(int time, int frameIndex);
	std::string getThumbName(int time, int frameIndex);
	
	void openFile();
	void writeFrame(int time,int frameIndex);
	void closeFile();
	
	
	void openMpeg7Frame(int time, int frame);
	void closePreviousMpeg7Frame(int duration);
	
	std::string getRelTimePoint(int time);
	std::string getMediaDuration(int time);
	
	std::ofstream _fileStream;
	int _videoDuration;
	int _previousFrameTime;
};


#endif
