package disque;

import java.nio.ByteBuffer;

import convertisseur.ByteEnInt;

public class SuperBlock {
	//inistialisation du superblock
	private int nbOfFiles;
	private int nbOfUsers;
	private int nbBlocksUsed;
	private int firstFreeByte;
	
	public SuperBlock(byte[] octet) {
		//encapsulation du tableau d'octet existant
		ByteBuffer byteBuff = ByteBuffer.wrap(octet);
		
		byte[] nbOfFile = new byte[4];
		byte[] nbOfUser = new byte[4];
		byte[] nbOfBlockU = new byte[4];
		byte[] firstFreeB = new byte[4];
		
		
		byteBuff.get(firstFreeB,0,firstFreeB.length);
		byteBuff.get(nbOfBlockU,0,nbOfBlockU.length);
		byteBuff.get(nbOfUser,0,nbOfUser.length);
		byteBuff.get(nbOfFile,0,nbOfFile.length);
		
		
		this.firstFreeByte = ByteEnInt.transfoByteEnInt(firstFreeB);
		this.nbBlocksUsed = ByteEnInt.transfoByteEnInt(nbOfBlockU);
		this.nbOfUsers = ByteEnInt.transfoByteEnInt(nbOfUser);
		this.nbOfFiles = ByteEnInt.transfoByteEnInt(nbOfFile);
		
		
			
	}

	public int getNbOfFiles() {
		return nbOfFiles;
	}

	public void setNbOfFiles(int nbOfFiles) {
		this.nbOfFiles = nbOfFiles;
	}

	public int getNbOfUsers() {
		return nbOfUsers;
	}

	public void setNbOfUsers(int nbOfUsers) {
		this.nbOfUsers = nbOfUsers;
	}

	public int getNbBlocksUsed() {
		return nbBlocksUsed;
	}

	public void setNbBlocksUsed(int nbBlocksUsed) {
		this.nbBlocksUsed = nbBlocksUsed;
	}

	public int getFirstFreeByte() {
		return firstFreeByte;
	}

	public void setFirstFreeByte(int firstFreeByte) {
		this.firstFreeByte = firstFreeByte;
	}
}
