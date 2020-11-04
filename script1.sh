#!/bin/bash

for counter in {1..30};
    do
        for distance in {250,1250,2500}
            do
                for ciclists in {10,50,100};
                    do
                        echo /usr/bin/time -f "%e segundos, %M Kilobytes -a -o "./outputs/time_d${distance}_n${ciclists}.txt" ./race $distance $ciclists d "./outputs/relatorio_d${distance}_n${ciclists}_${counter}.txt"

                        /usr/bin/time -f "%e segundos, %M Kilobytes" -a -o "./outputs/time_d${distance}_n${ciclists}.txt" ./race $distance $ciclists d "./outputs/relatorio_d${distance}_n${ciclists}_${counter}.txt"

                        echo "Fim"
                done
        done
done