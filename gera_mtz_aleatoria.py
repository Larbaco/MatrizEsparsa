#!/usr/bin/python
import sys
from random import random
if(len(sys.argv) != 4):
   print 'Uso: \n\t'+sys.argv[0]+' <nLinhas> <nColunas> <aSaida>\n'
else:
   arq = open(sys.argv[3],'w')
   for l in range(int(sys.argv[1])):
      for e in range(int(sys.argv[2])):
         arq.write("{valor} ".format(valor = int(random() * 100) -50))
      arq.write('\n')
