/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   idregister.h
 * @author Jan Mussler
 * @brief Defintion of IdRegister class
 *
 * This class is used to store associations between IDs and Descriptors
 */


#ifndef IDREGISTER_H
#define	IDREGISTER_H

#include <string>
#include <map>

namespace gi {
    namespace instrumenter {
        typedef unsigned int ContextIDType;

        class IdRegister {
        public:

            enum IdType {
                FUNCTION, LOOP, CALLSITE
            };

            /**
             * There is only one ID Store, use get() to access it
             *
             * @return Instance
             */
            static IdRegister& get();

            void enable();
            void disable();
            bool isEnabled();

            /**
             * Add a new ID with its type and name of the object
             *
             * @param t type that is associated with id: function, loop, or callsite
             * @param id the ID itself
             * @param value name of associated object
             */
            void addId(IdType t, ContextIDType id, std::string name);

            /**
             *
             * @param t type for the IDs returned in the list
             * @param sepIdIdent how to seperate id from associated name ( default = )
             * @param sepEntries how to seperate two ids ( default = \n )
             * @return a string with the list of ids and names
             */
            std::string getList(IdType t,std::string sepIdIdent = "=", std::string sepEntries = "\n");

        private:
            /* singleton pattern, use get() to ::get() instance*/
            static IdRegister* reg;

            /**
             * Is logging of IDs enabled?
             * @var enabled
             */
            bool enabled;

            /**
             * container for all types and used IDs with the associated object's name
             * @var store
             */
            std::map<IdType, std::map<ContextIDType, std::string> > store;

            IdRegister();
        };
    }
}
#endif	/* IDREGISTER_H */

