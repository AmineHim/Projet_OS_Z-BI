//////////////////////////////////////////
///\file couche4.h                       /
///\author groupe Z-BI                   /
///\date 04/2022                         /
///\brief header de la couche 4          /
//////////////////////////////////////////

#ifndef __COUCHE4_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "couche1.h"
#include "couche2.h"
#include "couche3.h"

int in_inodes_table(char *filename);
int write_file(char *filename, file_t *file);
int read_file(char *filename, file_t *file);
int delete_file(char *filename);
int load_file_from_host(char *filename);
int store_file_to_host(char *filename);

#define __COUCHE4_H__

#endif