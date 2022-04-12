#!/bin/bash

DETNUMS=9030;
RANDOMN=1234567;
SEQNUMB=1;
NEVENTS=100;

#Going to need to create the tar ball for the simulations

#First we want to generate the remoll macros, start with one and then make a loop
rm -f remoll.mac #can get rid of this after testing
touch remoll.mac

echo "/remoll/setgeofile                geometry/mollerMother.gdml"   >> remoll.mac;
echo "/remoll/parallel/setfile          geometry/mollerParallel.gdml" >> remoll.mac;
echo "/remoll/physlist/parallel/enable"                               >> remoll.mac;
echo "/run/initialize"                                                >> remoll.mac;
echo "/remoll/printgeometry             true"                         >> remoll.mac;
echo " "                                                              >> remoll.mac
echo "/remoll/addfield       map_directory/V2DSg.9.75cm.parallel.txt" >> remoll.mac;
echo "/remoll/addfield       map_directory/V2U.1a.50cm.parallel.txt"  >> remoll.mac;
echo "/remoll/evgen/beam/origin         0 0 -7.5 m"                   >> remoll.mac;
echo "/remoll/evgen/beam/rasx           5 mm"                         >> remoll.mac;
echo "/remoll/evgen/beam/rasy           5 mm"                         >> remoll.mac;
echo "/remoll/evgen/beam/corrx          0.065"                        >> remoll.mac;
echo "/remoll/evgen/beam/corry          0.065"                        >> remoll.mac;
echo "/remoll/evgen/beam/rasrefz       -4.5 m"                        >> remoll.mac;
echo "/remoll/beamene                   11 GeV"                       >> remoll.mac;
#echo "/remoll/evgen/set                 elastic"                      >> remoll.mac;
echo "/remoll/evgen/set                 beam"                         >> remoll.mac;
echo "/remoll/beamcurr                  65 microampere"               >> remoll.mac;
echo "/remoll/evgen/beamPolarization   +L"                            >> remoll.mac;
echo "/remoll/field/equationtype        2"                            >> remoll.mac;
echo "/remoll/field/steppertype         2"                            >> remoll.mac;
echo "/remoll/field/print"                                            >> remoll.mac;
echo " "                                                              >> remoll.mac

#add from cip job
echo "/remoll/SD/disable_all" >> remoll.mac;
#for det in detectorList:
    echo "/remoll/SD/enable"                                         >> remoll.mac;
    echo "/remoll/SD/detect            lowenergyneutral  ${DETNUMS}" >> remoll.mac;
    echo "/remoll/SD/detect            secondaries       ${DETNUMS}" >> remoll.mac;
    echo "/remoll/SD/detect            boundaryhits      ${DETNUMS}" >> remoll.mac;

echo " "                                                             >> remoll.mac
echo "/remoll/kryptonite/enable"                                     >> remoll.mac;
echo "/remoll/kryptonite/add           VacuumKryptonite"             >> remoll.mac;
echo "/remoll/kryptonite/add           Tungsten"                     >> remoll.mac;
echo "/remoll/kryptonite/add           Copper"                       >> remoll.mac;
echo "/remoll/kryptonite/add           Lead"                         >> remoll.mac;
echo "/remoll/kryptonite/add           CW95"                         >> remoll.mac;
echo "/remoll/kryptonite/list"                                       >> remoll.mac;
echo "/process/list"                                                 >> remoll.mac;

echo "/remoll/seed                    ${RANDOMN}"                    >> remoll.mac;
echo "/remoll/filename                remollout_${SEQNUMB}.root"     >> remoll.mac;
#    echo "/remoll/printtargetinfo" >> remoll.mac;
echo "/run/beamOn                     ${NEVENTS}"                    >> remoll.mac;
