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
#include <FrameHandler.hpp>
#include <sstream>

typedef struct {
	int days;
	int hours;
	int minutes;
	int seconds;
} TimeFormat;

#define TO_DAYS		86400
#define TO_HOURS	3600
#define TO_MINUTES	60

void formatTime(int seconds, TimeFormat & outputFormat) {
	outputFormat.days = static_cast<int>(seconds / TO_DAYS);
	seconds = seconds % TO_DAYS;
	outputFormat.hours = static_cast<int>(seconds / TO_HOURS);
	seconds = seconds % TO_HOURS;
	outputFormat.minutes = static_cast<int>(seconds / TO_MINUTES);
	seconds = seconds % TO_MINUTES;
	outputFormat.seconds = static_cast<int>(seconds);
}

void FrameHandler::analysisStarted() {
	if (_generateSequence) {
		openFile();
	}
}

void FrameHandler::frameDetected(const cv::Mat & frame, unsigned int time, int frameIndex) {
	if (frame.rows==0 || frame.cols==0) return;
	if (_generateSequence) {
		writeFrame(time, frameIndex);
	}
	if (_generateImages) {
		cv::imwrite(getImageName(time, frameIndex), frame);
	}
	if (_generateThumbnails) {
		cv::Mat thumbFrame;
		cv::Size thumbSize;
		thumbSize.width = _thumbWidth;
		thumbSize.height = frame.rows * _thumbWidth / frame.cols;
		cv::resize(frame, thumbFrame, thumbSize);
		cv::imwrite(getThumbName(time,frameIndex), thumbFrame);
	}
}

void FrameHandler::analysisFinished() {
	if (_generateSequence) {
		closeFile();
	}
}

std::string FrameHandler::getImageName(int time, int frameIndex) {
	std::stringstream imageFile;
	imageFile << _outputDir << _imagePrefix << "." << time << "." << _imageFormat;
	return imageFile.str();
}

std::string FrameHandler::getThumbName(int time, int frameIndex) {
	std::stringstream thumbFile;
	thumbFile << _thumbDir << _thumbPrefix << "." << time << "." << _thumbFormat;
	return thumbFile.str();
}

void FrameHandler::openFile() {
	std::string outFile = _outputDir + _sequenceFileName;
	_fileStream.open(outFile.c_str());
	_previousFrameTime = -1;
	if (!_mpeg7Output) {
		_fileStream << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
		_fileStream << "<sequence duration=\"" << _analyzer->getDuration() << "\">" << std::endl;
	}
	else {
		_fileStream << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
		_fileStream << "<Mpeg7 xmlns=\"urn:mpeg:mpeg7:schema:2001\" xmlns:mpeg7=\"urn:mpeg7:schema:2001\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance/\">" << std::endl;
		_fileStream << "	<Description xsi:type=\"ContentEntityType\">" << std::endl;
		_fileStream << "		<MultimediaContent xsi:type=\"VideoType\">" << std::endl;
		_fileStream << "			<Video id=\"videosegment\">" << std::endl;
		_fileStream << "				<MediaLocator>" << std::endl;
		_fileStream << "					<MediaUri>" << _mediaUri << "</MediaUri>" << std::endl;
		_fileStream << "				</MediaLocator>" << std::endl;
		_fileStream << "				<MediaTime>" << std::endl;
		_fileStream << "					<MediaRelTimePoint>T00:00:00:0F1000</MediaRelTimePoint>" << std::endl;
		_fileStream << "					<MediaDuration>" << getMediaDuration(_analyzer->getDuration()) << "</MediaDuration>" << std::endl;
		_fileStream << "				</MediaTime>" << std::endl;
		_fileStream << "				<TemporalDecomposition criteria=\"temporal\" gap=\"false\" overlap=\"false\">" << std::endl;
	}
}

void FrameHandler::writeFrame(int time, int frameIndex) {
	if (!_mpeg7Output) {
		_fileStream << "	<frame src=\"" << getImageName(time, frameIndex) << "\""
					<< " time=\"" << time << "\"";
		if (_generateThumbnails) {
			_fileStream << " thumb=\"" << getThumbName(time, frameIndex) << "\"";
		}
		_fileStream	<< " frameIndex=\"" << frameIndex << "\"></frame>" << std::endl;
	}
	else {
		if (_previousFrameTime>=0) {
			// Write duration of the previous frame
			closePreviousMpeg7Frame(time - _previousFrameTime);
		}
		openMpeg7Frame(time, frameIndex);
		_previousFrameTime = time;
	}
}

void FrameHandler::closeFile() {
	if (!_mpeg7Output) {
		_fileStream << "</sequence>" << std::endl;
		_fileStream.close();
	}
	else {
		if (_previousFrameTime>=0) {
			closePreviousMpeg7Frame(_analyzer->getDuration() - _previousFrameTime);
		}
		_fileStream << "				</TemporalDecomposition>" << std::endl;
		_fileStream << "			</Video>" << std::endl;
		_fileStream << "		</MultimediaContent>" << std::endl;
		_fileStream << "	</Description>" << std::endl;
		_fileStream << "</Mpeg7>" << std::endl;
		_fileStream.close();
	}
}

void FrameHandler::openMpeg7Frame(int time, int frame) {
	_fileStream << "					<VideoSegment id=\"" << _imagePrefix << "." << time << "\">" << std::endl;
	_fileStream << "						<MediaTime>" << std::endl;
	_fileStream << "							<MediaRelTimePoint>" << getRelTimePoint(time) << "</MediaRelTimePoint>" << std::endl;
}

void FrameHandler::closePreviousMpeg7Frame(int duration) {
	_fileStream << "							<MediaDuration>" << getMediaDuration(duration) << "</MediaDuration>" << std::endl;
	_fileStream << "						</MediaTime>" << std::endl;
	_fileStream << "					</VideoSegment>" << std::endl;
}

std::string FrameHandler::getRelTimePoint(int time) {
	TimeFormat formattedTime;
	formatTime(time, formattedTime);
	std::stringstream result;
	
	result << "T";
	if (formattedTime.hours<10) {
		result << "0";
	}
	result << formattedTime.hours << ":";
	
	if (formattedTime.minutes<10) {
		result << "0";
	}
	result << formattedTime.minutes << ":";
	
	if (formattedTime.seconds<10) {
		result << "0";
	}
	result << formattedTime.seconds << ":";
	
	result << "0F1000";
	
	return result.str();
}

std::string FrameHandler::getMediaDuration(int time) {
	TimeFormat formattedTime;
	formatTime(time, formattedTime);
	std::stringstream result;
	
	result << "P";
	
	if (formattedTime.days>0) {
		result << formattedTime.days << "D";
	}
	
	result << "T";
	if (formattedTime.hours>0) {
		result << formattedTime.hours << "H";
	}
	
	if (formattedTime.minutes>0) {
		result << formattedTime.minutes << "M";
	}
	
	if (formattedTime.seconds>0) {
		result << formattedTime.seconds << "S";
	}
	
	result << "0N1000F";
	
	return result.str();
}
