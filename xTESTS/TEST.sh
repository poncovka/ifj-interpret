#!/bin/sh
  dir="xTESTS"
  
  echo -e "\n================================================================"
  echo -e "=========================LEXIKALNI CHYBY========================"
  echo -e "================================================================\n" 
  sh $dir/simpleTest.sh 1 $dir/LEX_ERR
  
  echo -e "\n================================================================"
  echo -e "=======================SYNTAKTICKE CHYBY========================"
  echo -e "================================================================\n"
  sh $dir/simpleTest.sh 1 $dir/SYN_ERR
  
  echo -e "\n================================================================"
  echo -e "========================SEMANTICKE CHYBY========================"
  echo -e "================================================================\n"
  
  echo -e "\n=====================SEMANTICKE ve scriptu======================\n"
  sh $dir/simpleTest.sh 1 $dir/SEM_ERR
  
  echo -e "\n=================SEMANTICKE behem interpretac===================\n"
  sh $dir/simpleTest.sh 1 $dir/SEM_INT_ERR
  
  echo -e "\n================================================================"
  echo -e "======================INTERPRETACNI CHYBY======================="
  echo -e "================================================================\n"
  sh $dir/simpleTest.sh 1 $dir/INT_ERR

