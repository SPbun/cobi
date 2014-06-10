/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   idregister.cpp
 * @author Jan Mussler
 * @brief implementation of id register
 *
 * Necessary functions to store and write id @ROUTINE@ pairs
 */

#include <sstream>
#include <iostream>
#include <map>

#include "idregister.h"

using namespace std;

namespace gi {
    namespace instrumenter {

        IdRegister* IdRegister::reg = 0;

        IdRegister& IdRegister::get() {
            if (reg == 0) {
                reg = new IdRegister();
            }
            
            return *reg;
        }

        void IdRegister::enable() {
            enabled = true;
        }

        void IdRegister::disable() {
            enabled = false;
        }

        bool IdRegister::isEnabled() {
            return enabled;
        }

        void IdRegister::addId(IdType t, ContextIDType id, std::string name) {
            if(isEnabled()) {
                store[t][id] = name;
            }
        }

        std::string IdRegister::getList(IdType t, std::string sepIdIdent, std::string sepEntries) {
            stringstream ss;

            for(map<ContextIDType,string>::const_iterator i = store[t].begin();
                    i != store[t].end();
                    i++) {
                ss << i->first << sepIdIdent << i->second << sepEntries;
            }

            return ss.str();
        }

        IdRegister::IdRegister() : enabled(true) {

        }
    }
}
