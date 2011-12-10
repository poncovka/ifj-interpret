#!/bin/sh
exitCode=$1;
dir=$2


ls -1 $dir | grep --invert-match "^README$" | {  
  while read file; do
    pgr=`./project "$dir/$file"  2>&1`
    err=$?;
    val=`valgrind --leak-check=full --leak-resolution=high -q ./project "$dir/$file"  2>&1`
  
    if [ "$err" != "$exitCode" -o "$pgr" != "$val" ]; then
      printf "TEST FAIL - "  >&2;
      if [ "$pgr" != "$val" ] ; then
        printf "%s\n" $file >&2
        valgrind --leak-check=full --leak-resolution=high -q ./project "$dir/$file"
      else
        printf "[$err] %s\n" $file >&2 
      fi
    else  
      echo "TEST OK - $file";
    fi
  done
}