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
apy.configure('sample', log_level=int(os.environ.get('APY_LOG_LEVEL', 0)))

n = 2 #n is the number of bands being used
#apy.info is a dict
while len(apy.info()['response']['provisions']) < n:
	print('looking for a band to provision')
	pattern=apy.provision()
	print('found a band to provision; pattern is '+pattern)
	print('enter y to accept this pattern')
	if input()!='y': raise Exception('user rejected pattern')
	print('saving provision')
	apy.accept(pattern)


pid=apy.info()['response']['provisions'][0]