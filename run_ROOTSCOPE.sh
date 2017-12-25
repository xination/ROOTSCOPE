#!/bin/bash

rootfile=$1
path=$(pwd)
OUTFILE="${path}/rootscope_temp.C"

rootscope_path="${path}/myROOTSCOPE.C"

#
# create a root script
#
if [ -z $1 ] 
then
    #no argument
(
    cat <<EOF1
#include <${path}/myROOTSCOPE.C>
 void rootscope_temp(){
     new ROOTSCOPE ( gClient->GetRoot());
 }
EOF1
)>$OUTFILE

    echo "use ctrl+r to load a rootfile containing TH1 or TH2"
else

(
cat <<EOF1
#include <${rootscope_path}>
 void rootscope_temp(){
     new ROOTSCOPE ( gClient->GetRoot(), "$path/$rootfile" );
 }

EOF1
)>$OUTFILE

fi
root -l $OUTFILE
