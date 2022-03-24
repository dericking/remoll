#!/bin/bash

###################################################
#
#  THIS SCRIPT ALLOWS YOU TO CHOOSE A DIRECTORY IN THE 
#  BASE DIRECTORY TO CREATE A LIST OF ROOTFILES AVAILABLE
#  FOR ANALYSIS. 
#
###################################################

# EDIT THE BASE DIRECTORY
BASEDIR="/volatile/halla/moller12gev/ericking"
EXAMINE=""

COPYMAP="false"
COPYTAR="false"
COPYGEO="false"

rm -rf  files.list
rm -rf  failed.list

function SELECTDIR {
    select NAMEDIR in ${BASEDIR}/*; do
        if [ -d "${NAMEDIR}" ]; then
            EXAMINE=${NAMEDIR};
            break;  
        fi
    done
}

SELECTDIR

BASEDIR=$(basename "${EXAMINE}")

touch  ${BASEDIR}_files.list
touch  ${BASEDIR}_failed.list
echo  "The following directories contained no ROOT file: " >> ${BASEDIR}_failed.list


if [[ -d ${EXAMINE}/map_directory ]]; then
    echo "Map Directory exists."
else 
    echo "Map Directory of simultion does not exist in base directory."
    COPYMAP=true;
fi

if [[ -d ${EXAMINE}/geometry ]]; then
    echo "Map Directory exists."
else 
    echo "Map Directory of simultion does not exist in base directory."
    COPYGEO=true;
fi

if [[ -d ${EXAMINE}/default.tar.gz ]]; then
    echo "tarball of simulation software exists in base directory."
else
    echo "tarball of simulation software does not exist in base directory."
    COPYTAR=true;
fi

echo "COPYMAP: ${COPYMAP}";
echo "COPYGEO: ${COPYGEO}";
echo "COPYTAR: ${COPYTAR}";

for EACHDIR in ${EXAMINE}/*; do
    if [ -d "${EACHDIR}" ]; then
        echo "Checking ${EACHDIR}...";
        if [[ -f "${EACHDIR}/remollout.root" ]]; then

            #########################################################
            # DEAL WITH REDUNDANT MAP DIRECTORIES
            if [[ "${COPYMAP}" = true ]]; then
                echo "Copying MAP_DIRECTORY to base results directory";
                cp -r ${EACHDIR}/map_directory ${EACHDIR}/../
                wait;
                if [[ -d ${EACHDIR}/../map_directory ]]; then
                    COPYMAP=false
                    echo "MAP_DIRECTORY now exists in base directory... can delete from all contained subdirectories";
                    rm -r ${EACHDIR}/map_directory;
                fi
            else 
                if [[ -d ${EACHDIR}/map_directory ]]; then rm -r ${EACHDIR}/map_directory; fi
            fi

            ########################################################
            # DEAL WITH REDUNDANT GEOMETRY FILES
            if [[ "${COPYGEO}" = true ]]; then
                echo "Copying geometry to base results directory";
                cp -r ${EACHDIR}/geometry ${EACHDIR}/../
                wait;
                if [[ -d ${EACHDIR}/../geometry ]]; then
                    COPYGEO=false
                    echo "GEOMETRY now exists in base directory... can delete from all contained subdirectories";
                    rm -r ${EACHDIR}/geometry;
                fi
            else 
                if [[ -d ${EACHDIR}/geometry ]]; then rm -r ${EACHDIR}/geometry; fi
            fi

            ########################################################
            # DEAL WITH REDUNDANT TAR ARCHIVES, REMOLL and LIBREMOLL.SO
            if [[ "${COPYTAR}" = true ]]; then
                echo "Copying DEFAULT.TAR.GZ to base results directory";
                cp -r ${EACHDIR}/default.tar.gz ${EACHDIR}/../
                cp -r ${EACHDIR}/remoll ${EACHDIR}/../
                cp -r ${EACHDIR}/libremoll.so ${EACHDIR}/../
                wait;
                if [[ -f ${EACHDIR}/../default.tar.gz ]] && [[ -f ${EACHDIR}/../remoll ]] && [[ -f ${EACHDIR}/../libremoll.so ]]; then
                    COPYTAR=false
                    echo "DEFAULT.TAR.GZ now exists in base directory... can delete from all contained subdirectories";
                    rm ${EACHDIR}/default.tar.gz;
                fi
            else 
                if [[ -f ${EACHDIR}/default.tar.gz ]]; then rm -r ${EACHDIR}/default.tar.gz; fi
                if [[ -f ${EACHDIR}/remoll ]]; then rm -r ${EACHDIR}/remoll; fi
                if [[ -f ${EACHDIR}/libremoll.so ]]; then rm -r ${EACHDIR}/libremoll.so; fi
            fi

            echo "${EACHDIR}/remollout.root" >> ${BASEDIR}_files.list
        else 
            echo "${EACHDIR}" >> ${BASEDIR}_failed.list        
        fi
    fi
done
