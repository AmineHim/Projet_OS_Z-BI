//////////////////////////////////////////
///\file couche4.c                       /
///\author groupe Z-BI                   /
///\date 05/2022                         /
///\brief implementation de la couche 4  /
//////////////////////////////////////////

#include "couche4.h"
#include "sos_defines.h"

///\brief recherche si filename est dans la table d'inode
///\param char* filename: fichier à rechercher
///\return int : index du fichier -1 si absent
int in_inodes_table(char *filename) {
	for (int i = 0; i < INODE_TABLE_SIZE; i++)
		if (strcmp(virtual_disk_sos->inodes[i].filename, filename)==0)
			return i; 
	return -1; 
}


///\brief écrit un fichier dans la table d'inode
///\param char* filename: nom du fichier dans la table d'inode
///\param file_t* file: fichier à écrire
///\return int : index du fichier -1 si absent
int write_file(char *filename, file_t *file) {
	if (read_inodes_table()) {
		int index = in_inodes_table(filename);
		if (index >= 0) {
			inode_t *inode=malloc(sizeof(inode_t)); 

            memcpy(inode,&virtual_disk_sos->inodes[index],sizeof(inode_t)); 

			time_t timing = time(NULL);
			strcpy(inode->mtimestamp, ctime(&timing));

			if (file -> size <= inode->size) { // Plus petit = màj de l'inode
				// Modification de l'inode
				virtual_disk_sos->inodes[index].size = file -> size;
				virtual_disk_sos->inodes[index].nblock = compute_nblock(file -> size);
				strcpy(virtual_disk_sos->inodes[index].mtimestamp, inode->mtimestamp);
			} else { 
                
				if (!init_inode(filename, file -> size, virtual_disk_sos->super_block.first_free_byte)) {
					fprintf(stderr, "Erreur lors de la création d'une nouvelle inode\n");
					free(inode);
					return 1;
				}
                delete_inode(index);

				index = in_inodes_table(filename); 
				memcpy(&virtual_disk_sos->inodes[index],inode,inode->size);
			}

			fseek(virtual_disk_sos->storage, virtual_disk_sos->inodes[index].first_byte, SEEK_SET);
			fwrite(file -> data, file -> size, 1, virtual_disk_sos->storage);

			if (!write_inodes_table()) {
				fprintf(stderr, "problem lors de la mise à jour de la table\n");
				free(inode);
				return 2;
			}
		}
		else {
			if (!init_inode(filename, file -> size, virtual_disk_sos->super_block.first_free_byte)) {
				fprintf(stderr, "La table d'inode n'a pas pu être crée\n");
				return 3;
			}
		}
	} 
    else {
	    fprintf(stderr, "La table d'inode n'a pas pu être lue \n");
		return 4;
	}
	return 0;
}

///\brief lit un fichier de la table d'inode et le stocke 
///\param char* filename: nom du fichier à lire dans la table d'inode
///\param file_t* file: pointeur du fichier o`u la lecture est stockée
///\return int : 0 si réussi positif sinon
int read_file(char *filename, file_t *file) {
	if (read_inodes_table()) {
		int index = in_inodes_table(filename);
		if (index >= 0) {
			fseek(virtual_disk_sos->storage, virtual_disk_sos->inodes[index].first_byte, SEEK_SET);
			file->size = virtual_disk_sos->inodes[index].size;
			if (!file->size) return 1;//Cas fichier vide
			if (fread(file -> data, file->size, 1, virtual_disk_sos->storage) != 1) {
				fprintf(stderr, "Fichier illisible\n");
				return 1;
			}
		
		} else {
			fprintf(stderr, "Fichier absent de la table\n");
			return 2;
		}
	}
	else {
		fprintf(stderr, "Erreur lors de la lecture de la table d'inodes\n");
		return 3;
	}

	return 0;
}

///\brief effece un fichier dans la table d'inode
///\param char* filename: nom du fichier dans la table d'inode
///\return int : 0 si fichier effacé -1 si absent positif si erreur
int delete_file(char *filename) {
	if (read_inodes_table()) {
		int index = in_inodes_table(filename);
		if (index != -1) {
			delete_inode(index);
		} else {
			fprintf(stderr, "%s absent de la table des inodes\n",filename);
			return -1;
		}
	}
	else {
		fprintf(stderr, "Erreur lors de la lecture de la table d'inodes\n");
		return 1;
	}

	return 0;
}

///\brief charge un fichier dans la table d'inode depuis l'hôte de l'os
///\param char* filename: nom du fichier dans le système hôte 
///\param file_t* file: fichier à écrire
///\return int : 0 si succès positif en cas d'erreur
int load_file_from_host(char *filename) {

	FILE *filept;
	filept = fopen(filename, "r");
	if (!filept){
	    fprintf(stderr,"Erreur lors de l'ouverture de %s\n",filename);
		return 1;
	}

	file_t file;
	struct stat infos;

	if (stat(filename,&infos) == -1) {
		fprintf(stderr,"Erreur lors de la lecture des stats de %s\n",filename);
		return 2;
	}

	file.size = infos.st_size;
	if (file.size > MAX_FILE_SIZE) file.size = MAX_FILE_SIZE;

	if (fread(file.data, file.size, 1, filept) != 1) {
		fprintf(stderr, "Erreur lors de la lecture des données de %s\n",filename);
		return 3;
	}

	if (!write_file(filename, &file)) {
		fprintf(stderr, "Erreur lors de l'écriture du fichier \n");
		return 4;
	}

	return 0;
}

///\brief écrit un fichier chez l'hôte
///\param char* filename: nom du fichier dans la table d'inode
///\return int : index du fichier -1 si absent
int store_file_to_host(char *filename) {
	FILE *newFile = fopen(filename, "w");

	file_t file;
	if (!read_file(filename, &file)) {
		fprintf(stderr,"Erreur lors de la lecture de %s\n",filename);
		return 1;
	}

	if (fwrite(file.data, 1, file.size, newFile) == 0) {
		fprintf(stderr,"Erreur lors de l'écriture de %s\n",filename);
		return 2;
	}
	return 0;
}
