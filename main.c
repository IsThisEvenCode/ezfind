#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "linked_list.h"

/*** defines ***/
#define FILE_MEM_ERR 11

/*** data ***/

enum file_type { FT_FILE, FT_DIR, FT_UNKNOWN };

struct File {
    char name[NAME_MAX + 1];
    char path[PATH_MAX + 1];
    enum file_type type;
};

struct DataPool {
    struct LinkedList* file_list;
    struct LinkedList* dir_list;
    char* path;
    DIR* dir;
};

/*** functions ***/

struct File* record_file(struct dirent* entr, char* path, enum file_type type) {
    struct File* file = malloc(sizeof(struct File));
    if (file == NULL) {
        errno = FILE_MEM_ERR;
        return NULL;
    }

    strcpy(file->name, entr->d_name);
    strcpy(file->path, path);
    if (type == FT_FILE) {
        file->type = FT_FILE;
    } else if (type == FT_DIR) {
        file->type = FT_DIR;
    } else {
        file->type = FT_UNKNOWN;
    }

    return file;
}

void sort_dir_entries(struct DataPool* dp) {
// Iterate through dir entries
    struct dirent* entr;
    while ((entr = readdir(dp->dir)) != NULL) {
        if (strcmp(".", entr->d_name) == 0 || strcmp("..", entr->d_name) == 0) {
            continue;
        }

        // Sort entry by type
        if (entr->d_type == DT_DIR) {
            struct File* file = record_file(entr, dp->path, FT_FILE);
            llist_append_node(dp->dir_list, file);
        }
        else if (entr->d_type == DT_REG) {
            struct File* file = record_file(entr, dp->path, FT_DIR);
            llist_append_node(dp->file_list, file);
        }
    }
}

void print_dir(DIR* dir) {
    struct dirent* entr;
    while ((entr = readdir(dir)) != NULL) {
        if (strcmp(".", entr->d_name) == 0 || strcmp("..", entr->d_name) == 0) {
            continue;
        }
        printf("%s\n", entr->d_name);
    }
}

void find_in_dir(char* s, DIR* dir) {

}

int main() {
    struct LinkedList file_list;
    struct LinkedList dir_list;
    llist_init(&file_list, sizeof(struct File));
    llist_init(&dir_list, sizeof(struct File));

    DIR* dir = opendir("/home/markus");
    if (dir == NULL) {
        perror("Can't find or open the directory!\n");
    }

    // Fill in data pool for universal access
    struct DataPool dp = {&file_list, &dir_list, "/home/markus", dir};

    sort_dir_entries(&dp);

    void* file_arr = llist_return_arr(dp.file_list);
    for (int i = 0; i < file_list.count; i++) {
        struct File *file = file_arr + i * file_list.data_size;
        printf("%s\n", file->name);
    }

    printf("Error: %s", llist_strerror(errno));


    closedir(dir);

}
