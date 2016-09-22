#pragma once

#ifndef GenJson_hpp
#define GenJson_hpp

#include "json/src/json.hpp"

using nljson = nlohmann::json;

inline std::string get_init() {

	nljson j;
    j["path"] = "init/get";
    j["exchange"] = "*init*";
	return j.dump();
}

inline std::string finish() {

	nljson j;
	j["path"] = "finish/run";
    j["exchange"] = "*finish*";
	return j.dump();
}

inline std::string start_prov() {

	nljson j;
    j["path"] = "provision/run/start";
	j["exchange"] = "*provisioning*";
	return j.dump();
}

inline std::string accept_pattern(std::string pattern) {

	nljson j;
    j["path"] = "provision/pattern";
	j["exchange"] = "*provisioning*";
    j["request"]["pattern"]=pattern;
    j["request"]["action"]="accept";
	return j.dump();
}

inline std::string stop_prov() {

	nljson j;
    j["path"] = "provision/run/stop";
	j["exchange"] = "*provisioning*";
	return j.dump();
}

/* exchange here is expected to be non-empty, as it determines which field of info we are interested in*/
inline std::string get_info(std::string exchange) {
    
	nljson j;
	j["path"] = "info/get";
	j["exchange"] = exchange;
	return j.dump();
}

inline std::string get_random(std::string pid, std::string exchange = "") {
    
    nljson j;
    j["path"] = "random/run";
    j["request"]["pid"] = pid;
    j["exchange"] = exchange;
    return j.dump();
}

inline std::string create_symkey(std::string pid, bool guarded, std::string exchange = "") {
    
    nljson j;
    j["path"]="symmetricKey/run";
    j["request"]["pid"] = pid;
    j["request"]["guarded"]=guarded;
    j["exchange"] = exchange;
    return j.dump();
}

inline std::string get_symkey(std::string pid, std::string exchange = "") {

	nljson j;
    j["path"]="symmetricKey/get";
	j["request"]["pid"] = pid;
	j["exchange"] = exchange;
	return j.dump();
}

inline std::string sign_msg(std::string pid, std::string msghash, std::string exchange = "") {

	nljson j;
    j["path"]="sign/run";
    j["request"]["pid"]=pid;
    j["request"]["hash"]=msghash;
	j["exchange"] = exchange;
	return j.dump();
}

inline std::string set_totp(std::string pid, std::string totpkey, bool guarded, std::string exchange = "") {

	nljson j;
    j["path"]="totp/run";
	j["request"]["pid"] = pid;
	j["request"]["key"] = totpkey;
	j["request"]["guarded"] = guarded;
	j["exchange"] = exchange;
	return j.dump();
}

inline std::string get_totp(std::string pid, std::string exchange = "") {

	nljson j;
    j["path"]="totp/get";
	j["request"]["pid"] = pid;
	j["exchange"] = exchange;
	return j.dump();
}

inline std::string notify(std::string pid, bool notifyType, std::string exchange = "") {

	nljson j;
    j["path"] = "buzz/run";
	j["request"]["pid"] = pid;
	j["request"]["buzz"] = notifyType;
	j["exchange"] = exchange;
	return j.dump();
}

inline std::string enable_notification(bool enable, std::string state) {

	nljson j;
    j["path"] = "notifications/set";
	j["request"] = { {state, enable} };
    j["exchange"] = "*notifications*";
	return j.dump();
}

inline std::string get_state_notifications(){
    
    nljson j;
    j["path"] = "notifications/get";
    j["exchange"] = "*notifications*";
    return j.dump();
}

inline std::string revoke_provision(std::string pid, bool only_if_authenticated, std::string exchange = "") {

    nljson j;
    j["path"] = "revoke/run";
    j["request"]["pid"] = pid;
    j["request"]["onlyIfAuthenticated"] = only_if_authenticated;
    j["exchange"] = exchange;
    return j.dump();
}

inline std::string delete_key(std::string pid, std::string key_to_delete, std::string exchange = "") {

    nljson j;
    j["path"]="key/delete";
    j["request"]["pid"]=pid;
    j["request"][key_to_delete]=true;
    j["exchange"]= exchange;
    return j.dump();
}

#endif /* GenJson */