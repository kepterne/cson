// (c) 2025 ekrem.karacan@gmail.com
// This code is licensed under MIT license (see LICENSE.txt for details)
#ifndef	cobj_h
#define	cobj_h
#ifdef __cplusplus
extern	"C" {
#endif

#include	<stdio.h>
#include	<stdint.h>

#define	MAX_TEXT_LENGTH	128
#define	MAX_NAME_LENGTH	32
typedef	struct {
	int		next, prev, first, last, parent;
	int		idx, count, index;
	int		objtype;
	
#define	CO_NULL		0x0000
#define	CO_TEXT		0x0001
#define	CO_ARRAY	0x0002
#define	CO_OBJECT	0x0004
#define	CO_BOOLEAN	0x0008
#define	CO_INT		0x0010
#define	CO_FLOAT	0x0020
#define	CO_COMMENT	0x0040

#define	CO_JSON		0x1000
#define	CO_TEXTF	0x2000
#define	CO_CHANGED	0x4000
#define	CO_READONLY	0x8000

	int			firstfree, lastfree;
	uint64_t	ts;
	char		name[MAX_NAME_LENGTH];
	char		v[MAX_TEXT_LENGTH];
	char		extra[256];	// Ileride kullanmak uzere
	
} cobj;

void	_saveFileNamed(char *name);
void	cobj_init(cobj *r, int n);
cobj	*cobj_new(void);
void	cobj_free(cobj *co);
void	cobj_attach(cobj *parent, cobj *child);
cobj	*cobj_nth(cobj *obj, int index);
cobj	*cobj_findname(cobj *obj, const char *name);
cobj	*cobj_find(cobj *obj, const char *name);
void	print_tree(FILE *fp, cobj *obj, int level);
void	dump_tree(char *data, int *l, cobj *obj, int level);
int		cobj_loadjson(const char *fname, cobj *r, char *dname);
cobj	*cobj_createobj(cobj *obj, const char *name);
int		cobj_loadtext(const char *fname, char *dname);
int		cobj_printtext(FILE *fp, cobj *obj, int level, uint64_t ts);

int	cobj_dumptext(char *data, int *l, cobj *obj, int level, uint64_t ts);
void	cobj_printname(FILE *fp, cobj *o, int level);
void	cobj_dumpname(char *data, int *l, cobj *o, int level);
int		cobj_parsejson(const char *name, cobj *r, const char *data);
char	*cson_escape(char *in, char *out);

int	cobj_dumpindex(char *data, int *l, cobj *obj, int level, int maxl);

#ifdef	cobj_c
		cobj	*root = NULL;
#else
extern	cobj	*root;
#endif

#ifdef __cplusplus
}
#endif
#endif