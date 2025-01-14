# cson
## What is it
It is a very simple **c / c++** library to handle json.
## Platforms
Linux, possibly BSD, and Mac Os
### Dependencies
libpthread 
## What can it do
1. Share in memory json objects and share between processes
2. Share (or create), name=value or json files between processes
3. Mirror changes made on files to shm and vice versa.
## Building
```
git clone https://github.com/kepterne/cson.git
cd cson
make
```

It will create **release** and **debug** folders

**make** or **make release** will install **cson-tool** binary to 
**/usr/bin** or whatever **BINDIR** is set in **cson.mak** 

library version is set in **cson.mak** variable **LIBVER**
Shared library will be installed **cson.mak** variable **DEPLOYDIR**

.e.g. for **LIBVER=2.2.2** and **DEPLOYDIR=/usr/lib** it will create **/usr/lib/libcson.so.2.2.2**

**make** or **make debug** will create debug version of binary in **debug** folder
Debug version does not use shared library, it uses sources directly.

You can debug using **vscode**, vscode config files are already in **.vscode** folder
## How to use
### From shell
#### cson-tool
```
	USAGE cson-tool <arg>[<key>[<value>]]
		ver: version
		help: show this

		get <key> : get value of key (. for wildcard)
		getjson <key> : get value as json (. for wildcard)

		monitor <key> : monitor key, print value (or object) when changes (. for wildcard)
		monitorjson <key> : monitor key, print value (or json object) when changes (. for wildcard)

		del <key> : delete key
		set <key> <value> : create key (or object(s) if necessary) and set value

		loadjson <json file name> : load json file into shm
		loadtext <name=value type file name> : load text file into shm

		service : Forever monitor loaded files, and mirror changes in files to shm, and vice versa
```
1. #### In memory json example 
```
	cson-tool set test.name "kepterne"

	cson-tool set test.address.country Tr

	cson-tool set test.address.city Ankara

	cson-tool get test
	name=kepterne
	address.country=Tr
	address.city=Ankara

	cson-tool getjson test
	{
		"name" : "kepterne",
		"address" : {
			"country" : "Tr",
			"city" : "Ankara"
		}
	}
```
2. #### Create a json file and set values
```
	cson loadjson ident.json
	"ident" : {
		"created" : "2025-01-14 - 10:04:11"
	}
	```
	File is created, creation timestamp automatically added
	```
	cat ident.json
	{
	"created":"2025-01-14 - 10:04:11"
	}
```
	Lets set some values
```
	cson-tool set ident.persons[0].name "Kepterne"
	cson-tool set ident.persons[0].password "123"
	cson-tool set ident.persons[0].email "kepterne@gmail.com"
	cson-tool getjson ident
	{
		"created" : "2025-01-14 - 10:04:11",
		"persons" : [
			{
					"name" : "Kepterne",
					"password" : 123,
					"email" : "kepterne@gmail.com"
			}
		]
	}
```
	Lets see if changed are mirrored in the file
```
	cat ident.json 
	{
		"created" : "2025-01-14 - 10:04:11",
		"persons" : [
			{
					"name" : "Kepterne",
					"password" : 123,
					"email" : "kepterne@gmail.com"
			}
		]
	}
```
	If an instance of **cson-tool service** is running in the machine, changes will be applied to files, and changes in the file will be applied to shm in few secs.
	Otherwise it will take some time. 

### From C (C++)
#### As a shared library
#### From source



Store json (or name=value) files on shared memory. Save changed values to files, and set values on shared memory when corresponding files change.
