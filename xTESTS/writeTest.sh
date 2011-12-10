#!/bin/sh
writeTest(){
  exitCode=$1;
  dir=$2
   
  ls -1 $dir | grep --invert-match "^README$" | grep "^${exitCode}_"| ( 
    while read file; do
      pgr=`./project "$dir/$file"  2>&1`
      err=$?;
      
      val=`valgrind --leak-check=full --leak-resolution=high -q ./project "$dir/$file"  2>&1`
      
      if [ -e "$dir/ref_out_$file" ] ; then
        ref=`./project "$dir/$file" | diff - "$dir/ref_out_$file"`
      fi
      
      if [ "$err" != "$exitCode" -o "$pgr" != "$val" -o "$ref" != "" -o ! -e "$dir/ref_out_$file" ]; then
        printf "TEST FAIL - "  >&2;
        if [ ! -e "$dir/ref_out_$file" ] ; then
          echo "$file - Neexistuje soubor pro porovnani!" >&2
          return
        fi
        
        if [ "$pgr" != "$val" ] ; then
          printf "%s\n" $file >&2
          valgrind --leak-check=full --leak-resolution=high -q ./project "$dir/$file"
        else
          printf "[$err] %s\n" $file >&2
          ./project "$dir/$file" | diff - "$dir/ref_out_$file" 
        fi
      else  
        echo "TEST OK - $file";
      fi
    done
  )
} 

writeTest 0 $1
writeTest 1 $1
writeTest 2 $1
writeTest 3 $1
writeTest 4 $1

