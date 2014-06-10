/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   cobiconfig.h
 * @author Jan Mussler
 * @brief Cobiconfig class definition
 */

#ifndef COBICONFIG_H
#define	COBICONFIG_H

#include <string>
#include <map>

namespace gi {
    namespace instrumenter {

        /***
         *
         * Component to store configuration flags and values
         *
         * Currently only used for pass flags to the instrumentation like ignore no exit or entry points
         *
         **/
        class Cobiconfig {
        public:

            /**
             * Flag Keys
             */
            enum flags {
                FLOAT_SAVE_ENABLED, // in general is saven FPRs enabled ( Dyninst 6.1 )
                FLOAT_SAVE_ALLWAYS, // force save of FPRs ( not implemented in Dyninst 6.1 )

                IGNORE_NOENTRY, // instrument function even though no entry point available
                IGNORE_NOEXIT, // instrument function even though no exit point available

                LOAD_LIBRARIES, // open binary with all dependency libraries

                SHOW_IP_ADDRESSES, // output the Instrumentation Point addresses during instrumentation

                TRAMP_GUARD_ENABLED, // Tramp Guard enabled to guard from recursive instrumentation calls ( Dyninst 6.1 )

                LIST_INSTRUMENTED_FUNCTIONS // show the instrumented functions
            };

            /**
             * Value Keys
             */
            enum strKeys {
                FUNCTION_FOR_INIT_CODE, // where to place the init code default "main"

                FUNCTION_FOR_FINALIZE_CODE, // where to place any finalization

                ID_LIST_SEPERATOR_ITEMS, // how to sperate multiple @ID@=Routine name default "\n"

                ID_LIST_SEPERATOR_IDVALUE // how to seperate @ID@ and Routine name default "="
            };
            
            /**
             * Singleton pattern for one configuration object
             *
             * @return singleton instance
             */
            static Cobiconfig& getCfg();

            /**
             * return flag's value
             *
             * @param f flag key
             * @return flag value
             */
            bool getFlag(flags f);

            /**
             * Set flag's value
             *
             * @param f flag key
             * @param newValue flag's new value
             * @return self
             */
            Cobiconfig& setFlag(flags f, bool newValue);

            /**
             * get stored string to key
             *
             * @param k Key
             * @return associated string
             */
            std::string getString(strKeys k);

            /**
             * Set new key value
             *
             * @param k key
             * @param newValue
             * @return self
             */
            Cobiconfig& setString(strKeys k, std::string newValue);

            typedef std::map<flags, bool> mapFlagBool;
            typedef std::map<strKeys, std::string> mapStrKeyString;

        private:

            mapFlagBool flagValues;
            mapStrKeyString stringValues;

            Cobiconfig();
            virtual ~Cobiconfig();

        };

        // get config object, wraps Cobiconfig::getCfg to reduce writing
        Cobiconfig& getCfg();
    }
}
#endif	/* COBICONFIG_H */

