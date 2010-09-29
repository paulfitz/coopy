
make latex && (
    cd gendoc/latex
    latex refman && latex refman && latex refman && dvips refman.dvi && ps2pdf refman.ps && xpdf refman.pdf
)
