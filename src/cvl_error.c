#include <cvl/cvl_error.h>

// todo: this felt like a good idea until I started implementing it...

// Maps status codes to status messages.
const char *cvl_status_string(cvl_status_t s) {
    switch (s) {
        case CVL_OK:                return "OK";
        case CVL_ERR_INVALID_ARG:   return "Invalid argument";
        case CVL_ERR_INVALID_DIM:   return "Invalid dimensions";
        case CVL_ERR_TYPE_MISMATCH: return "Type mismatch";
        case CVL_ERR_OUT_OF_MEMORY: return "Out of memory";
        case CVL_ERR_FILE_IO:       return "File I/O error";
        case CVL_ERR_EMPTY:         return "Empty data";
        case CVL_ERR_OUT_OF_RANGE:  return "Out of range";
        case CVL_ERR_INTERNAL:      return "Internal error";
        default:                    return "Unknown error";
    }
}

// Print status message and return status code.
cvl_status_t cvl_perrorf(cvl_status_t code, const char *fmt, ...) {
    if (code == CVL_OK) return code;

    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "[CVL ERROR] (%s): ", cvl_status_string(code));
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");

    va_end(args);

    return code;
}
