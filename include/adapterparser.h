/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   adaperparser.h
 * @author Jan Mussler
 * @brief  Adapterparser definition
 */

#ifndef _ADAPTERPARSER_H
#define	_ADAPTERPARSER_H


#include <string>
#include <map>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include "readableadapter.h"

using namespace xercesc;
using namespace gi::instrumenter;

namespace gi {
    namespace xml {

        class AdapterParser {
        public:
            AdapterParser();
            ~AdapterParser();

            /**
             * parse adapter configuration file and return a readable configuration
             */
            static IReadableAdapterConfig* getConfigFromFile(std::string fileName, bool abortOnError = false);
        };
    }
}

#endif	/* _ADAPTERPARSER_H */
