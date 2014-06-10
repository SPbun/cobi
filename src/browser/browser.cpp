/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   browser.cpp
 * @author Jan Mussler
 * @brief Implementation of HTML report writer
 */

#include <algorithm>

#include "browser.h"

using namespace gi::browser;
using namespace gi::mutatee;
using namespace std;

string ui2s(unsigned int i) {
    stringstream s;
    s << i;
    return s.str();
}

/**
 * @param name string where < and > will be replaced with &lt; und &gt;
 * @return string printable in html doc
 */
string toHtmlEnt(string name) {
    while (name.find("<") != name.npos) {
        name.replace(name.find("<"), 1, "&lt;");
    }

    while (name.find(">") != name.npos) {
        name.replace(name.find(">"), 1, "&gt;");
    }
    return name;
}

HtmlView::HtmlView(BPatch& bp, string fileName) : id(1), listOfFunctions(0), binName(fileName), filterName("") {
    BPatch_binaryEdit* appBin = bp.openBinary(fileName.c_str());
    addSpace = static_cast<BPatch_addressSpace *> (appBin);
    appImage = addSpace->getImage();
    assert(appImage);
}

HtmlView::HtmlView(Mutatee* mutatee, std::string fileName, std::string filter) : binName(fileName), filterName(filter) {
    addSpace = mutatee->getAddressSpace();
    appImage = addSpace->getImage();

    set<IInstrumentableFunction*>* allFuncs = mutatee->getAllInstrumentableFunctions();

    // init map, map uses b-tree, faster than scanning the set
    for (set<IInstrumentableFunction*>::iterator i = allFuncs->begin();
            i != allFuncs->end();
            i++) {
        if((*i)->isMarkedForInstrumentation()) {
            functionsToShow[(*i)->getFunction()->getMangledName()] = true;
        }
    }

}

void HtmlView::writeHeader() {
    of << "<html>"
            << "<script>"
            "var activeLayer;"
            "function showLayer(layer) {"
            "l = document.getElementById(activeLayer);"
            "if(l) { l.style.display = 'none'; }"
            "activeLayer = layer;"
            "document.getElementById(layer).style.display='block';"
            "}"

            << "var activeFunction; "
            "function showFunctionLayer(layer) {"
            "l = document.getElementById(activeFunction);"
            "if(l) { l.style.display = 'none'; }"
            "activeFunction = layer;"
            "document.getElementById(layer).style.display='block';"
            "}"
            << "</script>"
            << "<style>"
            "td { font-size:11px; } "
            ".modHeader { background-color: #C2C2C2; font-size:12px; }"
            ".funcHeader { background-color: #C2C2C2; font-size:12px; }"
            ".modListHeader { background-color: #AAAAAA; font-size:12px; }"
            ".binInfoHeader { font-weight:800; background-color: #999999; font-size:12px; }"
            ".binInfo { font-size:12px; } "
            << "</style>"
            << "<body>"
            << "<table cellspacing=1 cellpadding=2><tr><td class=\"binInfoHeader\">Filename:</td><td class=\"binInfo\"> " << binName << "</td></tr>"
            "<tr><td class=\"binInfoHeader\">Filtername:</td><td class=\"binInfo\">" << filterName << "</td></tr>"
            "<tr><td class=\"binInfoHeader\">Functions in filter:</td><td class=\"binInfo\">" << (functionsToShow.size() > 0 ? ui2s(functionsToShow.size()) : "") << "</td></tr>"
            "</table>";
}

void HtmlView::writeFooter() {
    of << "</body></html>";
}

/* might be faster, but using map should work right now */
bool HtmlView::isInFilteredFunctions(Function& f) {
    if (listOfFunctions == 0) {
        return true;
    }
    else {
        return functionsToShow.find(f.getMangledName()) != functionsToShow.end();
    }
}

bool HtmlView::writeModule(Module m) {
    stringstream s;

    s << "m_" << getId();

    vector<Function>* fs = m.getFunctions();

    map<string, Function*> sorted;

    for (vector<Function>::iterator i = fs->begin();
            i != fs->end();
            i++) {
        if (isInFilteredFunctions(*i)) {
            sorted[i->getName()] = &(*i);
        }
    }

    if (sorted.size() == 0) {
        delete fs;
        return false;
    }

    moduleList += "<a href=\"#\" onClick=\"showLayer('";
    moduleList += s.str();
    moduleList += "');return false;\">";
    moduleList += m.getName() + " (" + ui2s(sorted.size()) + ")";
    moduleList += "</a>";
    moduleList += "<br>";

    functionList += "<div style=\"display:none;\" id=\"";
    functionList += s.str();
    functionList += "\">";

    string nsName = "";
    for (map<string, Function*>::const_iterator i = sorted.begin(); i != sorted.end(); ++i) {
        if (nsName != (i->second)->getNameSpace()) {
            nsName = (i->second)->getNameSpace();
            functionList += "<b>" + nsName + "</b>" + "<br>";
        }
        writeFunction(*(i->second));
    }

    functionList += "</div>";
    delete fs;
    return true;
}

void HtmlView::writeLoop(LoopTreeNode n, string indent) {
    functionContentList += indent + n.getName();
    functionContentList += "<br>";


    if (n.getBPNode()->numCallees() > 0) {
        vector<BPatch_function*> calls;
        n.getBPNode()->getCallees(calls, addSpace);
        for (vector<BPatch_function*>::iterator i = calls.begin(); i != calls.end(); i++) {
            Function f(*i);
            functionContentList += indent + "-&gt; <a href=\"#\" onClick=\"return false;\" alt=\"" + f.getName() + "\">" + toHtmlEnt(f.getFunctionName()) + "</a><br>";
        }
    }

    vector<LoopTreeNode> children = n.getChildren();

    for (vector<LoopTreeNode>::iterator i = children.begin();
            i != children.end();
            i++) {
        writeLoop((*i), indent + "&nbsp;&nbsp;");

    }

}

void HtmlView::writeFunction(Function f) {
    stringstream s;
    s << "f_" << getId();

    functionList += "<a href=# onClick=\"showFunctionLayer('";
    functionList += s.str();
    functionList += "')\">";
    functionList += toHtmlEnt(f.getFunctionName());
    functionList += "</a>";
    functionList += "<br>";

    functionContentList += "<div id=\"";
    functionContentList += s.str();
    functionContentList += "\" style=\"display:none;\"><b>Name:</b>";
    functionContentList += toHtmlEnt(f.getName());
    functionContentList += "<br><b>Mangled:</b>";
    functionContentList += f.getMangledName();
    functionContentList += "<br><b>Namespace:</b>";
    functionContentList += toHtmlEnt(f.getNameSpace());
    functionContentList += "<br><b>Class:</b>";
    functionContentList += toHtmlEnt(f.getClassName());

    functionContentList += "<br><br><b>CallSites in Function:</b><br>";

    if (f.getBPFunction()->getCFG()->containsDynamicCallsites()) {
        functionContentList += "<b>Dynamic CallSites in function!</b><br>";

    }

    // TODO : clear Vector und free Memory
    vector<IFunction*> called = f.getSubCalls();
    functionContentList += "Number of subroutine calls: ";
    {
        stringstream f;
        f << called.size();
        functionContentList += "" + f.str() + "<br>";
    }

    for (vector<IFunction*>::iterator i = called.begin(); i != called.end(); i++) {
        functionContentList += toHtmlEnt((*i)->getName());
        functionContentList += "<br>";
    }

    string& fcl = functionContentList;

    fcl += "<br><b>Loop Tree:</b><br>";
    LoopTree tree(f);
    vector<LoopTreeNode> children = tree.getChildren();

    for (vector<LoopTreeNode>::iterator i = children.begin();
            i != children.end();
            i++) {
        writeLoop((*i), "");
    }

    functionContentList += "</div>";
}

void HtmlView::output(string fileName) {
    of.open(fileName.c_str());
    if (!of.is_open()) {
        return;
    }
    writeHeader();

    vector<BPatch_module*>* modules = appImage->getModules();

    map<string, Module> sortedModules;
    for (vector<BPatch_module*>::iterator i = modules->begin();
            i != modules->end();
            i++) {
        Module m((*i));
        sortedModules[m.getName()] = m;
    }

    for (map<string, Module>::const_iterator i = sortedModules.begin(); i != sortedModules.end(); ++i) {
        writeModule(i->second);
    }


    of << "<table cellpadding=2 cellspacing=1><tr><td class=\"modListHeader\"><b>Modules</b></td>"
            << "<td class=\"modHeader\"><b>Functions in Module</b></td>"
            << "<td class=\"funcHeader\"><b>Function Detail:</b></td></tr><tr><td valign=top>";
    of << moduleList;
    of << "</td><td valign=top>";
    of << functionList;
    of << "</td><td valign=top>";
    of << functionContentList;
    of << "</td></tr></table>";

    writeFooter();
    of.close();
}

void HtmlView::createGraphFile(string fileName) {
    of.open(fileName.c_str());
    if (!of.is_open()) {
        return;
    }
    of << "digraph CallGraph {";

    map<string, string> functionToId;
    vector<string> functionList;
    vector<string> callList;

    vector<BPatch_module*>* modules = appImage->getModules();

    // build list of Function Nodes
    for (vector<BPatch_module*>::iterator i = modules->begin();
            i != modules->end();
            i++) {
        Module m((*i));

        vector<Function>* fs = m.getFunctions();
        for (vector<Function>::iterator i = fs->begin();
                i != fs->end();
                i++) {
            string s = (*i).getMangledName();
            if (functionsToShow.find(s) == functionsToShow.end()) {
                continue;
            }
            if ((*i).getName().substr(0, 3) == "std") continue;

            unsigned int id = getId();

            stringstream ss;
            ss << "f" << id;
            functionToId[s] = ss.str();
            ss << " [label=\"" << (*i).getName() << "\"];";
            functionList.push_back(ss.str());
        }
    }

    // Link Function Nodes if they call function
    // add new node if function calls external function
    for (vector<BPatch_module*>::iterator i = modules->begin();
            i != modules->end();
            i++) {
        Module m((*i));

        vector<Function>* fs = m.getFunctions();
        for (vector<Function>::iterator j = fs->begin();
                j != fs->end();
                j++) {

            if (functionsToShow.find((*j).getMangledName()) == functionsToShow.end()) continue;
            if ((*j).getName().substr(0, 3) == string("std")) continue;
            // std::cout << "calls from " << (*j).getName() << "\n";

            string origin = functionToId[(*j).getMangledName()];
            map<string, unsigned int> uniqueLookup;

            //TODO: clear Vector und free Memory
            vector<IFunction*> called = (*j).getSubCalls();
            for (vector<IFunction*>::iterator c = called.begin(); c != called.end(); c++) {
                string edge;

                if (functionToId.find((*c)->getMangledName()) != functionToId.end()) {
                    // function in programm
                    edge = origin;
                    edge += "->";
                    edge += functionToId[(*c)->getMangledName()];
                }
                else {
                    string tName = (*c)->getName();
                    if (tName.substr(0, 3) != string("MPI"))
                        continue;
                    // unresolved external function
                    edge += "\"";
                    edge += (*c)->getName();
                    edge += "\"";
                }

                edge += ";";

                if (uniqueLookup.find(edge) == uniqueLookup.end()) {
                    callList.push_back(edge);
                    uniqueLookup[edge] = 1;
                }
            }
        }
    }

    for (unsigned int i = 0; i < functionList.size(); ++i) {
        of << functionList[i] << "\n";
    }

    for (unsigned int i = 0; i < callList.size(); ++i) {
        of << callList[i] << "\n";
    }

    of << "}";
    of.close();
}

unsigned int HtmlView::getId() {
    return id++;
}
