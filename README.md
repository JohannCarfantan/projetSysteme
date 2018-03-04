# Shell simplifié
Le but de ce projet est d'ecrire un shell simplifié permettant de lancer des processus en ligne de commande dans un environnement Unix en binôme.

Le binôme est constitué de :
- Johann Carfantan
- Julien Gestin

# Utilisation du code
Afin de compiler le projet, il suffit de taper la commande suivante à la racine du projet :
```
make
```
Pour le lancer, dans le dossier /bin il suffit de faire :
```
./shell
```
# Architecture du projet
### /bin
Ce dossier contient uniquement le projet compilé et exécutable
### /src
Ce dossier contient tous les fichiers sources du projet non compilé.
### /fichier.txt
Ce fichier est un exemple de redirection de sortie que l'on peut utiliser dans notre shell simplifié
### /testcommandeX.txt
Ces fichiers sont des exemples de fichiers que l'on peut utiliser pour exécuter des fichiers dans notre shell.
### /Makefile
Ce fichier décrit les actions à faire afin de compiler les sources du projet pour en créer un fichier dans /bin.

Par Johann Carfantan et Julien Gestin 
dans le cadre de la formation IMR première année à l'ENSSAT
