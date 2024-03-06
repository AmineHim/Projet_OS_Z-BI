package convertisseur;

import java.nio.charset.StandardCharsets;

public class ByteEnString {
	public static String transfoByteEnString(byte[] octet) {
		return new String(octet,StandardCharsets.UTF_8);
	}
}
