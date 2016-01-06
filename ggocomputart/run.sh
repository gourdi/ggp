#!/bin/bash

#Examples:
#  sh run.sh 2 "2 -s 400 300" "01 02 03 04"

NUM=0
QUEUE=""
MAX_NPROC=$1

function execute {
PARAMS="$1"
DIR="_anim$2"
VIDEO="Video$2.mp4"
if [ -d $DIR ]; then
  rm -fr $DIR
fi
mkdir $DIR
./ggocomputartanim $PARAMS -d $DIR
pushd $DIR
ffmpeg -i %08d.bmp -r 25 -qscale 1 -vcodec mpeg4 -y $VIDEO
mv $VIDEO ..
popd
rm -fr $DIR
}

function queue {
QUEUE="$QUEUE $1"
NUM=$(($NUM+1))
}

function regeneratequeue {
OLDREQUEUE=$QUEUE
QUEUE=""
NUM=0
for PID in $OLDREQUEUE ; do
  if [ -d /proc/$PID ] ; then
    QUEUE="$QUEUE $PID"
    NUM=$(($NUM+1))
  fi
done
}

function checkqueue {
OLDCHQUEUE=$QUEUE
for PID in $OLDCHQUEUE ; do
  if [ ! -d /proc/$PID ] ; then
    regeneratequeue # at least one PID has finished
    break
  fi
done
}

for ANIM in $3
do
  execute "$2" $ANIM &
  PID=$!
  queue $PID
  
  sleep 5 # wait a bit otherwise random numbers are teh same across executions.

  while [ $NUM -ge $MAX_NPROC ] ; do
    checkqueue
    sleep 1
  done
done
wait
