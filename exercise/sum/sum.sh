#!/bin/bash

# find -regex '.*sample.*' -exec rm -rf {} \;
# awk 'BEGIN {
#    srand()
#    for (i=1;i<=30000000;i++){
#      print int(1 + rand() * 1000)
#    }
# }' > sample.data

NUMLINES=30000000
NUMFILES=4

#AWK Advance parallel multi core
STARTTIME=$(date +%s)
cat sample.data | parallel --pipe awk \'{sum+=\$1} END {print sum}\' | awk '{sum+=$1} END {print sum}'
ENDTIME=$(date +%s)
echo "AWK advance parallel method time: $(($ENDTIME - $STARTTIME))s"

#AWK common single core
STARTTIME=$(date +%s)
awk '{ sum += $1 } END { print sum }' < sample.data
ENDTIME=$(date +%s)
echo "AWK common single core method time: $(($ENDTIME - $STARTTIME))s"



#AWK Advance &
STARTTIME=$(date +%s)
SUBNUMLINES=$((NUMLINES / NUMFILES))
split -l $SUBNUMLINES sample.data split-
PIDARRAY=()

FILES=$(find -regex '.*split-.*')

for file in $FILES
do
    awk '{ sum += $1 } END { print sum }' < $file > "sum$(sed 's/\.\/split//' <<< "$file")"  &
    PIDARRAY+=("$!")
done
wait ${PIDARRAY[@]}

FILES=$(find -regex '.*sum-.*')
SUM=0
for file in $FILES
do 
    NUM=$(< $file)
    SUM=$(($SUM+$NUM))
done
echo $SUM
ENDTIME=$(date +%s)
echo "AWK advance with & method time: $(($ENDTIME - $STARTTIME))s"

find -regex '.*split.*' -exec rm -rf {} \;
find -regex '.*sum-.*' -exec rm -rf {} \;