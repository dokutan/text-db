#/bin/bash
# This script takes a list of ISBNs in form of a file and gets information about the books
# GPLv3 or later
# Depends on isbntools (https://github.com/xlcnd/isbntools)
# See the isbntools manual for options
# Usage: ./text-db-isbn-scraper isbn_list >> file

for isbn in $(cat "$1")
do
	isbn_meta "$isbn" | tail -n+2 | tr -s ' ' | sed 's/^/\t/;s/: /\t/;s/\tTitle\t//'
done
