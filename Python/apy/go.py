#!/usr/bin/env python

import argparse, ctypes.util, glob, os, platform, subprocess, sys, tarfile, zipfile
home=os.path.split(os.path.realpath(__file__))[0]
os.chdir(home)

parser=argparse.ArgumentParser()
parser.add_argument('--clean', '-c', action='store_true', help='remove stateful files')
parser.add_argument('--real', '-r', action='store_true', help='use real bands instead of nymulator')
parser.add_argument('--debug', '-d', action='store_true', help='ask NAPI for debug log level')
args=parser.parse_args()

if args.clean:
	file_names=[
		'napi.log',
		'provisions.json',
		'roaming_auth_server_database.txt'
	]
	for file_name in file_names:
		try: os.remove(file_name)
		except: pass

if 'APY_CUSTOM_LOCATION' in os.environ: pass
elif platform.system()=='Darwin':
	if not ctypes.util.find_library('napi'):
		print('Please install Nymi SDK!')
		sys.exit(1)
else:
	if not os.path.exists('sdk'):
		archives=sorted(glob.glob('*.zip'))+sorted(glob.glob('*.tar.gz'))
		if len(archives)==0:
			print('Please drop a Nymi SDK archive beside me!')
			sys.exit(1)
		archive=archives[-1]
		if archive.endswith('.zip'):
			print('extracting')
			zipfile.ZipFile(archives[-1]).extractall()
		elif archive.endswith('.tar.gz'):
			with tarfile.open(archive) as file: file.extractall()
		os.rename([i for i in os.listdir('.') if i.startswith('sdk') and os.path.isdir(i)][0], 'sdk')

if args.real: os.environ['APY_REAL']='1'
if args.debug: os.environ['APY_LOG_LEVEL']='2'
print('running app')
subprocess.check_call('python -i app.py', shell=True)
