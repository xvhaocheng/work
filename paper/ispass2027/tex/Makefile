.PHONY: all check clean
all: check
	latexmk -pdf -interaction=nonstopmode -halt-on-error main.tex
check:
	python3 check_claims.py
clean:
	latexmk -c main.tex
