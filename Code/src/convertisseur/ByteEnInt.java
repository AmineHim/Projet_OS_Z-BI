package convertisseur;

public class ByteEnInt {
	public static int transfoByteEnInt(byte[] octet) {
		
		int value = 0;
		for (byte b : octet) {
		    value = (value << 8) + (b & 0xFF);
		}
		return value;
	}
}
