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
