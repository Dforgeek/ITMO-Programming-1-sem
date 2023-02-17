#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    uint32_t size;
    uint32_t fileCnt;
} arcHeader;

#pragma pack(push, 1)
typedef struct {
    char name[128];
    uint32_t size;
} fileHeader;
#pragma pack(pop)

void createArc(char *arcName, int argc, char *argv[]) {
    char c;
    fileHeader secondHead;
    arcHeader mainHeader;
    mainHeader.size = 0;
    mainHeader.fileCnt = argc - 4;
    FILE *archive = fopen(arcName, "wb");
    fwrite(&mainHeader, sizeof(arcHeader), 1, archive);
    for (int i = 4; i < argc; i++) {
        FILE *arcElem = fopen(argv[i], "rb");
        fseek(arcElem, 0, SEEK_END);
        secondHead.size = ftell(arcElem);
        fseek(arcElem, 0, SEEK_SET);
        mainHeader.size += secondHead.size;
        memset(secondHead.name, '\0', 128);

        for (int j = 0; j < strlen(argv[i]); j++) {
            secondHead.name[j] = argv[i][j];
        }
        fwrite(&secondHead, sizeof(fileHeader), 1, archive);
        c = getc(arcElem);
        while (!feof(arcElem)) {
            fputc(c, archive);
            c = getc(arcElem);
        }
        fclose(arcElem);
    }
    fseek(archive, 0, SEEK_SET);
    fwrite(&mainHeader, sizeof(arcHeader), 1, archive);
    fclose(archive);
}

void extract(char *arcName) {
    FILE *archive = fopen(arcName, "rb");
    fileHeader secondHead;
    arcHeader mainHeader;
    fread(&mainHeader, sizeof(arcHeader), 1, archive);
    char name[128] = {0}, c;
    for (int i = 0; i < mainHeader.fileCnt; i++) {
        fread(&secondHead, sizeof(fileHeader), 1, archive);
        for (int j = 0; j < 128; j++) {
            name[j] = secondHead.name[j];
        }
        FILE *file = fopen(name, "wb");
        for (int j = 0; j < secondHead.size; j++) {
            c = getc(archive);
            fputc(c, file);
        }
        fclose(file);
    }

}

void list(char *arcName) {
    FILE *archive = fopen(arcName, "rb");
    fileHeader secondHead;
    arcHeader mainHeader;
    fread(&mainHeader, sizeof(arcHeader), 1, archive);
    for (int i = 0; i < mainHeader.fileCnt; i++) {
        char name[128];
        fread(&secondHead, sizeof(fileHeader), 1, archive);
        fseek(archive, secondHead.size, SEEK_CUR);
        for (int j = 0; j < 128; j++) {
            name[j] = secondHead.name[j];
        }
        printf("%d) %s\n", i + 1, name);
    }
    fclose(archive);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Wrong amount of arguments.\n");
        return 1;
    }
    char *archiveName;
    for (int i = 0; i < argc; i++) {
        if (!strcmp("--file", argv[i])) {
            archiveName = argv[i + 1];
        }
        if (!strcmp("--create", argv[i])) {
            createArc(archiveName, argc, argv);
            break;
        }
        if (!strcmp("--extract", argv[i])) {
            extract(archiveName);
        }
        if (!strcmp("--list", argv[i])) {
            list(archiveName);
        }
    }
    return 0;
}
