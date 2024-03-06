//////////////////////////////////////////
///\file test.c                          /
///\author groupe Z-BI                   /
///\date 03/2022                         /
///\brief fichier exe                    /
//////////////////////////////////////////

#include "couche1.c"
#include "couche2.c"
#include "couche3.c"
#include "couche4.c"
#include "couche5.c"


int main(void)
{
  if(!virtual_disk_sos)
    init_disk_sos("Dossier_Disk/Disk");

  if(!session){
      init_user_table();
  }
  if(login())
    console();

  save_disk();
}