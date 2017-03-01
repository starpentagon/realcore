#!/bin/bash

# testディレクトリ配下の全テストを実行する
date=`date +"%Y%m%d%H%M%S"`
log_file=`pwd`/test_log_${date}.log

for file_path in ./*; do
  if [ -d ${file_path} ]
  then
    echo -n ${file_path}
    echo -n "..."

    cd ${file_path}
    ./build.sh > /dev/null
    ./run.sh >> ${log_file}

    if [ $? -eq 0 ]
    then
      echo "OK"
    else
      echo "NG"
    fi

    cd ..
  fi
  
done