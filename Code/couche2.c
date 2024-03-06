//////////////////////////////////////////
///\file couche2.c                       /
///\author groupe Z-BI                   /
///\date 03/2022                         /
///\brief implementation de la couche 2  /
//////////////////////////////////////////

#include "couche2.h"

extern virtual_disk_t *virtual_disk_sos;

///\brief ecrit le super bloc au tout d´ebut du fichier disque
///\param void
///\return int : return 1 si ça ne marche ou 0 sinon


int init_super_block(){
  virtual_disk_sos->super_block.number_of_files = 0;
  virtual_disk_sos->super_block.number_of_users = 0;
  virtual_disk_sos->super_block.nb_blocks_used = 0;
  virtual_disk_sos->super_block.first_free_byte = INODES_START ;

  return write_super_block();
}



int write_super_block(){

  //positionnment du curseur sur le debut du fich
  if (fseek((virtual_disk_sos->storage),0,SEEK_SET)!=0) {
    return 0;
  }

  //ecriture du bloc
  if ((fwrite(&(virtual_disk_sos->super_block),BLOCK_SIZE,SUPER_BLOCK_SIZE,virtual_disk_sos->storage))!=SUPER_BLOCK_SIZE) {
    return 0;
  }

  return 1;

}

///\brief lit le super bloc au tout d´ebut du disque
///\param void
///\return int : return 1 si ça ne marche ou 0 sinon
int read_super_block(void){

  //positionnment du curseur sur le debut du fich
  if (fseek(virtual_disk_sos->storage,0,SEEK_SET)!=0) {
    return 0;
  }

  //lecture du bloc
  if (fread(&(virtual_disk_sos->super_block),BLOCK_SIZE,SUPER_BLOCK_SIZE,virtual_disk_sos->storage)!=SUPER_BLOCK_SIZE) {
    return 0;
  }

  return 1;
}


///\brief met a jour le champs first_free_byte du super bloc
///\param void
///\return int : return 1 si ça ne marche ou 0 sinon
void first_free_byte(uint b){
  if (b%BLOCK_SIZE!=0) {
    b+=(BLOCK_SIZE-((b)%BLOCK_SIZE));
  }
  virtual_disk_sos->super_block.first_free_byte=b;
}


///\brief charger la table d’inodes depuis le système et terminer l’initialisation
///\param inode_t bloc: bloc d'inode a lire
///\return int : return 1 si ça ne marche ou 0 sinon
int read_inodes_table(void){
  //positionnment du curseur sur le debut du fich
  if (fseek(virtual_disk_sos->storage,INODES_START,SEEK_SET)!=0) {
    return 0;
  }

  //lecture du bloc
  if (fread(&(virtual_disk_sos->inodes),INODE_SIZE,INODE_TABLE_SIZE,virtual_disk_sos->storage)!=INODE_TABLE_SIZE) {
    return 0;
  }
  fprintf(stderr,"Erreur lors de la lecture de la table d'inodes\n");
  return 1;
}


///\brief permettant d’ecrire la table d’inodes sur le systeme a la suite du super bloc
///\param inode_t bloc: bloc d'inode a ecrire
///\return int : return 1 si ça ne marche pas ou 0 sinon
int  write_inodes_table(void){
  //positionnment du curseur sur le debut du fich

  if (fseek(virtual_disk_sos->storage,INODES_START,SEEK_SET)!=0) {
    return 0;
  }

  //ecriture du bloc
  if (fwrite(&(virtual_disk_sos->inodes),INODE_SIZE,INODE_TABLE_SIZE,virtual_disk_sos->storage)!=INODE_TABLE_SIZE) {
    return 0;
  }
  fprintf(stderr,"Erreur lors de l'écriture de la table d'inodes\n");

  return 1;
}


///\brief a partir d’un indice dans la table d’inodes supprime l’inode correspondant et compacte la table
///\param int i :  indice dans la table d’inodes
///\param int inode :  inode
///\return int : return 1 si ça ne marche ou 0 sinon
int delete_inode(uint i){

  if (virtual_disk_sos->inodes[i].first_byte!=0) {

    virtual_disk_sos->super_block.number_of_files--;
    virtual_disk_sos->super_block.nb_blocks_used-=virtual_disk_sos->inodes[i].nblock;

    if ((i=INODE_TABLE_SIZE-1) || (virtual_disk_sos->inodes[i+1].first_byte==0)) {
      first_free_byte(virtual_disk_sos->inodes[i].first_byte);
    }

    int j;
    for (j = i; j < INODE_TABLE_SIZE-1; j++) {
      virtual_disk_sos->inodes[i]=virtual_disk_sos->inodes[i+1];
    }

    virtual_disk_sos->inodes[j].first_byte=0;
    return 0;
  }
  else{return 1;}
}


///\brief retourne l’indice du premier inode disponible dans la table ou un nombre négatif si elle est pleine
///\return l'indice du premier inode disponible -1 si ils sont tous occupés
int get_unused_inode(){
  for (int i = 0; i < INODE_TABLE_SIZE; i++) {
    if (virtual_disk_sos->inodes[i].first_byte==0) {
      return i;
    }
  }
  return -1;
}


///\brief initialise un inode `a partir d’un nom de fichier, de sa taille et de sa position sur le systeme
///\param int pos : inode
int  init_inode(char* file,uint taille,uint firstbyte){

  for (int i = 0; i < INODE_TABLE_SIZE; i++) {
    if (virtual_disk_sos->inodes[i].filename==file) {
      return -1;
    }
  }

  int nv_i;
  if ((nv_i=get_unused_inode())==-1) {
    return -1;
  }

  inode_t* inode = &virtual_disk_sos->inodes[nv_i];


  strcpy(inode->filename, file);
  inode->size=taille;
  inode->first_byte=firstbyte;
  inode->nblock=compute_nblock(taille);
  inode->oright=Rw;
  inode->uright=RW;
  time_t timing = time(NULL);

  strcpy(inode->ctimestamp, ctime(&timing));
  strcpy(inode->mtimestamp, ctime(&timing));

  virtual_disk_sos->super_block.nb_blocks_used += inode->nblock;
  virtual_disk_sos->super_block.number_of_files++;

  if (virtual_disk_sos->super_block.first_free_byte==firstbyte) {
    first_free_byte(firstbyte+taille);
  }
  return nv_i;
}
