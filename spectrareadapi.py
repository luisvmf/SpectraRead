import pickle
import codecs
globalxdata=""
globalydata=""
globalerrdata=""
globalwarningdata=""
globaltemp=""
def setxdata(x):
	global globalxdata
	globalxdata=x
def setydata(y):
	global globalydata
	globalydata=y
def seterr(err):
	global globalerrdata
	globalerrdata=err
def setinfo(infoguiapi):
	global globaltemp
	globaltemp=codecs.encode(pickle.dumps(infoguiapi), "base64").decode()
def setwarning(warning):
	global globalwarningdata
	globalwarningdata=warning
def pushval():
	global globalxdata
	global globalydata
	global globalerrdata
	global globalwarningdata
	global globaltemp
	intglobalwarningdata=globalwarningdata
	globalwarningdata=""
	intglobalerrdata=globalerrdata
	globalerrdata=""
	return [globalxdata,globalydata,intglobalerrdata,intglobalwarningdata,globaltemp]
