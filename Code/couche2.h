////////////////////////////////////////////////////
///\file couche2.h                                 /
///\author groupe Z-BI                             /
///\date 03/2022                                   /
///\brief implementation du header de la couche 2  /
////////////////////////////////////////////////////

#ifndef __COUCHE2_H__
#define __COUCHE2_H__
#include "sos_defines.h"
#include "couche1.h"


///\brief ecrit le super bloc au tout d´ebut du fichier disque
///\param void
///\return int : return 1 si ça ne marche ou 0 sinon
int write_super_block();


///\brief lit le super bloc au tout d´ebut du disque
///\param void
///\return int : return 1 si ça ne marche ou 0 sinon
int read_super_block(void);


///\brief met a jour le champs first_free_byte du super bloc
///\param void
///\return int : return 1 si ça ne marche ou 0 sinon
void first_free_byte(uint b);


///\brief permettant de lire la table d’inodes sur le systeme a la suite du super bloc
///\param inode_t bloc: bloc d'inode a lire
///\return int : return 1 si ça ne marche ou 0 sinon
int  read_inodes_table(void);


///\brief permettant d’ecrire la table d’inodes sur le systeme a la suite du super bloc
///\param inode_t bloc: bloc d'inode a ecrire
///\return int : return 1 si ça ne marche ou 0 sinon
int  write_inodes_table(void);


///\brief a partir d’un indice dans la table d’inodes supprime l’inode correspondant et compacte la table
///\param int i :  indice dans la table d’inodes
///\param int inode :  inode
///\return int : return 1 si ça ne marche ou 0 sinon
int delete_inode(uint i);


///\brief retourne l’indice du premier inode disponible dans la table
///\param int pos : innode
int get_unused_inode(void);


///\brief initialise un inode `a partir d’un nom de fichier, de sa taille et de sa position sur le systeme
///\param int pos : inode
int  init_inode(char* file,uint taille,uint firstbyte);

#endif
