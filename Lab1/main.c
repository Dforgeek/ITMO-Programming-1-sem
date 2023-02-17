#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        // Если количество аргументов меньше трёх требуемых, то выводим подсказку по формату ввода
        printf("Please follow this format of command: \'WordCont.exe [OPTION] filename\'");
    } else {
        int charr, prev_char = 0;
        FILE *fin;
        fin = fopen(argv[2], "r");
        // Описывается команда счётчика строк
        if (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--lines") == 0) {
            long line_cnt = 0;
            while ((charr = fgetc(fin)) != EOF) {
                if (charr == '\n') {
                    line_cnt++;
                }
                prev_char = charr;
            }
            // Проверяем, оканчивается ли текст переводом строки. Если нет, то прибавляем к счётчик оследнюю строку
            if (prev_char != '\n' && prev_char != 0) {
                line_cnt++;
            }
            printf("%ld lines in this file\n", line_cnt);
        }
            // Описывается команда счётчика слов
        else if (strcmp(argv[1], "-w") == 0 || strcmp(argv[1], "--words") == 0) {
            long word_cnt = 0;
            while ((charr = fgetc(fin)) != EOF) {
                if (charr == ' ' || charr == '\n') {
                    word_cnt++;
                }
                prev_char = charr;
            }
            if (prev_char != ' ' && prev_char != '\n' && prev_char != 0) {
                word_cnt++;
            }
            printf("%ld words in this file\n", word_cnt);
        }
            // Описывается команда подсчёта символов в кодировке ascii (8 бит) для вывода размера файла
        else if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--bytes") == 0) {
            long symbol_cnt = 0;
            while ((charr = fgetc(fin)) != EOF) {
                symbol_cnt++;
                prev_char = charr;
            }
            if (prev_char != 0) {
                symbol_cnt++;
            }
            printf("%ld bytes is a size of this file \n", symbol_cnt);
        }
            // При неправильном вводе выдаём подсказку по возможным командам и формату ввода
        else {
            printf("Please use one of the existing commands:\n"
                   "To count lines use \'-l\' or \'--lines\'\n"
                   "To count words use \'-w\' or \'--words\'\n"
                   "To get the size of a file use \'-c\' or \'--bytes\'\n"
                   "And follow this format of command: \'WordCont.exe [OPTION] filename\'\n");
        }
    }
    return 0;
}