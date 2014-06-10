/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   ruletree.cpp
 * @author Jan Mussler
 * @brief Implementations of essential classes in the rule tree
 */

#include <vector>
#include <assert.h>

#include "ruletree.h"
#include "ifunction.h"

using namespace std;
using namespace gi::mutatee;

namespace gi {
    namespace filter {

        AND::AND() {
        }

        OR::OR() {
        }

        /**
         * constructor
         */
        NOT::NOT() : child(0) {

        }

        /**
         * destructor
         */
        NOT::~NOT() {
            delete child;
        }

        /**
         * set child rule
         * @param r child rule
         */
        void NOT::setRule(IRule* r) {
            child = r;
        }

        /**
         * negate the child result
         * @param f function to be checked
         */
        bool NOT::match(IFunction* f) {
            return !(child->match(f));
        }

        /**
         * check if the function matches all child rules
         * @param f function to be checked
         */
        bool AND::match(IFunction* f) {
            bool result = true;
            for (vector<IRule*>::iterator i = children.begin(); i != children.end(); i++) {
                result = (*i)->match(f);
                if (result == false)
                    return false;
            }
            return result;
        }

        /**
         * look for any child rule that matches the incoming function
         * @param f function to be checked
         */
        bool OR::match(IFunction* f) {
            bool result = false;
            for (vector<IRule*>::iterator i = children.begin(); i != children.end(); i++) {
                result = (*i)->match(f);
                if (result == true)
                    return true;
            }
            return result;
        }

        /**
         * Destructor
         */
        ARuleSet::~ARuleSet() {
            for (vector<IRule*>::iterator i = children.begin(); i != children.end(); i++) {
                delete (*i);
            }
            children.clear();
        }

        /**
         * add new rule to children
         * @param r rule to be added
         */
        void ARuleSet::addRule(IRule* r) {
            assert(r != 0);
            children.push_back(r);
        }

        unsigned int ARuleSet::count() {
            return children.size();
        }

    }
}
