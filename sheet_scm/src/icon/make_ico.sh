
src=appicon.xpm
ct=0
rm -f tmp_*.png tmp_*.ico
for f in 16 32 64; do
    convert $src -geometry "$f!x$f!" png32:tmp_${ct}_$f.png
    identify tmp_${ct}_$f.png
    let ct=$ct+1
done

rm appicon.ico
convert tmp_*.png appicon.ico
identify appicon.ico
rm -f tmp_*.png tmp_*.ico
