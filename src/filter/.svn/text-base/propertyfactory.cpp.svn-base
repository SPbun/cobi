/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   propertyfactory.cpp
 * @author Jan Mussler
 * @brief Implementation of property factory
 *
 * Implements the Factory for the different properties according to their
 * registered names that are associated with a particular factory
 */

#include "properties.h"
#include "parserexceptions.h"
#include "implproperties.h"

using namespace std;
using namespace gi::filter;
using namespace gi::mutatee;
using namespace gi::filter::properties;

PropertyFactory PropertyFactory::pf;

PropertyFactory::PropertyFactory() {
    factories["path"] = new TemplatePropFactory<CallPath>();
    factories["depth"] = new TemplatePropFactory<Depth>();
    factories["mccabe"] = new TemplatePropFactory<Mccabe>();
    factories["cc"] = factories["mccabe"];
    factories["linesofcode"] = new TemplatePropFactory<SourceVolume>();
    factories["loc"] = factories["linesofcode"];
    factories["islibrarycall"] = new TemplatePropFactory<IsLibraryFunction>();    
    factories["hasloops"] = new TemplatePropFactory<HasLoop>();
    factories["countcallsites"] = new TemplatePropFactory<CountCallSites>();
    factories["countcallees"] = new TemplatePropFactory<CountCallees>();
    factories["countexit"] = new TemplatePropFactory<CountExits>();
    factories["countenter"] = new TemplatePropFactory<CountEnter>();
    factories["countcallees"] = new TemplatePropFactory<CountCallees>();
    factories["hasoverlay"] = new TemplatePropFactory<HasOverlay>();
    factories["returnsfloatordouble"] = new TemplatePropFactory<RetFloatOrDouble>();
    factories["calledinloop"] = new TemplatePropFactory<CalledInLoop>();
    factories["countinst"] = new TemplatePropFactory<CountInst>();
}

PropertyFactory& PropertyFactory::Get() {
    return PropertyFactory::pf;
}

IProperty* PropertyFactory::getPropertyByName(string name) {
    if(factories.find(name) != factories.end()) {
        return factories[name]->getProperty();
    }
    throw new ex::UnknownPropertyException();
}

IProperty* PropertyFactory::getProperty(string name) {
    IProperty* p;

    p = getPropertyByName(name);

    if ( p->needsInit() ) {
        needInit.push(p);
    }

    return p;
}

void PropertyFactory::InitProperties(Mutatee* mutatee) {
    while(!needInit.empty()) {
        IProperty* p = needInit.top();
        needInit.pop();

        p->init(mutatee);
    }
}

vector<string> PropertyFactory::getPropertyList() {
    vector<string> list;

    for(MapNamePropFactory::const_iterator i = factories.begin();
            i != factories.end();
            i++) {
        list.push_back((*i).first + "\n");
    }

    return list;
}
