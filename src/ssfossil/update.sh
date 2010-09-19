
# My memory of how to update fossil

if [ ! -e original.fsl ] ; then
    fossil clone http://www.fossil-scm.org/fossil original.fsl
fi

if [ ! -e original/_FOSSIL_ ] ; then
    mkdir -p original
    cd original
    fossil open ../original.fsl
    cd ..
fi

if [ ! -e pulled.txt ] ; then
    cd original
    fossil pull | tee ../pulled.txt
    cd ..
fi

cd original
fossil update `tail -n1 ../previous.uuid`
cd ..

if true; then
    for f in `cd original/src; ls *`; do
	echo "========================================================"
	echo $f
	cmp original/src/$f fossil/src/$f || (
	    echo Making diff
	    diff -u original/src/$f fossil/src/$f | tee patch/$f.patch
	)
    done
fi

