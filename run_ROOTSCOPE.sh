#!/bin/bash

rootfiles=$@
nRootFiles=$#
path=$(pwd)
OUTFILE="${path}/rootscope_temp.C"

 


if (( nRootFiles == 0 )) ;then

    # rootfiles are an empty string.
(
    cat <<EOF1
#include "${path}/myROOTSCOPE.C"
ROOTSCOPE* app;
void rootscope_temp(){
    app = new ROOTSCOPE ( gClient->GetRoot());
}
EOF1
)>$OUTFILE
else
    # we have one root file or more . 
    outStr=""
    NEWLINE=$'\n\t'
    for rootfile in ${rootfiles}
    do
        outStr="${outStr}app->AddRootFile(\"${rootfile}\");"$NEWLINE 
    done
(
    cat <<EOF1
#include "${path}/myROOTSCOPE.C"
ROOTSCOPE* app;
void rootscope_temp(){
    app = new ROOTSCOPE ( gClient->GetRoot());
    ${outStr}
}
EOF1
)>$OUTFILE
fi
 
# print msg to user 
for rootfile in $rootfiles; do
    echo  "open $rootfile "
done
root -l $OUTFILE
