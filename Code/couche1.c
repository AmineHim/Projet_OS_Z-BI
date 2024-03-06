//////////////////////////////////////////
///\file couche1.c                       /
///\author groupe Z-BI                   /
///\date 03/2022                         /
///\brief implementation de la couche 1  /
//////////////////////////////////////////

#include "couche1.h"

#define OK 0
#define KO_WRITE 1
#define KO_READ 2

virtual_disk_t *virtual_disk_sos;

///\brief initialisation du dique virtuel "d0" à partir du nom de repertoire en parametre
///\param char *dirname : pointeur nom du repertoire
void init_disk_sos(char *dirname)
{

    char chemin_disc[500];

    strcpy(chemin_disc, dirname);
    strcat(chemin_disc, "/d0");
    FILE *fich = fopen(chemin_disc,"r+");
    virtual_disk_sos = malloc(sizeof(virtual_disk_t));
    virtual_disk_sos->storage = fich;

}

///\brief sauvegarde du disque
///\param void
///\return int : return 1 si ça ne marche ou 0 si ça sauvegarde
int save_disk(void)
{
    if(fclose(virtual_disk_sos->storage) == EOF)
    {
        return 1;
    }
    free(virtual_disk_sos);
    return 0;
}

///\brief calcule le nombre de blocs necessaires pour stocker un nombre n d’octets
///\param int : nb_octets : nombre d'octets
///\return int : le nombre de blocs necessaires pour stocker un nombre n d’octets
int compute_nblock(int nb_octets)
{
    if(nb_octets % BLOCK_SIZE == 0)
        return nb_octets/BLOCK_SIZE;
    else
        return ((nb_octets/BLOCK_SIZE) + 1);
}

///\brief ecrit un bloc block, a la position pos sur le disque du systeme
///\param block_t bloc : bloc a ecrire dans le disque
///\param int pos : position d ecriture du bloc dans le dique
///\return int : renvoir 0 si ok ou 1 si ko
int write_block(block_t bloc, int pos)
{
    int retour = OK;
    //positionnment du curseur sur le debut du fich
    fseek(virtual_disk_sos->storage,pos,SEEK_SET);

    //ecriture du bloc
    size_t ecrit = fwrite(bloc.data,BLOCK_SIZE,1,virtual_disk_sos->storage);
    if(ecrit != 1 )
        retour = KO_WRITE;



    return retour;
}


///\brief lit un bloc block de donn ees, a la position pos sur le disque du systeme
///\param block_t bloc : pointeur du bloc a lire dans le disque
///\param int pos : position de lecture du bloc dans le disque
///\return int : le code retour
int read_block(block_t *bloc, int pos)
{
    int retour = OK;


    //positionnment du curseur sur le debut du fich
    fseek(virtual_disk_sos->storage,pos,SEEK_SET);

    //ecriture du bloc
    size_t lu = fread(bloc->data,BLOCK_SIZE,1,virtual_disk_sos->storage);
    if(lu != 1 )
        retour = KO_READ;


    return retour;
}

///\brief affichage a l’ecran des blocs de type block t en hexad ecimal
///\param int pos : position
void afficher_block(int pos)
{
    block_t bloc;
    if(read_block(&bloc,pos) == KO_READ)
        perror("Erreur de lecture");

    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        printf("Ox%x ",bloc.data[i]);
    }

}
