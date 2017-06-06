import atexit, ctypes, ctypes.util, json, os, platform

start=os.getcwd()
home=os.path.split(os.path.realpath(__file__))[0]

api=os.path.join(home, 'sdk', 'API')
locations=[]
#windows
locations.append(os.path.join(api, 'Release', 'napi'))#4.1
#osx
locations.append(ctypes.util.find_library('napi'))#4.1
#net
if 'APY_REAL' not in os.environ and platform.system()!='Windows':
	net_locations=[]
	for i in locations:
		net_locations.append(i.replace('napi', 'napiNymulator'))#4.1
	locations=net_locations
#custom
if 'APY_CUSTOM_LOCATION' in os.environ:
	locations.append(os.environ['APY_CUSTOM_LOCATION'])
#
locations=[i for i in locations if i]
for i in locations:
	try:
		_napi=ctypes.CDLL(i)
		print('loaded '+i)
		print('#^^above from ffi.py')
		break
	except: pass
else:
	raise Exception(
		"Couldn't load napi! Tried\n{}\nAre you sure the SDK's bit width matches this Python's? This Python's bit width is {}.".format(
			'\n'.join(locations),
			platform.architecture()[0],
		)
	)

_napi.napiConfigure.restype=ctypes.c_int
_napi.napiConfigure.argtypes=[
	ctypes.c_char_p,
	ctypes.c_char_p,
	ctypes.c_char_p,
	ctypes.c_int,
	ctypes.c_int,
	ctypes.c_char_p,
]

_napi.napiPut.restype=ctypes.c_int
_napi.napiPut.argtypes=[ctypes.c_char_p]

_napi.napiTryGet.restype=ctypes.c_int
_napi.napiTryGet.argtypes=[
	ctypes.c_char_p,
	ctypes.c_ulonglong,
	ctypes.POINTER(ctypes.c_longlong),
]

atexit.register(_napi.napiTerminate)

provisions_file_name='provisions.json'

if os.path.exists(provisions_file_name):
	with open(provisions_file_name) as file: provisions=file.read()
else:
	provisions='{}'

def configure(
	nea_name,
	log_directory='.',
	log_level=0,
	port=-1,
	host=''
):
	print('#configuring in ffi.py, line 74')
	assert(len(nea_name)>=6)
	if port==-1:
		if 'APY_REAL' not in os.environ: port=9088
		port=int(os.environ.get('APY_PORT', port))
	r=_napi.napiConfigure(
		nea_name.encode(),
		log_directory.encode(),
		provisions.encode(),
		log_level,
		port,
		host.encode(),
	)
	if r: raise Exception('abnormal config outcome {}'.format(r))

def put(**kwargs):
	r=_napi.napiPut(json.dumps(kwargs).encode())
	print('# r var equal to _napi.napiPut, line 91 of ffi.py')
	if r: raise Exception('abnormal put outcome {}'.format(r))
	else:
		print ('#r does not have a value, line 94 of ffi.py, which is good, goes back to apy.info')

get_history=[]

def get():
	max=256
	buffer=ctypes.create_string_buffer(max)
	len=ctypes.c_longlong()
	len_ptr=ctypes.cast(ctypes.addressof(len), ctypes.POINTER(ctypes.c_longlong))
	while True:
		r=_napi.napiTryGet(buffer, max, len_ptr)
		if len.value==0: return
		if len.value>max:
			max=len.value
			buffer=ctypes.create_string_buffer(max)
			continue
		if r: raise Exception('abnormal get outcome {}'.format(r))
		j=json.loads(buffer.value.decode('utf-8'))
		global get_history
		get_history.append(j)
		if j==None: continue
		else:
			print("# j does not == None here, ffi.get line 116")
			print("##########j: " + str(bool(j)))
		p=j.get('path', '')
		if p=='notifications/report/general-error': raise Exception(j['event']['err'])
		if p=='provisions/changed':
			with open(provisions_file_name, 'w') as file:
				file.write(json.dumps(j['response']['provisions']))
		print("##################################j: " + str(bool(j)))
		return j
