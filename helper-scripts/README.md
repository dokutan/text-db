# helper-scripts
This directory contains scripts that are intended to be used in addition to text-db.

# Conversion

## Markdown
sed "s/^/# /;s/# \t/ * **/;s/^#/\n\n#/;s/\t/**\t/" collection
## PDF
sed "s/^/# /;s/# \t/ * **/;s/^#/\n\n#/;s/\t/**\t/" collection | pandoc -f markdown -t pdf -o collection.pdf

