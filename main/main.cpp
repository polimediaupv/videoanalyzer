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
#include <iostream>
#include <opencv2/opencv.hpp>
#include <VideoAnalyzer.hpp>
#include <HistogramAnalyzer.hpp>
#include <FrameHandler.hpp>
#include <ArgumentParser.hpp>

class ArgumentCallback : public ArgumentParserCallback {
public:
	const std::string & videoPath() const { return _videoPath; }
	bool showWindow() const { return _showWindow; }
	float detectionTresshold() const { return _detectionTresshold; }
	int frameStep() const { return _frameStep; }
	bool debugLog() const { return _debugLog; }

	ArgumentCallback(FrameHandler * frameHandler)
		:_frameHandler(frameHandler)
		,_videoPath("")
		,_showWindow(false)
		,_detectionTresshold(97.0f)
		,_frameStep(24)
		,_debugLog(false)
	{
    }

    int parseParameter(const std::string & parameter) {
		_videoPath = parameter;
        return 0;
    }
	
    int parseOption(const std::string & optionName, const std::string & optionValue) {
        if (optionName=="--help") {
            printHelp();
            return 0;
        }
		else if (optionName=="--outputDir") {
			return setStringValue(optionValue, _frameHandler->outputDir(), "Invalid output path", true);
		}
		else if (optionName=="--mpeg7") {
			return setBooleanValue(optionValue, _frameHandler->mpeg7Output(), "mpeg7: you must specify Yes or No");
		}
		else if (optionName=="--generateImages") {
			return setBooleanValue(optionValue, _frameHandler->generateImages(), "Generate images: you must specify Yes or No");
		}
		else if (optionName=="--imagePrefix") {
			return setStringValue(optionValue, _frameHandler->imagePrefix(), "Invalid image name");
		}
		else if (optionName=="--imageFormat") {
			return setStringValue(optionValue, _frameHandler->imageFormat(), "Invalid image format");
		}
		else if (optionName=="--generateThumbs") {
			return setBooleanValue(optionValue, _frameHandler->generateThumbnails(), "Generate thumbnails: you must specify Yes or No");
		}
		else if (optionName=="--thumbnailDir") {
			return setStringValue(optionValue, _frameHandler->thumbDir(), "Invalid thumbnail output dir", true);
		}
		else if (optionName=="--thumbnailPrefix") {
			return setStringValue(optionValue, _frameHandler->thumbPrefix(), "Invalid thumbnail file prefix");
		}
		else if (optionName=="--thumbnailFormat") {
			return setStringValue(optionValue, _frameHandler->thumbFormat(), "Invalid thumbnail format");
		}
		else if (optionName=="--thumbnailWidth") {
			return setIntegerValue(optionValue, _frameHandler->thumbWidth(), "Invalid thumbnail width");
		}
		else if (optionName=="--generateSequence") {
			return setBooleanValue(optionValue, _frameHandler->generateSequence(), "Generate sequence: you must specify Yes or No");
		}
		else if (optionName=="--sequenceFile") {
			return setStringValue(optionValue, _frameHandler->sequenceFileName(),"Invalid sequence file name");
		}
		else if (optionName=="--tresshold") {
			return setFloatValue(optionValue, _detectionTresshold, "Tresshold: you must specify a float value higher than zero");
		}
		else if (optionName=="--frameStep") {
			int status = setIntegerValue(optionValue, _frameStep, "Frame step: you must specify an integer value higher than zero");
			return status;
		}
		else if (optionName=="--showWindow") {
			int status = setBooleanValue(optionValue, _showWindow, "Show window: you must specify Yes or No");
			return status;
		}
		else if (optionName=="--debugLog") {
			int status = setBooleanValue(optionValue, _debugLog, "Debug log: you must specify Yes or No");
			return status;
		}
		else if (optionName=="--mediaUri") {
			return setStringValue(optionValue, _frameHandler->mediaUri(), "Media URI: you must specify a valid URI to fill in the MediaLocator field");
		}
		else {
			std::cout << "Invalid option: " << optionName << std::endl;
			printHelp();
			return -1;
		}
    }
	
    
    void printHelp() {
        std::cout << "Usage: videoanalyzer [options] video_file" << std::endl
		<< "	--outputDir dir: output directory. Use . for current path (default: " << _frameHandler->outputDir() << "). The directory must exist." << std::endl
		<< "	--mpeg7 [Y|N]: generate mpeg7 output format. (default " << _frameHandler->mpeg7Output() << ")." << std::endl
		<< "	--generateImages [Y|N]: specify if you want to extract each detected frame into an image. (default " << _frameHandler->generateImages() << ")" << std::endl
		<< "	--imagePrefix name: the output images will match the pattern [image_prefix].[instant].[format] (default: " << _frameHandler->imagePrefix() << ")" << std::endl
		<< "	--imageFormat fmt: the output images will be saved in this format, example: jpg. (default: " << _frameHandler->imageFormat() << ")" << std::endl
		<< "	--generateThumbs [Y|N]: specify if you want to extract each detected frame thumbnail into an image file. (default " << _frameHandler->generateThumbnails() << ")" << std::endl
		<< "	--thumbnailDir dir: output thumnail directory, inside the main output directory. Use . for current path (--outputDir). (default: " << _frameHandler->thumbDir() << "). The directory must exist" << std::endl
		<< "	--thumbnailPrefix name: the output thumnail will match the pattern [thumbnail_prefix].[instant].[format] (default: " << _frameHandler->thumbPrefix() << ")" << std::endl
		<< "	--thumbnailFormat fmt: the output thumnails will be saved in this format, example: png. (default: " << _frameHandler->thumbFormat() << ")" << std::endl
		<< "	--thumbnailWidth width: thumnail size, in pixels. The image will be reduced proportionally. (default: " << _frameHandler->thumbWidth() << std::endl
		<< "	--generateSequence [Y|N]: specify if you want to generate the sequence file. (default " << _frameHandler->generateSequence() << std::endl
		<< "	--sequenceFile file: name of the data file. Will be stored in the output directory [--outputDir]. (default: " << _frameHandler->sequenceFileName() << ")"<< std::endl
		<< "	--tresshold t: tolerance to detect frame changes, major than zero. (default" << _detectionTresshold << ")" << std::endl
		<< "	--windowed [Y|N]: show debug window. (default " << _showWindow << ")" << std::endl
		<< "	--frameStep step: analyze only one of each [step] frames in the video. (default " << _frameStep << ")" << std::endl
		<< "	--debugLog [Y|N]: show console debug output (default " << _debugLog << ")" << std::endl
		<< "	--mediaUri uri: video URI. This parameter is used to fill in the field 'MediaLocator' in the mpeg7 output file. (default " << _frameHandler->mediaUri() << ")." << std::endl
		<< "	--help: print this help" << std::endl
		<< std::endl;
    }
	
protected:
	FrameHandler * _frameHandler;
	std::string _videoPath;
	bool _showWindow;
	float _detectionTresshold;
	int _frameStep;
	bool _debugLog;
};

int main(int argc, char ** argv) {
	FrameHandler * frameHandler = new FrameHandler();
	ArgumentParser arguments(1);
    ArgumentCallback * cb = new ArgumentCallback(frameHandler);
    arguments.setParser(cb);
	
	int status = arguments.parse(argc, const_cast<const char**>(argv));
	
	if (status!=0) {
		exit(status);
	}
	
	if (cb->showWindow()) {
        cv::namedWindow("mainWin");
        cv::moveWindow("mainWin", 0, 0);
        cv::resizeWindow("mainWin", 800, 600);
	}

	VideoAnalyzer va;
	HistogramAnalyzer * analyzer = new HistogramAnalyzer();
	va.setAnalyzerCallback(analyzer);
	va.addFrameCallback(frameHandler);
	analyzer->setDebug(cb->debugLog());
	analyzer->setDetectionTresshold(cb->detectionTresshold());
	
	if (va.loadVideo(cb->videoPath())) {
		va.setFrameStep(cb->frameStep());
		while (va.nextFrame()) {
			const cv::Mat currentFrame = va.getCurrentFrame();
			if (cb->showWindow() && currentFrame.rows>0 && currentFrame.rows>0) {
				cv::imshow("mainWin", currentFrame);
			}
		}
	}
	else {
		std::cout << "Error opening video " << argv[1] << std::endl;
	}
	
	va.release();
	
	if (cb->showWindow()) {
        cv::destroyWindow("mainWin");
	}
	return 0;
}
