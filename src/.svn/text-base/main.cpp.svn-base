/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   main.cpp
 * @author Jan Mussler
 * @brief Cobi main file
 */

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <time.h>
#include <set>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include "boost/program_options.hpp"
#include "boost/tokenizer.hpp"

#include "generator.h"
#include "codeparsertest.h"
#include "ruletree.h"
#include "browser.h"
#include "codeparser.h"
#include "filterparser.h"
#include "adapterconfig.h"
#include "readableadapter.h"
#include "adapterparser.h"
#include "callgraph.h"
#include "properties.h"
#include "idregister.h"
#include "cobiconfig.h"
#include "spectemplates.h"
#include "xmlparser.h"

/*
 * 
 */
using namespace xercesc;
using namespace std;

namespace po = boost::program_options;

using gi::browser::HtmlView;
using gi::xml::FilterParser;
namespace gii = gi::instrumenter;
namespace gig = gi::generator;
namespace gix = gi::xml;

bool fileExists(string fileName) {
    ifstream input;
    input.open(fileName.c_str(), ios::in);

    bool ok = false;

    if (input.is_open() && input.good()) {
        ok = true;
    }
    input.close();

    return ok;
}

/*
 Call-back function for Dyninst Warning/Error Messages
 */
void errorMsg(BPatchErrorLevel severity, int number, const char* const* params) {

    switch (severity) {
        case BPatchFatal: std::cout << "[Dyninst] Fatal\n";
            break;
        case BPatchSerious: std::cout << "[Dyninst] Serious\n";
            break;
        case BPatchWarning: std::cout << "[Dyninst] Warning\n";
            break;
        case BPatchInfo: std::cout << "[Dyninst] Info: ";
            break;
    }
    std::cout << params[0] << "\n";
}

/* used for testing the graph functionallity, requires examplemutatee to work properly */
void GraphTest(graph::CallGraph& gr) {
    FunctionNames* fn = new FunctionNames(MatchStrategyFactory::getStrategy(MatchStrategyFactory::SIMPLE));
    fn->addPattern("cp::d");
    fn->addPattern("cp::z*");

    std::cout << "== results for cp::c == \n";
    graph::displayQResult(gr.getFunctionNamesOnPathTo("cp::c"));

    std::cout << "== results for cp::d == \n";
    graph::displayQResult(gr.getFunctionNamesOnPathTo("cp::d"));

    std::cout << "== results for cp::vier == \n";
    graph::displayQResult(gr.getFunctionNamesOnPathTo("cp::vier"));

    std::cout << "== results for rule == \n";
    graph::displayQResult(gr.getFunctionNamesOnPathToRule(fn));
}

void writeScalascaFilter(string fileName, Mutatee* mutatee, filterResult* result, bool invertFilter = false) {
    // generate a filter file for scalasca with all the functions that are instrumented
    std::ofstream of;
    of.open(fileName.c_str());
    if (of.is_open()) {

        map<string, bool> functionList;
        map<string, bool> instrumentedByFilter;

        for (set<IInstrumentableFunction*>::const_iterator i = result->begin();
                i != result->end();
                i++) {
            instrumentedByFilter[(*i)->getFunction()->getMangledName()] = true;
        }

        set<IInstrumentableFunction*>* instFuncs = mutatee->getAllInstrumentableFunctions();
        for (set<IInstrumentableFunction*>::const_iterator i = instFuncs->begin();
                i != instFuncs->end();
                i++) {
            if ((instrumentedByFilter.find((*i)->getFunction()->getMangledName()) == instrumentedByFilter.end()) == invertFilter) {
                functionList[(*i)->getFunction()->getName()] = true;
            }
        }

        for (map < string, bool>::const_iterator i = functionList.begin(); i != functionList.end(); i++) {
            of << (*i).first << "\n";
        }

        of.close();

    }
    else {
        std::cout << "[WARNING]: could not open file to store Scalasca filter\n";
    }
}

void writeHtmlReport(const string& reportFileName, Mutatee* mutatee, const string& binaryFileName, const set<string>& useFilters) {
    string usedFilterName = "";

    if (useFilters.size() > 0) {
        for (set<string>::const_iterator i = useFilters.begin(); i != useFilters.end(); i++) {
            usedFilterName += ((i != useFilters.begin()) ? ", " : "") + (*i);
        }
    }
    else {
        usedFilterName = "no filter used";
    }

    HtmlView view(mutatee, binaryFileName, usedFilterName);
    view.output(reportFileName);
    view.createGraphFile(reportFileName + ".graph");
}

void writeIdsToFile(string idListFile) {
    std::ofstream of;
    of.open(idListFile.c_str());
    if (of.is_open()) {

        // save all IDs with their name to one file
        of << gii::IdRegister::get().getList(gii::IdRegister::FUNCTION,
                gii::getCfg().getString(gii::Cobiconfig::ID_LIST_SEPERATOR_IDVALUE),
                gii::getCfg().getString(gii::Cobiconfig::ID_LIST_SEPERATOR_ITEMS));
        of << "\n";
        of << gii::IdRegister::get().getList(gii::IdRegister::LOOP,
                gii::getCfg().getString(gii::Cobiconfig::ID_LIST_SEPERATOR_IDVALUE),
                gii::getCfg().getString(gii::Cobiconfig::ID_LIST_SEPERATOR_ITEMS));
        of << "\n";
        of << gii::IdRegister::get().getList(gii::IdRegister::CALLSITE,
                gii::getCfg().getString(gii::Cobiconfig::ID_LIST_SEPERATOR_IDVALUE),
                gii::getCfg().getString(gii::Cobiconfig::ID_LIST_SEPERATOR_ITEMS));

        of.close();
    }
    else {
        std::cout << "[WARNING]: could not open file to store list of IDs\n";
    }
}

void listProperties() {
    vector<string> s = PropertyFactory::Get().getPropertyList();
    std::cout << "\n=== Available Properties ===\n\n";
    for (unsigned int i = 0; i < s.size(); ++i) {
        std::cout << " " << s[i];
    }
    std::cout << "\n";
}

void setCobiConfigToAdapterConfig(IReadableAdapterConfig* config) {
    if(config->getFunctionForInitCode()!="") {
        Cobiconfig::getCfg().setString(Cobiconfig::FUNCTION_FOR_INIT_CODE,config->getFunctionForInitCode());
    }
    
    if(config->getFunctionForFinalizeCode()!="") {
        Cobiconfig::getCfg().setString(Cobiconfig::FUNCTION_FOR_FINALIZE_CODE,config->getFunctionForFinalizeCode());
    }

    if(config->saveAllFPRs()) {
        Cobiconfig::getCfg().setFlag(Cobiconfig::FLOAT_SAVE_ALLWAYS,true);
    }
}

int main(int argc, char** argv) {

    time_t tStart, tEnd;

    // vars to store seconds used in parts of instrumenter
    double secOpen, secParse, secGraph, secFilter, secInst, secInitProp, secWrite;

    // strings for application parameters (either command line or option file)
    string filterFileName;
    string adapterFileName;
    string binaryFileName;
    string modifiedFileName;
    string listOfFilters;
    string reportFileName;
    string scalascaFilter;
    string notScalascaFilter;
    string idListFile;
    string tplAdapter;
    string tplFilter;

    /* Setup command line and file parameters */
    po::options_description avail("Available Options");

    po::options_description fileparams("Input and Output files");
    fileparams.add_options()
            ("filters,f", po::value<string > (&filterFileName), "filter file to use")
            ("adapter,a", po::value<string > (&adapterFileName), "adapter file to use")
            ("bin,b", po::value<string > (&binaryFileName), "binary to modify")
            ("out,o", po::value<string > (&modifiedFileName), "output file")
            ("use", po::value<string > (&listOfFilters)->default_value(""), "use named filter (default use all filters in specified file)")
            ("report", po::value<string > (&reportFileName)->default_value(""), "file name html report")
            //            ("scalasca-filter", po::value<string > (&scalascaFilter)->default_value(""), "filter file with functions instrumented")
            //            ("not-scalasca-filter", po::value<string > (&notScalascaFilter)->default_value(""), "filter file with functions not instrumented")
            ("id-list-file", po::value<string > (&idListFile)->default_value(""), "file where to store ids with the region name")
            ("tpl-filter", po::value<string > (&tplFilter)->default_value(""), "write template filter to file specified")
            ("tpl-adapter", po::value<string > (&tplAdapter)->default_value(""), "write template adapter to file specified");

    po::options_description flags("Flags");
    flags.add_options()
            ("help", "lists available options")
            ("preview", "no instrumentation done, just show results of filter and produce report if specified")
            ("test", "run parser tests and exit")
            // this needs the example mutatee installed thus disabled
            //       ("graphTest", "run graph tests and exit")
            ("ignore-noentry", "instrument functions with no entry")
            ("ignore-noexit", "instrument functions with no exit")
            ("show-ipaddress", "show instrumentation point addresses of functions")
            ("list-all-properties", "list all properties that are supported")
            ("with-dependencies", "open all dependency libraries for instrumentation")
            ("list-functions", "lists all instrumented functions")
            ("show-timings", "show where instrumenter spent its time")
            // ("ignore-syntax-errors","continue with errors in adapter specification")
            ;
    //        ("loglevel", po::value<int>()->default_value(0), "loglevel");

    avail.add(fileparams).add(flags);

    /* END parameter setup */

    ifstream inputConfig;
    // inputConfig.open("options.in", ios::in); // uncomment if input file is needed ( e.g. debugging in IDE )

    po::variables_map parameterMap;

    if (!inputConfig.is_open()) { // input file > command line options, inputfile not enabled by default see above
        po::store(po::parse_command_line(argc, argv, avail), parameterMap);
    }
    else { // parse option file
        po::store(po::parse_config_file(inputConfig, avail), parameterMap);
        inputConfig.close();
    }

    po::notify(parameterMap);

    if (parameterMap.count("help")) {
        std::cout << avail << "\n";
        return EXIT_SUCCESS;
    }

    if (parameterMap.count("list-all-properties")) {
        listProperties();
        return EXIT_SUCCESS;
    }

    {
        bool exitTpl = false;

        if (tplFilter != "") {
            gii::writeFilterSpec(tplFilter);
            exitTpl = true;
        }

        if (tplAdapter != "") {
            gii::writeAdapterSpec(tplAdapter);
            exitTpl = true;
        }

        if (exitTpl)
            return EXIT_SUCCESS;
    }

    if (!parameterMap.count("test")
            && (!parameterMap.count("bin") || !parameterMap.count("filters") || (!parameterMap.count("adapter") && !parameterMap.count("preview")))) {
        std::cout << avail << "\n";
        return EXIT_SUCCESS;
    }

    if (parameterMap.count("ignore-noentry")) {
        gii::getCfg().setFlag(gii::Cobiconfig::IGNORE_NOENTRY, true);
    }

    if (parameterMap.count("ignore-noexit")) {
        gii::getCfg().setFlag(gii::Cobiconfig::IGNORE_NOEXIT, true);
    }

    if (parameterMap.count("show-ipaddress")) {
        gii::getCfg().setFlag(gii::Cobiconfig::SHOW_IP_ADDRESSES, true);
    }

    if (parameterMap.count("with-dependencies")) {
        gii::getCfg().setFlag(gii::Cobiconfig::LOAD_LIBRARIES, true);
    }

    if (parameterMap.count("list-functions")) {
        gii::getCfg().setFlag(gii::Cobiconfig::LIST_INSTRUMENTED_FUNCTIONS, true);
    }

    /* END OF Parameter check */

    std::cout << "====================================================\n";
    std::cout << "Setting up Dyninst\n";
    BPatch bp;
    std::cout << "Dyninst loaded...\n";
    bp.registerErrorCallback(errorMsg);
    std::cout << "Callback registered...\n";
    std::cout << "====================================================\n";
    std::cout << "\n";

    if (parameterMap.count("test")) {
        gig::parser::CodeParserTest::syntaxTest();
        gig::parser::CodeParserTest::treeOutputTest();
        return EXIT_SUCCESS;
    }

    if (modifiedFileName == "") {
        modifiedFileName = "instr_" + binaryFileName;
    }

    bool fileNotFoundError = false;

    if (!fileExists(adapterFileName)) {
        std::cout << "[Error]: Adapter Specification File not found!\n\t -> " << adapterFileName << "\n";
        fileNotFoundError = true;
    }

    if (!fileExists(filterFileName)) {
        std::cout << "[Error]: Filter Specification File not found!\n\t -> " << filterFileName << "\n";
        fileNotFoundError = true;
    }

    if (!fileExists(binaryFileName)) {
        std::cout << "[Error]: Target Binary not found!\n\t -> " << binaryFileName << "\n";
        fileNotFoundError = true;
    }

    if (fileNotFoundError) {
        return EXIT_FAILURE;
    }

    std::cout << "\n";
    std::cout << "====================================================\n";
    std::cout << "using the following settings:\n";
    std::cout << "binary to instrument: " << binaryFileName << "\n";
    std::cout << "save modified binary as: " << modifiedFileName << "\n";
    std::cout << "adapter description: " << adapterFileName << "\n";
    std::cout << "filter descriptions: " << filterFileName << "\n";
    std::cout << "using filter(s): " << listOfFilters << "\n";
    std::cout << "====================================================\n";
    std::cout << "\n";

    FilterParser fp;
    FilterSet* filters = fp.parseFile(filterFileName);
    if (filters == 0) {
        std::cout << "\n[Error]: Filter Specification errorneous!\n";
        return EXIT_FAILURE;
    }

    IReadableAdapterConfig* config = gi::xml::AdapterParser::getConfigFromFile(adapterFileName);
    if (config == 0) {
        std::cout << "\n[Error]: Adapter Specification errorneous!\n";
        return EXIT_FAILURE;
    }

    setCobiConfigToAdapterConfig(config);

    time(&tStart);
    Mutatee* mutatee = DynFactroy::getMutatee(bp, binaryFileName,config);
    assert(mutatee != 0);
    time(&tEnd);
    secOpen = difftime(tEnd, tStart);

    time(&tStart);

    // init mutatee, to get all functions from dyninst
    mutatee->getAllFunctions();

    time(&tEnd);
    secParse = difftime(tEnd, tStart);

    std::cout << "\n===     generating call graph      ===\n";
    time(&tStart);
    {
        graph::CallGraph& gr = mutatee->getCallGraph(); // do graph creation here explicitly to time
        gr.getNextVisitId(); // remove warning for unused variable
    }
    time(&tEnd);
    std::cout << "=== finished generating call graph ===\n\n";
    secGraph = difftime(tEnd, tStart);

    mutatee->setNameOfFunctionWhereToPlaceInit(gii::getCfg().getString(gii::Cobiconfig::FUNCTION_FOR_INIT_CODE));
    mutatee->setNameOfFunctionWhereToPlaceFinalize(gii::getCfg().getString(gii::Cobiconfig::FUNCTION_FOR_FINALIZE_CODE));

    mutatee->setAdapterRule(config->getAdapterFilterRule());

    mutatee->getAllInstrumentableFunctions();

    time(&tStart);
    PropertyFactory::Get().InitProperties(mutatee); // properties only setup during filter parsing
    time(&tEnd);
    secInitProp = difftime(tEnd, tStart);
    /*
     *  // removed requires example mutatee present

        if (parameterMap.count("graphTest")) {
            GraphTest(gr);
            return EXIT_SUCCESS;
        }
     */
    IFilter* filter = 0;
    filterResult* result = 0;

    set<string> useFilters = gix::getTokensFromString(",", listOfFilters);

    // no filters specified, use all filters in the file
    if (useFilters.size() == 0) {
        useFilters = filters->getFilterNames();
    }

    std::cout << "===     evaluating filters      ===\n";
    time(&tStart);
    for (set<string>::const_iterator i = useFilters.begin(); i != useFilters.end(); i++) {
        try {
            filter = filters->getFilterByName(*i);
            result = filter->getResult(mutatee->getAllInstrumentableFunctions());
            delete result; // instrumentation is marked in mutatee, result points to those functions only
        } catch (gif::ex::ReferencingUndefinedFilter& e) {
            std::cout << "\n[ERROR]: Undefined filter specified \"" << (*i) << "\"\n\n";
            exit(EXIT_FAILURE);
        }
    }

    time(&tEnd);
    std::cout << "=== evaluating filters finished ===\n\n";
    secFilter = difftime(tEnd, tStart);

    if (scalascaFilter != "") {
        writeScalascaFilter(scalascaFilter, mutatee, result);
    }

    if (notScalascaFilter != "") {
        writeScalascaFilter(notScalascaFilter, mutatee, result, true);
    }

    if (reportFileName != "") {
        writeHtmlReport(reportFileName, mutatee, binaryFileName, useFilters);
    }

    // if not preview, instrument all functions marked for instrumentation by the filters
    if (!parameterMap.count("preview")) {
        if (idListFile == "") {
            gii::IdRegister::get().disable();
        }

        time(&tStart);
        gii::instrument(mutatee, config);
        time(&tEnd);
        secInst = difftime(tEnd, tStart);

        time(&tStart);
        mutatee->save(modifiedFileName);
        time(&tEnd);
        secWrite = difftime(tEnd, tStart);

        if (idListFile != "") {
            writeIdsToFile(idListFile);
        }
    }
    else {
        secInst = secWrite = 0;
    }

    delete mutatee;

    if (parameterMap.count("show-timings")) {
        std::cout << "=== timing ===\n";

        std::cout << "open binary     : " << secOpen << " seconds\n";
        std::cout << "parse binary    : " << secParse << " seconds\n";
        std::cout << "compute graph   : " << secGraph << " seconds\n";
        std::cout << "init properties : " << secInitProp << " seconds\n";
        std::cout << "filter          : " << secFilter << " seconds\n";
        std::cout << "instrument      : " << secInst << " seconds\n";
        std::cout << "save            : " << secWrite << " seconds\n";

        std::cout << "==============\n";
    }

    return EXIT_SUCCESS;
}
