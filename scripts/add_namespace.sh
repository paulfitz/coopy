for f in `ls *.cpp`; do
    grep -v -q "coopy::store" $f && (
	perl -pe 's/(using namespace std;)/$1\nusing namespace coopy::store;/' < $f > $f.tmp
	#diff $f.tmp $f
	cp $f.tmp $f
    )
done
