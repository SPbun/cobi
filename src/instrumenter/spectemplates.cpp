/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   spectemplates.cpp
 * @author Jan Mussler
 * @brief functions used to write template specification files
 */

#include <iostream>
#include <fstream>
#include "spectemplates.h"

namespace gii = gi::instrumenter;
//
bool gii::writeAdapterSpec(std::string fileName) {
    std::ofstream of;
    of.open(fileName.c_str());
    if (!of.is_open()) {
        return false;
    }

    of << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << "\n";
    of << "<adapter>" << "\n";

    of << "<!-- <dependencies>" << "\n";
    of << "\t<library name=\"libadapter.so\" />" << "\n";
    of << "</dependencies> -->" << "\n";
    of << "" << "\n";

    of << "<adaptersettings>" << "\n";
    of << "\t<saveallfprs value=\"false\" />" << "\n";
    of << "\t<nonreturningfunctions>" << "\n";
    of << "" << "\n";
    of << "\t</nonreturningfunctions>" << "\n";
    of << "\t<!-- use: main, _start, or __libc_csu_init -->" << "\n";
    of << "\t<initfunctionname value=\"main\" />" << "\n";
    of << "\t<finalizefunctionname value=\"main\" />" << "\n";
    of << "</adaptersettings>" << "\n";
    of << "" << "\n";
    
    of << "<!-- <adapterinitialization>" << "\n";
    of << "</adapterinitialization> -->" << "\n";
    of << "" << "\n";
    of << "<!-- <adapterfinalization>" << "\n";
    of << "</adapterfinalization> -->" << "\n";
    of << "" << "\n";

    of << "<!-- <adapterfilter>" << "\n";
    of << "<or><property name=\"hasoverlay\" /></or>" << "\n";
    of << "</adapterfilter> -->" << "\n";
    of << "" << "\n";
    of << "<!-- <globalvars><var name=\"\" size=\"\" type=\"\" /></globalvars> -->" << "\n";
    of << "" << "\n";
    of << "" << "\n";
    of << "<code name=\"functions\">" << "\n";
    of << "<!-- <variables><var name=\"\" size=\"\" type=\"\" /></variables> -->" << "\n";
    of << "\t<init priority=\"0\"></init>" << "\n";
    of << "\t<before></before>" << "\n";
    of << "\t<enter></enter>" << "\n";
    of << "\t<exit></exit>" << "\n";
    of << "\t<after></after>" << "\n";
    of << "\t<finalize></finalize>" << "\n";
    of << "</code>" << "\n";

    of << "</adapter>" << "\n";

    of.close();
    return true;
};

bool gii::writeFilterSpec(std::string fileName) {
    
    std::ofstream of;
    of.open(fileName.c_str());
    if (!of.is_open()) {
        return false;
    }

    of << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << "\n";
    of << "<filters>" << "\n";
    
    of << "<filter name=\"nostd\" start=\"all\" instrument=\"functions\">" << "\n";

    of << "\t<exclude>" << "\n";
    of << "\t\t<namespaces match=\"prefix\">" << "\n";
    of << "\t\tstd __gnu_cxx" << "\n";
    of << "\t\t</namespaces>" << "\n";
    of << "\t</exclude>" << "\n";

    of << "</filter>" << "\n";

    of << "</filters>" << "\n";

    of.close();
    return true;
};