////////////////////////////////////////////////////
///\file couche1.h                                 /
///\author groupe Z-BI                             /
///\date 03/2022                                   /
///\brief implementation du header de la couche 1  /
////////////////////////////////////////////////////

#ifndef __COUCHE1_H__
#define __COUCHE1_H__
#include "sos_defines.h"


///\brief initialisation du dique virtuel "d0" à partir du nom de repertoire en parametre
///\param char *dirname : pointeur nom du repertoire
///\return virtual_disk_t : le disque initsialise
void init_disk_sos(char *dirname);


///\brief sauvegarde du disque
///\param void
///\return int : return 1 si ça ne marche ou 0 si ça sauvegarde
int save_disk(void);


///\brief calcule le nombre de blocs necessaires pour stocker un nombre n d’octets
///\param int : nb_octets : nombre d'octets
///\return int : le nombre de blocs necessaires pour stocker un nombre n d’octets
int compute_nblock(int nb_octets);


///\brief ecrit un bloc block, a la position pos sur le disque du systeme
///\param block_t bloc : bloc a ecrire dans le disque
///\param int pos : position d ecriture du bloc dans le dique
///\return int : renvoir 0 si ok ou 1 si ko
int write_block(block_t bloc, int pos);


///\brief lit un bloc block de donn ees, a la position pos sur le disque du systeme
///\param block_t bloc : pointeur du bloc a lire dans le disque
///\param int pos : position de lecture du bloc dans le disque
///\return int : le code retour
int read_block(block_t *bloc, int pos);


///\brief affichage a l’ecran des blocs de type block t en hexad ecimal
///\param int pos : position
void afficher_block(int pos);

#endif
