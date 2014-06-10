/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   cobiconfig.cpp
 * @author Jan Mussler
 * @brief Implementation of Cobiconfig class storing configuration settings
 */

#include "cobiconfig.h"

namespace gi {
    namespace instrumenter {

        Cobiconfig& getCfg()
        {
            return Cobiconfig::getCfg();
        }

        /**
         * Constructor
         *
         * default values for flags and strings should be set here
         *
         */
        Cobiconfig::Cobiconfig() {
            flagValues[IGNORE_NOENTRY] = false;
            flagValues[IGNORE_NOEXIT] = false;

            flagValues[SHOW_IP_ADDRESSES] = false;

            flagValues[FLOAT_SAVE_ENABLED] = true;
            flagValues[FLOAT_SAVE_ALLWAYS] = true;

            flagValues[TRAMP_GUARD_ENABLED] = true;
            
            flagValues[LOAD_LIBRARIES] = false;
            flagValues[LIST_INSTRUMENTED_FUNCTIONS] = false;

            stringValues[ID_LIST_SEPERATOR_IDVALUE] = "=";
            stringValues[ID_LIST_SEPERATOR_ITEMS] = "\n";
            stringValues[FUNCTION_FOR_INIT_CODE] = "main";
            stringValues[FUNCTION_FOR_FINALIZE_CODE] = "main";
        }

        Cobiconfig::~Cobiconfig() {
        }

        Cobiconfig& Cobiconfig::getCfg() {
            static Cobiconfig cf;
            return cf;
        }

        bool Cobiconfig::getFlag(flags f) {
            return flagValues[f];
        }

        Cobiconfig& Cobiconfig::setFlag(flags f, bool newValue) {
            flagValues[f] = newValue;
            return *this;
        }

        std::string Cobiconfig::getString(strKeys k) {
            return stringValues[k];
        }

        Cobiconfig& Cobiconfig::setString(strKeys k, std::string newValue) {
            stringValues[k] = newValue;
            return *this;
        }
    }
}
