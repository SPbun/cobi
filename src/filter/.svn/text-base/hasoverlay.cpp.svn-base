/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   hasoverlay.cpp
 * @author Jan Mussler
 * @brief Implementation of the hasoverlay property
 */

#include "implproperties.h"

using namespace gi::mutatee;

namespace gi {
    namespace filter {
        namespace properties {

            HasOverlay::HasOverlay() {
            }

            HasOverlay::~HasOverlay() {
            }

            bool HasOverlay::init(Mutatee* mutatee) {
                return true;
            }

            bool HasOverlay::match(gi::mutatee::IFunction* f) {
                BPatch_function* func = f->getBPFunction();

                vector<BPatch_function*> overlayFuncs;

                bool res = func->findOverlapping(overlayFuncs);
                if( res ) {
                    /*
                    std::cout << "f: " << f->getName() << " overlayed by " << overlayFuncs.size() << "\n";
                    for(int i = 0; i < overlayFuncs.size(); i++) {
                        Function o(overlayFuncs[i]);
                        std::cout << "by " << o.getName() << "\n";
                    }
                    */
                }
                return res;
            }

            bool HasOverlay::needsInit() {
                return false;
            }

            bool HasOverlay::setUp(gix::FilterParser* fp, DOMElement* headElement) {
                return true;
            }
        }
    }
}
