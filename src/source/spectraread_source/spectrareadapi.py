#MIT License
#
#Copyright (c) 2018 Luis Victor Muller Fabris
#
#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:
#
#The above copyright notice and this permission notice shall be included in all
#copies or substantial portions of the Software.
#
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#SOFTWARE.

import pickle
import codecs
globalxdata=""
globalydata=""
globalerrdata=""
globalwarningdata=""
globaltemp=""
globalxlabelinta=""
globalylabelinta=""
notset=False
lastinfoint=""
def setxdata(x):
	global globalxdata
	global notset
	notset=False
	globalxdata=x
def setxlabel(x):
	global globalxlabelinta
	global notset
	notset=False
	globalxlabelinta=x
def setylabel(y):
	global globalylabelinta
	global notset
	notset=False
	globalylabelinta=y
def setydata(y):
	global globalydata
	global notset
	notset=False
	globalydata=y
def seterr(err):
	global globalerrdata
	global notset
	notset=False
	globalerrdata=err
def setinfo(infoguiapi):
	global globaltemp
	global notset
	global lastinfoint
	notset=False
	if(lastinfoint!=infoguiapi):
		globaltemp=codecs.encode(pickle.dumps(infoguiapi), "base64").decode()
		lastinfoint=infoguiapi
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
	return [globalxdata,globalydata,intglobalerrdata,intglobalwarningdata,globaltemp,globalxlabelinta,globalylabelinta]
def npushval():
	global notset
	global globalxdata
	global globalydata
	global globalerrdata
	global globalwarningdata
	global globaltemp
	if(notset==False):
		notset=True
		intglobalwarningdata=globalwarningdata
		globalwarningdata=""
		intglobalerrdata=globalerrdata
		globalerrdata=""
		return ["","",intglobalerrdata,intglobalwarningdata,globaltemp,globalxlabelinta,globalylabelinta]
	else:
		return False
