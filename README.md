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
### From C (C++)
#### As a shared library
#### From source



Store json (or name=value) files on shared memory. Save changed values to files, and set values on shared memory when corresponding files change.
