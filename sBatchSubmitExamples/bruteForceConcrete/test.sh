#!/bin/bash

CONFIG="remoll-ferrous-2022-08-31-bruteForceConcrete";
NUMSIMS=25;
DETNUMA=(9999) #TODO: WILL EVENTUALLY WANT TO READ FROM FERROUS DET.LIST
RANDOMN=1234567;         #DO WE WANT TO RANDOMIZE THIS???
SEQSTRT=26;               #SEQUENCE START
NEVENTS=200000;
SECSIMN=500000;

SEQENDS=$(( ${SEQSTRT} + ${NUMSIMS} - 1 ))

echo "Sequence starts ${SEQSTRT}";
echo "Seqence ends ${SEQENDS}";

for THISSIM in $(seq ${SEQSTRT} ${SEQENDS}); do
  FILE="remoll_${THISSIM}.mac";
  touch ${FILE}
  echo "/remoll/setgeofile                geometry/mollerMother.gdml"   >> ${FILE};
  echo "/remoll/parallel/setfile          geometry/mollerParallel_ferrous.gdml" >> ${FILE};
  echo "/remoll/physlist/parallel/enable"                               >> ${FILE};
  echo "/run/initialize"                                                >> ${FILE};
  echo "/remoll/printgeometry             true"                         >> ${FILE};
  echo "/remoll/addfield                  map_directory/V2DSg.9.75cm.parallel.txt" >> ${FILE};
  echo "/remoll/addfield                  map_directory/V2U.1a.50cm.parallel.txt"  >> ${FILE};
  echo "/remoll/evgen/beam/origin         0 0 -7.5 m"                   >> ${FILE};
  echo "/remoll/evgen/beam/rasx           5 mm"                         >> ${FILE};
  echo "/remoll/evgen/beam/rasy           5 mm"                         >> ${FILE};
  echo "/remoll/evgen/beam/corrx          0.065"                        >> ${FILE};
  echo "/remoll/evgen/beam/corry          0.065"                        >> ${FILE};
  echo "/remoll/evgen/beam/rasrefz       -4.5 m"                        >> ${FILE};
  echo "/remoll/beamene                   11 GeV"                       >> ${FILE};
  echo "/remoll/evgen/set                 beam"                         >> ${FILE};
  echo "/remoll/beamcurr                  65 microampere"               >> ${FILE};
  echo "/remoll/evgen/beamPolarization   +L"                            >> ${FILE};
  echo "/remoll/field/equationtype        2"                            >> ${FILE};
  echo "/remoll/field/steppertype         2"                            >> ${FILE};
  echo "/remoll/field/print"                                            >> ${FILE};
  echo " "                                                              >> ${FILE};
  echo "/remoll/SD/disable_all"                                             >> ${FILE};
  for I in ${!DETNUMA[@]}; do
    echo "/remoll/SD/enable                                   ${DETNUMA[$I]}" >> ${FILE};
    echo "/remoll/SD/detect                lowenergyneutral   ${DETNUMA[$I]}" >> ${FILE};
    echo "/remoll/SD/detect                secondaries        ${DETNUMA[$I]}" >> ${FILE};
    echo "/remoll/SD/detect                boundaryhits       ${DETNUMA[$I]}" >> ${FILE};
  done
  echo "           "                                                    >> ${FILE};
  echo "/remoll/kryptonite/enable"                                      >> ${FILE};
  echo "/remoll/kryptonite/add           VacuumKryptonite"              >> ${FILE};
  echo "/remoll/kryptonite/add           Kryptonite"                    >> ${FILE};
  echo "/remoll/kryptonite/add           Tungsten"                      >> ${FILE};
  echo "/remoll/kryptonite/add           Copper"                        >> ${FILE};
#  echo "/remoll/kryptonite/add           Lead"                          >> ${FILE};
  echo "/remoll/kryptonite/add           CW95"                          >> ${FILE};
  echo "/remoll/kryptonite/list"                                        >> ${FILE};
  echo "/process/list"                                                  >> ${FILE};

  echo "/remoll/seed                     ${THISSIM}"                    >> ${FILE};
  echo "/remoll/filename                 remollout_${THISSIM}.root"     >> ${FILE};
  echo "/run/beamOn                      ${NEVENTS}"                    >> ${FILE};

done;

