import os
import shelve
def initconfig(backendname):
	directory = os.path.dirname(os.path.expanduser("~")+"/.spectraread/")
	if not os.path.exists(directory):
		os.makedirs(directory)
	#Always use file spectrareaddevicesettings-[modulename] for settings were [modulename] is the name of the folder were this files are.
	#This is to avoid more than one module from using the same configuration file.
	try:
		intconfigfile = shelve.open(os.path.expanduser("~")+"/.spectraread/spectrareaddevicesettings-"+str(backendname)+"")
	except:
		try:
			os.remove(os.path.expanduser("~")+"/.spectraread/spectrareaddevicesettings-"+str(backendname)+"")
		except:
			print("")
		try:
			os.remove(os.path.expanduser("~")+"/.spectraread/spectrareaddevicesettings-"+str(backendname)+"")
		except:
			print("")
		try:
			os.remove(os.path.expanduser("~")+"/.spectraread/spectrareaddevicesettings-"+str(backendname)+"")
		except:
			print("")
		intconfigfile = shelve.open(os.path.expanduser("~")+"/.spectraread/spectrareaddevicesettings-"+str(backendname)+"")
	return intconfigfile
