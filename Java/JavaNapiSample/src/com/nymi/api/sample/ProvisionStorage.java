package com.nymi.api.sample;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class ProvisionStorage {

	String filename;
	
	ProvisionStorage (String fn) {
		filename = fn;
	}
	
	String read() {
		try {
			BufferedReader br = new BufferedReader(new FileReader(filename));
			String provisions = br.readLine();
			br.close();
			if (provisions.isEmpty()) return "{}";
			return provisions;
		}
		catch (Exception e) {
			return "{}";
		}
	}
	
	boolean write(String provisions) {
		try {
			BufferedWriter bw = new BufferedWriter(new FileWriter(filename));
			bw.write(provisions);
			bw.close();
			return true;
		}
		catch (IOException e) {
			System.out.println("ERROR: Unable to writing provision file: " + e.getLocalizedMessage());
			return false;
		}
	}
}
