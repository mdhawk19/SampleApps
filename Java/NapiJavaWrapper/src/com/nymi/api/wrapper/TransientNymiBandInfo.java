package com.nymi.api.wrapper;

import java.util.ArrayList;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class TransientNymiBandInfo {

	private JSONObject deviceInfo;
	
	public TransientNymiBandInfo(JSONObject jobj) {
		deviceInfo = jobj;
	}

	public int getRssiLast(){
	    try {
	        return deviceInfo.getInt("RSSI_last");
	    } catch (JSONException e) {
	    	return 0;
	    }
	}

	public int getRssiSmoothed(){
	    try {
	        return deviceInfo.getInt("RSSI_smoothed");
	    } catch (JSONException e) {
	    	return 0;
	    }
	}

	public List<String> getCommandsQueued() {
		List<String> commands = new ArrayList<>();
		try {
	    	JSONArray cmdarr = deviceInfo.getJSONArray("commandQueue");
	        for (int i = 0 ; i < cmdarr.length() ; i++)
	            commands.add(cmdarr.getString(i));
		} catch (JSONException e) {
			return commands;
		}
        return commands;
	}

	public String getFirmwareVersion(){
	    try {
	        return deviceInfo.getString("firmwareVersion");
	    } catch (JSONException e) {
	    	return "";
	    }
	}

	public NymiJavaApi.FoundStatus getFoundState(){
	    try {
	        return NymiJavaApi.FoundStatus.valueOf(deviceInfo.getString("found").toUpperCase());
	    } catch (JSONException e) {
	    	return NymiJavaApi.FoundStatus.ERROR;
	    }
	}

	public NymiJavaApi.PresenceStatus getPresenceState(){
		try {
			return NymiJavaApi.PresenceStatus.valueOf("DEVICE_PRESENCE_" + deviceInfo.getString("present").toUpperCase());
		} catch(JSONException e) {
	    	return NymiJavaApi.PresenceStatus.ERROR;
		}
	}

	public boolean isProvisioned(){
	    if (deviceInfo.has("isProvisioned"))
	        return true;
	    else
	    	return false;
	}

	public double getSinceLastContact(){
	   try {
	        return deviceInfo.getDouble("sinceLastContact");
	   } catch (JSONException e) {
	    	return 0.0;
	   }
	}

	public double getAuthenticationWindowRemaining() {
		try {
			return deviceInfo.getJSONObject("provisioned").getDouble("authenticationWindowRemaining");
	    }
		catch (JSONException e) {
			return 0.0;
		}
	}

	public int getNumCommandsQueued(){
		try {
	    return deviceInfo.getJSONObject("provisioned").getInt("commandsQueued");
		} catch (JSONException e) {
			return 0;
		}
	}

	public boolean enabledRoamingAuthentication(){
		try {
		    return deviceInfo.getJSONObject("provisioned").getBoolean("enabledRoamingAuthSetup");
			} catch (JSONException e) {
				return false;
			}
	}

	public boolean enabledSigning(){
		try {
		    return deviceInfo.getJSONObject("provisioned").getBoolean("enabledSigning");
			} catch (JSONException e) {
				return false;
			}
	}

	public boolean enabledSymmetricKeys(boolean value){
		try {
		    return deviceInfo.getJSONObject("provisioned").getBoolean("enabledSymmetricKeys");
			} catch (JSONException e) {
				return false;
			}
	}
	
	public boolean enabledTOTP(){
		try {
		    return deviceInfo.getJSONObject("provisioned").getBoolean("enabledTOTP");
			} catch (JSONException e) {
				return false;
			}
	}

	public String getPid(){
		try {
		    return deviceInfo.getJSONObject("provisioned").getString("pid");
			} catch (JSONException e) {
				return "";
			}
	}
	
}
