package disque;

import java.nio.ByteBuffer;

import convertisseur.ByteEnInt;
import convertisseur.ByteEnString;

public class Inodes {
	
	//inistialisation inodes
	private String filename;
	private int size; 
	private int uid; 
	private int uright; 
	private int oright;
	private String ctimestamp;
	private String mtimestamp;
	private int nblock; 	  
	private int firstbyte;
	
	public Inodes(byte[] octet) {
		//encapsulation du tableau d'octet existant
		ByteBuffer byteBuff = ByteBuffer.wrap(octet);
		
		byte[] FileN = new byte[32];
		byte[] Siz = new byte[4];
		byte[] id = new byte[4];
		byte[] Uright = new byte[4];
		byte[] Oright = new byte[4];
		byte[] ctstamp = new byte[26];
		byte[] mtstamp = new byte[26];
		byte[] nbBlock = new byte[4];
		byte[] FirstB = new byte[4];
		
		byteBuff.get(FirstB, 0, FirstB.length);
		byteBuff.get(nbBlock, 0, nbBlock.length);
		byteBuff.get(mtstamp, 0, mtstamp.length);
		byteBuff.get(ctstamp, 0, ctstamp.length);
		byteBuff.get(Oright, 0, Oright.length);
		byteBuff.get(Uright, 0, Uright.length);
		byteBuff.get(id, 0, id.length);
		byteBuff.get(Siz, 0, Siz.length);
		byteBuff.get(FileN, 0, FileN.length);

		this.firstbyte = ByteEnInt.transfoByteEnInt(FirstB);
		this.nblock = ByteEnInt.transfoByteEnInt(nbBlock);
		this.mtimestamp = ByteEnString.transfoByteEnString(mtstamp);
		this.ctimestamp = ByteEnString.transfoByteEnString(ctstamp);
		this.oright = ByteEnInt.transfoByteEnInt(Oright);
		this.uright = ByteEnInt.transfoByteEnInt(Uright);
		this.uid = ByteEnInt.transfoByteEnInt(id);
		this.size = ByteEnInt.transfoByteEnInt(Siz);
		this.filename = ByteEnString.transfoByteEnString(FileN);
		
	}

	public String getFilename() {
		return filename;
	}

	public void setFilename(String filename) {
		this.filename = filename;
	}

	public int getSize() {
		return size;
	}

	public void setSize(int size) {
		this.size = size;
	}

	public int getUid() {
		return uid;
	}

	public void setUid(int uid) {
		this.uid = uid;
	}

	public int getUright() {
		return uright;
	}

	public void setUright(int uright) {
		this.uright = uright;
	}

	public int getOright() {
		return oright;
	}

	public void setOright(int oright) {
		this.oright = oright;
	}

	public String getCtimestamp() {
		return ctimestamp;
	}

	public void setCtimestamp(String ctimestamp) {
		this.ctimestamp = ctimestamp;
	}

	public String getMtimestamp() {
		return mtimestamp;
	}

	public void setMtimestamp(String mtimestamp) {
		this.mtimestamp = mtimestamp;
	}

	public int getNblock() {
		return nblock;
	}

	public void setNblock(int nblock) {
		this.nblock = nblock;
	}

	public int getFirstbyte() {
		return firstbyte;
	}

	public void setFirstbyte(int firstbyte) {
		this.firstbyte = firstbyte;
	}
	
	
	
	
}
