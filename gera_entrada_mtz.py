#!/usr/bin/python
import sys
if(len(sys.argv) != 3):
   print 'Uso: \n\t'+sys.argv[0]+' <entrada> <saida>\n'
else:
   arq = open(sys.argv[1]);
   mtz = []
   for l in arq:
      mtz += [[int(e) for e in l.split()]]
   arq = open(sys.argv[2],'w')
   arq.write('L C V\n')
   for l in range(len(mtz)):
      for e in range(len(mtz[l])):
         if mtz[l][e] != 0:
            arq.write("{linha} {coluna} {valor}\n".format(linha = l+1,coluna = e+1, valor = mtz[l][e]))

#************************************#
