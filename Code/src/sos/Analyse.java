package sos;

import java.io.PrintStream;

import disque.Disk;
import disque.Inodes;
import frame.Fenetre;

public class Analyse {

    public static void main(String[] args) {
    	
    	Fenetre window = new Fenetre();
    	window.window();
    	
    	
        System.out.println("Analyse de l'os : ");
        Disk disk = new Disk("Code/Dossier_Disk/Disk/d0");
        System.out.println("Le nombre d'utilisateurs : "+disk.getsBlock().getNbOfUsers());
        System.out.println("Le nombre de fichiers : "+disk.getsBlock().getNbOfFiles());
        System.out.println("Le nombre de blocs utilisé : "+disk.getsBlock().getNbBlocksUsed());
        PrintStream inode = System.out;
        Inodes[] inode1 = disk.getInode();
        inode.println("Nom de fichier :"+ inode1[0].getFilename());

    }

}