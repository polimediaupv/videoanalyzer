#VideoAnalizerGUI

This project is a Graphic User Interface for find changes on frames in a source video depending on some threshold. 

Its just a GUI where you can select some different features and routes for the input and output files, then with all this info it throws a command shell showing the process.

## Windows Requirements
###For use VideoAnalizerGui under Windows you must have at the same directory these files.

	- OpenCV dlls compiled with the same compiler and architecture (x64 default).
	- Microsoft Visual Studio dlls x64 (msvcpXXX.dll, msvcrXXX.dll, vccorlibXXX.dll)
	- Qt main dlls and platform dlls.


## Example Project under Windows
### This project was tested with:
	-Windows 8.1 x64
	-Qt 5.3 with MSVC2013 x64
	-Cmake 3.1.0-rc2	
	-OpenCV 3.0

### Files Hierarchy:
	-platforms/qwindows.dll
	-icudt52.dll
	-icuin52.dll
	-icuuc52.dll
	-libEGL.dll
	-libGLESv2.dll
	-msvcp120.dll
	-msvcr120.dll
	-opencv_calib3d300.dll
	-opencv_core300.dll
	-opencv_features2d300.dll
	-opencv_ffmpeg300.dll
	-opencv_flann300.dll
	-opencv_highgui300.dll
	-opencv_imgcodecs300.dll
	-opencv_imgproc300.dll
	-opencv_ml300.dll
	-opencv_objdetect300.dll
	-opencv_photo300.dll
	-opencv_shape300.dll
	-opencv_stitching300.dll
	-opencv_superres300.dll
	-opencv_video300.dll
	-opencv_videoio300.dll
	-opencv_videostab300.dll
	-Qt5Core.dll
	-Qt5Gui.dll
	-Qt5Widgets.dll
	-vccorlib120.dll
	-videoAnalizerGUI.exe
	-videoanalyzer.exe

