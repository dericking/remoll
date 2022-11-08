#!/bin/bash

CONFIG="remoll-ferrous-2022-07-06-gem-asym";
NUMSIMS=1000;
DETNUMA=(9201 9202 9203 9204 9205 9206 9207 9208) #TODO: WILL EVENTUALLY WANT TO READ FROM FERROUS DET.LIST
RANDOMN=1234567;         #DO WE WANT TO RANDOMIZE THIS???
SEQNUMB=1;               #FIXME: IS THIS USED???
NEVENTS=500000;
SECSIMN=100000;

MYUSER="ericking@jlab.org"; #IF WANT TO SUBMIT VIA XML THEN NEED THIS
SRCDIR="/work/halla/moller12gev/ericking/remoll5/remoll";
OUTDIR="/volatile/halla/moller12gev/ericking/${CONFIG}";
SKIMFL="/w/halla-scshelf2102/moller12gev/ericking/remoll5/remoll/rootfiles/primaryskims/skimTreeMulti.C"
#SRCDIR="/home/tup89084/Remoll/EricRemoll/remoll";
#OUTDIR="./OUTPUT";

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
# MAKE TARBALL, COPY TO JOBS FOR RECORD, AND MOVE TO $SOUTDIR
############################################################################################
tar czf default.tar.gz -C ${SRCDIR}/build remoll libremoll.so reroot\
    -C ${SRCDIR} ./include/remollRint.hh ./include/remolltypes.hh \
    -C ${SRCDIR} ./map_directory/DS_Asym_CP2B_pm_1_3.txt \
    -C ${SRCDIR} ./map_directory/US_Asym_CP2B_pm_1_3.txt \
    -C ${SRCDIR} ./geometry \
    -C ${SRCDIR}/analysis/ferrous skimTreeMulti.C ferrous_analysis.C ferrous_analysis.h run_ferrous_analysis.C;
cp default.tar.gz ${SRCDIR}/jobs/${CONFIG}/;
mv default.tar.gz ${SOUTDIR};

############################################################################################
# WRITE SLURM BATCH FILE FOR PRIMARY SIMULATIONS
############################################################################################
#SLURMFLP="${SRCDIR}/jobs/${CONFIG}/${CONFIG}_slurm_primary.sh";
FILE="slurm_primary.sh";
touch ${FILE}
echo "#!/bin/bash"                                   >> ${FILE};
echo "#SBATCH --partition=production"                >> ${FILE};
echo "#SBATCH --job-name=${CONFIG}"                  >> ${FILE};
echo "#SBATCH --ntasks=1"                            >> ${FILE};
echo "#SBATCH --output=${SOUTDIR}/primsim_%a.out"    >> ${FILE};
echo "#SBATCH --error=${SOUTDIR}/primsim_%a.err"     >> ${FILE};
echo "#SBATCH --array=1-${NUMSIMS}"                  >> ${FILE};
echo "#SBATCH --account=halla"                       >> ${FILE};
echo "#SBATCH --mem-per-cpu=1000"                    >> ${FILE};
#echo "cd ${SOUTDIR}"                                 >> ${FILE};
#echo "tar -zxf default.tar.gz"                       >> ${FILE};
echo "./remoll remoll_\${SLURM_ARRAY_TASK_ID}.mac"   >> ${FILE};

cp ${FILE} ${SRCDIR}/jobs/${CONFIG}/ ;
mv ${FILE} ${SOUTDIR}/ ;

############################################################################################
# GENERATE PRIMARY SIM REMOLL MACROS AND MOVE TO $SOUTDIR
############################################################################################
for THISSIM in $(seq 1 ${NUMSIMS}); do
  FILE="remoll_${THISSIM}.mac";
  touch ${FILE}
  echo "/remoll/setgeofile                geometry/mollerMother.gdml"   >> ${FILE};
  echo "/remoll/parallel/setfile          geometry/mollerParallel_ferrous.gdml" >> ${FILE};
  echo "/remoll/physlist/parallel/enable"                               >> ${FILE};
  echo "/run/initialize"                                                >> ${FILE};
  echo "/remoll/printgeometry             true"                         >> ${FILE};
  echo "/remoll/addfield                  map_directory/DS_Asym_CP2B_pm_1_3.txt" >> ${FILE};
  echo "/remoll/addfield                  map_directory/US_Asym_CP2B_pm_1_3.txt" >> ${FILE};
  echo "/remoll/evgen/beam/origin         0 0 -7.5 m"                   >> ${FILE};
  echo "/remoll/evgen/beam/rasx           5 mm"                         >> ${FILE};
  echo "/remoll/evgen/beam/rasy           5 mm"                         >> ${FILE};
  echo "/remoll/evgen/beam/corrx          0.065"                        >> ${FILE};
  echo "/remoll/evgen/beam/corry          0.065"                        >> ${FILE};
  echo "/remoll/evgen/beam/rasrefz       -4.5 m"                        >> ${FILE};
  echo "/remoll/beamene                   11 GeV"                       >> ${FILE};
  #echo "/remoll/evgen/set                 elastic"                      >> ${FILE};
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
  echo "/remoll/kryptonite/add           Tungsten"                      >> ${FILE};
  echo "/remoll/kryptonite/add           Copper"                        >> ${FILE};
  echo "/remoll/kryptonite/add           Lead"                          >> ${FILE};
  echo "/remoll/kryptonite/add           CW95"                          >> ${FILE};
  echo "/remoll/kryptonite/list"                                        >> ${FILE};
  echo "/process/list"                                                  >> ${FILE};

  echo "/remoll/seed                     ${THISSIM}"                    >> ${FILE};
  echo "/remoll/filename                 remollout_${THISSIM}.root"     >> ${FILE};
  #echo "/remoll/printtargetinfo"                                        >> ${FILE}
  echo "/run/beamOn                      ${NEVENTS}"                    >> ${FILE};

  cp ${FILE} ${SRCDIR}/jobs/${CONFIG}/ ;
  mv ${FILE} ${SOUTDIR}/ ;
done;

############################################################################################
# CREATE THE FERROUS MACROS FOR SECOND EXTGEN SIMULATIONS -- THEY MAY NOT ALL BE USED
############################################################################################
for I in ${!DETNUMA[@]}; do
  FILE="ferrous_${DETNUMA[$I]}_V2parallel.mac";
  echo "/remoll/setgeofile                  geometry/mollerMother_ferrous.gdml"      >> ${FILE};
  echo "/remoll/parallel/setfile            geometry/mollerParallel.gdml"            >> ${FILE};
  echo "/remoll/physlist/parallel/enable"                                            >> ${FILE};
  echo "/run/initialize"                                                             >> ${FILE};
  echo "/remoll/addfield                    map_directory/DS_Asym_CP2B_pm_1_3.txt"   >> ${FILE};
  echo "/remoll/addfield                    map_directory/US_Asym_CP2B_pm_1_3.txt"   >> ${FILE};
  echo "/remoll/SD/disable_all"                                                      >> ${FILE};
  echo " "                                                                           >> ${FILE};
  echo "#/remoll/SD/enable                  ${DETNUMA[$I]}"                          >> ${FILE};
  echo "#/remoll/SD/detect lowenergyneutral ${DETNUMA[$I]}"                          >> ${FILE};
  echo "#/remoll/SD/detect secondaries      ${DETNUMA[$I]}"                          >> ${FILE};
  echo "#/remoll/SD/detect boundaryhits     ${DETNUMA[$I]}"                          >> ${FILE};
  echo " "                                                                           >> ${FILE};
  echo "/remoll/SD/enable                  28"                                       >> ${FILE};
  echo "/remoll/SD/detect lowenergyneutral 28"                                       >> ${FILE};
  echo "/remoll/SD/detect secondaries      28"                                       >> ${FILE};
  echo "/remoll/SD/detect boundaryhits     28"                                       >> ${FILE};
  echo "/remoll/SD/enable                  29"                                       >> ${FILE};
  echo "/remoll/SD/detect lowenergyneutral 29"                                       >> ${FILE};
  echo "/remoll/SD/detect secondaries      29"                                       >> ${FILE};
  echo "/remoll/SD/detect boundaryhits     29"                                       >> ${FILE};
  echo "/remoll/SD/enable                  30"                                       >> ${FILE};
  echo "/remoll/SD/detect lowenergyneutral 30"                                       >> ${FILE};
  echo "/remoll/SD/detect secondaries      30"                                       >> ${FILE};
  echo "/remoll/SD/detect boundaryhits     30"                                       >> ${FILE};
  echo "/remoll/SD/enable                  35"                                       >> ${FILE};
  echo "/remoll/SD/detect lowenergyneutral 35"                                       >> ${FILE};
  echo "/remoll/SD/detect secondaries      35"                                       >> ${FILE};
  echo "/remoll/SD/detect boundaryhits     35"                                       >> ${FILE};
  echo "/remoll/SD/enable                  1007"                                     >> ${FILE};
  echo "/remoll/SD/detect lowenergyneutral 1007"                                     >> ${FILE};
  echo "/remoll/SD/detect secondaries      1007"                                     >> ${FILE};
  echo "/remoll/SD/detect boundaryhits     1007"                                     >> ${FILE};
  echo " "                                                                           >> ${FILE};
  echo "## External Generator"                                                       >> ${FILE};
  echo "/remoll/evgen/set                  external"                                 >> ${FILE};
  echo "/remoll/evgen/external/file        o_remollSkimTree${DETNUMA[$I]}.root"      >> ${FILE};
  echo "/remoll/evgen/external/detid       ${DETNUMA[$I]}"                           >> ${FILE};
  echo "/remoll/evgen/external/zOffset     0.001"                                    >> ${FILE};
  echo "/remoll/filename                   o_ferrous_extgen_${DETNUMA[$I]}_V2parallel.root"    >> ${FILE};
  echo "/remoll/kryptonite/enable"                                                   >> ${FILE};
  echo "/run/beamOn                        ${SECSIMN}"                               >> ${FILE};

  cp ${FILE} ${SRCDIR}/jobs/${CONFIG}/;
  mv ${FILE} ${SOUTDIR}/;
done;

############################################################################################
# LET'S START DOING THINGS -- MOVE TO THE VOLATILE DIRECTORY
############################################################################################
cd   ${SOUTDIR};
tar -xzf default.tar.gz
wait;

#Submit slurm array batch for primary simulations
sbatch --wait slurm_primary.sh;
wait;

#Extract list of root files returned
find ${SOUTDIR}/remollout_*.root -maxdepth 1 -type f | tee ${SOUTDIR}/rootfiles.list;

#Skim the root files for the detectors; this creates a bunch of root files
#SkimTreeMulti outputs a text file with stats called ferrous_skimTree_results.txt with total hits as last entry
#./reroot -b -q skimTreeMulti.C+'("rootfiles.list","'${DETLIST}'",0)';
./reroot -b -q skimTreeMulti.C+'("rootfiles.list","'${DETLIST}'",0)';

############################################################################################
# GET SKIMMED DATA INFORMATION TO PREP FOR SECONDARY EXTGEN SIMULATIONS
############################################################################################
skimDataLine=$(head -n 1 ferrous_skimTree_results.txt) # How many good events were there
successfulEv=$(cut -d ":" -f2 <<< ${skimDataLine})

PRIMEVT=() # Blank array to be filled with primary events for each detector
REGEXP='^[0-9]+$'

# Remove no-hit sims from DETNUMA and create PRIMEVT list
while read -r line ; do
    DETECT=$(echo $line | awk '{print $1}');
    TOTHIT=$(echo $line | awk '{print $NF}');
    if [[ ${TOTHIT} =~ ${REGEXP} ]]; then
      if [[ "$TOTHIT" -eq "0" ]]; then
        DETNUMA=( ${DETNUMA[@]/$DETECT} )
      else
	PRIMEVT+=(${TOTHIT})
      fi
    fi
done < <(grep -v '#' ferrous_skimTree_results.txt)

EVTLIST=$(IFS=, ; echo "${PRIMEVT[*]}"); # Create EVTLIST primary event list to pass to analysis 
DETLIST=$(IFS=, ; echo "${DETNUMA[*]}"); # Create updated DETLIST for secondary slurm batch script


############################################################################################
# WRITE SLURM FILE FOR SECONDARY EXTGEN SIMULATIONS
############################################################################################
FILE="slurm_secondary.sh";
touch ${FILE}
echo "#!/bin/bash"                                   >> ${FILE};
echo "#SBATCH --partition=production"                >> ${FILE};
echo "#SBATCH --job-name=${CONFIG}"                  >> ${FILE};
echo "#SBATCH --ntasks=1"                            >> ${FILE};
echo "#SBATCH --output=${SOUTDIR}/secsim_%a.out"     >> ${FILE};
echo "#SBATCH --error=${SOUTDIR}/secsim_%a.err"      >> ${FILE};
echo "#SBATCH --array=${DETLIST}"                    >> ${FILE};
echo "#SBATCH --account=halla"                       >> ${FILE};
echo "#SBATCH --mem-per-cpu=1000"                    >> ${FILE};
echo " "                                             >> ${FILE};
echo "cd ${SOUTDIR}"                                 >> ${FILE};
echo " "                                             >> ${FILE};
echo "./remoll ferrous_\${SLURM_ARRAY_TASK_ID}_V2parallel.mac" >> ${FILE};

cp ${FILE} ${SRCDIR}/jobs/${CONFIG}/ ;

#This will execute slurm batch array for secondary simulations
sbatch --wait slurm_secondary.sh;
wait;

################## The remaining task will be analyzing the secondary outputs with the ferrous_analysis.C script.
root -b -q run_ferrous_analysis.C+'('${TPRIEVT}',"'${DETLIST}','${PRIMEVT}'")';

############################################################################################
# LET'S CLEAN EVERYTHING UP
############################################################################################
mkdir primaryMacros;
mkdir secondaryMacros;
mkdir primaryRootFiles;
mkdir secondaryRootFiles;
mkdir skimmedRootFiles
mkdir slurmOutputFiles;
mkdir slurmShellScripts;

## REMOVE AFTER TESTING
mv remoll_*.mac    primaryMacros;
mv ferrous_*.mac   secondaryMacros;
#mv remollout_*.root   primaryRootFiles;
#mv o_ferrous*.root    secondaryRootFiles;
#mv o_remollSkim*.root skimmedRootFiles;
mv *err            slurmOutputFiles;
mv *out            slurmOutputFiles;
mv slurm_*.sh      slurmShellScripts;
