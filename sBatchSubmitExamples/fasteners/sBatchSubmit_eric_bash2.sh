#!/bin/bash

CONFIG="remoll-ferrous-2022-10-26-fasteners-new";
NUMSIMS=2000;
DETNUMA=(9010 9020 9030) #TODO: WILL EVENTUALLY WANT TO READ FROM FERROUS DET.LIST
RANDOMN=1234567;         #DO WE WANT TO RANDOMIZE THIS???
SEQSTRT=1;               #SEQUENCE START
NEVENTS=5000000;
SECSIMN=500000;

SEQENDS=$(( ${SEQSTRT} + ${NUMSIMS} - 1 ))

MYUSER="ericking@jlab.org"; #IF WANT TO SUBMIT VIA XML THEN NEED THIS
SRCDIR="/work/halla/moller12gev/ericking/remoll6/remoll";
OUTDIR="/volatile/halla/moller12gev/ericking/${CONFIG}";
SKIMFL="/w/halla-scshelf2102/moller12gev/ericking/remoll5/remoll/rootfiles/primaryskims/skimTreeMulti.C"

#DIRECTORY NAMES TO KEEP EVERYTHING ORGANIZED IN OUTPUT
PRIMACDIR="zz_primaryMacroFiles";
SECMACDIR="zz_secondaryMacroFiles";
PRIROOTDR="zz_primaryRootFiles";
SECROOTDR="zz_secondaryRootFiles";
SKMROOTDR="zz_skimmedRootFiles";
SLRMOUTPT="zz_slurmOutputFiles";
SLRMSHELL="zz_slurmShellScripts";

#CLEANUP JUST IN CASE ANYTHING REMAINS
rm -f define.tar.gz;

#CREATE CSV STRING OF DET NUMBERS
DETLIST=$(IFS=, ; echo "${DETNUMA[*]}");
echo Detector List: ${DETLIST};
echo ${#DETNUMA[@]}

#TOTAL PRIMARY EVENTS
TPRIEVT=$( bc -l <<< "${NUMSIMS} * ${NEVENTS}" )
echo "Total primary events: $TPRIEVT";

############################################################################################
#CREATE SLURM WORK DIRECTORIES, REMOLL JOBS DIRECTORY (AS A RECORD)
############################################################################################
[ -d ${SRCDIR}/jobs/${CONFIG} ] || mkdir ${SRCDIR}/jobs/${CONFIG};
SOUTDIR="${OUTDIR}/${NEVENTS}_EVT_x_${NUMSIMS}";
[ -d ${OUTDIR} ] || mkdir ${OUTDIR};
[ -d ${SOUTDIR} ] || mkdir ${SOUTDIR};
#TODO: GOING TO NEED A CONDITION HERE THAT IF THE DIRECTORY EXISTS
#THEN STOP AND GET NEW CONFIGURATION NAME

############################################################################################
# CREATE THE FERROUS MACROS FOR SECOND EXTGEN SIMULATIONS -- THEY MAY NOT ALL BE USED
############################################################################################
for I in ${!DETNUMA[@]}; do
  FILE="ferrous_${DETNUMA[$I]}_V2parallel_0.mac";
  echo "/remoll/setgeofile                  geometry/mollerMother_ferrous.gdml"      >> ${FILE};
  echo "/remoll/parallel/setfile            geometry/mollerParallel.gdml"            >> ${FILE};
  echo "/remoll/physlist/parallel/enable"                                            >> ${FILE};
  echo "/run/initialize"                                                             >> ${FILE};
  echo "/remoll/addfield                    map_directory/V2DSg.9.75cm.parallel.txt" >> ${FILE};
  echo "/remoll/addfield                    map_directory/V2U.1a.50cm.parallel.txt"  >> ${FILE};
  echo "#/remoll/addfield                    map_directory/V2DSg.9.75cm.txt"         >> ${FILE};
  echo "#/remoll/addfield                    map_directory/V2U.1a.50cm.txt"          >> ${FILE};
  echo "/remoll/SD/disable_all"                                                      >> ${FILE};
  echo " "                                                                           >> ${FILE};
  echo "#/remoll/SD/enable                  ${DETNUMA[$I]}"                          >> ${FILE};
  echo "#/remoll/SD/detect lowenergyneutral ${DETNUMA[$I]}"                          >> ${FILE};
  echo "#/remoll/SD/detect secondaries      ${DETNUMA[$I]}"                          >> ${FILE};
  echo "#/remoll/SD/detect boundaryhits     ${DETNUMA[$I]}"                          >> ${FILE};
  echo " "                                                                           >> ${FILE};
  echo "/remoll/SD/enable                  9928"                                     >> ${FILE};
  echo "/remoll/SD/detect lowenergyneutral 9928"                                     >> ${FILE};
  echo "/remoll/SD/detect secondaries      9928"                                     >> ${FILE};
  echo "/remoll/SD/detect boundaryhits     9928"                                     >> ${FILE};
  echo " "                                                                           >> ${FILE};
  echo "/remoll/SD/enable                  28"                                       >> ${FILE};
  echo "/remoll/SD/detect lowenergyneutral 28"                                       >> ${FILE};
  echo "/remoll/SD/detect secondaries      28"                                       >> ${FILE};
  echo "/remoll/SD/detect boundaryhits     28"                                       >> ${FILE};
  echo " "                                                                           >> ${FILE};
  echo "/remoll/SD/enable                  9911"                                     >> ${FILE};
  echo "/remoll/SD/detect lowenergyneutral 9911"                                     >> ${FILE};
  echo "/remoll/SD/detect secondaries      9911"                                     >> ${FILE};
  echo "/remoll/SD/detect boundaryhits     9911"                                     >> ${FILE};
  echo " "                                                                           >> ${FILE};
  echo "## External Generator"                                                       >> ${FILE};
  echo "/remoll/evgen/set                  external"                                 >> ${FILE};
  echo "/remoll/evgen/external/file        o_remollSkimTree${DETNUMA[$I]}_0.root"      >> ${FILE};
  echo "/remoll/evgen/external/detid       ${DETNUMA[$I]}"                           >> ${FILE};
  echo "/remoll/evgen/external/zOffset     0.001"                                    >> ${FILE};
  echo "/remoll/filename                   o_ferrous_extgen_${DETNUMA[$I]}_V2parallel_0.root"    >> ${FILE};
  echo "/remoll/kryptonite/enable"                                                   >> ${FILE};
  echo "/run/beamOn                        ${SECSIMN}"                               >> ${FILE};

  cp ${FILE} ${SRCDIR}/jobs/${CONFIG}/;
  mv ${FILE} ${SOUTDIR}/;
done;

for I in ${!DETNUMA[@]}; do
  FILE="ferrous_${DETNUMA[$I]}_V2parallel_1.mac";
  echo "/remoll/setgeofile                  geometry/mollerMother_ferrous.gdml"      >> ${FILE};
  echo "/remoll/parallel/setfile            geometry/mollerParallel.gdml"            >> ${FILE};
  echo "/remoll/physlist/parallel/enable"                                            >> ${FILE};
  echo "/run/initialize"                                                             >> ${FILE};
  echo "/remoll/addfield                    map_directory/V2DSg.9.75cm.parallel.txt" >> ${FILE};
  echo "/remoll/addfield                    map_directory/V2U.1a.50cm.parallel.txt"  >> ${FILE};
  echo "#/remoll/addfield                    map_directory/V2DSg.9.75cm.txt"         >> ${FILE};
  echo "#/remoll/addfield                    map_directory/V2U.1a.50cm.txt"          >> ${FILE};
  echo "/remoll/SD/disable_all"                                                      >> ${FILE};
  echo " "                                                                           >> ${FILE};
  echo "#/remoll/SD/enable                  ${DETNUMA[$I]}"                          >> ${FILE};
  echo "#/remoll/SD/detect lowenergyneutral ${DETNUMA[$I]}"                          >> ${FILE};
  echo "#/remoll/SD/detect secondaries      ${DETNUMA[$I]}"                          >> ${FILE};
  echo "#/remoll/SD/detect boundaryhits     ${DETNUMA[$I]}"                          >> ${FILE};
  echo " "                                                                           >> ${FILE};
  echo "/remoll/SD/enable                  9928"                                     >> ${FILE};
  echo "/remoll/SD/detect lowenergyneutral 9928"                                     >> ${FILE};
  echo "/remoll/SD/detect secondaries      9928"                                     >> ${FILE};
  echo "/remoll/SD/detect boundaryhits     9928"                                     >> ${FILE};
  echo " "                                                                           >> ${FILE};
  echo "/remoll/SD/enable                  28"                                       >> ${FILE};
  echo "/remoll/SD/detect lowenergyneutral 28"                                       >> ${FILE};
  echo "/remoll/SD/detect secondaries      28"                                       >> ${FILE};
  echo "/remoll/SD/detect boundaryhits     28"                                       >> ${FILE};
  echo " "                                                                           >> ${FILE};
  echo "/remoll/SD/enable                  9911"                                     >> ${FILE};
  echo "/remoll/SD/detect lowenergyneutral 9911"                                     >> ${FILE};
  echo "/remoll/SD/detect secondaries      9911"                                     >> ${FILE};
  echo "/remoll/SD/detect boundaryhits     9911"                                     >> ${FILE};
  echo " "                                                                           >> ${FILE};
  echo "## External Generator"                                                       >> ${FILE};
  echo "/remoll/evgen/set                  external"                                 >> ${FILE};
  echo "/remoll/evgen/external/file        o_remollSkimTree${DETNUMA[$I]}_1.root"      >> ${FILE};
  echo "/remoll/evgen/external/detid       ${DETNUMA[$I]}"                           >> ${FILE};
  echo "/remoll/evgen/external/zOffset     0.001"                                    >> ${FILE};
  echo "/remoll/filename                   o_ferrous_extgen_${DETNUMA[$I]}_V2parallel_1.root"    >> ${FILE};
  echo "/remoll/kryptonite/enable"                                                   >> ${FILE};
  echo "/run/beamOn                        ${SECSIMN}"                               >> ${FILE};

  cp ${FILE} ${SRCDIR}/jobs/${CONFIG}/;
  mv ${FILE} ${SOUTDIR}/;
done;


