## -*- Makefile -*-
##
## User: jmussler
## Time: Sep 9, 2010 10:35:14 AM
##

VERSION = 1.0

#goto make.conf to set the necessary variables to libraries and folders
include Makefile.conf

filterSourceFiles = calledinloop.o callgraph.o callpath.o countcallee.o countcallsites.o countinst.o depth.o enterpoints.o exitpoints.o filter.o

filterSourceFiles += filterelement.o filterparser.o filterset.o hasloop.o hasoverlay.o islibraryfunction.o matchstrings.o mccabe.o nodehandlers.o

filterSourceFiles += patterns.o propertyfactory.o retfloatordouble.o ruletree.o sourcevolume.o templatepropfactory.o

generatorObjFiles = assignment.o binaryop.o concatconstants.o conditional.o constant.o context.o contextconstant.o functioncall.o iexpression.o implcontexts.o sequence.o term.o gvariable.o boolexpression.o functionparam.o

grammarObjFiles = codeparser.o codeparsertest.o snippetfactory.o

instObjFiles = adapterconfig.o adapterparser.o callsiteinstrumentation.o dependency.o functioninstrumentation.o instrument.o instrumentablefunction.o loopinstrumentation.o variable.o idregister.o cobiconfig.o spectemplates.o

mutateeObjFiles = dynfactory.o function.o loop.o module.o mutatee.o virtualfunction.o

browserObjFiles = browser.o

all: link

$(generatorObjFiles): %.o: src/generator/%.cpp
	$(CXX) $(DEBUG) $(OPT) $(WARN) -c -o $(OBJ_DIR)$@ $(INC) $<

$(grammarObjFiles): %.o: src/generator/%.cpp
	$(CXX) $(DEBUG) $(OPT) $(WARN) -c -o $(OBJ_DIR)$@ $(INC) $<

$(instObjFiles): %.o: src/instrumenter/%.cpp
	$(CXX) $(DEBUG) $(OPT) $(WARN) -c -o $(OBJ_DIR)$@ $(INC) $<

$(mutateeObjFiles): %.o: src/mutatee/%.cpp
	$(CXX) $(DEBUG) $(OPT) $(WARN) -c -o $(OBJ_DIR)$@ $(INC) $<

$(browserObjFiles): %.o: src/browser/%.cpp
	$(CXX) $(DEBUG) $(OPT) $(WARN) -c -o $(OBJ_DIR)$@ $(INC) $<

$(filterSourceFiles): %.o: src/filter/%.cpp
	$(CXX) $(DEBUG) $(OPT) $(WARN) -c -o $(OBJ_DIR)$@ $(INC) $<

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

main.o:
	$(CXX) $(DEBUG) $(OPT) $(WARN) -c -o $(OBJ_DIR)/main.o $(INC) src/main.cpp

xmlparser.o:
	$(CXX) $(DEBUG) $(OPT) $(WARN) -c -o $(OBJ_DIR)/xmlparser.o $(INC) src/xml/xmlparser.cpp

cicomp: $(OBJ_DIR) $(filterSourceFiles) $(generatorObjFiles) $(grammarObjFiles) $(instObjFiles) $(mutateeObjFiles) $(browserObjFiles) xmlparser.o main.o 

link: cicomp
	$(CXX) $(DEBUG) $(OPT) $(WARN) -o $(BIN_NAME) $(OBJ_DIR)*.o $(LIB)

onlylink:
	$(CXX) $(DEBUG) $(OPT) $(WARN) -o $(BIN_NAME) $(OBJ_DIR)*.o $(LIB)

install:
	cp $(BIN_NAME) $(INSTALL_DIR)

clean:
	rm -R -f $(OBJ_DIR)
	rm -f $(BIN_NAME)

test:
	./$(BIN_NAME) --test

dist: doc
	rm -rf cobi-$(VERSION) cobi-$(VERSION).tar.gz
	mkdir cobi-$(VERSION)
	cp -r COPYRIGHT INSTALL Makefile Makefile.conf manual.pdf README include src scripts tests tutorialcode xmlschemas xmlspecs cobi-$(VERSION)/
	tar -cf cobi-$(VERSION).tar cobi-$(VERSION)
	gzip cobi-$(VERSION).tar
	rm -rf cobi-$(VERSION)

distcheck: dist
	tar -xf cobi-$(VERSION).tar.gz
	(cd cobi-$(VERSION); make; make test)
	rm -rf cobi-$(VERSION)

manual.pdf: manual/*.tex manual/images/* manual/*.bib
	(cd manual; pdflatex main.tex; pdflatex main.tex)
	mv manual/main.pdf manual.pdf

doc: manual.pdf
