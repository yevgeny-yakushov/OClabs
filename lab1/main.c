#include "arc.h"
#include "stdio.h"

int main(int argc, char* argv[])
{
    if(!strcmp(argv[1], "--help")){
            printf("--------------------------------------------------------\n");
            printf("Для того, чтобы произвести архивацию, введите команду:\n -arc *путь_к_архивируемой_директории* *имя_архива*\n");
            printf("Для того, чтобы произвести разархивацию, введите команду:\n -unarc *имя_архива* *путь_разархивации*\n");
            printf("--------------------------------------------------------\n");
            return 0;
    }

    if (argc < 4)
    {
        printf("Слишком мало аргументов!\n");
        return 1;
    }

    if (!strcmp(argv[1], "-arc")){
        return arc(argv[2], argv[3]);
    }else if(!strcmp(argv[1], "-unarc")){
        return unarc(argv[2], argv[3]);
    }else {
        printf("Что-то ты намудрил с параметрами...\n");
        return 1;
    }

    return 1;
}
