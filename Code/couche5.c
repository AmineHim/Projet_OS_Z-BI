//////////////////////////////////////////
///\file couche5.c                       /
///\author groupe Z-BI                   /
///\date 05/2022                         /
///\brief implementation de la couche 5  /
//////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "couche1.h"
#include "couche2.h"
#include "couche3.h"
#include "couche4.h"
#include "couche5.h"
#include "sos_defines.h"
#include "sha256_utils.h"

///////////CREATION DE LA COMMANDE
///\brief Creation de la structure de la commande
///\param char *clavier: Chaine de caractere lue au clavier
///\return cmd: Commande: int nbArgs,char tabArgs[]
cmd_t *creerCommande(char *clavier){

	int argc=1;

	for (int i=0; i<(int)strlen(clavier);i++){
		if (clavier[i]==' '){
			argc++;
		}
	}
	size_t taille = sizeof(argc);
	cmd_t *cmd=malloc(sizeof(cmd_t));
	cmd->nbArgs=0;
	cmd->tabArgs=malloc(sizeof(char *)*argc);

	int debMot=0;
	int finMot=0;
	for (int j=0; j<(int)strlen(clavier);j++){
		if (clavier[j]==' '){
			int longueurMot=j-debMot;
			taille+=(longueurMot+1)*sizeof(char);
			cmd->tabArgs[finMot]=malloc(sizeof(char)*longueurMot+1);
			strncpy(cmd->tabArgs[finMot],clavier+debMot,longueurMot);
			cmd->tabArgs[finMot][longueurMot]=0;
			debMot=j+1;
			finMot++;
		}
	}
	int longueurMot=(int)strlen(clavier)-debMot;
	taille+=(longueurMot+1)*sizeof(char);
	cmd->tabArgs[finMot]=malloc(sizeof(char)*longueurMot+1);
	strncpy(cmd->tabArgs[finMot],clavier+debMot,longueurMot);
	cmd->tabArgs[finMot][longueurMot]=0;
	cmd->nbArgs=argc;
	return cmd;
}
///////////////////LS
///\brief liste le contenu du catalogue.
///\brief "ls" pour nom et taille, "ls -l" pour tout les détails
///\param char *lsl: "ls" ou "ls -l"
void ls(char *lsl){
	if(!strcmp(lsl,"ls")){
		for(int i = 0; i<INODE_TABLE_SIZE; i++){
			if(strcmp(virtual_disk_sos->inodes[i].filename, "")){
				inode_t inode = virtual_disk_sos->inodes[i];
				printf("Nom:%-*s Taille:%-*d\n", FILENAME_MAX_SIZE, inode.filename,INODE_SIZE, inode.size);
			}
		}
	}else{
		for(int i = 0; i<INODE_TABLE_SIZE; i++){
			if(strcmp(virtual_disk_sos->inodes[i].filename, "")){
				inode_t inode = virtual_disk_sos->inodes[i];
				printf("\nInode [%d]\n",i);
      	printf("Nom:%s\nTaille:%d\nUID:%d\nDroits du createur:%d\nDroits des autres:%d\nDate de creation:%s\nDerniere modification:%s\nNombre de blocs:%d\nPremier octet:%d\n"
		  		, inode.filename, inode.size, inode.uid, inode.uright, inode.oright, inode.ctimestamp
				, inode.mtimestamp, inode.nblock, inode.first_byte);
     	}
		}
	}
}
///////////////////////CAT
///\brief affiche à l’écran le contenu d’un fichier si l’utilisateur a les droits
///\param char* fichier: nom du fichier a afficher
void cat(char* fichier){
	file_t* file=malloc(sizeof(file_t));
	if (in_inodes_table(fichier)!=-1){
		if (virtual_disk_sos->inodes[in_inodes_table(fichier)].oright<Rw || virtual_disk_sos->inodes[in_inodes_table(fichier)].uright<Rw){
			fprintf(stderr,"Pas de droits en lecture pour ce fichier\nImpossible de l'afficher\n");
			return;
		}
		read_file(fichier,file);
		if (file->size>0){
			printf("Nom du fichier:%s\n%s\n", fichier, file->data);
		}
	}else{
		fprintf(stderr,"Aucun fichier trouvé\n");
	}
	free(file);
}
//////////////////////RM
///\brief supprime un fichier du système si l’utilisateur a les droits
///\param char* fichier: nom du fichier à supprimer
void rm(char* fichier){
	if (in_inodes_table(fichier)!=-1){
		if (virtual_disk_sos->inodes[in_inodes_table(fichier)].uright!=RW || virtual_disk_sos->inodes[in_inodes_table(fichier)].uright!=RW){
			fprintf(stderr,"Pas de droits de suppresion pour ce fichier\nImpossible de le supprimer\n");
			return;
		}else{
			delete_file(fichier);
			printf("Fichier supprimé avec succés\n");
		}
	}else{
		fprintf(stderr,"Aucun fichier trouvé\n");
	}
}
///////////////////////////CR
///\brief crée un nouveau fichier sur le système, le propriétaire est l’utilisateur
///\param char *fichier: nom du nouveau fichier
void cr(char *fichier){
  init_inode(fichier, 0, virtual_disk_sos->super_block.first_free_byte);
}
///////////////////////EDIT
///\brief edite un fichier pour modifier son contenu si l’utilisateur a les droits
///\param char* fichier: nom du fichier à editer
void edit(char *fichier){
	file_t* file=malloc(sizeof(file_t));

	if (in_inodes_table(fichier)!=-1){
		if (virtual_disk_sos->inodes[in_inodes_table(fichier)].uright!=rW || virtual_disk_sos->inodes[in_inodes_table(fichier)].uright!=RW){
			fprintf(stderr,"Pas de droits d'édition pour ce fichier\nImpossible de l'éditer\n");
			return;
		}
		//Nouveau fichier sur lequel on editera
		FILE* nvFich=fopen("edition.txt","wt");
		if (nvFich==NULL){
			fprintf(stderr,"Erreur de creation du fichier d'édition\n");
			return;
		}
		//Copie du fichier de base sur le fichier d'édition
		read_file(fichier,file);
		if (file->size>0){
			fprintf(nvFich,"%s",file->data);
			fclose(nvFich);
		}
		//Edition sur le fichier d'édition
		if (system("nano edition.txt")!=0){
			fprintf(stderr,"Erreur d'édition\n");
			return;
		}
		//Lecture du fichier d'édition
		FILE* fichEdit=fopen("edition.txt","rt");
		if (fichEdit==NULL){
			perror("edition.txt");
			return;
		}
		//Vérification de la taille du fichier d'édition
		fseek(fichEdit,0,SEEK_END);
		uint taille=ftell(fichEdit);
		if (taille>MAX_FILE_SIZE){
			fprintf(stderr,"Taille du fichier d'édition>Taille du fichier destination\n");
			remove("edition.txt");
			return;
		}
		//Copie du fichier d'edition vers le fichier destination
		fseek(fichEdit,0,SEEK_SET);
		if (fgets((char*)file->data,taille,fichEdit)==NULL){
			fprintf(stderr,"Impossible de lire le fichier\n");
			return;
		}
		file->data[taille]='\0';
		file->size=taille;
		write_file(fichier,file);
		fclose(fichEdit);
		remove("edition.txt");
	}else{
		fprintf(stderr,"Aucun fichier trouvé\n");
	}
	free(file);
}
/////////////////////LOAD
///\brief copie le contenu d’un fichier du systeme hote sur le systeme avec le meme nom
///\param char* fichier: nom du fichier à copier
void load(char* fichier){
  if (load_file_from_host(fichier)==0){
    printf("La copie a été effectuée avec succés\n");
  }else{
  	fprintf(stderr,"La copie n'a pas pu être effectuée\n");
  }
}
//////////////////STORE
///\brief copie le contenu d’un fichier du systeme sur hote avec le meme nom
///\param char* fichier: nom du fichier à copier
void store(char* fichier){
  if (store_file_to_host(fichier)==0){
    printf("La copie a été effectuée avec succés\n");
  }else{
  	fprintf(stderr,"La copie n'a pas pu être effectuée\n");
  }
}
///////////////////CHOWN
///\brief change le propriétaire d’un fichier s'il a les droits
///\param char* fichier: nom du fichier qui va changer de proprietaire
///\param* login: login du nouveau proprietaire
void chown_(char* fichier, char* login){
	int id=0;
	if (virtual_disk_sos->inodes[in_inodes_table(fichier)].uright!=RW){
			fprintf(stderr,"Vous n'avez pas les droits pour changer le propriétaire\n");
			return;
		}
	while(virtual_disk_sos->super_block.number_of_users>id && strcmp(login,virtual_disk_sos->users_table[id].login)){
		id++;
	}
	if (in_inodes_table(fichier)!=-1){
		virtual_disk_sos->inodes[in_inodes_table(fichier)].uid=id;
		printf("Proprietaire changé avec succés\n");
	}
}
//////////////CHMOD
///\brief change les droits d’un fichier pour tous les autres utilisateurs
///\param char* fichier: nom du fichier dont les droits seront modifiés
///\param char* mode: nouveaux droits du fichier
void chmod_(char* fichier,char* mode){
	int index=in_inodes_table(fichier);
	if(index!=-1){
		if (virtual_disk_sos->inodes[in_inodes_table(fichier)].uright!=RW){
			fprintf(stderr,"Vous n'avez pas les droits pour changer les droits de ce fichier\n");
			return;
		}
		if (!strcmp(mode,"rw")){
			virtual_disk_sos->inodes[index].oright = rw;
		}else if (!strcmp(mode,"rW")){
			virtual_disk_sos->inodes[index].oright = rW;
		}else if (!strcmp(mode,"Rw")){
			virtual_disk_sos->inodes[index].oright = Rw;
		}else if (!strcmp(mode,"RW")){
			virtual_disk_sos->inodes[index].oright = RW;
		}else{
			fprintf(stderr,"Ce droit n'existe pas\n");
			return;
		}
		printf("Droits modifiés avec succés\n");
		write_inodes_table();
	}else{
		fprintf(stderr,"Aucun fichier trouvé\n");
	}
}
/////////////////LISTUSERS
///\brief Affiche la liste d'utilisateurs
void listusers(){
	for(int id=0;id<NB_USERS;id++){
		if(strcmp(virtual_disk_sos->users_table[id].login,"")){
			printf("UID:%d Login:%s\n", id, virtual_disk_sos->users_table[id].login);
		}
	}
}
///////////////////////ADDUSER
///\brief Ajouter un utilisateur.
///\brief La commande demande le login et creer un mot de passe.
void adduser(){
	char login[FILENAME_MAX_SIZE];
	char hashRes[SHA256_BLOCK_SIZE*2+1];
	char pwd[FILENAME_MAX_SIZE];
	if (NB_USERS>virtual_disk_sos->super_block.number_of_users){
		printf("Login: ");
		if (scanf("%s",login)==EOF){
			fprintf(stderr,"Impossible d'ajouter l'utilisateur\n");
			return;
		}/*
		printf("\nMot de passe: ");
		if (scanf("%s",pwd)==EOF){
			fprintf(stderr,"Impossible d'ajouter l'utilisateur\n");
			return;
		}*/
	sha256ofString((BYTE*)pwd,hashRes);
	add_user(login); //FONCTION couche3 regarder si ça marche
	}
}
//////////COMMANDES
///\brief Verification de la commande à effectuer
///\param cmd_t *cmd: commande à effectuer
///\return 0 si pas d'erreurs, sinon 1
int executerCommande(cmd_t *cmd){
	if (cmd->nbArgs==0){
		fprintf(stderr,"Aucun argument");
		return 1;
	}
	if (!strcmp(cmd->tabArgs[0],"ls") && cmd->nbArgs==1){
		ls("ls");
	}
	else if (!strcmp(cmd->tabArgs[0],"ls") &&!strcmp(cmd->tabArgs[1],"-l") && cmd->nbArgs==2){
		ls("lsl");
	}
	else if (!strcmp(cmd->tabArgs[0], "cat") && cmd->nbArgs==2){
    	cat(cmd->tabArgs[1]);
  	}
  	else if (!strcmp(cmd->tabArgs[0], "rm") && cmd->nbArgs==2){
  		rm(cmd->tabArgs[1]);
  	}
	else if (!strcmp(cmd->tabArgs[0],"cr") && cmd->nbArgs==2){
		cr(cmd->tabArgs[1]);
	}
	else if (!strcmp(cmd->tabArgs[0],"edit") && cmd->nbArgs==2){
		edit(cmd->tabArgs[1]);
	}
	else if (!strcmp(cmd->tabArgs[0],"load") && cmd->nbArgs==2){
		load(cmd->tabArgs[1]);
	}
	else if (!strcmp(cmd->tabArgs[0],"store") && cmd->nbArgs==2){
		store(cmd->tabArgs[1]);
	}
	else if (!strcmp(cmd->tabArgs[0],"chown") && cmd->nbArgs==3){
		chown_(cmd->tabArgs[1],cmd->tabArgs[2]);
	}
	else if (!strcmp(cmd->tabArgs[0],"chmod") && cmd->nbArgs==3){
		chmod_(cmd->tabArgs[1],cmd->tabArgs[2]);
	}
	else if (!strcmp(cmd->tabArgs[0],"listusers") && cmd->nbArgs==1){
		listusers();
	}
	else if (!strcmp(cmd->tabArgs[0],"quit") && cmd->nbArgs==1){
		printf("Arret du programme");
		return 1;
	}
	else if (!strcmp(cmd->tabArgs[0],"adduser") && cmd->nbArgs==1){
		adduser();
	}
	else{
		printf("Commande saisie n'a pas pu etre executée, reesayez\n");
	}
  return 0;
}
///////////LIBERER LA MEMOIRE DE LA COMMANDE
///\brief Libération de la mémoire allouée pour la commande
///\param cmd_t *cmd: commande à liberer
void libererCommande(cmd_t *cmd){
  for(int i=0; i<cmd->nbArgs; i++){
    free(cmd->tabArgs[i]);
  }
  free(cmd->tabArgs);
}
//////////////LOGIN
///\brief Demande de login à l'utilisateur
///\return int: 0 si pas d'erreurs, 1 sinon
int login(){
	int nbEssais=3;
	char pwd[FILENAME_MAX_SIZE];
	printf("Login: ");
	while(nbEssais!=0){
		if (fgets(pwd,FILENAME_MAX_SIZE,stdin)==NULL){
			fprintf(stderr,"Mot de passe invalide, réessayez\n");
			return 1;
		}
		if (verify_user(pwd)){
			printf("Login effectué avec succés\n");
			return 0;
		}
		nbEssais--;
		if (nbEssais>0){
			printf("Il vous reste %d essais\n",nbEssais);
		}
	}
	printf("Nombre maximum d'essais atteints\n");
	return 1;
}
///////////////CONSOLE
///\brief Console à afficher dans le terminal
///\return int: 0 si pas d'erreurs, sinon 1
int console(void){
  cmd_t *cmd;
  char *login="root";
  int quit=0;
  char *commande=malloc(sizeof(char)*FILENAME_MAX_SIZE);
  while(!quit){
    printf("Utilisateur [%s] Quel est votre commande: ",login);
    if (fgets(commande, FILENAME_MAX_SIZE, stdin)==NULL) {
			fprintf(stderr, "Erreur lors de la saisie de votre commande\n");
			return 1;
		}
    commande[strcspn(commande, "\n")]=0;
    cmd=creerCommande(commande);
    quit=executerCommande(cmd);
    libererCommande(cmd);
    free(cmd);
  }
  free(commande);
  return 0;
}
