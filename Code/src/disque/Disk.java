package disque;

import java.io.File;
import java.io.FileInputStream;
import java.nio.ByteBuffer;


public class Disk {
    private File fichier;
    private byte[] donnee;
    private String  chemin;
    private SuperBlock sBlock;
    private Inodes[] inode = new Inodes[10];

    public Disk(String chemin) {
        this.chemin = chemin;
        this.fichier = new File(chemin);
        this.donnee = new byte[(int)this.fichier.length()];


        try {
            FileInputStream fileInput = new FileInputStream(this.fichier);
            fileInput.read(this.donnee);
            fileInput.close();
        }catch(Exception e) {
            e.printStackTrace();
        }

        ByteBuffer byteBuff = ByteBuffer.wrap(this.donnee);
        byte [] superBlock = new byte[16];
        byteBuff.get(superBlock,0,superBlock.length);
        this.sBlock = new SuperBlock(superBlock);

        for(int i = 0; i < 10; i++) {
            byte[] b = new byte[108];
            byteBuff.get(b,0,b.length);
            this.inode[i] = new Inodes(b);
        }
    }

    public File getFichier() {
        return fichier;
    }

    public byte[] getDonnee() {
        return donnee;
    }

    public String getChemin() {
        return chemin;
    }

    public SuperBlock getsBlock() {
        return sBlock;
    }

    public Inodes[] getInode() {
        return inode;
    }

    public void setInode(Inodes[] inode) {
        this.inode = inode;
    }

}
