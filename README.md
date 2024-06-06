# Quad_trees_project


Compression d'img avec des quadtrees

Calcul de moyenne
Recherche de l'erreur
Erreur la plus grande

Prendre des char au lieu des unsigned int 

Pour avoir la moyenne : Double boucle for imbriqué de 512 pixels
On doit récupérer chaque pixel (fonction MLV_get_pixel_img pour récupérer le rgba de chaque pixel) où on récupère leur rgba et on fait la somme et on divise par le nbr de pixels 
-> On va avoir la moyenne du rouge / bleu / vert pour obtenir la couleur moyenne ()
-> On fabrique notre premier qnoeud initial

Il faut refaire par une double boucle for imbriqué de 512 pour calculer l'erreur 
C'est quoi l'erreur : on fait une comparaison pour chaque pixel entre la couleur moyenne et de l'image dont on est parti
On va provoquer une erreur colossale 
Une image uni -> suffit d'un seul noeud 

On va subdiviser le noeud de plusieurs erreurs :
Puis on va spliter les plus grosses erreurs

Plus l'erreur est grande, plus elle sera prioritaire à calculer 

L'algorithimque d'approximation d'une image c'est de faire la moyenne, je calcule l'erreur, je prends le noeud qui correspond le moins à la réalité(plus grosse différence de la comparaison) je le coupe en 4
En coupant en 4 l'erreur la plus grande, le pixel va mieux correspondre à la réalité et refais un parcours

Pour avoir l'erreur du rond rouge 2:1 : on check le up-left et down-right / Vu que ça été coupé en 4 puis coupé encore en 4, ce sera des boucles de taille 128 par 128. Et je calcule l'erreur en regardant pour chaque pixel à quelle distance il est du rouge. Si l'erreur est nulle, plus besoin de calculer dans cette zone.
L'image de la vache kiri est parfaite par exemple car au lieu d'avoir la description de chaque pixel, on a économisé de l'emplacement mémoire dès lorsque la fonction estime qu'il n'y plus d'erreur de comparaison.
Une zone de 128x128 de la même couleur va obtenir qu'un seul qnoeud au lieu d'un split de plusieurs qnoeud de 128x128 (10000 pixels en +)

Calcul de la moyenne / Calcul de l'erreur sont de même complexité car il s'agit de double boucles for imbriqué sur un métapixel
Moyenne : Double boucle for imbriqué du up-left au down-right en appelant la libMLV pour obtenir le rgba de chaque pixel et fait la moyenne en faisant la somme et en divisant par le nombre de pixels du métapixel
Erreur : Double boucle for imbriqué sur la zone du métapixel où on fait la moyenne par rapport au vrai pixel. L'erreur permet de savoir si l'erreur est bon où il faut subdiviser car il est pas assez proche de la réalité du pixel réel


Tas max pour l'erreur = meilleur solution 
TAS = 
                        i 
(fils de gauche = 2i + 1 ) / (fils de droite = 2i + 2)


