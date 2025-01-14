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

**make shared** only creates shared library

**make clean** clears all

**make release** builds and installs library, and cson-tool

**make debug** creates cson-tool_dbg in bin folder

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
##### GET FUNCTIONS
	int		cson_getString(cobj *o, const char*key, char *dest, const char*defval);
	/*	
		If key exists set dest to value 
		If not set dest to defval */

	int		cson_getStringf(cobj *o, char *dest, const char *defval, const char *fmt, ...);
	/*	
		Same as cson_getString and
		you can use like
		cson_getStringf(NULL, value, "default", "cameras.%s.streams[%d].uri", "camera1", 1);
	*/

	int64_t	cson_getInt(cobj *o, const char*key, int64_t defval);
	/*
		If key exists and value is number, returns that as int64
		Otherwise returns defval
	*/

	int64_t	cson_getIntf(cobj *o, int64_t defval, const char *fmt, ...);
	/*	
		Same as cson_getInt and
		you can use like
		width = cson_getIntf(NULL, 1920, "cameras.%s.streams[%d].width", "camera1", 1);
	*/

	
	int		cson_getBool(cobj *o, const char*key, int defval);
	/*
		If value is integer and non zero, returns 1,
		If value is "true" returns 1
		Otherwise or value is "false" returns 0 
		If key not exists returns defval
	*/

	float		cson_getFloat(cobj *o, const char*key, float defval);
	/*
		Returns float value if value is parsable as float,
		otherwise returns defval
	*/

##### SET FUNCTIONS
	
	int		cson_setfString(cobj *o, const char *val, char *fmt, ...);

	int		cson_setfInt(cobj *o, uint64_t val, char *fmt, ...);

	int		cson_setIntf(cobj *o, int64_t defval, const char *fmt, ...);
	
	int		cson_setString(cobj *o, const char*key, const char*val);

	int		cson_setInt(cobj *o, const char*key, int64_t val);

	int		cson_setFloat(cobj *o, const char*key, float val);

	int		cson_setBool(cobj *o, const char*key, int val);

	int		cson_setStringf(cobj *o, const char*key, char *fmt, ...);

##### SEARCH FUNCTIONS	

	cobj		*cson_find(cobj *o, const char*key);

	cobj		*cson_find_child(cobj *o, const char*key);

	cobj		*cson_findf(cobj *o, const char *fmt, ...);

##### SORT AND MATCH FUNCTIONS

	int     	json_name_match(char *t, char *f);

	int     	json_name_scanf(char *text, char *format, ...);

	int		cson_sort(char *name, int (*f)(cobj *, cobj *));

##### ITERATION FUNCTIONS

	cobj		*cson_next(cobj *o);

	cobj		*cson_prev(cobj *o);

	cobj		*cson_first(cobj *o);

	cobj		*cson_last(cobj *o);

##### TIME FUNCTIONS

	uint64_t	ts_monotonic(void);

	uint64_t	startTimer(uint64_t *v);

	uint64_t	checkTimer(uint64_t *v);

	uint64_t	ts_monotonic64(void);

	uint64_t	startTimer64(uint64_t *v);

	uint64_t	checkTimer64(uint64_t *v);

	char		*string_time(time_t t);

	int		current_wday(void);

	int		current_seconds(void);

	char		*current_time(void);

##### FILE AND SYNC FUNCTIONS

	void		cson_sync(void);

	void		cson_syncreset(char *key);

	int		cson_dumpText(char *data, int *l, cobj *obj, uint64_t ts);

	int		cson_sendJson(char *data, int *l, const char *key);

	void		cson_delete(cobj *ob);

	int		cson_loadJson(const char*fname);

	int		cson_loadOrCreateJson(const char *fname);

	int		cson_loadOrCreateText(const char *fname);

	int		cson_loadJsonTo(const char *fname, cobj *c);

	int		cson_loadText(const char*fname);

	int		cson_loadOrCreateJsonWithDate(const char *fname, int created);

	int		cson_saveJson(const char*fname, const char*key);

	int		cson_saveText(const char *fname, const char *key, uint64_t ts);

	void		cson_save(char *name);

	void		cson_check(void);

	int		cson_reset(void);

##### OTHER FUNCTIONS

	char    	*cson_ucase(char *p);

	int		countLines(char *fname);

	int		write_gpio(int k, int v);

	int		cson_parseJson(const char *data, char *name);

	int		cson_parseHTTPResponse(const char *data, char *name);



	
	

Soon, but not too soon
#### As a shared library
Just add -lcson to your build.
#### From source
Add jsonparser.c cson.c cobj.c to your project.
You can use example Makefile used to build cson-tool as an example

# LICENSE

(c) 2025 ekrem.karacan@gmail.com

This code is licensed under MIT license (see LICENSE.txt for details)