# Morpion-3D
Jeu Morpion 3D en c
#Contexte — 
Le but de ce projet est d’implanter une plate-forme de jeu de type morpion
3D. Ce jeu se présente sous la forme d’un tableau de taille nn dans lequel sont placés des
piles de jetons marquées par un X ou par un O. Initialement aucun jeton n’est présent. Le
jeu se joue à deux joueurs, l’un possède les jetons X, l’autre les jetons O. Le but du jeu est
de faire apparaître une ligne de n jetons de sa couleur.
#Manuel —
L’utilisation des flèches est fortement recommandé cependant pour se conformer au sujet
j’ai implémenté aussi les boutons "i,j,k,l"
"r" pour retirer un jetons
"p" pour poser un jeton
"q" pour quitter
"entrer" pour sélectionner lorsque le menu est affiché
#Limites —
taille minimale de la fenêtre : (x,y)=60*18
taille maximale du tableau : n=min(x/4,(y-2)/2)
taille maximale de la pile : 50*n
pour quitter lors de la saisie de n il faut saisir une chaîne qui commence par "q" car le jeu
attend le "line feed".
lorsque le joueur effectue une opération non valide un message d’erreur est affiché et le jeu
se met en "pause" pour une seconde.
la taille de la fenêtre doit rester supérieur à ce qu’elle était lors de la saisie de n.
