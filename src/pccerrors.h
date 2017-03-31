#ifndef PCC_ERRORS_H
#define PCC_ERRORS_H

typedef enum pcc_error {
    INVALID_NAME  = 0,
    INVALID_LABEL  = 1,
    OUT_OF_MEMORY = 2,
    TOO_MANY_VALUES = 3,
} pcc_error;

#endif /* ifndef PCC_ERRORS_H */
