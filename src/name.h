#ifndef PCC_LABEL_H
#define PCC_LABEL_H
#include <stdbool.h>

static inline bool
validate_name(const char *name) {
    char c = *name;
    while(c) {
        if ((c >= 'A' && c <= 'Z') ||
                (c >= 'a' && c <= 'z') ||
                (c >= '0' && c <= '9') ||
                (c == '_') ||
                (c == ':')) {
            c = *(++name);
            continue;
        }

        return false;
    }

    return true;
}

static inline bool
validate_label(const char *name) {
    char c = *name;
    while(c) {
        if ((c >= 'A' && c <= 'Z') ||
                (c >= 'a' && c <= 'z') ||
                (c >= '0' && c <= '9') ||
                (c == '_')) {
            c = *(++name);
            continue;
        }

        return false;
    }

    return true;
}

#endif /* ifndef PCC_LABEL_H */
