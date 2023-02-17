#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

unsigned long long int UINT_MAXX = 4294967296;

struct uint_1024_t {
    unsigned int uint_1024[32];
};

void uint_to_hex(unsigned int n, bool last_digit) {
    unsigned int temp;
    char hex[8];
    for (int i = 7; i >= 0; i--) {
        temp = n % 16;
        hex[i] = temp;
        n = n / 16;
    }
    int k = 0, j;
    if (last_digit == true) {
        for (j = 0; j < 8; j++) {
            if (hex[j] != 0) {
                break;
            }
        }
        k = j;
    }

    for (int i = k; i < 8; i++) {
        temp = hex[i];
        if (temp == 15) {
            printf("F");
        } else if (temp == 14) {
            printf("E");
        } else if (temp == 13) {
            printf("D");
        } else if (temp == 12) {
            printf("C");
        } else if (temp == 11) {
            printf("B");
        } else if (temp == 10) {
            printf("A");
        } else {
            printf("%d", temp);
        }
    }
}

bool is_1_bigger_than_2(struct uint_1024_t x, struct uint_1024_t y) {
    for (int i = 31; i >= 0; i--) {
        if (x.uint_1024[i] >= y.uint_1024[i]) {
            return true;
        } else if (x.uint_1024[i] < y.uint_1024[i]) {
            return false;
        }
    }
    return false;
}

int uint_1024_size(struct uint_1024_t x) {
    int size = 0, zeros_cnt = 0;
    for (int i = 0; i < 32; i++) {
        if (x.uint_1024[i] == 0) {
            zeros_cnt++;
        } else {
            size += zeros_cnt + 1;
            zeros_cnt = 0;
        }
    }
    return size;
}

struct uint_1024_t move_digit(struct uint_1024_t x, int n) {
    struct uint_1024_t result;
    for (int i = 0; i < 32; i++) {
        result.uint_1024[i] = 0;
    }
    int size_of_num = uint_1024_size(x);
    for (int i = 0; i < size_of_num; i++) {
        result.uint_1024[i + n] = x.uint_1024[i];
    }
    return result;
}

struct uint_1024_t from_uint(unsigned int a) {
    struct uint_1024_t num;
    for (int i = 0; i < 32; i++) {
        num.uint_1024[i] = 0;
    }
    num.uint_1024[0] = a;
    return num;
}

struct uint_1024_t add_op(struct uint_1024_t x, struct uint_1024_t y) {
    struct uint_1024_t result;
    unsigned int next_pos = 0;
    unsigned long long int sum, temp_long;
    for (int i = 0; i < 32; i++) {
        temp_long = x.uint_1024[i];
        sum = temp_long + y.uint_1024[i] + next_pos;
        result.uint_1024[i] = sum % UINT_MAXX;
        next_pos = sum / UINT_MAXX;
    }
    if (next_pos == 0) {
        return result;
    } else {
        printf("Undefined Behaviour in add_op function!\n");
        return result;
    }
}

struct uint_1024_t subtr_op(struct uint_1024_t x, struct uint_1024_t y) {
    struct uint_1024_t result;
    unsigned int prev_pos = 0;
    long long int sub_res, temp_long;
    if (is_1_bigger_than_2(x, y) == true) {
        for (int i = 0; i < 32; i++) {
            temp_long = x.uint_1024[i];
            sub_res = temp_long - y.uint_1024[i] - prev_pos;
            if (sub_res < 0) {
                prev_pos = 1;
                result.uint_1024[i] = UINT_MAXX + sub_res;
            } else {
                prev_pos = 0;
                result.uint_1024[i] = sub_res;
            }
        }
    } else {
        printf("Your first number is smaller than the second!\n Uint_1024 was not changed.");
        return x;
    }
    return result;
}

struct uint_1024_t mult_op(struct uint_1024_t x, struct uint_1024_t y) {
    struct uint_1024_t result, temp_sum;
    unsigned long long int mult_sum, temp_long, next_pos = 0;
    for (int i = 0; i < 32; i++) {
        result.uint_1024[i] = 0;
    }
    for (int i = 0; i < 32; i++) {
        temp_long = y.uint_1024[i];
        for (int j = 0; j < 32; j++) {
            mult_sum = temp_long * x.uint_1024[j] + next_pos;
            next_pos = mult_sum / UINT_MAXX;
            temp_sum.uint_1024[j] = mult_sum % UINT_MAXX;
        }
        if (next_pos != 0) {
            printf("Undefined Behaviour in mult_op function!\n");

        }
        result = add_op(result, move_digit(temp_sum, i));
    }
    return result;
}

void printf_value(struct uint_1024_t x) {
    bool is_num_started = false, highest_digit = true;
    for (int i = 31; i >= 0; i--) {
        if (x.uint_1024[i] != 0) {
            is_num_started = true;
        }
        if (is_num_started) {
            uint_to_hex(x.uint_1024[i], highest_digit);
            highest_digit = false;

        }
    }
    printf("\n");
}

void scanf_value(struct uint_1024_t *x) {
    char str[257], temp;

    scanf("%s", str);
    for (int i = 0; i < strlen(str); i++) {
        temp = str[i];
        if (temp == 'F') {

            str[i] = 15;
        } else if (temp == 'E') {
            str[i] = 14;
        } else if (temp == 'D') {
            str[i] = 13;
        } else if (temp == 'C') {
            str[i] = 12;
        } else if (temp == 'B') {
            str[i] = 11;
        } else if (temp == 'A') {
            str[i] = 10;
        } else if (temp == '9') {
            str[i] = 9;
        } else if (temp == '8') {
            str[i] = 8;
        } else if (temp == '7') {
            str[i] = 7;
        } else if (temp == '6') {
            str[i] = 6;
        } else if (temp == '5') {
            str[i] = 5;
        } else if (temp == '4') {
            str[i] = 4;
        } else if (temp == '3') {
            str[i] = 3;
        } else if (temp == '2') {
            str[i] = 2;
        } else if (temp == '1') {
            str[i] = 1;
        } else if (temp == '0') {
            str[i] = 0;
        }
    }
    for (int i = 31; i >= 0; i--) {
        x->uint_1024[i] = 0;
    }
    int j = 0;
    for (int i = strlen(str) - 1; i >= 0; i--) {
        x->uint_1024[j / 8] += str[i] * lround(pow(16, j%8));
        j++;
    }
}

int main() {
    struct uint_1024_t mega_n, sum, a, b;
    unsigned int n = 4294967295;
    char str[32];
    mega_n = from_uint(n);
    scanf_value(&a);
    scanf_value(&b);
    sum = mult_op(a, b);
    printf_value(sum);

    return 0;
}