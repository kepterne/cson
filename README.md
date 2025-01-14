# cson
## or How I Learned to Stop Worrying and Love the shm
I am open to all ideas, contrubitions and suggestions.

I hate json, I really really hate it.

I hate oversized, overcomplicated, overused json libraries.

I needed to access settings saved in json file, and if settings changed do some stuff. 

And fill some data in json files (e.g. scan for onvif cameras, and fill a json file with details of these cameras), and in another app display streams from these cameras with a layout stored in a json file. 

This code helped me a lot. I learned a lot from hundreds of github users, so I wanted to give back a little.

Code may be complex and quirky because I am quirky and complex too. If you hate that and want to clean and fix it, be my guest. 

I would love to have a collaborator or two of any kind. 

**I hate json.**

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
3. #### Create a json file and set values, monitor changes, and see if it mirrors changes made on files to shm, and vice versa
	First run **cson-tool service** on a seperate terminal window
	```
	cson-tool service
	```
	Lets create a json file with cson-tool
	```
	cson-tool loadjson example.json
	cson-tool set example.address.city Ankara
	cson-tool set example.address.country Turkey
	cson-tool set example.address.gps.lon 32.771245
	cson-tool set example.address.gps.lat 39.950090

	cson-tool get example
	created=2025-01-14 - 10:24:39
	address.city=Ankara
	address.country=Turkey
	address.gps.lat=39.950090
	address.gps.lon=32.771245
	```
	While on the previous terminal we run **cson-tool service** we see changes are saved to file(s)
	```
	SAVING JSON : /***/*****/******/cson/example.json
	SAVING JSON : /***/*****/******/cson/example.json
	```
	Lets try monitoring the changes from another terminal window
	```
	cson-tool monitor example
	created=2025-01-14 - 10:24:39
	address.city=Ankara
	address.country=Turkey
	address.gps.lat=39.950090
	address.gps.lon=32.771245
	```
	Lets change change a value
	```
	cson-tool set example.address.city Istanbul
	```
	We will see changes on the monitor window
	```
	address.city=Istanbul
	```
	We will see changes in the files
	```
	cat example.json 
	{
		"created" : "2025-01-14 - 10:24:39",
		"address" : {
			"city" : "Istanbul",
			"country" : "Turkey",
			"gps" : {
					"lat" : "39.950090",
					"lon" : "32.771245"
			}
		}
	}
	```
	Lets change the file and set city to "antananaviro"
	We must see in *monitor* window
	```
	address.city=Antananaviro
	```
	Lets set city to correct value with cson-tool
	```
	cson-tool set example.address.city "06 Ankara"
	```
	#### CONCLUSION
	cson library creates a memory structure in shared memory, 
	all apps using this library can access and change values with simple functions, 
	create or save json or txt files,
	monitor changes, iterate objects etc.
	Details are covered in ***From C*** part. 
	File details will be stored in ***system*** object.
	```
	cson-tool getjson system
	{
		"files" : {
			"deneme" : {
					"filename" : "/mnt/udisk/movita/cson/deneme.json",
					"at" : "2025-01-14 - 08:47:38"
			},
			"ident" : {
					"filename" : "/mnt/udisk/movita/cson/ident.json",
					"at" : "2025-01-14 - 10:07:09"
			},
			"example" : {
					"filename" : "/mnt/udisk/movita/cson/example.json",
					"at" : "2025-01-14 - 10:37:57"
			}
		}
	}
	```
	cson-tool service, checks file dates (a better way is to use inotify mechanism), if files are changed reloads them, if the objects are changed saves them to files.

	It waits a little (e.g. if the object is not changes at least a second), to make sure if mass changes are done, all is finished.

	All object names are **case insensitive**
### From C (C++)
Soon
#### API Functions
Soon, but not too soon
#### As a shared library
Just add -lcson to your build.
#### From source



Store json (or name=value) files on shared memory. Save changed values to files, and set values on shared memory when corresponding files change.
