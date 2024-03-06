package disque;

import java.nio.ByteBuffer;

public class Catalogue {
	public void verifCatalogue(byte[] octet) {
		///lire les bloc de 5 à 274;
		ByteBuffer byteBuff = ByteBuffer.wrap(octet);
		for(int i = 5; i<=274;i++) {
			byte[] catalog = new byte[i];
		}
	}
}
