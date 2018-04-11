#!/bin/sh

set -u

PROG=${PROG:-./a.out}

DIR=/tmp/tp-dir.$$	# répertoire de test
RES=/tmp/tp-res.$$	# fichier de résultat
LOG=test-tp.log		# log : dans le répertoire courant

GROSFICHIER=/usr/include/unistd.h	# > 1000 lignes sur Linux

# trap "rm -rf $DIR $RES" EXIT	# pour supprimer les fichiers à la sortie

annoncer ()
{
    COURANT="$1"
}

fail ()
{
    echo "==> Échec du test '$COURANT'"
    echo "==> Log :"
    cat $LOG
    echo "==> Exit"
    exit 1
}

creer_fichiers ()
{
    local n=$1
    local dir=$2
    local i j

    rm -rf $dir
    mkdir -p $dir

    # créer un fichier de $i lignes (borné par la taille de GROSFICHIER)
    i=1
    while [ $i -le $n ]
    do
	sed "$i q" $GROSFICHIER > "$dir/fichier$i"
	i=$((i+1))
    done
}

attendu ()
{
    local dir=$1
    local liste f n sep

    liste=""
    sep=""
    for f in $(ls $dir)
    do
	if [ -f "$dir/$f" ]
	then
	    n=$(wc -l < "$dir/$f")
	    liste="$liste$sep$n"
	    sep="
"
	fi
    done
    echo "$liste" | sort -n
}

exec 2> $LOG
set -x

#
# Tester un répertoire vide
#

annoncer "Répertoire vide"
creer_fichiers 0 $DIR
$PROG $DIR > $RES && cmp -s /dev/null $RES			|| fail

#
# Tester un répertoire avec 5 fichiers
#

annoncer "Répertoire avec 5 fichiers"
creer_fichiers 5 $DIR
$PROG $DIR | sort -n > $RES && attendu $DIR | diff -q - $RES	|| fail

# #
# # Tester un répertoire avec 10000 fichiers
# #
# 
# annoncer "Répertoire avec 10000 fichiers"
# creer_fichiers 10000 $DIR
# ATTENDU="$(liste $DIR)"
# $PROG $DIR | sort -n > $RES && attendu $DIR | diff -q - $RES	|| fail

rm -rf $DIR $RES

exit 0
