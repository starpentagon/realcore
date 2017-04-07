#!/bin/bash

diagram_db='game_record_db_raw.csv'

if [ -e ${diagram_db} ]; then
  rm ${diagram_db}
fi

for csv_file in `ls *.csv`; do
  head -n 1 ${csv_file} > header.tmp
  break
done

if [ -e tmp ]; then
  rm tmp
fi

for csv_file in `ls *.csv`; do
  tail -n +2 ${csv_file} >> tmp
done

cat header.tmp tmp > ${diagram_db}

rm header.tmp
rm tmp
