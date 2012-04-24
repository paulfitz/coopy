
if [ ! -e coopy_source.txt ] ; then
    echo "Please run from build directory"
    exit 1
fi

SRC=`cat coopy_source.txt`

if [ ! -e ssdiff_core.js ] ; then

    EMCC_CFLAGS="--ignore-dynamic-linking" make ssdiff || exit 1
    cp bin/ssdiff ssdiff.bc || exit 1
    rm -rf /tmp/emscripten_temp
    
# EMCC_DEBUG=1 emcc lib/libcoopy_csv_sql.so lib/libcoopy_light.so lib/libcoopy_core.so lib/libcoopy_csv.so lib/libcoopy_full.so --embed-file broken_bridges.csv --embed-file bridges.csv -O3 --llvm-opts 2 x.bc -o foo.js
    
    EMCC_DEBUG=1 emcc lib/libcoopy_csv_sql.so lib/libcoopy_light.so lib/libcoopy_core.so lib/libcoopy_csv.so lib/libcoopy_full.so --pre-js $SRC/scripts/emscripten/pre.js -O3 --llvm-opts 2 x.bc -o ssdiff_core.js || exit 1

fi

{
cat<<EOF
function ssdiff(local, remote) {
    var Module = {
      local: local,
      remote: remote
    };
EOF
cat ssdiff_core.js
cat <<EOF
    return Module['return'];
}
EOF
} > ssdiff.js || exit 1
echo "Created ssdiff.js (remove ssdiff_core.js to recompile)"

{
cat <<EOF
<html>
<head>
<title>Testing ssdiff.js</title>
<link href='http://fonts.googleapis.com/css?family=Just+Me+Again+Down+Here' rel='stylesheet' type='text/css' />
<style>
textarea {
  width:300px; height:75px;
  background:ffc;
}
body {
  font-family: 'Just Me Again Down Here', cursive;
}
</style>

</head>

<body>
<h1>Testing ssdiff.js</h1>
EOF
} >> test_ssdiff_prefix.html || exit 1

{
cat <<EOF
<form>

<h2>Input: local table (csv)</h2>

<textarea id="t1">
NAME,DIGIT
one,1
too,2
three,3
four,4
five,5
</textarea>

<h2>Input: remote table (csv)</h2>

<textarea id="t2">
NAME,DIGIT
one,1
two,2
three,3
four,4
five,5
</textarea>

<h2>Output: difference (hilite)</h2>

<input type="button" value="run ssdiff!" onclick="document.getElementById('t3').value = ssdiff(document.getElementById('t1').value,document.getElementById('t2').value); return false;" /><br />

<textarea id="t3">
</textarea>


</form>

</body>
</html>

EOF
} > test_ssdiff_postfix.html || exit 1

{
    cat test_ssdiff_prefix.html
    echo '<script type="text/javascript" src="ssdiff.js"></script>'
    cat test_ssdiff_postfix.html
} > test_ssdiff.html || exit 1

{
    cat test_ssdiff_prefix.html
    echo '<script type="text/javascript">'
    cat ssdiff.js
    echo '</script>'
    cat test_ssdiff_postfix.html
} > test_ssdiff_single_file.html || exit 1

rm -f test_ssdiff_prefix.html test_ssdiff_postfix.html
echo "Created test_ssdiff.html test_ssdiff_single_file.html"


