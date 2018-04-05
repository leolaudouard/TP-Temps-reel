Ce programme de temps réel est pour l'asservisement et les surveillances de conditions de gyropode.

Commande pour la complition de programme:
-->make
Avant de la compilation, il faut l'utilisateur supprimer l'ancien executable 
-->make clean
Commande pour lancer de l'executable de programme:
-->sudo ./main


Les répatititions de ce programme est comme suivantes:

1. Main.cpp
	-Contenant des créations, lancement de tâches
	-Initialisation des semaphores, mutex de la file de message 
	-Initialisation de log 

2.	i. Fonction.cpp
	-Contentant tous les descriptions de fonctions pour les tâches de temps réel

	ii. Fonction.h
	-Contentant tous les headers des fonctions pour les tâches de temps réel
	
3. i. Global.cpp
	-Contentant tous les déclarations des tâches, des mutex, des sémaphores, de la file de message et des variables partagées 

	ii. Global.h
	-Contentant tous les définitions de constants, exportations  des tâches, des mutex, des sémaphores, de la file de message et des variables partagées 

4. i. Include.h
	-Contentant tous les libraires besoin pour des tâches de temps réel, création de socket et aussi des librairies standard.

Dans le dossier lib
5. lib.h
	-Fichier pour include tous les libraires des variables et fonctions 

--->Dans le dossier lib/inc
	-tous les headers de libraires
--->Dans le dossier lib/src
	6. angles.cpp
		-  Implémentation et déclaration de la classe Angles
	7. battery.cpp
		-  Implémentation et déclaration de la classe Battery
	8. consgine.cpp
		-  Implémentation et déclaration de la classe Consigne
	9. beta.cpp // pas utile pour simulateur 
		-  Implémentation et déclaration de la classe Beta
	10. lib_serial.c
		-  Librairie pour tous les opérations lié au UART
	11. lib_msg_GUI.c
		-  Librairie pour tous les opérations lié au GUI / Socket
	12.	lib_monitor.c
		-  Librairie pour tous les opérations lié au log / Socket


