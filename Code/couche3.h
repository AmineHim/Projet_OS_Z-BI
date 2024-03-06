//////////////////////////////////////////
///\file couche3.h                       /
///\author groupe Z-BI                   /
///\date 03/2022                         /
///\brief header de la couche 3          /
//////////////////////////////////////////



#ifndef __COUCHE3_H__

#define __COUCHE3_H__
#include "sha256_utils.h"
#include "sos_defines.h"
#include "stdio.h"
#include "sha256.h"
#define ROOT_UID 0


int init_user_table();

int add_user(char login[FILENAME_MAX_SIZE]);

void sha256ofString(BYTE * str,char hashRes[SHA256_BLOCK_SIZE*2 + 1]);

int change_current_user(char* login);

int verify_user(char* pwd);

#endif