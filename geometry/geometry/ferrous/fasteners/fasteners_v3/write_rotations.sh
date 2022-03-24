#!/bin/bash

rm -rf rotations.txt
touch rotations.txt

for N in {1..93}
do
  echo '<rotation name="fasteners_clamp_rotation'${N}'" x="fasteners_clamp_m['${N}',4]" y="fasteners_clamp_m['${N}',5]" z="fasteners_clamp_m['${N}',6]" unit="deg" />' >> rotations.txt
done
