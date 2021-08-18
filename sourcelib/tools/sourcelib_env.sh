#!/bin/bash
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd )
export SOURCELIB_ROOT=$HOME/csse3010/sourcelib
#export STM32_QEMU=$HOME/qemu/arm-softmmu
export PATH=$SOURCELIB_ROOT/tools:$PATH
#source ~/.bash-git-prompt/gitprompt.sh
#export GIT_PROMPT_ONLY_IN_REPO=1
#export GIT_PROMPT_FETCH_REMOTE_STATUS=1
#export GIT_PROMPT_THEME=Solarized
#export GIT_PROMPT_END=$($NP2_ROOT/tools/np2_check.sh)
