#include	<stdio.h>
#include	<string.h>
#include	<strings.h>
#include	<unistd.h>

#include	"cson/cson.h"

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

int	main(int argc, char **argv) {
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
			"\tget <key> : get value of key (. for wildcard)\r\n"
			"\tgetjson <key> : get value as json (. for wildcard)\r\n"
			"\tmonitor <key> : monitor key, print value (or object) when changes (. for wildcard)\r\n"
			"\tmonitorjson <key> : monitor key, print value (or json object) when changes (. for wildcard)\r\n"
			"\tdel <key> : delete key\r\n"
			"\tset <key> <value> : create key (or object(s) if necessary) and set value\r\n"
			"\r\n");
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
l0:
	return 0;
}
