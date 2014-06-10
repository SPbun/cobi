echo
echo "Configurable Binary Instrumenter - Dependency Archive Build Script"
echo

if [ "x$1" = "x" ]
then
    INSTBASE=`pwd`
else
    if [ "x$1" = "x--help" ]
    then
        echo "Usage: $0 [<install_destination>]"
        echo " <install_destination> is the installation path of the dependencies and"
        echo "                       Cobi. By default everything is installed under"
        echo "                       the current working directory. Full pathnames"
        echo "                       are required."
        exit 0
    else
        if [ "x$1" = "x-h" ]
        then
            echo "Usage: $0 [<install_destination>]"
            echo " <install_destination> is the installation path of the dependencies and"
            echo "                       Cobi. By default everything is installed under"
            echo "                       the current working directory. Full pathnames"
            echo "                       are required."
            exit 0
        else
            INSTBASE=$1
        fi
    fi
fi

SRCBASE=`pwd`

mkdir -p $INSTBASE

if [ ! -d $INSTBASE ]
then
  echo "[Error] $INSTBASE is not a valid folder!"
  exit 0
fi

echo "Installing to folder: " $INSTBASE

read -s -n1 -p "Continue? y=yes or *=no" INVAR
if [ ! $INVAR = "y" ]
then
	echo
	echo "exiting..."
	exit 0
fi
echo

UNAME=`uname -m`
if [ $UNAME = "x86_64" ] 
then
	echo "Building Dyninst for Platform: x86_64-unknown-linux2.4"
else
	echo "Building Dyninst for Platform: i386-unknown-linux2.4"
fi

#####################################################################
echo "starting extraction of libraries..."
#####################################################################

echo
if [ ! -d $BOOST ]
then
    echo "extracting boost"
    tar xvf ${BOOST}.tar.gz > /dev/null
    rm ${BOOST}.tar.gz
fi

if [ ! -d $DYNINST ]
then
    echo "extracting Dyninst"
    tar xvf ${DYNINST}.tar.gz > /dev/null
    rm ${DYNINST}.tar.gz
fi

if [ ! -d $DWARF ]
then
    echo "extracting LibDwarf"
    tar xvf ${DWARF}.tar.gz > /dev/null
    rm ${DWARF}.tar.gz
fi

if [ ! -d $LIBELF ]
then
    echo "extracting LibElf"
    tar xvf ${LIBELF}.tar.gz > /dev/null
    rm ${LIBELF}.tar.gz
fi

if [ ! -d ${XERCES} ]
then
    echo "extracting Xerces"
    tar xvf ${XERCES}.tar.gz > /dev/null
    rm ${XERCES}.tar.gz
fi

if [ ! -d ${NASM} ]
then
    echo "extracting NASM"
    tar xvf ${NASM}.tar.gz > /dev/null
    rm ${NASM}.tar.gz
fi

#####################################################################
#                                                             Xerces
#####################################################################
echo "starting building of libraries..."
cd $SRCBASE

if [ ! -e $SRCBASE/logs ]
then
	mkdir $SRCBASE/logs
fi

if [ ! -e $INSTBASE/xerces3/lib/libxerces-c-3.1.so ]
then
    echo "Configuring Xerces-C..."
    cd ${XERCES}
    ./configure --prefix=$INSTBASE/xerces3 > ../logs/xerces_configure.log
    echo "Building Xerces-C..."
    make > ../logs/xerces_make.log
    make install > ../logs/xerces_make.log
fi

if [ ! -e $INSTBASE/xerces3/lib/libxerces-c-3.1.so ]
then
    echo "xerces3 build failed"
    exit 0
else
    rm -R -f $INSTBASE/${XERCES}
fi

#####################################################################
#                                                             libelf
#####################################################################
cd $SRCBASE
if [ ! -e $INSTBASE/libelf/lib/libelf.so ]
then
    echo "Configuring libelf..."
    cd ${LIBELF}
    ./configure --prefix=$INSTBASE/libelf > ../logs/libelf_configure.log
    echo "Building libelf..."
    make > ../logs/libelf_make.log
    make install > ../logs/libelf_install.log
fi

if [ ! -e $INSTBASE/libelf/lib/libelf.so ]
then
    echo "libelf build failed"
    exit 0
fi

cd $SRCBASE

#####################################################################
#                                                           libdwarf
#####################################################################
if [ ! -e $INSTBASE/libdwarf/libdwarf.so ]
then
    echo "Configuring libdwarf..."
    cd ${DWARF}/libdwarf
    export CFLAGS="-I$INSTBASE/libelf/include -fPIC"
    export CPPFLAGS="-I$INSTBASE/libelf/include -fPIC"
    ./configure --prefix=$INSTBASE/libdwarf --includedir=$INSTBASE/libelf/include --libdir=$INSTBASE/libelf/lib --enable-shared > $SRCBASE/logs/libdwarf_configure.log
    echo "Building libdwarf..."
    make > $SRCBASE/logs/libdwarf_make.log
    mkdir -p $INSTBASE/libdwarf/include
    cp libdwarf.h dwarf.h $INSTBASE/libdwarf/include/
    mkdir -p $INSTBASE/libdwarf/lib
    cp libdwarf.so libdwarf.a $INSTBASE/libdwarf/lib/
fi

if [ ! -e $INSTBASE/libdwarf/lib/libdwarf.so ]
then
    echo "libdwarf build failed"
    exit 0
fi

cd $SRCBASE

#####################################################################
#                                                               nasm
#####################################################################
if [ ! -e $INSTBASE/nasm/bin/nasm ]
then
    echo "Configuring NASM..."
    cd ${NASM}
    ./configure --prefix=$INSTBASE/nasm > ../logs/nasm_configure.log
    echo "Building NASM..."
    make > ../logs/nasm_make.log
    make install > ../logs/nasm_install.log
fi

if [ ! -e $INSTBASE/nasm/bin/nasm ]
then
    echo "nasm build failed"
    exit 0
fi

#####################################################################
#                                                            dyninst
#####################################################################
cd $SRCBASE
if [ $UNAME = "x86_64" ]
then
    export PLATFORM=x86_64-unknown-linux2.4
else
    export PLATFORM=i386-unknown-linux2.4
fi

if [ ! -e $INSTBASE/dyninstAPI/$PLATFORM/lib/libdyninstAPI.so ]
then
    echo "Configuring Dyninst..."

    export PATH=$PATH:$INSTBASE/nasm/bin
    export LDFLAGS="-L$INSTBASE/libdwarf -L$INSTBASE/libelf/lib"
   
    cd dyninstAPI/src/dyninst
    ./configure \
            --prefix=$INSTBASE/dyninstAPI \
            --with-libdwarf-incdir=$INSTBASE/libdwarf/include \
            --with-libdwarf-libdir=$INSTBASE/libdwarf/lib \
            --with-libelf-incdir=$INSTBASE/libelf/include \
            --with-libelf-libdir=$INSTBASE/libelf/lib > $SRCBASE/logs/dyninst_configure.log
    #	--with-libxml2-incdir=$INSTBASE/libxml2/inc \
    #	--with-libxml2-libdir=$INSTBASE/libxml2/lib

    echo "Building Dyninst..."
    make DyninstAPI > $SRCBASE/logs/dyninst_make.log
    #make install  > $SRCBASE/logs/dyninst_make.log
    cd $INSTBASE/dyninstAPI
    ln -s $PLATFORM/lib lib
fi

if [ ! -e $INSTBASE/dyninstAPI/lib/libdyninstAPI.so ]
then
    echo "Dyninst build failed"
    exit 0
fi

cd $SRCBASE


#####################################################################
#                                                              boost
#####################################################################
if [ ! -e $INSTBASE/boost/lib/libboost_regex.so ]
then
    echo "Configuring Boost"
    cd ${BOOST}
    ./bootstrap.sh --prefix=$INSTBASE/boost > ../logs/boost_configure.log
    echo "Building Boost using bjam (this may take a few minutes!)"
    ./bjam --clean > ../logs/boost_make.log
    ./bjam install --prefix=$INSTBASE/boost > ../logs/boost_install.log
fi

if [ ! -e $INSTBASE/boost/lib/libboost_regex.so ]
then
    echo "boost build failed"
    exit 0
fi

cd $SRCBASE

#####################################################################
#                                                               Cobi
#####################################################################
echo "...finished building dependencies"
echo
echo "Choose to continue and cobi will be build automatically!"
echo ""
echo "Otherwise you need to set LD_LIBRARY_PATH to include the dependencies:"
echo ""
echo "export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:$INSTBASE/boost/lib:$INSTBASE/dyninstAPI/lib:$INSTBASE/libdwarf:$INSTBASE/libelf/lib:$INSTBASE/xerces3/lib"
echo
echo "And run 'make' in cobi/ if present"
echo
read -s -n1 -p "Continue to build cobi? y=yes or *=no" INVAR
if [ ! $INVAR = "y" ]
then
	echo	
	echo "You need to set DYNINSTAPI_RT_LIB if you want to execute cobi:"
	#need to figure out what to set for 32bit RT lib in 64bit setup
	if [ $UNAME="x86_64" ]
	then
		echo "export DYNINSTAPI_RT_LIB=$INSTBASE/dyninstAPI/lib/libdyninstAPI_RT.so.1"
	else
		echo "export DYNINSTAPI_RT_LIB=$INSTBASE/dyninstAPI/lib/libdyninstAPI_RT.so.1"
	fi
	echo
	exit 0
fi

echo
echo "Building cobi in $INSTBASE/cobi"
echo

tar -xf cobi-${VERSION}.tar.gz

echo "changing Makeinstrumenter.conf"
sed -i "s#DEPBASE=\$(HOME)/local/cobi-deps/#DEPBASE=${INSTBASE}/#" cobi-$VERSION/Makefile.conf

#mv $SRCBASE/cobi-${VERSION}/Makefile.conf $SRCBASE/cobi-${VERSION}/Makefile.conf.template
#echo "DEPBASE=$INSTBASE/" > $SRCBASE/cobi-${VERSION}/Makefile.conf
#grep -v "DEPBASE=" $SRCBASE/cobi-${VERSION}/Makefile.conf.template >> $SRCBASE/cobi-${VERSION}/Makefile.conf

echo "write expvars.sh"
EXP_FILE=$SRCBASE/cobi-${VERSION}/scripts/expvars.sh

echo " #!/bin/bash" > $EXP_FILE
echo "export LD_LIBRARY_PATH=$INSTBASE/boost/lib:$INSTBASE/dyninstAPI/lib:$INSTBASE/libdwarf/lib:$INSTBASE/libelf/lib:$INSTBASE/xerces3/lib:\$LD_LIBRARY_PATH" >> $EXP_FILE
echo "export DYNINSTAPI_RT_LIB=$INSTBASE/dyninstAPI/lib/libdyninstAPI_RT.so" >> $EXP_FILE
echo "export PATH=$INSTBASE/cobi/bin:$PATH" >> $EXP_FILE

cd $SRCBASE/cobi-${VERSION}

export LD_LIBRARY_PATH=$INSTBASE/boost/lib:$INSTBASE/dyninstAPI/lib:$INSTBASE/libdwarf/lib:$INSTBASE/libelf/lib:$INSTBASE/xerces3/lib:$LD_LIBRARY_PATH
export DYNINSTAPI_RT_LIB=$INSTBASE/dyninstAPI/lib/libdyninstAPI_RT.so.1

make
if [ ! -e $SRCBASE/cobi-${VERSION}/cobi ]
then
	echo "build cobi failed"
	exit 0
fi

mkdir -p $INSTBASE/cobi/bin
cp $SRCBASE/cobi-${VERSION}/cobi $INSTBASE/cobi/bin/

cd $SRCBASE


echo
echo "build successful"
echo
echo "You may now go to cobi-${VERSION}/tests and execute:"
echo "make"
echo "make instrument"
echo
echo "But remember to update PATH, LD_LIBRARY_PATH and DYNINSTAPI_RT_LIB"
echo
echo "export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:$INSTBASE/boost/lib:$INSTBASE/dyninstAPI/lib:$INSTBASE/libdwarf/lib:$INSTBASE/libelf/lib:$INSTBASE/xerces3/lib"
echo "export DYNINSTAPI_RT_LIB=$INSTBASE/dyninstAPI/lib/libdyninstAPI_RT.so"
echo "export PATH=$INSTBASE/cobi/bin:\$PATH"
echo
echo "Or run '. $SRCBASE/cobi-${VERSION}/scripts/expvars.sh'"
echo

