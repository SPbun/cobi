/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   filterset.cpp
 * @author Jan Mussler
 * @brief Implementation of filterset class
 *
 * a filterset is a set of filters, represtens the filters tag containing
 * all filters defined in one filter specification
 */

#include "filters.h"

using namespace gi::filter;

FilterSet::FilterSet() {
}

FilterSet::~FilterSet() {
    for (mapStringIFilter::iterator i = filters.begin();
            i != filters.end();
            i++) {
        delete (*i).second;
    }
    filters.clear();
}

void FilterSet::addFilter(std::string aName, IFilter* aFilter) {
    filters[aName] = aFilter;
}

set<string> FilterSet::getFilterNames() {
    set<string> names;
    for (mapStringIFilter::iterator i = filters.begin();
            i != filters.end();
            i++) {
        names.insert(i->first);
    }
    return names;
}

IFilter* FilterSet::getFilterByName(std::string aName) {
    if (filters.find(aName) != filters.end()) {
        return filters[aName];
    }
    throw ex::ReferencingUndefinedFilter();
}

vector<IFilter*> FilterSet::getInstrumentingFilters() {
    vector<IFilter*> result;

    for (mapStringIFilter::iterator i = filters.begin();
            i != filters.end();
            i++) {
        if ((*i).second->isInstrumenting()) {
            result.push_back((*i).second);
        }
    }

    return result;
}
