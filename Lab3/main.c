#include <stdio.h>
#include <time.h>
#include <string.h>
#include <malloc.h>

int mon_i(const char *month) {
    const char *all_months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12; i++) {
        if (strcmp(all_months[i], month) == 0) {
            return i;
        }
    }
    return 0;
}

struct tm string_parse(char *str, unsigned long long int *error_cnt) {
    struct tm t;
    char month[3];
    int day, year, hour, min, sec, status;
    sscanf(strchr(str, '['), "[%2d/%3s/%4d:%2d:%2d:%2d%", &day, &month, &year, &hour, &min, &sec);
    sscanf(strrchr(str, '\"'), "\" %3d", &status);
    if (status / 100 == 5) {
        (*error_cnt)++;
    }
    t.tm_year = year - 1900, t.tm_mon = mon_i(month), t.tm_mday = day, t.tm_hour = hour, t.tm_min = min, t.tm_sec = sec;
    t.tm_wday = 0;
    return t;
}


int main() {
    FILE *fin = fopen("access_log_Jul95.txt", "r");
    if (fin == NULL) {
        printf("The file is empty.");
        return 0;
    }
    long int i = -1;
    char s[400], *str;
    struct tm tm_a;
    long long int error_cnt = 0, max_request_cnt = -1;
    time_t user_tm_w, max_from, max_to, *time_arr;
    str = fgets(s, sizeof(s), fin);
    if (str==NULL){
        printf("The file is empty!");
        return 0;
    }
    printf_s("Set the time period (in seconds) for which you want to find the maximum number of requests\n>");

    user_tm_w=10;
    while (str != NULL) {
        i++;
        time_arr = realloc(time_arr, (i + 1) * sizeof(time_t));
        tm_a = string_parse(str, &error_cnt);
        time_arr[i] = mktime(&tm_a);
        if ((unsigned int) difftime(time_arr[i], time_arr[0]) > user_tm_w) {
            do{
                for (int j=0; j<i; j++){
                    time_arr[j]=time_arr[j+1];
                }
                i--;
            }while ((unsigned int) difftime(time_arr[i], time_arr[0]) > user_tm_w);
        }

        if (i+1 > max_request_cnt) {
            max_request_cnt = i+1;
            max_from = time_arr[0];
            max_to = time_arr[i];
        }
        str = fgets(s, sizeof(s), fin);
    }

    printf("%s%llu\n", ctime(&max_from), max_request_cnt);
    printf("%s%llu\n", ctime(&max_to));
    printf("There are %llu errors in file.", error_cnt);
    free(time_arr);
    //printf_s("%d/%d/%d %d:%d:%d\n",
    //         a.tm_year+1900, a.tm_mon + 1, a.tm_mday, a.tm_hour, a.tm_min, a.tm_sec)
    return 0;
}