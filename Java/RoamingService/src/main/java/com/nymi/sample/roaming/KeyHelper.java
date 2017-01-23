package com.nymi.sample.roaming;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.math.BigInteger;
import java.security.KeyPair;
import java.security.PublicKey;
import java.security.Security;
import java.security.interfaces.ECPublicKey;
import java.util.Arrays;

import javax.xml.bind.DatatypeConverter;

import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.bouncycastle.openssl.PEMKeyPair; 
import org.bouncycastle.openssl.PEMParser;
import org.bouncycastle.openssl.jcajce.JcaPEMKeyConverter; 

public class KeyHelper {

	private KeyPair _keypair = null;
	
	public KeyHelper() {
	    try {
	        Security.addProvider(new BouncyCastleProvider());
	        ClassLoader classLoader = Thread.currentThread().getContextClassLoader();
	        InputStream pemstream = classLoader.getResourceAsStream("roaming.pem");
	        if (pemstream == null)
	        	throw new IOException("Loading \"roaming.pem\" failed.");
	        Reader pemreader = new InputStreamReader(pemstream);
	        PEMParser pemParser = new PEMParser(pemreader);
	        JcaPEMKeyConverter converter = new JcaPEMKeyConverter().setProvider("BC");
	        Object object = pemParser.readObject();
	        while (object.getClass().getTypeName() != "org.bouncycastle.openssl.PEMKeyPair") {
	        	object = pemParser.readObject();
	        }
	        _keypair = converter.getKeyPair((PEMKeyPair) object);
	        pemParser.close();
	    } catch (Exception e) {
	        System.err.println("Could not load key pair from roaming.pem: " + e.getLocalizedMessage());
	    }
	    if (_keypair == null)
	    	System.err.println("FATAL ERROR: Could not load key pair from roaming.pem.  System will not function without it.");
	    else
	    	System.err.println("Successfully loaded key from roaming.pem");
	}

	public KeyPair getKeyPair () { return _keypair; }
	
	public String getEncodedPubKey () {
		if (_keypair == null)
			return "";
		PublicKey binaryKey = _keypair.getPublic();
		return encodePublicKey(binaryKey);
	}
	
	public static String encodePublicKey (PublicKey binaryKey) {
		if (binaryKey == null)
			return "";
		ECPublicKey key = (ECPublicKey) binaryKey;
		BigInteger keyX = key.getW().getAffineX();
		BigInteger keyY = key.getW().getAffineY();
		
		byte[] xComponent = toUnsignedByteArray(keyX);
		byte[] yComponent = toUnsignedByteArray(keyY);
		
//		System.err.println("Encoding " + (xComponent.length + yComponent.length) + " byte public key. X size == " + xComponent.length + " and Y size == " + yComponent.length);
//		System.err.println("Hex encoded X == " + DatatypeConverter.printHexBinary(xComponent));
//		System.err.println("Hex encoded Y == " + DatatypeConverter.printHexBinary(yComponent));
		
		byte[] complete = new byte[xComponent.length + yComponent.length];
		System.arraycopy(xComponent, 0, complete, 0, xComponent.length);
		System.arraycopy(yComponent, 0, complete, xComponent.length, yComponent.length);
		
		return DatatypeConverter.printHexBinary(complete);
	}

    public static byte[] toUnsignedByteArray(BigInteger value) throws IllegalArgumentException {
        byte[] signedValue = value.toByteArray();
        if(signedValue[0] != 0x00) {
            throw new IllegalArgumentException("value must be a positive integer");
        }
        return Arrays.copyOfRange(signedValue, 1, signedValue.length);
    }

}
