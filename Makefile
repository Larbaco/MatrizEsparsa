all:
	gcc MatrizEsparsa.c -o matriz
debug:
	gcc MatrizEsparsa.c -o matriz -g -Wall
executa:
	clear
	gcc MatrizEsparsa.c -o matriz
	./gera_mtz_aleatoria.py 5 5 entrada
	./gera_entrada_mtz.py entrada entrada
	./matriz -help
