#!/bin/sh                                                                                            
#                                                                                                    
# This script should be executed on the target.  It loads the specified                              
# module and then reads the major device number from /proc/devices.  Then,                           
# a device file is created in /dev.                                                                  
#                                                                                                    
# usage: mkdev.bash <module name>  //do not include .ko in the name                                  
#                                                                                                    
                                                                                                     
NAME=$1                                                                                              
module="${NAME}"                                                                                     
device="${NAME}"                                                                                     
mode="664"                                                                                           
minor="0"                                                                                            
                                                                                                     
/bin/rm -f /dev/${device}${minor}                                                                    
                                                                                                     
major=`grep ${module} /proc/devices | cut -f1 -d' '`                                                 
                                                                                                     
/bin/mknod /dev/${device}${minor} c ${major} ${minor}                                                
                                                                                                     
group="root"                                                                                         
                                                                                                     
/bin/chgrp $group /dev/${device}${minor}                                                             
/bin/chmod $mode /dev/${device}${minor}                                                              
                                                                                                     
echo `/bin/ls -l /dev/${module}*` 

