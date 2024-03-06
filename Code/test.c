//////////////////////////////////////////
///\file test.c                          /
///\author groupe Z-BI                   /
///\date 03/2022                         /
///\brief fichier test                   /
//////////////////////////////////////////

#include "couche1.c"
#include "couche2.c"
#include "couche3.c"
#include "couche4.c"
#include "couche5.c"

int main(void)
{
  /////// TEST COUCHE 1 ///////////
    init_disk_sos("Dossier_Disk/Disk");

    block_t b;
    b.data[0] =1;
    b.data[1] =2;
    b.data[2] =3;
    b.data[3] =4;

    
    write_block(b,0);

    afficher_block(0);

    
  /////// TEST COUCHE 2 ///////////
    
    write_super_block();
  
    write_inodes_table();
    

    printf("Taille inode 0 : %d    \nCréation inode\n\n",virtual_disk_sos->inodes[0].size );
    
    char *filename="salut";
    init_inode(filename,4,virtual_disk_sos->super_block.first_free_byte);
    
    
    write_block(b,virtual_disk_sos->inodes[0].first_byte);
    
    printf("Inode 0 :\n" );
    
    afficher_block(virtual_disk_sos->inodes[0].first_byte);
    printf("\nName : %s\n",virtual_disk_sos->inodes[0].filename );
    printf("Taille inode 0 : %d\n",virtual_disk_sos->inodes[0].size );
    
    
    /////// TEST COUCHE 3 ///////////


    char username[FILENAME_MAX_SIZE];
    init_user_table();
    strcpy(username,"yeye");
    add_user(username);
    change_current_user(username);
    printf("current uid : %d\n",session->userid);
    if(verify_user("a")>=0)//doit ajouter \n au mot de passe à vérifier
      printf("utilisateur vérifié\n");
    else
      printf("erreur validité\n");
    if(verify_user("test\n")>=0)
      printf("ne doit pas s'afficher\n");

    /////// TEST COUCHE 4 ///////////
    load_file_from_host("testfich.txt");
    //store_file_to_host("salut");
    int index=in_inodes_table("testfich.txt");
    printf("inode[%d]:\n",index);
    /////// TEST COUCHE 5 ///////////

    if(!login())
      console();
    save_disk();
    return 0;
}
