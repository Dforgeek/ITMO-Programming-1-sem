#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <direct.h>


typedef uint32_t DWORD;
typedef uint16_t WORD;
typedef int32_t LONG;

#pragma pack(push, 1)
typedef struct {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD K1;
    DWORD K2;
    DWORD K3;
    DWORD K4;
} BITMAPINFOHEADER;
#pragma pack(pop)

//int edgeSolution(int **mtrx, int x, int y, int h, int w) {
//    if (y >= h) {
//        y = 0;
//    }
//    if (y < 0) {
//        y = h - 1;
//    };
//    if (x >= w) {
//        x = 0;
//    }
//    if (x < 0) {
//        x = w - 1;
//    }
//    return mtrx[y][x];
//}

//int envCheck(int **mtrx, int i, int j, int h, int w) {
//    int cnt = 0;
//    cnt += edgeSolution(mtrx, i - 1, j - 1, h, w);
//    cnt += edgeSolution(mtrx, i - 1, j, h, w);
//    cnt += edgeSolution(mtrx, i - 1, j + 1, h, w);
//    cnt += edgeSolution(mtrx, i, j - 1, h, w);
//    cnt += edgeSolution(mtrx, i, j + 1, h, w);
//    cnt += edgeSolution(mtrx, i + 1, j - 1, h, w);
//    cnt += edgeSolution(mtrx, i + 1, j, h, w);
//    cnt += edgeSolution(mtrx, i + 1, j + 1, h, w);
//    return cnt;
//}

void GameLife(int **mtrx, int h, int w) {
    int tmp[h][w];
    int envCnt;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            tmp[i][j] = mtrx[i][j];
        }
    }
    for (int i = 1; i < h - 1; i++) {
        for (int j = 1; j < w - 1; j++) {
            envCnt = mtrx[i - 1][j - 1] + mtrx[i - 1][j] + mtrx[i - 1][j + 1] + mtrx[i][j - 1] + mtrx[i][j + 1] +
                     mtrx[i + 1][j - 1] + mtrx[i + 1][j] + mtrx[i + 1][j + 1];

            if (mtrx[i][j] == 1 && (envCnt < 2 || envCnt > 3)) {
                tmp[i][j] = 0;
            } else if (mtrx[i][j] == 0 && envCnt == 3) {
                tmp[i][j] = 1;
            }
        }
    }
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            mtrx[i][j] = tmp[i][j];
        }
    }
}

void intToBin(int integer, int *binStr) {
    for (int i = 7; integer > 0; i--) {
        binStr[i] = integer % 2;
        integer /= 2;
    }
}

int binToInt(int *binStr) {
    int sum = 0;
    for (int i = 0; i < 8; i++) {
        if (binStr[i] == 0) {
            sum += (int) pow(2, 7 - i);
        }
    }
    return sum;
}


int main(int argc, char *argv[]) {
    int bmpDataInd = -1, binInd, maxIter = 1, genFreq = 1, outputFlag = 0, inputFlag = 1;
    FILE *origPic;
    BITMAPFILEHEADER file_h;
    BITMAPINFOHEADER info_h;
    unsigned char *bmpData;
    char *dir;
    int **matrix, byteStr[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    if (argc < 5 || argc % 2 == 0) {
        printf("Incorrect input.\n");
        return 1;
    }
    for (int i = 0; i < argc; i++) {
        if (strcmp("--input", argv[i]) == 0) {
            inputFlag = 1;
            origPic = fopen(argv[i + 1], "rb");
            if (origPic == NULL) {
                printf("Input file doesn't exist.\n");
                fclose(origPic);
                return 1;
            }
        }
        if (strcmp("--output", argv[i]) == 0) {
            outputFlag = 1;
            dir = argv[i + 1];
            mkdir(dir);
        }
        if (strcmp("--max_iter", argv[i]) == 0) {
            maxIter = strtol(argv[i + 1], 0, 10);
        }
        if (strcmp("--dump_freq", argv[i]) == 0) {
            genFreq = strtol(argv[i + 1], 0, 10);
        }
    }
    if (outputFlag + inputFlag != 2) {
        printf("Input/Output error.\n");
        return 1;
    }
    //считываем данные с картинки
    fread(&file_h, sizeof(BITMAPFILEHEADER), 1, origPic);
    if (file_h.bfType != 0x4D42) {
        printf("Incorrect format of .bmp file.\n");
        fclose(origPic);
        return 1;
    }
    fread(&info_h, sizeof(BITMAPINFOHEADER), 1, origPic);
    fseek(origPic, file_h.bfOffBits, 0);
    bmpData = malloc(info_h.biSizeImage);
    fread(bmpData, info_h.biSizeImage, 1, origPic);
    //создаём матрицу на которой происходит игра
    matrix = malloc(info_h.biHeight * sizeof(int *));
    for (int i = 0; i < info_h.biHeight; i++) {
        matrix[i] = malloc(info_h.biWidth * sizeof(int));
    }
    //переносим картинку на матрицу
    for (int i = info_h.biHeight - 1; i >= 0; i--) {
        binInd = 0;
        bmpDataInd++;
        memset(byteStr, 0, sizeof(int) * 8);
        intToBin(bmpData[bmpDataInd], byteStr);
        for (int j = 0; j < info_h.biWidth; j++) {
            if (byteStr[binInd] == 0) {
                matrix[i][j] = 1;
            } else {
                matrix[i][j] = 0;
            }
            binInd++;
            if (binInd % 8 == 0) {
                binInd = 0;
                bmpDataInd++;
                memset(byteStr, 0, sizeof(int) * 8);
                intToBin(bmpData[bmpDataInd], byteStr);
            }
        }
    }

    for (int k = 1; k <= maxIter; k++) {
        GameLife(matrix, info_h.biHeight, info_h.biWidth);
        if (k % genFreq == 0) {
            char path[50], elem[k];
            strcpy(path, dir);
            strcat(strcat(path, "\\"), strcat(itoa(k, elem, 10), ".bmp"));
            FILE *newPic = fopen(path, "wb");
            //переносим матрицу обратно в байтовый массив
            memset(byteStr, 0, sizeof(int) * 8);
            bmpDataInd = -1;
            for (int i = info_h.biHeight - 1; i >= 0; i--) {
                if (bmpDataInd != -1) {
                    bmpData[bmpDataInd] = binToInt(byteStr);
                }
                memset(byteStr, 0, sizeof(int) * 8);
                binInd = 0;
                bmpDataInd++;
                for (int j = 0; j < info_h.biWidth; j++) {
                    byteStr[binInd] = matrix[i][j];
                    binInd++;
                    if (binInd % 8 == 0) {
                        binInd = 0;
                        bmpData[bmpDataInd] = binToInt(byteStr);
                        bmpDataInd++;
                        memset(byteStr, 0, sizeof(int) * 8);
                    }
                }
            }
            //записываем все данные в новую картинку
            fwrite(&file_h, sizeof(BITMAPFILEHEADER), 1, newPic);
            fwrite(&info_h, sizeof(BITMAPINFOHEADER), 1, newPic);
            fseek(newPic, file_h.bfOffBits, 0);
            fwrite(bmpData, info_h.biSizeImage, 1, newPic);
            fclose(newPic);
        }
    }
    fclose(origPic);
    return 0;
}