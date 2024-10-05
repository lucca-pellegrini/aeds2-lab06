NAME       = Laboratório_06_-_AEDs_II_-_Quicksort_e_seu_Pivô_-_Lucca_M._A._Pellegrini_842986
MAIN       = main
TEXT       = *.tex
ETC        = Makefile
FIG        = ../build/fig/*
VER        = $(shell git describe --long --tags \
              | sed 's/^v//;s/\([^-]*-g\)/r\1/;s/-/./g')
LATEX      = pdflatex
LATEXFLAGS = -halt-on-error -interaction=batchmode -shell-escape
RELEASE    = $(NAME)_$(VER)

.PHONY: all clean release

all: $(MAIN).pdf

release: $(RELEASE).pdf

clean:
	$(RM) *.pdf *.ps *.idx *.bbl *.brf *.glo *.dvi *.toc *.lof *.aux   \
		*.log *.blg *.ilg *.ind *.out *.wsp *.fls *.synctex* *.zip \
		*.tar.gz *.*~ text/*/*.aux

$(MAIN).pdf: $(MAIN).tex $(ETC) $(FIG) $(TEXT)
	$(LATEX) $(LATEXFLAGS) $(MAIN).tex
	$(LATEX) $(LATEXFLAGS) $(MAIN).tex

$(RELEASE).pdf: $(MAIN).pdf
	gs -sDEVICE=pdfwrite -dCompatibilityLevel=1.5 -dPDFSETTINGS=/prepress \
		-dNOPAUSE -dQUIET -dBATCH -sOutputFile=$(RELEASE).pdf         \
		$(MAIN).pdf