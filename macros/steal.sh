#!/bin/bash
prefix=B1
for source in $(ls include/*.hh); do
  newSource=${source/$prefix/} # replace all occurence of prefix with zero character
  echo $source "-->" $newSource
  mv $source $newSource
  sed -i '' "s/$prefix//g" $newSource
  sed -i '' -e '1,28d' $newSource
done

for source in $(ls src/*.cc); do
  newSource=${source/$prefix/} # replace all occurence of prefix with zero character
  echo $source "-->" $newSource
  mv $source $newSource
  sed -i '' "s/$prefix//g" $newSource
  sed -i '' -e '1,28d' $newSource
done
