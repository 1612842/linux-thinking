#!/bin/bash


RUNUSER=$(whoami)
SIZE=102400

TARGETDIR=/home/${RUNUSER}/Desktop/linux-thinking/exercise/deleteOrCompress/test
LOG=/home/${RUNUSER}/Desktop/linux-thinking/exercise/deleteOrCompress/log_${RUNUSER}.log
FILES=($(find ${TARGETDIR} -type f -size +${SIZE}c))
 


while [[ ${#FILES[@]} -gt 0 ]]; do
        
echo -e "\nShowing all files larger than ${SIZE} bytes in ${TARGETDIR}"
echo $(find ${TARGETDIR} -type f -size +${SIZE}c | wc -l) files
find ${TARGETDIR} -type f -size +${SIZE}c -print0 | xargs -0 -I {} bash -c "echo -n '{} --------- '  && stat -c%s '{}' | tr -d '\n' && echo ' bytes.'"
        
PS3="Select an option:"
echo -e "\nDo you want to REMOVE or ZIP these files?"
 
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
        find ${TARGETDIR} -type f ! -name '*.gz' -size +${SIZE}c -exec gzip "{}" \;
        echo "$(date "+%F %T") all above file are compressed."
                ;;
                
        "REMOVE" )
        echo "You chose REMOVE. Removing file now..."
       
        find ${TARGETDIR} -type f -size +${SIZE}c -exec rm {} \;
        for file in ${FILES[@]}
        do
        echo "$(date "+%F %T") ${file} removed." | tee -a ${LOG}
        done
                ;;
                
        "BACK" )
        continue
                ;;
esac
 
FILES=($(find ${TARGETDIR} -type f -size +${SIZE}c))


 
done