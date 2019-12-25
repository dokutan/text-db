#/bin/bash
# This script can be used to add information about images in a directory to a file
# GPLv3 or later
# Depends on Imagemagick
# Usage: ./text-db-imag-scraper path/to/directory >> file

for file in $1/*
do
	echo `basename "$file"`
	echo -e "\twidth\t"`identify -format "%w\n" "$file"`
	echo -e "\theight\t"`identify -format "%h\n" "$file"`
	echo -e "\tformat\t"`identify -format "%m\n" "$file"`
	echo -e "\tmodified\t"`stat "$file" -c %y | sed "s/\.[0-9]* //"`
done
