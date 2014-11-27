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
#include "ArgumentParser.hpp"

#include <cstdlib>

int ArgumentParserCallback::setStringValue(const std::string & value,std::string & param, const std::string & errorMessage, bool isPath) {
	if (value=="" || value=="true") {
		std::cerr << errorMessage << std::endl;
		return -1;
	}
	if (isPath && value==".") {
		param = "";
	}
	else {
		param = value;
	}
	return 0;
}

int ArgumentParserCallback::setIntegerValue(const std::string & value, int & param, const std::string & errorMessage, int minVal,int maxVal) {
	int intValue = atoi(value.c_str());
	if (value=="" || value=="true" || intValue<minVal || intValue>maxVal) {
		std::cerr << errorMessage << std::endl;
		return -1;
	}
	param = intValue;
	return 0;
}

int ArgumentParserCallback::setFloatValue(const std::string & value, float & param, const std::string & errorMessage, float minVal, float maxVal) {
	float floatValue = static_cast<float>(atof(value.c_str()));
	if (value=="" || value=="true" || floatValue<minVal || floatValue>maxVal) {
		std::cerr << errorMessage << std::endl;
		return -1;
	}
	param = floatValue;
	return 0;
}

int ArgumentParserCallback::setBooleanValue(const std::string & value, bool & param, const std::string & errorMessage) {
	if (value=="yes" || value=="true"  || value=="TRUE" || value=="YES" || value=="True" || value=="Yes" || value=="Y" || value=="y" || value=="T" || value=="t" || value=="1") {
		param = true;
		return 0;
	}
	else if (value=="no" || value=="false"  || value=="NO" || value=="FALSE" || value=="No" || value=="False" || value=="N" || value=="n" || value=="f" || value=="F" || value=="0") {
		param = false;
		return 0;
	}
	else if (value=="") {
		std::cerr << errorMessage << std::endl;
		return -1;
	}
	return -1;
}



ArgumentParser::ArgumentParser() :_minArguments(0) {
    _parserCallback = NULL;
}

ArgumentParser::ArgumentParser(int minArguments) :_minArguments(minArguments) {
    _parserCallback = NULL;    
}

ArgumentParser::~ArgumentParser() {
    if (_parserCallback==NULL) {
        delete _parserCallback;
    }
}

int ArgumentParser::parse(int argc, const char ** argv) {
    int i;
    std::string currentOption = "";

    for (i=1; i<argc; ++i) {
        if (isOption(argv[i])) {
            if (currentOption!="") {
                _optionMap.insert(std::pair<std::string, std::string>(currentOption,"true"));
            }
            currentOption = argv[i];
        }
        else if (currentOption!="") {
            _optionMap.insert(std::pair<std::string,std::string>(currentOption,argv[i]));
            currentOption = "";
        }
        else {
            _parameterVector.push_back(argv[i]);
        }
    }
    
    if (currentOption!="") {
        _optionMap.insert(std::pair<std::string,std::string>(currentOption,"true"));
        currentOption = "";
    }
    
    if (i<_minArguments) {
        return -1;
    }
    
    std::map<std::string,std::string>::iterator optionIt;
    std::vector<std::string>::iterator paramIt;
    int result = 0;

    if (_parserCallback!=NULL) {
        for (optionIt = _optionMap.begin(); optionIt!=_optionMap.end() && result==0; ++optionIt) {
            result = _parserCallback->parseOption(optionIt->first, optionIt->second);
        }
        for (paramIt = _parameterVector.begin(); paramIt!=_parameterVector.end() && result==0; ++paramIt) {
            result = _parserCallback->parseParameter(*paramIt);
        }
    }
    return result;
}

void ArgumentParser::setParser(ArgumentParserCallback * callback) {
    if (_parserCallback!=NULL) delete _parserCallback;
    _parserCallback = callback;
}

std::string & ArgumentParser::getOption(const std::string & optionName) {
    return _optionMap[optionName];
}

std::string & ArgumentParser::getParameter(int index) {
    return _parameterVector[index];
}

bool ArgumentParser::isOption(const std::string & opt) {
    if (opt[0]=='-') {
        return true;
    }
    else {
        return false;
    }
}
