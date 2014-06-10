/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   dynfactory.cpp
 * @author Jan Mussler
 * @brief  Mutatee factory implementation
 */

#include "mutatee.h"
#include "readableadapter.h"
#include "cobiconfig.h"

namespace gii = gi::instrumenter;

using namespace gi::mutatee;
using namespace std;

Mutatee* DynFactroy::getMutatee(BPatch& bp, std::string fileName, gii::IReadableAdapterConfig* config) {

    BPatch_binaryEdit* appBin = bp.openBinary(fileName.c_str(),
                                gii::getCfg().getFlag(gii::Cobiconfig::LOAD_LIBRARIES));

    assert(appBin != 0);

    bp.setTrampRecursive(gii::getCfg().getFlag(gii::Cobiconfig::TRAMP_GUARD_ENABLED));

    bp.setSaveFPR(gii::getCfg().getFlag(gii::Cobiconfig::FLOAT_SAVE_ENABLED));

    // uncomment for Dyninst 7.0, will save FPR registers all the time if set to true
#ifdef COBI_ENABLE_FPR_SAVE
    bp.forceSaveFPR(gii::getCfg().getFlag(gii::Cobiconfig::FLOAT_SAVE_ALLWAYS));
#endif

    /* Dyninst 7.0 adding of non returning functions from adapter specification
    set<string> funcs = config->getNonReturningFunctions();
    for(set<string>::const_iterator i = funcs.begin(); i != funcs.end(); i++ )
    {
        Dyninst::ParseAPI::addNonReturning(*i);
    }
     */

    BPatch_image* appImage = appBin->getImage();
    assert(appImage != 0);
    
    return new Mutatee(appBin,appImage);
}
