#!/bin/bash
mkdir linux_practice
cd linux_practice
mkdir docs
mkdir backup
cd docs
touch readme.txt notes.log temp.tmp
rm temp.tmp
mv notes.log daily_report.txt
echo "Project Status: Active" > daily_report.txt
echo "$(date)" >> daily_report.txt
cd ..
cp docs/*.txt backup/
cd backup
chmod 444 readme.txt
chmod 444 daily_report.txt
echo "Archive Complete. File readme and daily_report is now read-only."
