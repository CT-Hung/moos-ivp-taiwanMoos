#!/bin/bash -e
#----------------------------------------------------------
#  Script: launch.sh
#  Author: Michael Benjamin
#  LastEd: May 20th 2019
#----------------------------------------------------------
#  Part 1: Set Exit actions and declare global var defaults
#----------------------------------------------------------
trap "kill -- -$$" EXIT SIGTERM SIGHUP SIGINT SIGKILL
TIME_WARP=10
COMMUNITY_SHORESIDE="shoreside"
COMMUNITY_ALPHA="alpha"
COMMUNITY_BRAVO="bravo"
GUI="yes"

#----------------------------------------------------------
#  Part 2: Check for and handle command-line arguments
#----------------------------------------------------------
for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
	echo "launch.sh [SWITCHES] [time_warp]   "
	echo "  --help, -h           Show this help message            " 
	exit 0;
    elif [ "${ARGI}" = "--nogui" ] ; then
	GUI="no"
    elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then 
        TIME_WARP=$ARGI
    else 
        echo "launch.sh Bad arg:" $ARGI " Exiting with code: 1"
        exit 1
    fi
done


#----------------------------------------------------------
#  Part 3: Launch the processes
#----------------------------------------------------------
echo "Launching $COMMUNITY_SHORESIDE MOOS Community with WARP:" $TIME_WARP
pAntler $COMMUNITY_SHORESIDE.moos --MOOSTimeWarp=$TIME_WARP >& /dev/null &

echo "Launching $COMMUNITY_ALPHA MOOS Community with WARP:" $TIME_WARP
pAntler $COMMUNITY_ALPHA.moos --MOOSTimeWarp=$TIME_WARP >& /dev/null &

echo "Launching $COMMUNITY_BRAVO MOOS Community with WARP:" $TIME_WARP
pAntler $COMMUNITY_BRAVO.moos --MOOSTimeWarp=$TIME_WARP >& /dev/null &

uMAC -t $COMMUNITY_SHORESIDE.moos

printf "Killing all processes ... \n"
kill %1 
mykill
printf "Done killing processes.   \n"