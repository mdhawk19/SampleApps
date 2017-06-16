import binascii, hashlib, json, os, subprocess, sys
try: input=raw_input
except: pass
home=os.path.split(os.path.realpath(__file__))[0]
start=os.getcwd()

class Database:
	def __init__(self, file_name):
		print("Debug: created a data base")
		self._file_name=file_name
		self._read()
		self.__setitem__("key2","value") #debug, this created a a roaming_auth_server_database file :)
		#self._write() #debug

	def __setitem__(self, key, value):
		self.items[key]=value
		self._write()

	def __getitem__(self, key): return self.items[key]

	def _write(self):
		with open(self._file_name, 'w') as file:
			file.write(json.dumps(self.items))

	def _read(self):
		if os.path.exists(self._file_name):
			with open(self._file_name) as file:
				self.items=json.loads(file.read())
		else: self.items={}

db=Database('roaming_auth_server_database.txt')

os.chdir(home)
if not os.path.exists(os.path.join('python-rubenesque')):
	repo='https://github.com/latchset/python-rubenesque'
	print('I need to clone {} in {}. Enter y if this is OK'.format(repo, os.getcwd()))
	if input()!='y': raise Exception('user declined rubenesque cloning')
	subprocess.check_call('git clone '+repo, shell=True)
os.chdir(start)

sys.path.append(os.path.join(home, 'python-rubenesque'))
from rubenesque import curves
from rubenesque.signatures import ecdsa
secp256r1=curves.find('secp256r1')

def create_key_pair():
	partner_private_key=secp256r1.private_key()
	partner_public_key=secp256r1.generator()*partner_private_key
	partner_public_key_string='{:064x}{:064x}'.format(partner_public_key.x, partner_public_key.y)
	db[partner_public_key_string]=str(partner_private_key)
	return partner_public_key_string

def store(roaming_auth_key, roaming_auth_key_id):
	db[roaming_auth_key_id]=roaming_auth_key

def get_partner_public_keys():
	return [i for i, j in db.items.items() if len(i)==128]

def sign(nymi_band_nonce, partner_public_key):
	server_nonce=binascii.hexlify(os.urandom(32)).decode()
	signature='{:064x}{:064x}'.format(*ecdsa.sign(
		secp256r1,
		int(db[partner_public_key]),
		hashlib.sha256(bytearray.fromhex(nymi_band_nonce+server_nonce)).digest(),
	))
	return (signature, server_nonce)

def verify(server_nonce, roaming_auth_key_id, signature):
	return ecdsa.verify(
		secp256r1(
			int(db[roaming_auth_key_id][ 0: 64], 16),
			int(db[roaming_auth_key_id][64:128], 16),
		),
		hashlib.sha256(bytearray.fromhex(server_nonce)).digest(),
		int(signature[ 0: 64], 16),
		int(signature[64:128], 16),
	)
