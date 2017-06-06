import json, os, sys

try: input=raw_input
except: pass

class Quitter:
	def __repr__(self):
		print('Quitting!')
		sys.exit()
q=Quitter()

sys.path.append('.')
import apy
print ('#line 14 of app.py')
apy.configure('sample', log_level=int(os.environ.get('APY_LOG_LEVEL', 0)))
print ('#in app.py after apy.configure, line 15')

#apy.info is a dict
if len(apy.info()['response']['provisions'])==0:
	print("GOOD CONDTION HERE....GOOD CONDITION HERE...")
	print('looking for a band to provision')
	pattern=apy.provision()
	print('found a band to provision; pattern is '+pattern)
	print('enter y to accept this pattern')
	if input()!='y': raise Exception('user rejected pattern')
	print('saving provision')
	apy.accept(pattern)
else:
	print('#something did not equal 0, problem in if statement, line 18 of app.py')
	print( len(apy.info()['response']['provisions']))
	print('#^^^should needs to be equal to 0')

pid=apy.info()['response']['provisions'][0]

def run_gauntlet(pid=pid):
	print('random: '+apy.random(pid))
	print('buzzing')
	apy.buzz(pid)
	ks=apy.roaming_auth_get_partner_public_keys()
	if not ks:
		print('setting up roaming auth')
		k=apy.roaming_auth_setup(pid)
	else: k=ks[0]
	print('performing roaming auth')
	assert(apy.roaming_auth_go(apy.closest()['tid'], k))
	print('auth!')
