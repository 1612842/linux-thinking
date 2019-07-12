#!/bin/bash

SCRIPT=${0##*/}
RUNUSER=$(whoami)
SIZE=100 
TARGETDIR=/home/${RUNUSER}/Desktop/02-linux-thinking/exercise/deleteOrCompress/test/
LOG=/home/${RUNUSER}/Desktop/02-linux-thinking/exercise/deleteOrCompress/${SCRIPT%%.sh}_${RUNUSER}.log
FILES=($(find ${TARGETDIR} -type f -size +${SIZE}k))
 

echo "Showing all files larger than ${SIZE} bytes in ${TARGETDIR} sorted by size."
find ${TARGETDIR} -type f -size +${SIZE}k -ls | sort -k2n
echo -e "\n\n------File menu------"
while [[ ${#FILES[@]} -gt 0 ]]; do
        PS3="Please choose a file:"
        select file in ${FILES[@]}
        do
            [[ -z ${file} ]] && {
                  echo "Invalid choice!"
            } || {
                break
            }
        done    
        
PS3="Select an option:"
echo -e "\nYou have selected the below file. Do you want to REMOVE or ZIP the file?"
ls -ltr ${file}
 
select option in ZIP REMOVE BACK
do
        [[ -z ${option} ]] && {
                echo "Invalid choice!"
        } || {
                break
        }
done
 
case ${option} in
        "ZIP" )
        echo "You chose ZIP. Zipping file now..."
        gzip ${file}
        echo "$(date "+%F %T") ${file} compressed."
                ;;
                
        "REMOVE" )
        echo "You chose REMOVE. Removing file now..."
        rm ${file}
        echo "$(date "+%F %T") ${file} removed." | tee -a ${LOG}
                ;;
                
        "BACK" )
        continue
                ;;
esac
 
FILES=($(find ${TARGETDIR} -type f -size +${SIZE}k))
 
done
