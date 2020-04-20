#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>

#define MAX_DIR_NAME 1024
#define ARC_DIR_ID 1
#define ARC_FILE_ID 2
#define ARC_UNIQUE_KEY "myarchivationsuperutilita"

void dirwalk(char *);

int arc(char *, char *);

int unarc(char *, char *);

void addToArchive(char *);

void writeFileFromArchive(char *, long);

void writeFileToArchive(char *);

char *rootDir;

FILE* archive;
