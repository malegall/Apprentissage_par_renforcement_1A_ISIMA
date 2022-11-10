# Projet d'apprentissage par renforcement

Tout d'abord, une démonstration du projet est disponible sur:
https://perso.isima.fr/~rilamrani/projet_1a_isima/siteweb/projet_1a.html

Le projet se décompose en quatres phases :
+ La première phase consiste à réaliser plusieurs exercices personnels afin de faciliter la création d'un jeu de groupe.
+ La deuxième phase a été de créer un jeu de la vie
+ La troisième phase a été de créer un jeu d'arcarde en 2D. Ce jeu implémente différentes contraines telles que les chaines de markov.
+ La quatrième phase a été de créer un jeu qui utilise l'apprentissage par renforcement.

## Première phase

Nous avons pris en main la SDL2 à travers des exercices tels qu'un snake, un X de fenêtre et des mouvements d'un sprite.

## Deuxième phase

Pour le jeu de la vie : 
Nous avons 4 options:
-Jeu en monde normal
-Jeu en mode torique
-Sauvegarde d'une configuration
-Chargement d'une configuration

Pour le monde normal, notre jeu est borné; les cellules ne peuvent pas dépasser les bords de la fenêtre.  Pour le monde torique, les cellules peuvent dépasser les bords de la fenêtre pour obtenir une configuration.
Le choix du type de monde se fait en appuyant sur n pour monde normal et t pour monde torique.
Pour sauvegarder une configuration, il faut appuyer sur la touche s. Pour charger une configuration, il faut appuyer sur c.
Dans les deux mondes, quand nous atteignons une configuration fixe, un message signalant une configuration fixe s'affiche à l'écran.
Afin de détecter une configuration fixe, nous faisons une comparaison entre notre grille actuelle et la grille de l'état suivant.

## Troisième phase

Pour le premier chef d'oeuvre, nous avons décidé de créer un jeu d'arcarde en 2D.  Ce jeu implémente différentes contraines telles que les chaines de markov.  Nous avons décidé que la matrice des transitions agirait sur le déplacement de personnages.  Notre jeu consiste à ramasser le plus de diamant possible en évitant de se faire tuer.  Dès que le personnage ramasse un diamant, un ennemi en plus arrive sur la map.  Plus il y a d'ennemis, plus le jeu devient difficile jusqu'à mourir si un ennemi nous touche.

## Quatrième phase

La dernière partie consiste en la création d'un jeu qui utilise l'apprentissage par renforcement. Nous avons donc créé un jeu dont l'entité apprend à jouer et à atteindre un objectif final par ses propres moyens. Le principe de notre jeu se rapproche plus ou moins de celui d'un labyrinthe.
