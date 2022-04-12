#!/bin/bash

MYUSER="ericking@jlab.org"; #IF WANT TO SUBMIT VIA XML THEN NEED THIS
#SRCDIR="/work/halla/moller12gev/ericking/remoll3";
#OUTDIR="/volatile/halla/moller12gev/ericking/${CONFIG}";
SRCDIR="/home/tup89084/Remoll/EricRemoll/remoll";
OUTDIR="./OUTPUT";

CONFIG="remoll_ferrous_04_12_22";

NUMSIMS=10;
DETNUMA=(9010 9020 9030) #TODO: WILL EVENTUALLY WANT TO READ FROM FERROUS DET.LIST
RANDOMN=1234567;
SEQNUMB=1;
NEVENTS=100;

#CLEANUP JUST IN CASE ANYTHING REMAINS
rm -f remoll_*.mac;
rm -f define.tar.gz;

#CREATE SLURM WORK DIRECTORIES, WRITE SCRIPT AND MOVE IT TO REMOLL JOBS DIR
SLURMFL="${SRCDIR}/jobs/${CONFIG}.sh";
SOUTDIR="${OUTDIR}/${CONFIG}__${NEVENTS}_EVT_x_${NUMSIMS}";

[ -d ${OUTDIR} ] || mkdir ${OUTDIR};
[ -d ${SOUTDIR} ] || mkdir ${SOUTDIR};
#TODO: GOINT TO NEED A CONDITION HERE THAT IF THE DIRECTORY EXISTS
#THEN STOP AND GET NEW CONFIGURATION NAME
touch ${SLURMFL}
echo "#!/bin/bash"                                   >> ${SLURMFL};
echo "#SBATCH --partition=production"                >> ${SLURMFL};
echo "#SBATCH --job-name=${CONFIG}"                  >> ${SLURMFL};
echo "#SBATCH --ntasks=1"                            >> ${SLURMFL};
echo "#SBATCH --output=${SOUTDIR}/sim_%a.out"        >> ${SLURMFL};
echo "#SBATCH --error=${SOUTDIR}/sim_%a.err"         >> ${SLURMFL};
echo "#SBATCH --array=1-10"                          >> ${SLURMFL};
echo "#SBATCH --account=halla"                       >> ${SLURMFL};
echo "#SBATCH --mem-per-cpu=5000"                    >> ${SLURMFL};
echo "cd ${SOUTDIR}"                                 >> ${SLURMFL};
echo "tar -zxf default.tar.gz"                       >> ${SLURMFL};
echo "./remoll remoll_\$\{SLURM_ARRAY_TASK_ID\}.mac" >> ${SLURMFL};

#MAKE TARBALL AND MOVE TO $SOUTDIR
tar czf default.tar.gz \
         ${SRCDIR}/build/remoll \
         ${SRCDIR}/build/libremoll.so \
         ${SRCDIR}/map_directory/V2DSg.9.75cm.parallel.txt \
         ${SRCDIR}/map_directory/V2U.1a.50cm.parallel.txt \
         ${SRCDIR}/geometry;
mv default.tar.gz ${SOUTDIR};

#GENERATE REMOLL MACROS AND MOVE TO $SOUTDIR
for THISSIM in $(seq 1 ${NUMSIMS}); do
  touch remoll_${THISSIM}.mac

  echo "/remoll/setgeofile                geometry/mollerMother.gdml"   >> remoll_${THISSIM}.mac;
  echo "/remoll/parallel/setfile          geometry/mollerParallel.gdml" >> remoll_${THISSIM}.mac;
  echo "/remoll/physlist/parallel/enable"                               >> remoll_${THISSIM}.mac;
  echo "/run/initialize"                                                >> remoll_${THISSIM}.mac;
  echo "/remoll/printgeometry             true"                         >> remoll_${THISSIM}.mac;
  echo "/remoll/addfield                  map_directory/V2DSg.9.75cm.parallel.txt" >> remoll_${THISSIM}.mac;
  echo "/remoll/addfield                  map_directory/V2U.1a.50cm.parallel.txt"  >> remoll_${THISSIM}.mac;
  echo "/remoll/evgen/beam/origin         0 0 -7.5 m"                   >> remoll_${THISSIM}.mac;
  echo "/remoll/evgen/beam/rasx           5 mm"                         >> remoll_${THISSIM}.mac;
  echo "/remoll/evgen/beam/rasy           5 mm"                         >> remoll_${THISSIM}.mac;
  echo "/remoll/evgen/beam/corrx          0.065"                        >> remoll_${THISSIM}.mac;
  echo "/remoll/evgen/beam/corry          0.065"                        >> remoll_${THISSIM}.mac;
  echo "/remoll/evgen/beam/rasrefz       -4.5 m"                        >> remoll_${THISSIM}.mac;
  echo "/remoll/beamene                   11 GeV"                       >> remoll_${THISSIM}.mac;
  #echo "/remoll/evgen/set                 elastic"                      >> remoll_${THISSIM}.mac;
  echo "/remoll/evgen/set                 beam"                         >> remoll_${THISSIM}.mac;
  echo "/remoll/beamcurr                  65 microampere"               >> remoll_${THISSIM}.mac;
  echo "/remoll/evgen/beamPolarization   +L"                            >> remoll_${THISSIM}.mac;
  echo "/remoll/field/equationtype        2"                            >> remoll_${THISSIM}.mac;
  echo "/remoll/field/steppertype         2"                            >> remoll_${THISSIM}.mac;
  echo "/remoll/field/print"                                            >> remoll_${THISSIM}.mac;
  echo " "                                                              >> remoll_${THISSIM}.mac
  for I in ${!DETNUMA[@]}; do
    echo "/remoll/SD/disable_all"                                       >> remoll_${THISSIM}.mac;
    echo "/remoll/SD/enable                                   ${DETNUMA[$I]}" >> remoll_${THISSIM}.mac;
    echo "/remoll/SD/detect                lowenergyneutral   ${DETNUMA[$I]}" >> remoll_${THISSIM}.mac;
    echo "/remoll/SD/detect                secondaries        ${DETNUMA[$I]}" >> remoll_${THISSIM}.mac;
    echo "/remoll/SD/detect                boundaryhits       ${DETNUMA[$I]}" >> remoll_${THISSIM}.mac;
  done
  echo "           "                                                    >> remoll_${THISSIM}.mac;
  echo "/remoll/kryptonite/enable"                                      >> remoll_${THISSIM}.mac;
  echo "/remoll/kryptonite/add           VacuumKryptonite"              >> remoll_${THISSIM}.mac;
  echo "/remoll/kryptonite/add           Tungsten"                      >> remoll_${THISSIM}.mac;
  echo "/remoll/kryptonite/add           Copper"                        >> remoll_${THISSIM}.mac;
  echo "/remoll/kryptonite/add           Lead"                          >> remoll_${THISSIM}.mac;
  echo "/remoll/kryptonite/add           CW95"                          >> remoll_${THISSIM}.mac;
  echo "/remoll/kryptonite/list"                                        >> remoll_${THISSIM}.mac;
  echo "/process/list"                                                  >> remoll_${THISSIM}.mac;

  echo "/remoll/seed                     ${RANDOMN}"                    >> remoll_${THISSIM}.mac;
  echo "/remoll/filename                 remollout_${THISSIM}.root"     >> remoll_${THISSIM}.mac;
  #    echo "/remoll/printtargetinfo" >> remoll_${THISSIM}.mac;
  echo "/run/beamOn                      ${NEVENTS}"                    >> remoll_${THISSIM}.mac;

  mv remoll_${THISSIM}.mac ${SOUTDIR}/
done;
