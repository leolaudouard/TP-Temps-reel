[ Code de l'interface graphique utilisée lors du TP sur le Gyropode (ainsi que sur la maquette) ]

Se lance avant le programme de temps réel, sur une machine du même réseau que la Raspberry Pi
Noter l'adresse IP de la machine éxécutant ce programme dans le fichier "../TR/includes.h" (code source du programme Xenomai)
Ce programme communique avec le programme Xenomai par deux sockets UDP afin d'obetnir les informations à afficher (voir gui.c)

[ Packages nécéssaires ]
- glib 2.0
- gtk+ 3.0

[ Compilation ]
cc -Wall -g -o GUI *.c $(pkg-config --cflags --libs glib-2.0 gtk+-3.0) -lm -rdynamic
