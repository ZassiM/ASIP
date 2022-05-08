#!/bin/bash

if [ "$BASH_ARGV" = "" ] ; then
  echo -e "\x1b[1;31m[FEHLER]\x1b[0m Das Skript 'setup.sh' wurde ausgeführt. Es muss aber ge\"source\"d werden, damit die Shell konfiguriert wird. Verwende \x1b[1;32msource setup.sh\x1b[0m"
  exit -1
fi

source /net/home/common-profiles/bashrc

module load gcc/9.2.0
module load asip-designer/2020.09-SP2
module load synthesis/O-2018.06-SP5-5
module load primetime-x/2016.06-SP1
module load questa_core_prime_10.7c
module load vcs_mx/2016.06_x64
module load soc_encounter/13.1
module load formality/J-2014.09-SP1
module load python/3.6.9

if [ -d ${HOME}/.local/bin ]; then
  export PATH=${HOME}/.local/bin:$PATH
fi

export SYNOPSYS_SYN_ROOT=$(dirname $(dirname $(which design_vision-xg)))

export PROJDIR=$(dirname $(readlink -f ${BASH_SOURCE[0]}))

export PYTHONPATH=$PROJDIR/framework:$PYTHONPATH

export SETUP_DONE=1
