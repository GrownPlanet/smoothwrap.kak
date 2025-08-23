#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char *data;
    int len;
} String_t;

int line_ends[50] = { 0 };
int line_ends_len = 1;
int x, y;

// check if a certain character can break for a newline
bool is_brakable_char(char ch) {
    switch (ch) {
        case ' ':
        case '(': case ')':
        case '[': case ']':
        case '{': case '}':
            return true;
        default:
            return false;
    }
}

// calculate the distance to the next space or end of word
int distance_to_boundry(String_t line, int x, int wrap_width) {
    if (is_brakable_char(line.data[x])) return 1;

    for (int i = x; i < line.len; i++) {
        if (is_brakable_char(line.data[i])) {
            int dist = i - x;
            return dist > wrap_width ? wrap_width : dist;
        }
    }

    int dist = line.len - x;
    return dist > wrap_width ? wrap_width : dist;
}

// calculate the positions where the line wraps
void calc_line_ends(String_t line, int wrap_width) {
    int position = 0;
    int visual_column = 0;

    while (position < line.len) {
        int dist = distance_to_boundry(line, position, wrap_width);
        position += dist;
        visual_column += dist;
        if (visual_column > wrap_width) {
            line_ends[line_ends_len] = position - dist;
            line_ends_len++;
            visual_column = dist;
        }
    }

    line_ends[line_ends_len] = line.len;
    line_ends_len++;
}

// turn the colmn into an x, y position
void column_to_position(int column) {
    for (int i = 0; i < line_ends_len - 1; i++) {
        if (line_ends[i] <= column && column < line_ends[i + 1]) {
            x = column - line_ends[i] + 1;
            y = i + 1;
            return;
        }
    }
}

int position_to_column(int x, int y) {
    int mapped_x = x;
    if (x >= line_ends[y] - line_ends[y - 1]) {
        mapped_x = line_ends[y] - line_ends[y - 1];
    }
    return line_ends[y - 1] + mapped_x;
}

int main(int argc, char *argv[]) {
    if (argc < 5) return 1;

    char* movement = argv[1];
    int column = atoi(argv[2]) - 1;
    String_t line = {
        .data = argv[3],
        .len = strlen(argv[3])
    };
    int wrap_width = atoi(argv[4]);

    calc_line_ends(line, wrap_width);
    column_to_position(column);

    if (strcmp(movement, "down") == 0) {
        printf("gh");
        int pos = position_to_column(x, y + 1);
        if (y + 1 != line_ends_len) {
            printf("%dl", pos - 1);
        } else {
            printf(": set-option global go_to_next_line true\n");
            printf(": set-option global visual_column %d\n", x);
            fprintf(stderr, "set: %d\n", x);
            printf("j");
        }
    }

    else if (strcmp(movement, "up") == 0) {
        printf("gh");
        int pos = position_to_column(x, y - 1);
        if (y > 1) {
            if (pos > 1) printf("%dl", pos - 1);
        } else {
            printf(": set-option global go_to_previous_line true\n");
            printf(": set-option global visual_column %d\n", x);
            printf("k");
        }
    }

    else if (strcmp(movement, "next") == 0) {
        int pos = position_to_column(column + 1, 1);
        printf("gh");
        if (pos != 1) printf("%dl", pos - 1);
    }

    else if (strcmp(movement, "previous") == 0) {
        int pos = position_to_column(x, line_ends_len - 1);
        printf("gh");
        if (pos != 1) printf("%dl", pos - 1);
    }
}
