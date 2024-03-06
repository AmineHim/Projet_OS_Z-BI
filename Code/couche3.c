////////////////////////////////////////////////////
///\file couche3.c                                 /
///\author groupe Z-BI                             /
///\date 04/2022                                   /
///\brief implementation du code de la couche 3    /
////////////////////////////////////////////////////

#include "couche3.h"
#include "string.h"
#include "sha256_utils.h"

#ifndef session
session_t *session;
#endif

#ifndef virtual_disk_sos
virtual_disk_t *virtual_disk_sos;
#endif 

///\brief initialise la table d'utilisateur
///\return int : return 0 si ça marche négatif sinon 
int init_user_table(){
  char pwd[FILENAME_MAX_SIZE];
	char hash_of_pwd[SHA256_BLOCK_SIZE*2 + 1];
  session=malloc(sizeof(session_t));
  strcpy(virtual_disk_sos->users_table[ROOT_UID].login, "root");

  printf("Définir un mot de passe super utilisateur: ");
  if (!fgets(pwd, FILENAME_MAX_SIZE, stdin)) {
		fprintf(stderr, "Erreur à l'entrée du mot de passe\n");
		return -1;
	}

  pwd[strspn(pwd,"\n")]='\0';
  sha256ofString((BYTE *)pwd,hash_of_pwd);
  memcpy(virtual_disk_sos->users_table[ROOT_UID].passwd, hash_of_pwd,strlen(hash_of_pwd));
  session->userid=ROOT_UID;
  virtual_disk_sos->super_block.number_of_users=1;

  return 0;
}

///\brief ajout d'un utilisateur
///\param char[] : login : login du nouvel utilisateur
///\param char* : pwd : mot de passe du nouvel utilisateur
///\return int : return l'uid du nouvel utilisateur si ça marche négatif sinon 
int add_user(char login[FILENAME_MAX_SIZE]){
    char hash_of_pwd[SHA256_BLOCK_SIZE*2 + 1];
    char pwd[FILENAME_MAX_SIZE];

    if(virtual_disk_sos->super_block.number_of_users<NB_USERS){



      printf("Définir un mot de passe utilisateur: ");
      if (!fgets(pwd, FILENAME_MAX_SIZE, stdin)) {
		  fprintf(stderr, "Erreur à l'entrée du mot de passe\n");
		    return -2;
	    }
      pwd[strspn(pwd,"\n")]='\0';
      sha256ofString((BYTE *)pwd,hash_of_pwd);
    
      memcpy(virtual_disk_sos->users_table[virtual_disk_sos->super_block.number_of_users].login,login,strlen(login));
      memcpy(virtual_disk_sos->users_table[virtual_disk_sos->super_block.number_of_users].passwd,hash_of_pwd,strlen(hash_of_pwd));
      virtual_disk_sos->super_block.number_of_users++;
      return virtual_disk_sos->super_block.number_of_users-1;
    }
    return -1;//utilisateur non créé
}

///\brief change d'utilisateur 
///\param char* : login : nom de l'utilisateur 
///\return int :  renvoie 0 si l'utilisateur a changé valeur positive sinon
int change_current_user(char* login){
    char hash_of_pwd[SHA256_BLOCK_SIZE*2 + 1];
    char pwd[FILENAME_MAX_SIZE];

    for (int i=0;virtual_disk_sos->super_block.number_of_users>i;i++){
      if(strcmp(virtual_disk_sos->users_table[i].login,login)){
        
        printf("Entrez le mot de passe de %s: ",login);
        if (!fgets(pwd, FILENAME_MAX_SIZE, stdin)) {
		      fprintf(stderr, "Erreur à l'entrée du mot de passe\n");
		      return 2;
	      }
        pwd[strspn(pwd,"\n")]='\0';
        sha256ofString((BYTE *)pwd,hash_of_pwd);
        if(!strcmp(virtual_disk_sos->users_table[i].passwd,hash_of_pwd)){
          return session->userid=i;
          
        }
        else{
          fprintf(stderr,"mdp erroné\n");
          return 1;  
          }
        }
    }
    return 0;
}

///\brief demande le mot de passe de l'utilisateur  
///\param char* : pwd : mot de passe à vérifier 
///\return int :  renvoie 0 si l'utilisateur a été vérifié valeur positive sinon
int verify_user(char *pwd){
  char hash_of_pwd[SHA256_BLOCK_SIZE*2 + 1];
  //pwd[strspn(pwd,"\n")]='\0';
  sha256ofString((BYTE *)pwd,hash_of_pwd);
  return strcmp(virtual_disk_sos->users_table[session->userid].passwd,hash_of_pwd) ;
}
