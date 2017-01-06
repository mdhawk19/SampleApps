package com.nymi.sample.roamingnea;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.HttpClientBuilder;
import org.json.JSONObject;

public class ServiceConnector {
	HttpClient client = null;
	String baseUrl = "";
	
	public ServiceConnector (String base) {
		client = HttpClientBuilder.create().build();
		baseUrl = base;
	}
	
	public String getPublicKey () {
		String requestUrl = baseUrl + "/provision/getpubkey";
		System.out.println("Invoking URL " + requestUrl);
		
		HttpGet request = new HttpGet(requestUrl);
		HttpResponse response;
		try {
			response = client.execute(request);
		} catch (IOException e) {
			System.err.println("Failed to send request for roaming service public key. Error: " + e.getLocalizedMessage());
			return "";
		}

		System.out.println("Response Code : "
		                + response.getStatusLine().getStatusCode());

		BufferedReader rd;
		try {
			rd = new BufferedReader(
				new InputStreamReader(response.getEntity().getContent()));
		} catch (UnsupportedOperationException | IOException e) {
			System.err.println("Failed to send request for roaming service public key. Error: " + e.getLocalizedMessage());
			return "";
		}

		StringBuffer result = new StringBuffer();
		String line = "";
		try {
			while ((line = rd.readLine()) != null) {
				result.append(line);
			}
		} catch (IOException e) {
			System.err.println("Failed to read response to request for roaming service public key. Error: " + e.getLocalizedMessage());
			return "";
		}
		
		JSONObject jsonResult = new JSONObject (result.toString());
		if (jsonResult.has("successful"))
			if (jsonResult.getBoolean("successful"))
				if (jsonResult.has("response"))
					if (jsonResult.getJSONObject("response").has("partnerPublicKey"))
						return jsonResult.getJSONObject("response").getString("partnerPublicKey");

		return "";
	}
	
	public boolean createUser(String userid, String pubkey, String keyId) {
		String requestUrl = baseUrl + "/provision/newuser";
		System.out.println("Invoking URL " + requestUrl);

		JSONObject request = new JSONObject();
		request.put("userID", userid);
		request.put("nbPublicKey", pubkey);
		request.put("nbPublicKeyID", keyId);
		JSONObject reqJson = new JSONObject();
		reqJson.put("request", request);
		
		HttpPost post = new HttpPost(requestUrl);
		try {
			post.setHeader("Content-Type", "application/x-www-form-urlencoded");
			post.setEntity(new StringEntity(reqJson.toString()));
		} catch (UnsupportedEncodingException e) {
			System.err.println("Failed to encode request to create new user. Error: " + e.getLocalizedMessage());
			return false;
		}
		
		HttpResponse response;
		try {
			response = client.execute(post);
		} catch (IOException e) {
			System.err.println("Failed to execute request to create new user. Error: " + e.getLocalizedMessage());
			return false;
		}
		BufferedReader rd;
		try {
			rd = new BufferedReader(new InputStreamReader(response.getEntity().getContent()));
		} catch (UnsupportedOperationException | IOException e) {
			System.err.println("Failed to read response to request to create new user. Error: " + e.getLocalizedMessage());
			return false;
		}

		StringBuffer result = new StringBuffer();
		String line = "";
		try {
			while ((line = rd.readLine()) != null) {
				result.append(line);
			}
		} catch (IOException e) {
			System.err.println("Failed to read response to request to create new user. Error: " + e.getLocalizedMessage());
			return false;
		}
		
		JSONObject jsonResult = new JSONObject(result.toString());
		if (jsonResult.has("successful"))
			return jsonResult.getBoolean("successful");
		
		System.err.println("Received malformed response from server: " + jsonResult.toString());
		return false;
	}

	public boolean deleteUser(String userid) {
		String requestUrl = baseUrl + "/provision/deleteuser";
		System.out.println("Invoking URL " + requestUrl);

		JSONObject request = new JSONObject();
		request.put("userID", userid);
		JSONObject reqJson = new JSONObject();
		reqJson.put("request", request);
		
		HttpPost post = new HttpPost(requestUrl);
		try {
			post.setHeader("Content-Type", "application/x-www-form-urlencoded");
			post.setEntity(new StringEntity(reqJson.toString()));
		} catch (UnsupportedEncodingException e) {
			System.err.println("Failed to encode request to delete user. Error: " + e.getLocalizedMessage());
			return false;
		}
		
		HttpResponse response;
		try {
			response = client.execute(post);
		} catch (IOException e) {
			System.err.println("Failed to execute request to delete user. Error: " + e.getLocalizedMessage());
			return false;
		}
		BufferedReader rd;
		try {
			rd = new BufferedReader(new InputStreamReader(response.getEntity().getContent()));
		} catch (UnsupportedOperationException | IOException e) {
			System.err.println("Failed to read response to request to delete user. Error: " + e.getLocalizedMessage());
			return false;
		}

		StringBuffer result = new StringBuffer();
		String line = "";
		try {
			while ((line = rd.readLine()) != null) {
				result.append(line);
			}
		} catch (IOException e) {
			System.err.println("Failed to read response to request to delete user. Error: " + e.getLocalizedMessage());
			return false;
		}
		
		JSONObject jsonResult = new JSONObject(result.toString());
		if (jsonResult.has("successful"))
			return jsonResult.getBoolean("successful");
		
		System.err.println("Received malformed response from server: " + jsonResult.toString());
		return false;
	}

	public JSONObject startAuth (String nonce, String exchange) {
		String requestUrl = baseUrl + "/sign";
		System.out.println("Invoking URL " + requestUrl);

		JSONObject request = new JSONObject();
		request.put("nymibandNonce", nonce);
		request.put("exchange", exchange);
		JSONObject reqJson = new JSONObject();
		reqJson.put("request", request);
		
		HttpPost post = new HttpPost(requestUrl);
		try {
			post.setHeader("Content-Type", "application/x-www-form-urlencoded");
			post.setEntity(new StringEntity(reqJson.toString()));
		} catch (UnsupportedEncodingException e) {
			System.err.println("Failed to encode request to start roaming authentication. Error: " + e.getLocalizedMessage());
			return (JSONObject) null;
		}
		
		HttpResponse response;
		try {
			response = client.execute(post);
		} catch (IOException e) {
			System.err.println("Failed to execute request to start roaming authentication. Error: " + e.getLocalizedMessage());
			return (JSONObject) null;
		}
		BufferedReader rd;
		try {
			rd = new BufferedReader(new InputStreamReader(response.getEntity().getContent()));
		} catch (UnsupportedOperationException | IOException e) {
			System.err.println("Failed to read response to request to start roaming authentication. Error: " + e.getLocalizedMessage());
			return (JSONObject) null;
		}

		StringBuffer result = new StringBuffer();
		String line = "";
		try {
			while ((line = rd.readLine()) != null) {
				result.append(line);
			}
		} catch (IOException e) {
			System.err.println("Failed to read response to request to start roaming authentication. Error: " + e.getLocalizedMessage());
			return (JSONObject) null;
		}
		
		System.out.println("Got response for request to start roaming authentication: " + result.toString());
		JSONObject jsonResult = new JSONObject(result.toString());
		if (jsonResult.has("response"))
				return jsonResult.getJSONObject("response");
		
		System.err.println("Received malformed response from server: " + jsonResult.toString());
		return (JSONObject) null;
	}
	
	public String completeAuth (String nonce, String exchange, String keyId) {
		String requestUrl = baseUrl + "/auth";
		System.out.println("Invoking URL " + requestUrl);

		JSONObject request = new JSONObject();
		request.put("signedNonces", nonce);
		request.put("exchange", exchange);
		request.put("nbPublicKeyID", keyId);
		JSONObject reqJson = new JSONObject();
		reqJson.put("request", request);
		
		HttpPost post = new HttpPost(requestUrl);
		try {
			post.setHeader("Content-Type", "application/x-www-form-urlencoded");
			post.setEntity(new StringEntity(reqJson.toString()));
		} catch (UnsupportedEncodingException e) {
			System.err.println("Failed to encode request to start roaming authentication. Error: " + e.getLocalizedMessage());
			return "";
		}
		
		HttpResponse response;
		try {
			response = client.execute(post);
		} catch (IOException e) {
			System.err.println("Failed to execute request to start roaming authentication. Error: " + e.getLocalizedMessage());
			return "";
		}
		BufferedReader rd;
		try {
			rd = new BufferedReader(new InputStreamReader(response.getEntity().getContent()));
		} catch (UnsupportedOperationException | IOException e) {
			System.err.println("Failed to read response to request to start roaming authentication. Error: " + e.getLocalizedMessage());
			return "";
		}

		StringBuffer result = new StringBuffer();
		String line = "";
		try {
			while ((line = rd.readLine()) != null) {
				result.append(line);
			}
		} catch (IOException e) {
			System.err.println("Failed to read response to request to start roaming authentication. Error: " + e.getLocalizedMessage());
			return "";
		}
		
		System.out.println("Got response from auth service: " + result.toString());

		JSONObject jsonResult = new JSONObject(result.toString());
		
		if (jsonResult.has("successful"))
			if (!jsonResult.getBoolean("successful"))
				return "";
			else
				return jsonResult.getString("userid");
		else {
			System.err.println("Received malformed response from server: " + jsonResult.toString());
			return "";
		}
	}

}
