import ffi, roaming_auth_server

from ffi import configure

def wait(path, condition=lambda j: True, timeout=15, resolution=0.01):
	print ('#in apy.wait, line 6')
	for i in range(int(timeout/resolution)):
		j=ffi.get()
		#print('#ffi.get succesful, line 9 of apy.py')
		#print('********j:' + str(bool(j)))
		if j and j.get('path', None)==path and condition(j): return j
		else:
			print ('#conditions on line 10 in apy.wait not satisfied')	
		#	print('\t #j: ' + str(bool(j)) + " need to be a true value")
		#	if j:
		#		print("\t #j.get('path',None)==path: " + str(j.get('path', None)==path) + " ,needs to be a true value")
		#	else:
		#		print("\t \t #no j value so also not j.get('path.... value")
		#	print('\t #condition(j):' + str(condition(j)) + ' ,needs to be a true value')
		import time
		time.sleep(resolution)
	raise Exception('timed out')

def create_exchange(name, pid=''):
	r='apy '+name
	if pid: r+=' '+pid
	import random
	r+=' '+hex(random.getrandbits(128))[2:-1]
	return r
#info() returns a dict
def info():
	print('#call to info  in apy.py line 32')
	ffi.put(path='info/get', exchange=create_exchange('info'))
	#changed code here used to be:
	#return wait('info/get')
	dict1 = wait ('info/get')
	if dict1:
		print("dict is not empty")
	else:
		print("dict is empty")
	return dict1

def closest():
	return max(info()['response']['nymiband'], key=lambda x: x['RSSI_smoed'])

def provision():
	ffi.put(path='provision/run/start', exchange=create_exchange('provision_start'))
	pattern=wait('provision/report/patterns')['event']['patterns'][0]
	ffi.put(path='provision/run/stop', exchange=create_exchange('provision_stop'))
	return pattern

def accept(pattern):
	ffi.put(
		path='provision/pattern',
		request={'action': 'accept', 'pattern': pattern},
		exchange=create_exchange('accept'),
	)
	wait('provisions/changed', lambda j: len(j['response']['provisions'])>0)

def buzz(pid, positive=True):
	ffi.put(
		path='buzz/run',
		request={'pid': pid, 'buzz': 'positive' if positive else 'negative'},
		exchange=create_exchange('buzz', pid),
	)

def random(pid):
	ffi.put(
		path='random/run',
		request={'pid': pid},
		exchange=create_exchange('random', pid),
	)
	return wait('random/run', lambda j: 'pseudoRandomNumber' in j['response'])['response']['pseudoRandomNumber']

def roaming_auth_setup(pid):
	partner_public_key=roaming_auth_server.create_key_pair()
	ffi.put(
		path='roaming-auth-setup/run',
		request={'pid': pid, 'partnerPublicKey': partner_public_key},
		exchange=create_exchange('roaming_auth_setup', pid),
	)
	r=wait('roaming-auth-setup/run', lambda j: j['completed'])['response']
	roaming_auth_server.store(r['RAKey'], r['RAKeyId'])
	return partner_public_key

def roaming_auth_get_partner_public_keys():
	return roaming_auth_server.get_partner_public_keys()

def roaming_auth_go(tid, partner_public_key):
	exchange=create_exchange('roaming_auth', str(tid))
	ffi.put(
		path='roaming-auth/run',
		request={'tid': tid},
		exchange=exchange,
	)
	server_signature, server_nonce=roaming_auth_server.sign(
		wait('roaming-auth/report/nonce')['event']['nymibandNonce'],
		partner_public_key,
	)
	ffi.put(
		path='roaming-auth-sig/run',
		request={
			'serverSignature': server_signature,
			'serverNonce': server_nonce,
			'partnerPublicKey': partner_public_key,
		},
		exchange=exchange,
	)
	r=wait('roaming-auth-sig/run')['response']
	return roaming_auth_server.verify(server_nonce, r['raKeyId'], r['nymibandSig'])
