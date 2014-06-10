/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   properties.h
 * @author Jan Mussler
 * @brief Property base class and factory defintions
 */

#ifndef _PROPERTIES_H
#define	_PROPERTIES_H

#include <string>
#include <stack>
#include <vector>
#include "xmlparser.h"
#include "ruletree.h"
#include "mutatee.h"
#include "callgraph.h"

using namespace xercesc;

namespace gim = gi::mutatee;

namespace gi {
    namespace xml {
        class FilterParser;
    }
}

namespace gix = gi::xml;

namespace gi {
    namespace filter {

        /**
         * Base class for properties
         */
        class IProperty : public IRule {
        public:
            /**
             * used to setup a property with the domelement from the filter spec
             */
            virtual bool setUp(gix::FilterParser* fp, DOMElement* headElement) = 0;

            /**
             * used to do one time calculation before filtering the mutatee
             */
            virtual bool init(gim::Mutatee* mutatee) = 0;

            /**
             * if property needs initialisation, return true
             */
            virtual bool needsInit() = 0;
        };

        typedef std::stack<IProperty*> PropertyList;

        class AbstractPropertyFactory {
        public:
            virtual ~AbstractPropertyFactory() {};
            virtual IProperty* getProperty() = 0;
        };

        typedef std::map<std::string, AbstractPropertyFactory*> MapNamePropFactory;

        class PropertyFactory {
        private:
            // add property here if it need initialisation

            static PropertyFactory pf;

            PropertyList needInit;
            MapNamePropFactory factories;

            IProperty* getPropertyByName(std::string name);
            PropertyFactory();
        public:
            static PropertyFactory& Get();
            IProperty* getProperty(std::string name);
            std::vector<std::string> getPropertyList();
            void InitProperties(gim::Mutatee* mutatee);
            void registerProperty(std::string name, AbstractPropertyFactory* af);
        };

        template <typename P> class TemplatePropFactory : public AbstractPropertyFactory {
        public:
            IProperty* getProperty() {
                return new P();
            }
        };
    }
}
#endif	/* _PROPERTIES_H */
