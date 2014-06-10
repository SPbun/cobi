/*****************************************************************************
 **  Cobi    http://www.scalasca.org/                                       **
 *****************************************************************************
 **  Copyright (c) 2009-2010                                                **
 **  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
 **                                                                         **
 **  See the file COPYRIGHT in the base directory for details               **
 *****************************************************************************/

/**
 * @file   .h
 * @author Jan Mussler
 * @brief Call graph related class definitions
 */

#ifndef _CALLGRAPH_H
#define	_CALLGRAPH_H

#include <string>
#include <map>
#include <vector>
#include <set>
#include <stack>
#include "ruletree.h"
#include "ifunction.h"

namespace gif = gi::filter;

namespace gi {
    namespace mutatee {
        namespace graph {

            class Node;
            class CallGraph;

            typedef std::set<std::string> GraphQueryResult;
            typedef std::set<Node*> nodeEdgeSet;

            class Node {
            protected:
                /**
                 * CallGraph containing node
                 */
                CallGraph* graph;

                /**
                 * function which the node represents
                 */
                IFunction* function;

                /**
                 * set of nodes, presenting nodes calling this node
                 */
                nodeEdgeSet incoming;

                /**
                 * set of nodes, representing functions called
                 */
                nodeEdgeSet outgoing;

                /**
                 * last visit id used when visiting, used to mark visited nodes
                 */
                unsigned int visited;

                /**
                 * last depth, updated if shorter path is found
                 */
                unsigned int depth;
            public:
                Node(CallGraph* gr,IFunction* f);

                /**
                 * add a node to the list of incoming nodes                 
                 */
                void addIncoming(Node* n, bool rec = true);
                /**
                 * add a node to the list of outgoing nodes
                 */
                void addOutgoing(Node* n, bool rec = true);

                /**
                 * visit nodes from node for function named in origin, and visit all nodes within depth on call path
                 * @param addSelf wether origin function should me added or not
                 * @param names set of function names on the callpath
                 */
                void visitUntilDepth(GraphQueryResult* names, unsigned int newDepth, unsigned int visitNr, bool addSelf = true);

                /**
                 * visit the node and registered it with the result set
                 * continue with all the the nodes stored in incoming
                 *
                 * used to traverse the call path
                 *
                 * @param addSelf wether to add this function to the list or not
                 * @param names set of function names on the callpath
                 */
                void visit(GraphQueryResult* names, unsigned int visitNr,bool addSelf = true);
                
                /**
                 * same as visit, just walks all functions called by this one
                 **/
                void visitForward(GraphQueryResult* names, unsigned int visitNr,bool addSelf = true);

                /**
                 * get the name of the function represented by this node
                 */
                std::string getName();

                /**
                 * get the function represented by this node
                 */
                IFunction* getFunction();

                /**
                 * @return int number of incoming edges
                 */
                unsigned int countIncoming();

                /**
                 * @return set<Node*> set of function nodes calling this function
                 */
                nodeEdgeSet getIncomingNodes();

                /**
                 * @return int number of outgoing edges
                 */
                unsigned int countOutgoing();

                /**
                 * @return set<Node*> set of function nodes, being called by this node
                 */
                nodeEdgeSet getOutgoingNodes();
            };

            class InstNode : public Node {
            public:
                /**
                 * node using a function coming from the mutatee
                 */
                InstNode(CallGraph* gr, IFunction* f);
            };

            class NoInstNode : public Node {
            public:
                /**
                 * node using an external function
                 */
                NoInstNode(CallGraph* g, std::string name);
            };

            typedef std::map <std::string, Node*> mapNameToNode;
            typedef std::map<std::string, std::stack<Node*>* > mapNameToNodeStack;

            class CallGraph {
            private:
                /**
                 * @var currently used id for graph traversal, used to identify nodes already visited
                 */
                unsigned int visitId;

                /**
                 * all nodes, referenced by name of function
                 */
                mapNameToNode nameToNode;

                /**
                 * all outgoing calls to functions referenced by function name and stored in a stack
                 */
                mapNameToNodeStack tmpCalls;
            public:
                CallGraph();
                
                /**
                 * get a new id used in the traversal of the nodes to identfy which nodes are visited and which are not
                 */
                unsigned int getNextVisitId();

                /**
                 * add a new function to the call graph
                 */
                void addFunction(IFunction* func);

                /**
                 * register a call from node n to a node represented by the function name
                 */
                void callNodeFrom(Node* n, const std::string to);

                /**
                 * returns set of names containing all functions which may lead to the function named
                 * does not take into account dynamic call path, relys on static information only
                 * 
                 * @param calling target function name
                 * @return set of function names on path
                 */
                GraphQueryResult* getFunctionNamesOnPathTo(std::string calling);

                /**
                 * returns a result set with functions leading to calls to functions which conform to the rule
                 * @param rule rule specifying which functions will have to be called
                 * @return set of function names on path
                 */
                GraphQueryResult* getFunctionNamesOnPathToRule(gif::IRule* rule);

                /**
                 * returns a result set with functions being called by functions matchin the rule
                 *
                 * planned usage includes removing the adapter functions
                 *
                 * @param rule rule specifying which functions will have to be called
                 * @return set of function names on path
                 */
                GraphQueryResult* getFunctionNamesOnPathFromRule(gif::IRule* rule);

                /**
                 * return a set of functions within depth steps on the callpath
                 * @param origin name of function where to start
                 * @param depth max distance of functions to return
                 * @return set of function names on path
                 */
                GraphQueryResult* getFunctionsWithinDepthOf(std::string origin, unsigned int depth);

                /**
                 * transform unresolved calls to leaf nodes in graph, aggregating incoming calls
                 * this is necessary for function calls to external functions which are not resolved
                 */
                void transformTmpCalls();

                /**
                 *
                 * @param name mangled function name
                 * @return Node* get Node representing functions
                 */
                Node* getNodeForName(std::string name);
            };

            /**
             * produce some text output to the input result set
             */
            void displayQResult(graph::GraphQueryResult* r);
        }
    }
}

#endif	/* _CALLGRAPH_H */
