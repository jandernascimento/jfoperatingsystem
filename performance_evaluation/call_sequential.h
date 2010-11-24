#ifndef __CALL_SEQUENTIAL__
#define __CALL_SEQUENTIAL__

/* Structure passee a chaque participant au calcul contenant son numero, les
 * bornes de l'intervalle d'indices sur lequel il travaille, le tableau
 * d'elements, la taille du tableau et les arguments construits par l'appel a
 * initialisation */
typedef struct {
//    int num;
    int inf;
    int sup;
    int *vector;
    int size;
//    void *args_algo;
} arguments_t;

/* Fonction appelee dans le programme principal avant creation des threads
 * Initialise les structures de donnees propre a l'algo et renvoie un pointeur
 * sur ces structures
 * Les arguments sont :
 * - le degre de parallelisme (nombre de thread, ==0 pour un algo sequentiel)
 * - le tableau d'elements et sa taille
 * - l'argument passe en ligne de commande (ou NULL si aucun argument) */
void *initialization(int parallelism, int *vector, int size, char *arg);

/* Fonction appelee dans chaque thread (traitement effectue en parallele) avec
 * en argument la structure decrite au debut de ce fichier */
void dowork(arguments_t *args);

/* Fonction appelee dans le programme principal apres terminaison des threads
 * Les arguments sont :
 * - le degre de parallelisme
 * - le tableau des structures passees a chacun des threads lors du calcul (ce
 *   tableau contient min(1, parallelism) structures correspondantes au modele
 *   decrit au debut de ce fichier) */
void collect_results(int parallelism, arguments_t *args);

#endif
