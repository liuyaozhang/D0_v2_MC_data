#!/bin/bash

echo "setup cmssw"
cd /afs/cern.ch/user/z/zhangli/scratch1
source setupfile
cd CMSSW_10_2_15/src
eval `scramv1 runtime -sh` #cd mycmsw_x_y_x/src is equivalent to eval 'scram runtime -sh'
cd /afs/cern.ch/user/z/zhangli/public/FlowCorr/batch
echo PWD: $PWD

#../bin/corr2D_trg_d0 list/data.list.$1 $2
../bin/corr2D_trg_ref list/data.list.ref.$1
