/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   callgraph.cpp
 * @author Jan Mussler
 * @brief Callgraph and node implementation
 */

#include <iostream>

#include "callgraph.h"
#include "mutatee.h"

using namespace std;

using namespace gi::mutatee;
using namespace gi::mutatee::graph;
using namespace gi::filter;

void Node::addIncoming(Node* n, bool rec) {
    assert(n != 0);
    incoming.insert(n);
    if (rec == true) n->addOutgoing(this, false);
}

void Node::addOutgoing(Node* n, bool rec) {
    assert(n != 0);
    outgoing.insert(n);
    if (rec == true) n->addIncoming(this, false);
}

void Node::visitUntilDepth(GraphQueryResult* names, unsigned int newDepth, unsigned int visitNr, bool addSelf) {

    if (visitNr >= visited && !(newDepth > depth)) {
        return;
    }

    visited = visitNr;
    depth = newDepth;

    if (addSelf) {
        names->insert(getName());
    }

    for (nodeEdgeSet::iterator i = outgoing.begin(); i != outgoing.end(); ++i) {
        if (newDepth > 1) {
            (*i)->visitUntilDepth(names, newDepth - 1, visitNr);
        }
    }

}

void Node::visit(GraphQueryResult* names, unsigned int visitNr, bool addSelf) {
    if (visited >= visitNr) {
        return;
    }

    visited = visitNr;
    if (addSelf) {
        names->insert(getName());
    }

    for (nodeEdgeSet::iterator i = incoming.begin(); i != incoming.end(); ++i) {

        (*i)->visit(names, visitNr);
    }
}

void Node::visitForward(GraphQueryResult* names, unsigned int visitNr, bool addSelf) {
    if (visited >= visitNr) {
        return;
    }

    visited = visitNr;
    if (addSelf) {
        names->insert(getName());
    }

    for (nodeEdgeSet::iterator i = outgoing.begin(); i != outgoing.end(); ++i) {

        (*i)->visitForward(names, visitNr);
    }
}

Node::Node(CallGraph* gr, IFunction* f) : graph(gr), function(f), visited(0) {

}

unsigned int Node::countIncoming() {
    return outgoing.size();
}

nodeEdgeSet Node::getIncomingNodes() {
    return incoming;
}

nodeEdgeSet Node::getOutgoingNodes() {
    return outgoing;
}

unsigned int Node::countOutgoing() {
    return incoming.size();
}

IFunction* Node::getFunction() {
    return function;
}

NoInstNode::NoInstNode(CallGraph* gr, string name) : Node(gr, 0) {
    function = new ExternalFunction(name);
}

string Node::getName() {
    return function->getMangledName();
}

InstNode::InstNode(CallGraph* gr, IFunction* f) : Node(gr, f) {
    vector<IFunction*> fs = f->getSubCalls();
    assert(gr != 0);
    for (vector<IFunction*>::iterator i = fs.begin();
            i != fs.end(); i++) {
        graph->callNodeFrom(this, (*i)->getMangledName());
        delete (*i);
    }
    fs.clear();
}

CallGraph::CallGraph() : visitId(0) {

}

void CallGraph::callNodeFrom(Node* n,const string to) {
    if (to == "") return;

    mapNameToNode::iterator i = nameToNode.find(to);

    if (i == nameToNode.end()) {
        mapNameToNodeStack::iterator j = tmpCalls.find(to);
        if(j==tmpCalls.end()) {
            j = tmpCalls.insert(pair<string,stack<Node*>*>(to,new stack<Node*>())).first;
        }
        j->second->push(n);
        
        return;
    }
    
    (*i).second->addIncoming(n);
}

void CallGraph::addFunction(IFunction* func) {
    if (nameToNode.find(func->getMangledName()) == nameToNode.end()) {
        Node* n = new InstNode(this, func);
        
        nameToNode.insert(pair<string,Node*>(n->getName(),n));

        mapNameToNodeStack::iterator i = tmpCalls.find(n->getName());

        if (i != tmpCalls.end()) {
            stack<Node*>* s = i->second;
            while (!s->empty()) {
                n->addIncoming(s->top());
                s->pop();
            }
            tmpCalls.erase(n->getName());
        }
    }
}

unsigned int CallGraph::getNextVisitId() {
    return ++visitId;
}

GraphQueryResult* CallGraph::getFunctionNamesOnPathTo(string calling) {
    GraphQueryResult* result = new GraphQueryResult();

    if (!tmpCalls.empty()) {
        std::cout << "warning: there are deferred calls to functions still unresolved in call graph!\n";
    }

    mapNameToNode::iterator start = nameToNode.find(calling);
    if (start != nameToNode.end()) {
        unsigned int curVisitId = getNextVisitId();
        (*start).second->visit(result, curVisitId, false);
    }

    return result;
}

GraphQueryResult* CallGraph::getFunctionNamesOnPathToRule(IRule* rule) {
    GraphQueryResult* result = new GraphQueryResult();

    if (!tmpCalls.empty()) {
        std::cout << "warning: there are deferred calls to functions still unresolved in call graph!\n";
    }

    unsigned int curVisitId = getNextVisitId();

    for (mapNameToNode::iterator node = nameToNode.begin();
            node != nameToNode.end();
            node++) {
        if (rule->match((*node).second->getFunction())) {
            // start the path from rule matching pattern and add function names to result set
            (*node).second->visit(result, curVisitId, false);
        }
    }

    return result;
}

GraphQueryResult* CallGraph::getFunctionNamesOnPathFromRule(IRule* rule) {
    GraphQueryResult* result = new GraphQueryResult();

    if (!tmpCalls.empty()) {
        std::cout << "warning: there are deferred calls to functions still unresolved in call graph!\n";
    }

    unsigned int curVisitId = getNextVisitId();

    for (mapNameToNode::iterator node = nameToNode.begin();
            node != nameToNode.end();
            node++) {
        if (rule->match((*node).second->getFunction())) {
            // start the path from rule matching pattern and add function names to result set
            (*node).second->visitForward(result, curVisitId, false);
        }
    }

    return result;
}

GraphQueryResult* CallGraph::getFunctionsWithinDepthOf(string origin, unsigned int depth) {
    GraphQueryResult* result = new GraphQueryResult();

    if (!tmpCalls.empty()) {
        std::cout << "warning: there are deferred calls to functions still unresolved in call graph!\n";
    }

    //increase depth, because we start in origin
    depth++;

    mapNameToNode::iterator start = nameToNode.find(origin);
    if (start != nameToNode.end()) {
        unsigned int curVisitId = getNextVisitId();
        (*start).second->visitUntilDepth(result, depth, curVisitId, false);
    }
    else {
        std::cout << "warning: origin in filter for depth not found (" << origin << ")\n";
    }

    return result;
}

void graph::displayQResult(graph::GraphQueryResult* r) {
    std::cout << "\nresult size: " << r->size() << "\n";
    for (GraphQueryResult::iterator i = r->begin();
            i != r->end(); i++) {
        std::cout << "\t" << (*i) << "\n";
    }
    std::cout << "\n";
}

/**
 * Create nodes in CallGraph for functions not within the binary or not beeing instrumented
 *
 * this will in most cases create nodes for leaf nodes representing library calls that could not be resolved
 * and are referenced by name
 */
void CallGraph::transformTmpCalls() {
    if (!tmpCalls.empty()) {
        std::cout << "  evaluating " << tmpCalls.size() << " deferred calls\n";
        for (mapNameToNodeStack::iterator i = tmpCalls.begin();
                i != tmpCalls.end(); i++) {
            
            Node* n = new NoInstNode(this, (*i).first);
            nameToNode[n->getName()] = n;

            stack<Node*>* s = (*i).second;

            while (!s->empty()) {
                n->addIncoming(s->top(), true);
                s->pop();
            }
        }
        tmpCalls.clear();
    }
}

Node* CallGraph::getNodeForName(string name) {
    mapNameToNode::iterator i = nameToNode.find(name);

    if (i == nameToNode.end()) {
        return 0;
    }

    return (*i).second;
}
