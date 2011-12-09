#!/bin/sh
exitCode=$1;
dir=$2


ls -1 $dir | grep --invert-match "^README$" | {  
  while read file; do
    pgr=`./project "$dir/$file"  2>&1`
    err=$?
    val=`valgrind --leak-check=full --leak-resolution=high -q ./project "$dir/$file"  2>&1`
  
    if [ $err -ne $exitCode -o "$pgr" != "$val" ]; then
      if [ $err -eq exitCode ] ; then
        valgrind --leak-check=full --leak-resolution=high -q ./project "$dir/$file"  
      fi
      echo "TEST FAIL - $file"  >&2;
    else  
      echo "TEST OK - $file";
    fi
  done
}