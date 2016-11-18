package com.nymi.api.wrapper;

import org.json.JSONObject;

public class GenJson {

	public static String getInit() {
	    return("{\"path\":\"init/get\",\"exchange\":\"*init*\"}");
	}

	public static String finish() {
		return("{\"path\":\"finish/run\",\"exchange\":\"*finish*\"}");
	}

	public static String startProvisioning() {
		return("{\"path\":\"provision/run/start\",\"exchange\":\"*provisioning*\"}");
	}

	public static String stopProvisioning() {
		return("{\"path\":\"provision/run/stop\",\"exchange\":\"*provisioning*\"}");
	}

	/* exchange here is expected to be non-empty, as it determines which field of info we are interested in*/
	public static String getInfo(String exchange) {
		if (exchange.equals("")) return "";
			else return("{\"path\":\"info/get\",\"exchange\":\"" + exchange + "\"}");
	}
	
	public static String acceptPattern(String pattern) {
		JSONObject json = new JSONObject();
		json.put("path","provision/pattern");
		json.put("exchange","*provisioning*");
		JSONObject jsonrequest = new JSONObject(); // sub-object with request data
		jsonrequest.put("pattern", pattern);
		jsonrequest.put("action","accept");
		json.put ("request",jsonrequest);
		return json.toString();
	}

	public static String getRandom(String pid, String exchange) {
		JSONObject json = new JSONObject();
		json.put("path","random/run");
		json.put("exchange",exchange);
		JSONObject jsonrequest = new JSONObject(); // sub-object with request data
		jsonrequest.put("pid", pid);
		json.put ("request",jsonrequest);
		return json.toString();
	}

	public static String createSymmetricKey(String pid, Boolean guarded, String exchange) {
		JSONObject json = new JSONObject();
		json.put("path","symmetricKey/run");
		json.put("exchange",exchange);
		JSONObject jsonrequest = new JSONObject(); // sub-object with request data
		jsonrequest.put("pid", pid);
		jsonrequest.put("guarded", guarded);
		json.put ("request",jsonrequest);
		return json.toString();
	}

	public static String getSymmetrickey(String pid, String exchange) {
		JSONObject json = new JSONObject();
		json.put("path","symmetricKey/get");
		json.put("exchange",exchange);
		JSONObject jsonrequest = new JSONObject(); // sub-object with request data
		jsonrequest.put("pid", pid);
		json.put ("request",jsonrequest);
		return json.toString();
	}

	public static String signMessage(String pid, String msghash, String exchange) {
		JSONObject json = new JSONObject();
		json.put("path","sign/run");
		json.put("exchange",exchange);
		JSONObject jsonrequest = new JSONObject(); // sub-object with request data
		jsonrequest.put("pid", pid);
		jsonrequest.put("hash", msghash);
		json.put ("request",jsonrequest);
		return json.toString();
	}

	public static String setTotp(String pid, String totpkey, Boolean guarded, String exchange) {
		JSONObject json = new JSONObject();
		json.put("path","totp/run");
		json.put("exchange",exchange);
		JSONObject jsonrequest = new JSONObject(); // sub-object with request data
		jsonrequest.put("key", totpkey);
		jsonrequest.put("guarded", guarded);
		jsonrequest.put("pid", pid);
		json.put ("request",jsonrequest);
		return json.toString();
	}

	public static String getTotp(String pid, String exchange) {
		JSONObject json = new JSONObject();
		json.put("path","totp/get");
		json.put("exchange",exchange);
		JSONObject jsonrequest = new JSONObject(); // sub-object with request data
		jsonrequest.put("pid", pid);
		json.put ("request",jsonrequest);
		return json.toString();
	}

	public static String notify(String pid, Boolean notifyType, String exchange) {
		JSONObject json = new JSONObject();
		json.put("path","buzz/run");
		json.put("exchange",exchange);
		JSONObject jsonrequest = new JSONObject(); // sub-object with request data
		jsonrequest.put("pid", pid);
		jsonrequest.put("buzz", notifyType);
		json.put ("request",jsonrequest);
		return json.toString();
	}

	public static String enableNotification(Boolean enable, String state) {
		JSONObject json = new JSONObject();
		json.put("path","notifications/set");
		json.put("exchange","*notifications*");
		JSONObject request = new JSONObject();
		request.put(state, enable);
		json.put("request", request);
		return json.toString();
	}

	public static String getStateNotifications(){
		return("{\"path\":\"notifications/get\",\"exchange\":\"*notifications*\"}");
	}

	public static String revokeProvision(String pid, Boolean only_if_authenticated, String exchange) {
		JSONObject json = new JSONObject();
		json.put("path","revoke/run");
		json.put("exchange",exchange);
		JSONObject jsonrequest = new JSONObject(); // sub-object with request data
		jsonrequest.put("pid", pid);
		jsonrequest.put("onlyIfAuthenticated", only_if_authenticated);
		json.put ("request",jsonrequest);
		return json.toString();
	}

	public static String deleteKey(String pid, String key_to_delete, String exchange) {
		JSONObject json = new JSONObject();
		json.put("path","key/delete");
		json.put("exchange",exchange);
		JSONObject jsonrequest = new JSONObject(); // sub-object with request data
		jsonrequest.put("pid", pid);
		jsonrequest.put(key_to_delete, true);
		json.put ("request",jsonrequest);
		return json.toString();
	}

	
}
