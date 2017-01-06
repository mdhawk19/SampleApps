package com.nymi.sample.roaming;

import java.io.IOException;
import java.math.BigInteger;
import java.security.InvalidKeyException;
import java.security.KeyPair;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.Signature;
import java.security.SignatureException;
import java.security.interfaces.ECPrivateKey;
import java.security.interfaces.ECPublicKey;
import java.util.Arrays;

import javax.xml.bind.DatatypeConverter;
import java.security.SecureRandom;

import org.bouncycastle.crypto.params.ECDomainParameters;
import org.bouncycastle.crypto.params.ECPublicKeyParameters;
import org.bouncycastle.crypto.signers.ECDSASigner;
import org.bouncycastle.jce.ECNamedCurveTable;
import org.bouncycastle.jce.spec.ECNamedCurveParameterSpec;
import org.bouncycastle.math.ec.ECCurve;
import org.bouncycastle.math.ec.ECPoint;
import org.json.JSONObject;

import sun.security.util.DerInputStream;
import sun.security.util.DerValue;


public class Signer {
	ECPublicKey	_rpPublicKey = null;
	ECPrivateKey _rpPrivateKey = null;
	String _encodedPublicKey = "";
	
	public Signer() {
	    KeyHelper kh = new KeyHelper();
	    
	    KeyPair roamingKeys = kh.getKeyPair();
	    if (roamingKeys == null) {
	    	System.err.println("Unable to load system keys");
	    	return;
	    }

	    _rpPublicKey = (ECPublicKey) roamingKeys.getPublic();
	    _rpPrivateKey = (ECPrivateKey) roamingKeys.getPrivate();
	    _encodedPublicKey = kh.getEncodedPubKey();
	}
	
	JSONObject sign(String bandNonce, String exchange) {

		if (_rpPrivateKey == null)
			return makeError("No system private key available");
		
		System.err.println("Got Nymi Band nonce: " + bandNonce);
		
		// Make sure Nymi Band nonce is the correct size
		if (bandNonce.length() != 32)
			return new JSONObject("{}");
		
		// Set up signing object
	    Signature ecdsa = null;
		try {
			ecdsa = Signature.getInstance("SHA256withECDSA");
		} catch (NoSuchAlgorithmException e) {
			return makeError("Non such algorithm SHA256withECDSA");
		}
	
	    try {
			ecdsa.initSign(_rpPrivateKey);
		} catch (InvalidKeyException e) {
			return makeError ("Invalid private key");
		}
	
	    // Generate new server nonce for this transaction
	    SecureRandom rand;
	    byte[] serverNonceBytes = new byte[32];
	    byte[] bothNonceBytes = new byte[48];

	    try {
			rand = SecureRandom.getInstance("SHA1PRNG");
		} catch (NoSuchAlgorithmException e2) {
			return makeError("No such algorithm SHA1PRNG");
		}
	    rand.nextBytes(serverNonceBytes);
	    
	    System.err.println("New server nonce: " + DatatypeConverter.printHexBinary(serverNonceBytes));
	    
	    // Concatenate nonces: bandNonce + serverNonce
	    // Then convert whole thing to a byte array for hashing/signing
	    String bothNonces = bandNonce.concat(DatatypeConverter.printHexBinary(serverNonceBytes));
	    
	    System.err.println("Combined nonces: " + bothNonces);
	    
	    bothNonceBytes = DatatypeConverter.parseHexBinary(bothNonces);
	    
	    
		// Sign nonces
		try {
			ecdsa.update(bothNonceBytes);
		} catch (SignatureException e) {
			System.err.println("Failed to initialize signature: " + e.getLocalizedMessage());
			return makeError("Failed to initialize signature: " + e.getLocalizedMessage());
		}

		byte[] hashSig = null;
		try {
			hashSig = ecdsa.sign();
		} catch (SignatureException e) {
			System.err.println("Failed to sign nonces: " + e.getLocalizedMessage());
			return makeError("Failed to sign nonces: " + e.getLocalizedMessage());
		}
		
		
		// Once everything is verified, save server nonce for later signature verification
		NonceModel nm = new NonceModel();
		
		// Hash the server nonce for later signature verification
		byte[] serverNonceHash = getSHA256hash(serverNonceBytes);
		if (serverNonceHash == null) {
			System.err.println("Unable to generate server nonce hash");
			return makeError("Unable to generate server nonce hash");
		}

		if (!nm.saveNonce(exchange, DatatypeConverter.printHexBinary(serverNonceHash)))
			return makeError("Failed to save server nonce");
		
		// Strip DER encoding from signature so we send only the variable r and s bytes to the band
	    DerInputStream derInputStream;
	    byte[] r = null,s = null;
		try {
			derInputStream = new DerInputStream(hashSig);
	      DerValue[] values = derInputStream.getSequence(2);
	      r = values[0].getPositiveBigInteger().toByteArray();
	      s = values[1].getPositiveBigInteger().toByteArray();
		} catch (IOException e) {
			System.err.println("Failed to decode  signature: " + e.getLocalizedMessage());
			return makeError("Failed to initialize signature: " + e.getLocalizedMessage());
		}

		// Pull out r and s variables and copy to a new byte array
		byte[] decodedSignature = new byte[64];
		System.arraycopy(r, r.length > 32 ? 1 : 0, decodedSignature, r.length < 32 ? 1 : 0, r.length > 32 ? 32 : r.length);
		System.arraycopy(s, s.length > 32 ? 1 : 0, decodedSignature, s.length < 32 ? 33 : 32, s.length > 32 ? 32 : s.length);
		
		System.out.println("Full Signature length: " + decodedSignature.length + " r length: " + r.length + " s length" + s.length);
	      
		String serverSignature = DatatypeConverter.printHexBinary(decodedSignature);

		System.err.println("Got signed nonces: " + serverSignature);

		// Return JSON object with everything
		JSONObject retJson = new JSONObject("{}");
	    retJson.put("serverSignature", serverSignature);
	    retJson.put("serverNonce", DatatypeConverter.printHexBinary(serverNonceBytes));
	    retJson.put("partnerPublicKey", _encodedPublicKey);
	    return retJson;
	}
	
	boolean verifySignature (String pubkey, String signedNonce, String serverNonceHash) {
		try {
			// Check the inputs are the right length
			if (pubkey.length() != 128) {
				System.err.println("Invalid public key for signature verification: incorrect size");
				return false;
			}
			
			if (signedNonce.length() != 128) {
				System.err.println("Invalid server signature for verification: incorrect size");
				return false;
			}
			
			if (serverNonceHash.length() != 64) {
				System.err.println("Invalid server nonce hash for signature verification: incorrect size");
				return false;
			}

			// Break apart the public key data, which is the concatenated values
			// of the X and Y coordinates of the key on the elliptic curve
			byte[] pubBytes = DatatypeConverter.parseHexBinary(pubkey);
			BigInteger curvex = new BigInteger (1, Arrays.copyOfRange(pubBytes, 0, 32));
			BigInteger curvey = new BigInteger (1, Arrays.copyOfRange(pubBytes, 32, 64));

			// Generate elliptic curve specs for signer from public key X and Y
			ECNamedCurveParameterSpec parameterSpec = ECNamedCurveTable.getParameterSpec("secp256r1");
		    ECDomainParameters domainParams = new ECDomainParameters(parameterSpec.getCurve(), parameterSpec.getG(), parameterSpec.getN(), parameterSpec.getH(), parameterSpec.getSeed());
	        ECCurve curve = parameterSpec.getCurve();
	        ECPoint signerPoint = curve.createPoint(curvex, curvey);	
		    ECPublicKeyParameters signerParams = new ECPublicKeyParameters(signerPoint, domainParams);

			// Break signature string into r and s variables
			byte[] signedNonceBytes = DatatypeConverter.parseHexBinary(signedNonce);
			BigInteger r = new BigInteger (1, Arrays.copyOfRange(signedNonceBytes, 0, signedNonceBytes.length/2));
			BigInteger s = new BigInteger (1, Arrays.copyOfRange(signedNonceBytes, signedNonceBytes.length/2, signedNonceBytes.length));
			
			// Finally set up ECDSA signer in verification mode and verify signature
		    ECDSASigner signer = new ECDSASigner();
		    signer.init(false, signerParams);
		    try {
		        return signer.verifySignature(DatatypeConverter.parseHexBinary(serverNonceHash), r, s);
		    } catch (NullPointerException e) {
		        System.out.println("Caught NPE inside bouncy castle: " + e.getLocalizedMessage());
		        return false;
		    }
		} catch (ClassCastException e) {
			System.err.println("Unable to verify signature: " + e.getLocalizedMessage());
			return false;
		}
	}
	
	private byte[] getSHA256hash (byte[] tohash) {
		byte[] outbuf = new byte[32];
		try {
			MessageDigest digest = MessageDigest.getInstance("SHA-256");
			outbuf = digest.digest(tohash); 
		} catch (NoSuchAlgorithmException e1) {
			e1.printStackTrace();
			return null;
		}
		return outbuf;
	}
	
	JSONObject makeError(String errMsg) {
    	JSONObject err = new JSONObject("{\"response\":\"error: signing error: " + errMsg + "\",\"successful\":false}");
    	return err;
    }

}
