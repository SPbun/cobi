/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   sequence.cpp
 * @author Jan Mussler
 * @brief Implementation of Sequence AST node
 */

#include "generator.h"

using namespace gi::generator;

Sequence::Sequence() {
}

Sequence::~Sequence() {
    for(std::vector<IExpression*>::iterator i = statements.begin();
            i != statements.end();
            i++) {
        delete (*i);
    }
    statements.clear();
}

/**
 * add statement to sequence
 */
void Sequence::addStatement(IExpression* e) {
    statements.push_back(e);
}

/**
 * get bpatch_sequence of snippets
 */
BPatch_snippet* Sequence::getSnippet(IContext* c) {
    std::vector<BPatch_snippet*> seq;

    for (std::vector<IExpression*>::const_iterator i = statements.begin();
            i != statements.end();
            i++) {
        
        // evaluate and add to sequence
        seq.push_back((*i)->getSnippet(c));
    }
    
    return new BPatch_sequence(seq);
}
