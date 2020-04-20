#include "arc.h"

int arc(char* path, char* archivePath)
{
    if (!(archive = fopen(archivePath, "w"))){
        printf("arc: нет доступа к %s\n", archivePath);
        return 1;
    }

    fputs(ARC_UNIQUE_KEY, archive);
    int i;
    for (i = strlen(path)-1; i >= 0 && path[i] != '/'; i--);
    if (i >= 0) {
        rootDir = (char *) calloc(1, (i+2) * sizeof(char));
        strncpy(rootDir, path, i+1);
    }else{
        rootDir = (char *) calloc(1, 1 * sizeof(char));
        i = 0;
    }

    addToArchive(path+i);

    free(rootDir);
    fclose(archive);
    return 0;
}

int unarc(char* archivePath, char* path)
{
    DIR *dfd;
    char filekey[strlen(ARC_UNIQUE_KEY)+1];
    int id;
    char *name;
    char *fullpath;
    long size;

    register int c;
    int i;

    if (!(archive = fopen(archivePath, "rb"))){
        printf("unarc: нет доступа к %s\n", archivePath);
        return 1;
    }

    fgets(filekey, strlen(ARC_UNIQUE_KEY)+1, archive);
    if(strcmp(filekey, ARC_UNIQUE_KEY)){
        printf("unarc: указан неверный файл архива\n");
        fclose(archive);
        return 2;
    }

    if ((dfd = opendir(path)) == NULL){
        if (mkdir(path, 0777) == -1){
            printf("unarc: не удалось создать папку %s\n", path);
            return 3;
        }
    }else{
       closedir(dfd);
    }

    while (fread(&id, sizeof(int), 1, archive)){
        for (i = 0; (c=fgetc(archive)) != '\0'; i++);
        fseek(archive, -i, SEEK_CUR);
        name = calloc(1, i+1);
        fgets(name, i+1, archive);

        fullpath = calloc(1, strlen(path)+strlen(name)+2);
        sprintf(fullpath, "%s/%s", path, name);

        if (id == ARC_FILE_ID)
        {
            fread(&size, sizeof(long), 1, archive);
            writeFileFromArchive(fullpath, size);
        }else if (id == ARC_DIR_ID)
        {
            mkdir(fullpath, 0777);
        }

        free(fullpath);
        free(name);
    }

    return 0;
}

void addToArchive(char *path)
{
    int dir_id = ARC_DIR_ID;
    int file_id = ARC_FILE_ID;

    char *full_path;
    struct stat stbuf;

    full_path = (char *) calloc(strlen(rootDir)+strlen(path)+2, sizeof(char));
    sprintf(full_path, "%s%s",rootDir, path);

    if (stat(full_path, &stbuf) == -1) {
        free(full_path);
        printf("addToArchive: нет доступа к %s\n", path);
        return;
    }

    if (S_ISDIR(stbuf.st_mode))
        fwrite(&dir_id, sizeof(int), 1, archive);
    else
        fwrite(&file_id, sizeof(int), 1, archive);

    fputs(path, archive);
    fputc('\0', archive);

    if (S_ISDIR(stbuf.st_mode)) {
        dirwalk(path);
    }else{
        fwrite(&stbuf.st_size, sizeof(long), 1, archive);
        writeFileToArchive(full_path);
    }

    free(full_path);
}

void dirwalk(char *dir)
{
    char *full_dir_path;
    char *path;

    struct dirent *dp;
    DIR *dfd;

    full_dir_path = (char *) calloc(strlen(rootDir)+strlen(dir)+2, sizeof(char));
    sprintf(full_dir_path, "%s/%s",rootDir, dir);
    if ((dfd = opendir(full_dir_path)) == NULL) {
        free(full_dir_path);
        printf("dirwalk: не могу открыть %s\n", dir);
        return;
    }

    while ((dp = readdir(dfd)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        path = (char *) calloc(strlen(dir)+strlen(dp->d_name)+3, sizeof(char));
        sprintf(path, "%s/%s",dir, dp->d_name);
        addToArchive(path);
        free(path);
    }

    free(full_dir_path);
    closedir(dfd);
}

void writeFileToArchive(char* fileName)
{
    register int c;

    FILE *file;
    if (!(file = fopen(fileName, "rb"))){
        printf("addToArchive: нет доступа к %s\n", fileName);
        exit(1);
    }

    while ((c = fgetc(file))!= EOF) fputc(c, archive);

    fclose(file);
}

void writeFileFromArchive(char* fileName, long size)
{
    register int c;

    FILE *file;
    if (!(file = fopen(fileName, "wb"))){
        printf("unArchive: нет доступа к %s\n", fileName);
        exit(1);
    }

    while (size-- > 0) {
        c = fgetc(archive);
        fputc(c, file);
    }

    fclose(file);
}

