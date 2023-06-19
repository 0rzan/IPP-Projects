#!/bin/bash

prog="$1"
dir="$2"
tests_succeded=0
tests_failed=0

for f in "$dir"/*.in
do
    echo "Sprawdzam test $f:"
    valgrind --leak-check=full --log-fd=3 "$prog" <"$f" >tmp.out 2> tmp.err 3>tmp.valgrind
    if ! diff tmp.out "${f%in}out" >/dev/null 2>&1
    then
        tests_failed=$((tests_failed+1))
        echo "Błąd w teście $f, niezgodne wyjście!"
    elif ! diff tmp.err "${f%in}err" >/dev/null 2>&1
    then
        tests_failed=$((tests_failed+1))
        echo "Błąd w teście $f, niezgodny błąd!"
    elif ! ( grep -q "no leaks are possible" tmp.valgrind || grep -q "ERROR 0" tmp.err )
    then
        tests_failed=$((tests_failed+1))
        echo "Wyciek pamięci w teście $f!"
    else
        tests_succeded=$((tests_succeded+1))
        echo "Test $f zakończony powodzeniem!"
        rm -f tmp.out tmp.err tmp.valgrind
    fi
done
echo "Ilość testów poprawnych: $tests_succeded"
echo "Ilość testów niepoprawnych: $tests_failed"
