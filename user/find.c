#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

// Regexp matcher from Kernighan & Pike,
// The Practice of Programming, Chapter 9.

int matchhere(char*, char*);
int matchstar(int, char*, char*);

int
match(char *re, char *text)
{
  if(re[0] == '^')
    return matchhere(re+1, text);
  do{  // must look at empty string
    if(matchhere(re, text))
      return 1;
  }while(*text++ != '\0');
  return 0;
}

// matchhere: search for re at beginning of text
int matchhere(char *re, char *text)
{
  if(re[0] == '\0')
    return 1;
  if(re[1] == '*')
    return matchstar(re[0], re+2, text);
  if(re[0] == '$' && re[1] == '\0')
    return *text == '\0';
  if(*text!='\0' && (re[0]=='.' || re[0]==*text))
    return matchhere(re+1, text+1);
  return 0;
}

// matchstar: search for c*re at beginning of text
int matchstar(int c, char *re, char *text)
{
  do{  // a * matches zero or more instances
    if(matchhere(re, text))
      return 1;
  }while(*text!='\0' && (*text++==c || c=='.'));
  return 0;
}



void find(char *dirname, char *patten)
{
    char buf[512], *p;
    struct dirent de;
    struct stat st;
    int fd;

    if ((fd = open(dirname, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", dirname);
        exit(1);
    } 
    if (strlen(dirname) + 1 + DIRSIZ + 1 > sizeof(buf))
    {
        fprintf(2, "find: path too long\n");
        close(fd);
        exit(1);
    }
    strcpy(buf, dirname);
    p = buf + strlen(buf);
    *p++ = '/';
    // read dir entry
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        // unlink
        if (de.inum == 0)
        {
            continue;
        }
        // except "." and ".."
        if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
        {
            continue;
        } 
        // note dir entry name
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        // check dir entry type
        if (stat(buf, &st) < 0)
        {
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        // if type is dir
        if (st.type == T_DIR)
        {
            find(buf, patten);
        }
        else if (patten == 0 || match(patten, de.name))
        {
            printf("%s\n", buf);
        }
    }
}

void main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "Usage: %s <dirname> <patten>\n", argv[0]);
        exit(1);
    }
    if (argc == 2)
    {
        find(argv[1], 0);
    }
    find(argv[1], argv[2]);
    exit(0);
}