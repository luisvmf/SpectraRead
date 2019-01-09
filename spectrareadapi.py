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
def settemp(temp):
	global globaltemp
	globaltemp=temp
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
