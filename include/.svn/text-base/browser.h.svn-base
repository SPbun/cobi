/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   browser.h
 * @author Jan Mussler
 * @brief  HTML Report writer
 */

#ifndef _BROWSER_H
#define	_BROWSER_H

#include "mutatee.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <map>

#include "BPatch.h"
#include "BPatch_addressSpace.h"
#include "BPatch_image.h"
#include "BPatch_Vector.h"
#include "BPatch_function.h"
#include "BPatch_point.h"
#include "BPatch_snippet.h"
#include "filters.h"

namespace gim = gi::mutatee;

namespace gi {
    namespace browser {

        /**
         * class to produce html representation of mutatee information
         */
        class HtmlView {
        private:
            unsigned int id;
            BPatch_addressSpace* addSpace;
            BPatch_image *appImage;
            gi::filter::filterResult* listOfFunctions;
            map<string,bool> functionsToShow;
            
            std::ofstream of;
            
            std::string moduleList;
            std::string functionList;
            std::string functionContentList;

            std::string binName;
            std::string filterName;

            void writeHeader();
            void writeFooter();
            bool writeModule(gi::mutatee::Module);
            void writeFunction(gi::mutatee::Function);
            void writeLoop(gi::mutatee::LoopTreeNode n,std::string indent);
            unsigned int getId();
            bool isInFilteredFunctions(Function& f);
        public:
            HtmlView(BPatch& bp, std::string fileName);
            HtmlView(gim::Mutatee* mutatee, std::string fileName,std::string filter = "");
            void output(std::string fileName);
            void createGraphFile(std::string fileName);
        };

    }
}

#endif	/* _BROWSER_H */
