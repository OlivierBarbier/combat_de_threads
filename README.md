# Combat de threads

On simule un jeu au tour par tour dans lequel 2 threads 
s'affrontent.

La condition à observer est que lorsque un thread joue 
l'autre doit attendre.

Le jeux consistera pour un thread à tirer aléatoirement 2
entiers et à constater s'ils ont la même parité (tous 
les 2 pairs ou tous les 2 impairs).

Dans le cas où la parité est la même pour les 2 entiers,
on incrémente un compteur de points associé au thread
courant et on répète l'opération jusqu'à ce que :

1. La parité soit différente, auquel cas :

    1.1. on incrémente un compteur de tirage associé au thread,

    1.2. on laisse la main à l'autre thread.

2. Le nombre de points soit égal à 10, auquel cas :

    2.1 on incrémente un compteur de tirage associé au thread,

    2.2 on lève un drapeau "gameover",

    2.3 on avertit l'autre thread pour qu'il se termine,

    2.4 et on termine le thread courant. 

Lorsque les 2 threads ont fini de jouer, on affiche 
depuis le programme principal pour chaque thread :

Thread #<numero_du_thread> Nb tirages : <nb_tirage> Nb points : <nb_points>
