Reponse a la question 5:
Nous recreons une fonction calloc qui alloue de la memoire et l'initialise a 0 (équivalent a '\0' en ASCII)

Reponse a la question 6:
La fonction free dealloue un bloc de memoire alloue par un appel de un des fonctions malloc, calloc ou realloc

Reponse a la question 20:
Si le nombre de caracteres saisis est egal ou superieur a la taille du buffer, le caractere residuel et le saut de ligne seront utilises comme entree pour l'appel mini_scanf suivant
De plus, comme le buffer est plein de caracteres qui ne sont pas des caracteres de fin de ligne, des erreurs peuvent se produire dans la fonction mini_printf
Donc, nous devons laisser un espace pour le caractere de fin de ligne, nous ne pouvons donc lire qu'au plus size_buffer - 1 caracteres
Nous devons egalement lire tous les caracteres residuels afin qu'ils ne soient pas lus de maniere indesirable par la prochaine fonction mini_scanf

Reponse a la question 22:
Si la taille de la chaine de destination de strcpy est inferieure a celle de la source, il y a un risque de depassement de memoire
La solution consiste a ne copier qu'au plus dest_size - 1 du caractere de la chaine (laissez 1 espace pour le caractere de fin de ligne) source ou dest_size est la taille de la chaine de destination
Si les deux chaines saisies dans strcmp ne se terminent pas par un caractere nul, il existe un risque de depassement de memoire
La solution est d'avoir une limite au nombre de caracteres compares