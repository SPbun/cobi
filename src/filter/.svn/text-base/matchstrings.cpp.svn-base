/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   matchstrings.cpp
 * @author Jan Mussler
 * @brief implementation of the different match string strategies
 */

#include "patterns.h"
#include <string>
#include "boost/regex.hpp"
#include "boost/regex/pattern_except.hpp"

using namespace std;
using namespace gi::filter;

bool MatchEquals::match(std::string pattern, std::string test) const {
    return pattern == test;
}

bool MatchSimplePattern::match(std::string pattern, std::string test) const {
    std::size_t pos = pattern.find("*");
    if( pos == std::string::npos) {
        return pattern==test;
    }
    else {
        return pattern.substr(0,pos) == test.substr(0,pos);
    }
}


bool MatchRegex::validPattern(std::string pattern) const {
    try {
        boost::regex r(pattern);
        return true;
    }
    catch ( boost::regex_error &e ) {
        return false;
    }
}

bool MatchRegex::match(std::string pattern, std::string test) const {
    boost::regex r(pattern);
    return boost::regex_match(test,r);
}

bool MatchPrefix::match(std::string pattern, std::string test) const {
    return pattern == test.substr(0,pattern.size());
}

bool MatchSuffix::match(std::string pattern, std::string test) const {
    return pattern == test.substr(test.size()-pattern.size(),test.npos);
}

bool MatchFind::match(std::string pattern, std::string test) const {
    return std::string::npos != test.find(pattern);
}
