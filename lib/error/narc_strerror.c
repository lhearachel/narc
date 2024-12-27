#include <api/error.h>

const char *narc_strerror(enum narc_error error)
{
    switch (error) {
    default:
    case NARCERR_NONE:
        return "(null)";
    case NARCERR_MAGIC:
        return "Invalid magic marker in NARC header";
    case NARCERR_BOM:
        return "Invalid byte-order marker in NARC header";
    case NARCERR_VERSION:
        return "Invalid version marker in NARC header";
    case NARCERR_HEADER_SIZE:
        return "Invalid NARC header size";
    case NARCERR_NUM_SECTIONS:
        return "Invalid section count in NARC header";
    case NARCERR_ERRNO:
        return "Standard error; refer to errno for details";
    }
}
