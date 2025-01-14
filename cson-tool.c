#include	<stdio.h>
#include	<string.h>
#include	<strings.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<sys/stat.h>

#include	"cson/cson.h"

void	CheckFiles(void) {
	struct stat	st;
	time_t	sec;
	struct tm	*tmp;
	char		cur_time[128];
		
	cobj	*o = cson_find(NULL, "system.files");
	if (!o)
		return;
	for (o = cson_first(o); o; o = cson_next(o)) {
		char	filename[64];
		char	at[64];
		cson_getString(o, "filename", filename, "");
		cson_getString(o, "at", at, "");
		if (lstat(filename, &st) < 0)
			continue;
		sec = st.st_mtim.tv_sec;
		tmp = localtime(&sec);
		strftime(cur_time, sizeof(cur_time), "%Y-%m-%d - %T", tmp);
		if (strcmp(cur_time, at) <= 0)
			continue;
		printf("Changed : %s, %s\r\n", filename, cur_time);
		if (strstr(filename, ".js") || strstr(filename, ".json") )
			cson_loadJson(filename);
		else
			cson_loadText(filename);
		cson_syncreset(o->name);
	}
	cson_sync();
}

int	print_cson_as_text(char *key, uint64_t *ts) {
	cobj	*o;
	uint64_t	tss = *ts;
	
	if (key == NULL)
		o = root;
	else
		o = cson_find(NULL, key);
	if (!o) {
		printf("\r\n");	// KEY NOT FOUND
		*ts = 0;
		return 0;
	} 
	if (!*ts)
		*ts = o->ts;
	if (!(o->objtype & (CO_ARRAY | CO_OBJECT))) {
		if (tss <= o->ts)
			printf("%s\r\n", o->v);		// SIMPLE VALUE
		return 1;
	}

	cson_saveText(NULL, key, tss);	// COMPLEX VALUE
	return 2;
}


int	print_cson_as_json(char *key, uint64_t *ts) {
	cobj	*o;
	
	if (key == NULL)
		o = root;
	else
		o = cson_find(NULL, key);
	if (!o) {
		printf("\r\n");
		*ts = 0;
		return 0;
	} 
	*ts = o->ts;
	if (!(o->objtype & (CO_ARRAY | CO_OBJECT))) {
		printf("%s\r\n", o->v);
		return 1;
	}

	cson_saveJson(NULL, key);
	return 2;
}
int	keyname(char *fname, char *key) {
	char	*p, *ps = NULL;
	for (p = strchr(fname, '/'); p; p = strchr(ps, '/')) {
		ps = p;
	}
	if (!ps)
		ps = fname;
	if ((p = strchr(fname, '.'))) {
		strncpy(key, ps, p - ps);
		key[p - ps] = 0;
		return 1;
	} else
		return 0;
}

int	main(int argc, char **argv) {
	if (argc == 1)
		goto l_help;
	if (strcasecmp(argv[1], "ver") == 0) {
		printf("LIBVER : %s\r\n", LIBVER);
		goto l0;
	}
	cson_find(NULL, "HEY");
	if (strcasecmp(argv[1], "help") == 0 || argc == 1) {
l_help:
		// help
		printf("USAGE cson-tool <arg>[<key>[<value>]]\r\n"
			"\tver: version\r\n"
			"\thelp: show this\r\n"
			"\r\n"
			"\tget <key> : get value of key (. for wildcard)\r\n"
			"\tgetjson <key> : get value as json (. for wildcard)\r\n"
			"\r\n"
			"\tmonitor <key> : monitor key, print value (or object) when changes (. for wildcard)\r\n"
			"\tmonitorjson <key> : monitor key, print value (or json object) when changes (. for wildcard)\r\n"
			"\r\n"
			"\tdel <key> : delete key\r\n"
			"\tset <key> <value> : create key (or object(s) if necessary) and set value\r\n"
			"\r\n"
			"\tloadjson <json file name> : load json file into shm\r\n"
			"\tloadtext <name=value type file name> : load text file into shm\r\n"
			"\r\n"
			"\tservice	: Forever monitor loaded files, and mirror changes in files to shm, and vice versa\r\n");
		goto l0;
	}

	if (strcasecmp(argv[1], "set") == 0) {
		if (argc < 4)
			goto l_help;
		char	*toset = argv[2];
		char	*value = argv[3];
		cson_setString(NULL, toset, value);
		goto l0;
	}

	
	if (strcasecmp(argv[1], "get") == 0) {
		uint64_t	ts = 0;
		if (argc < 3)
			goto l_help;
		char	*toset = argv[2];
		if (strcmp(toset, ".") == 0)
			toset = NULL;
		print_cson_as_text(toset, &ts);
		goto l0;
	}
	
	if (strcasecmp(argv[1], "getjson") == 0) {
		uint64_t	ts = 0;
		if (argc < 3)
			goto l_help;
		char	*toset = argv[2];
		if (strcmp(toset, ".") == 0)
			toset = NULL;
		print_cson_as_json(toset, &ts);
		goto l0;
	}
	
	if (strcasecmp(argv[1], "monitor") == 0) {
		uint64_t	ts = 0, tss = 0;
		cobj		*o;
		char		*key;

		if (argc < 3)
			goto l_help;
		char	*toset = argv[2];
		
		for ( ; ; usleep(100000)) {		
			if (strcmp(toset, ".") == 0) {
				o = root;
				key = NULL;
			} else  {
				o = cson_find(NULL, toset);
				key = toset;
			}
		
			if (!o) {
				tss = 0;
				continue;
			} else if (tss >= o->ts)
				continue;		
			if (!(o->objtype & (CO_ARRAY | CO_OBJECT))) {
				printf("%s\r\n", o->v);		// SIMPLE VALUE
			} else 
				cson_saveText(NULL, key, tss);
			tss = o->ts;
		}
	}
	
	if (strcasecmp(argv[1], "monitorjson") == 0) {
		uint64_t	ts = 0, tss = 0;
		cobj		*o;
		char		*key;

		if (argc < 3)
			goto l_help;
		char	*toset = argv[2];
		
		for ( ; ; usleep(100000)) {		
			if (strcmp(toset, ".") == 0) {
				o = root;
				key = NULL;
			} else  {
				o = cson_find(NULL, toset);
				key = toset;
			}
		
			if (!o) {
				tss = 0;
				continue;
			} else if (tss >= o->ts)
				continue;		
			if (!(o->objtype & (CO_ARRAY | CO_OBJECT))) {
				printf("%s\r\n", o->v);		// SIMPLE VALUE
			} else 
				cson_saveJson(NULL, key);
			tss = o->ts;
		}
	}
	if (strcasecmp(argv[1], "del") == 0) {
		uint64_t	ts = 0, tss = 0;
		cobj		*o;
		char		*key;

		if (argc < 3)
			goto l_help;
		char	*toset = argv[2];
				
		o = cson_find(NULL, toset);
		if (!o)
			goto l0;
		cson_delete(o);
	}
	if (strcasecmp(argv[1], "loadjson") == 0) {
		

		if (argc < 3)
			goto l_help;
		char	*file = argv[2];
		char	filename[512];
		char	fkey[512];
		if (!realpath(file, filename)) {
			;
		}
		if (keyname(file, fkey)) {
			cson_loadOrCreateJson(filename);
			printf("\"%s\" : ", fkey);
			cson_saveJson(NULL, fkey);
			return 1;
		}
		return 0;
	}
	if (strcasecmp(argv[1], "loadtext") == 0) {
		

		if (argc < 3)
			goto l_help;
		char	*file = argv[2];
		char	filename[512];
		char	fkey[512];
		if (!realpath(file, filename)) {
			;
		}
		if (keyname(file, fkey)) {
			cson_loadOrCreateText(filename);
			printf("%s.\r\n", fkey);
			cson_saveText(NULL, fkey, 0);
			return 1;
		}
		return 0;
	}
	if (strcasecmp(argv[1], "service") == 0) {
		for ( ; ; usleep(100000)) {
			CheckFiles();
		}
	}
l0:
	return 0;
}
