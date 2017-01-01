#!/bin/env python
#-*- encoding=utf8 -*-

import os
import shutil

# Windows
if (os.path.isfile("RdsOnMysql.sdf")):
	os.remove("RdsOnMysql.sdf")
if (os.path.isfile("RdsOnMysql.v12.suo")):
	os.remove("RdsOnMysql.v12.suo")

#if (os.path.exists("./x64")):
#	shutil.rmtree("./x64",True)

#Linux

if (not os.path.exists("./bin")):
	os.makedirs("./bin")
