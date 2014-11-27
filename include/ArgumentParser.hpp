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
#ifndef _ARGUMENTPARSER_HPP_
#define _ARGUMENTPARSER_HPP_

#include <map>
#include <vector>
#include <string>
#include <iostream>

class ArgumentParserCallback {
public:
    virtual int parseParameter(const std::string & parameter) = 0;
    virtual int parseOption(const std::string & optionName, const std::string & optionValue) = 0;
	
	virtual ~ArgumentParserCallback() {}

protected:
	int setStringValue(const std::string & value,std::string & param, const std::string & errorMessage, bool isPath = false);
	int setIntegerValue(const std::string & value, int & param, const std::string & errorMessage, int minVal=0,int maxVal=1000);
	int setFloatValue(const std::string & value, float & param, const std::string & errorMessage, float minVal=0.0f, float maxVal=1000.0f);
	int setBooleanValue(const std::string & value, bool & param, const std::string & errorMessage);
};

class ArgumentParser {
public:
    ArgumentParser();
    ArgumentParser(int minArguments);
    virtual ~ArgumentParser();
    
    int parse(int argc, const char ** argv);

    int getMinArguments() const { return _minArguments; }
    void setMinArguments(int min) { _minArguments = min; }
    std::string & getOption(const std::string & optionName);
    std::string & getParameter(int index);
    void setParser(ArgumentParserCallback * callback);
    ArgumentParserCallback * getParser() { return _parserCallback; }

protected:
    int _minArguments;
    std::map<std::string,std::string> _optionMap;
    std::vector<std::string> _parameterVector;
    ArgumentParserCallback * _parserCallback;
    
    bool isOption(const std::string & opt);
};

#endif