#pragma once

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h> /* NULL */
#include <stdint.h>

#include "atb/export.h"

#if defined(__cplusplus)
extern "C" {
#endif

/// Underlying Error Category type
typedef uint8_t atb_ErrorCategory_t;

/// Underlying Error Code type
typedef int32_t atb_ErrorCode_t;

/// Error described using an error code (int) and an error category, use to
/// describe and distinguish code values between different domains.
struct atb_Error {
  union {
    struct {
      atb_ErrorCategory_t category; /*!< TODO */
      atb_ErrorCode_t code;         /*!< TODO */
    };
    uint64_t raw; /*!< TODO */
  };
};

static_assert(sizeof(struct atb_Error) == sizeof(uint64_t), "");

/* Format string helper ****************************************************/

#define K_ATB_ERROR_FMT_RAW "%#.8X"
#define atb_Error_FMT_RAW_VA_ARG(err) (err).raw

#define K_ATB_ERROR_FMT \
  "{.raw=" K_ATB_ERROR_FMT_RAW " -> .category=%#.2X, .code=%i}"
#define atb_Error_FMT_VA_ARG(err) \
  atb_Error_FMT_RAW_VA_ARG(err), (err).category, (err).code

/* Constant ****************************************************************/

/// Sentinel value used to signal that we wish to ignore an error when calling a
/// function
#define K_ATB_ERROR_IGNORED NULL

/* Operations **************************************************************/

/**
 *  \brief TODO
 *
 *  \param[in] category
 *
 *  \return bool TODO
 */
extern bool atb_ErrorCategory_HasFormatter(atb_ErrorCategory_t category)
    ATB_PUBLIC;

/// TODO
struct atb_ErrorFormatter {
  void *data;
  bool (*Describe)(void *data, atb_ErrorCode_t code, char *d_first,
                   size_t d_size, size_t *written); /*!< TODO */
};

/**
 *  \brief TODO
 *
 *  \param[in] category
 *  \param[in] name
 *  \param[in] fmt
 *  \param[out] err
 *
 *  \return bool TODO
 */
extern bool atb_ErrorCategory_AddFormatter(
    atb_ErrorCategory_t category, const char *name,
    struct atb_ErrorFormatter fmt, struct atb_Error *const err) ATB_PUBLIC;

/**
 *  \brief TODO
 *
 *  \param[in] category TODO
 *
 *  \return bool TODO
 */
extern bool atb_ErrorCategory_RemoveFormatter(atb_ErrorCategory_t category)
    ATB_PUBLIC;

/**
 *  \return bool True whenever the error is IGNORED. False otherwise.
 *  \important An ignored error is not usable what so ever.
 */
static inline bool atb_Error_IsIgnored(struct atb_Error const *const self);

/**
 *  \brief Set the underlying error to the category/code pair, IFF the error is
 *         not ignored.
 *
 *  \param[in] cat The new category associated to the error code
 *  \param[in] code The corresponding error code of this error
 */
static inline void atb_Error_Set(struct atb_Error *const self,
                                 atb_ErrorCategory_t cat, atb_ErrorCode_t code);

/**
 *  \brief Describe the underlying error (if possible) into the destination
 *         string buffer pointed by [d_first, d_first + d_size)
 *
 *  If the category is KNOWN (i.e. category's formatter exists),
 *  the description often takes the following format:
 *  "<CATEGORY NAME> (<CODE VALUE>): <CODE DESCRIPTION>"
 *  If the error system doesn't have any formatter for the given category, the
 *  output format is simply:
 *  "0x<CATEGORY HEX VALUE>: <CODE VALUE>"
 *
 *  \important This function DOESN'T ADD NULL CHAR ('\0') at the end of the
 *             dest buffer, NOR does it include the NULL char within its return
 *             count value.
 *
 *  \note Giving a NULL ptr as \a d_first still provides the max number of char
 *        needed to ouput the description (same as snprintf functions).
 *
 *  \param [in] d_first Begin of the destination string buffer.
 *                      If NULL, update \a written with the expected size of
 *                      the description.
 *  \param [in] d_size Size of the destination string buffer
 *  \param [out] written Number of bytes written into d_first
 *
 *  \pre written != NULL
 *
 *  \return bool True if the description could be written into d_first (written
 *               is updated accordinlgy). False when d_size is too small. The
 *               destination buffer is left unchanged in case of faillure.
 */
extern bool atb_Error_Describe(struct atb_Error const *const self,
                               char *d_first, size_t d_size,
                               size_t *written) ATB_PUBLIC;

/***************************************************************************/
/*                      PRE-DEFINED Error categories                       */
/***************************************************************************/

/// TODO
#define K_ATB_ERROR_RAW 0

/// TODO
#define K_ATB_ERROR_GENERIC K_ATB_ERROR_RAW + 1

/* RAW - (No category) *******************************************************/

/// TODO
static inline void atb_RawError_Set(struct atb_Error *const self,
                                    atb_ErrorCode_t code);

/* GENERIC - (errno) *******************************************************/

/// TODO
typedef enum {
  K_ATB_ERROR_GENERIC_SUCCESS = 0,
  K_ATB_ERROR_GENERIC_ADDRESS_FAMILY_NOT_SUPPORTED = EAFNOSUPPORT,
  K_ATB_ERROR_GENERIC_ADDRESS_IN_USE = EADDRINUSE,
  K_ATB_ERROR_GENERIC_ADDRESS_NOT_AVAILABLE = EADDRNOTAVAIL,
  K_ATB_ERROR_GENERIC_ALREADY_CONNECTED = EISCONN,
  K_ATB_ERROR_GENERIC_ARGUMENT_LIST_TOO_LONG = E2BIG,
  K_ATB_ERROR_GENERIC_ARGUMENT_OUT_OF_DOMAIN = EDOM,
  K_ATB_ERROR_GENERIC_BAD_ADDRESS = EFAULT,
  K_ATB_ERROR_GENERIC_BAD_FILE_DESCRIPTOR = EBADF,
  K_ATB_ERROR_GENERIC_BAD_MESSAGE = EBADMSG,
  K_ATB_ERROR_GENERIC_BROKEN_PIPE = EPIPE,
  K_ATB_ERROR_GENERIC_CONNECTION_ABORTED = ECONNABORTED,
  K_ATB_ERROR_GENERIC_CONNECTION_ALREADY_IN_PROGRESS = EALREADY,
  K_ATB_ERROR_GENERIC_CONNECTION_REFUSED = ECONNREFUSED,
  K_ATB_ERROR_GENERIC_CONNECTION_RESET = ECONNRESET,
  K_ATB_ERROR_GENERIC_CROSS_DEVICE_LINK = EXDEV,
  K_ATB_ERROR_GENERIC_DESTINATION_ADDRESS_REQUIRED = EDESTADDRREQ,
  K_ATB_ERROR_GENERIC_DEVICE_OR_RESOURCE_BUSY = EBUSY,
  K_ATB_ERROR_GENERIC_DIRECTORY_NOT_EMPTY = ENOTEMPTY,
  K_ATB_ERROR_GENERIC_EXECUTABLE_FORMAT_ERROR = ENOEXEC,
  K_ATB_ERROR_GENERIC_FILE_EXISTS = EEXIST,
  K_ATB_ERROR_GENERIC_FILE_TOO_LARGE = EFBIG,
  K_ATB_ERROR_GENERIC_FILENAME_TOO_LONG = ENAMETOOLONG,
  K_ATB_ERROR_GENERIC_FUNCTION_NOT_SUPPORTED = ENOSYS,
  K_ATB_ERROR_GENERIC_HOST_UNREACHABLE = EHOSTUNREACH,
  K_ATB_ERROR_GENERIC_IDENTIFIER_REMOVED = EIDRM,
  K_ATB_ERROR_GENERIC_ILLEGAL_BYTE_SEQUENCE = EILSEQ,
  K_ATB_ERROR_GENERIC_INAPPROPRIATE_IO_CONTROL_OPERATION = ENOTTY,
  K_ATB_ERROR_GENERIC_INTERRUPTED = EINTR,
  K_ATB_ERROR_GENERIC_INVALID_ARGUMENT = EINVAL,
  K_ATB_ERROR_GENERIC_INVALID_SEEK = ESPIPE,
  K_ATB_ERROR_GENERIC_IO_ERROR = EIO,
  K_ATB_ERROR_GENERIC_IS_A_DIRECTORY = EISDIR,
  K_ATB_ERROR_GENERIC_MESSAGE_SIZE = EMSGSIZE,
  K_ATB_ERROR_GENERIC_NETWORK_DOWN = ENETDOWN,
  K_ATB_ERROR_GENERIC_NETWORK_RESET = ENETRESET,
  K_ATB_ERROR_GENERIC_NETWORK_UNREACHABLE = ENETUNREACH,
  K_ATB_ERROR_GENERIC_NO_BUFFER_SPACE = ENOBUFS,
  K_ATB_ERROR_GENERIC_NO_CHILD_PROCESS = ECHILD,
  K_ATB_ERROR_GENERIC_NO_LINK = ENOLINK,
  K_ATB_ERROR_GENERIC_NO_LOCK_AVAILABLE = ENOLCK,
  K_ATB_ERROR_GENERIC_NO_MESSAGE_AVAILABLE = ENODATA,
  K_ATB_ERROR_GENERIC_NO_MESSAGE = ENOMSG,
  K_ATB_ERROR_GENERIC_NO_PROTOCOL_OPTION = ENOPROTOOPT,
  K_ATB_ERROR_GENERIC_NO_SPACE_ON_DEVICE = ENOSPC,
  K_ATB_ERROR_GENERIC_NO_SUCH_DEVICE_OR_ADDRESS = ENXIO,
  K_ATB_ERROR_GENERIC_NO_SUCH_DEVICE = ENODEV,
  K_ATB_ERROR_GENERIC_NO_SUCH_FILE_OR_DIRECTORY = ENOENT,
  K_ATB_ERROR_GENERIC_NO_SUCH_PROCESS = ESRCH,
  K_ATB_ERROR_GENERIC_NOT_A_DIRECTORY = ENOTDIR,
  K_ATB_ERROR_GENERIC_NOT_A_SOCKET = ENOTSOCK,
  K_ATB_ERROR_GENERIC_NOT_CONNECTED = ENOTCONN,
  K_ATB_ERROR_GENERIC_NOT_ENOUGH_MEMORY = ENOMEM,
  K_ATB_ERROR_GENERIC_NOT_SUPPORTED = ENOTSUP,
  K_ATB_ERROR_GENERIC_OPERATION_CANCELED = ECANCELED,
  K_ATB_ERROR_GENERIC_OPERATION_IN_PROGRESS = EINPROGRESS,
  K_ATB_ERROR_GENERIC_OPERATION_NOT_PERMITTED = EPERM,
  K_ATB_ERROR_GENERIC_OPERATION_NOT_SUPPORTED = EOPNOTSUPP,
  K_ATB_ERROR_GENERIC_OPERATION_WOULD_BLOCK = EWOULDBLOCK,
  K_ATB_ERROR_GENERIC_OWNER_DEAD = EOWNERDEAD,
  K_ATB_ERROR_GENERIC_PERMISSION_DENIED = EACCES,
  K_ATB_ERROR_GENERIC_PROTOCOL_ERROR = EPROTO,
  K_ATB_ERROR_GENERIC_PROTOCOL_NOT_SUPPORTED = EPROTONOSUPPORT,
  K_ATB_ERROR_GENERIC_READ_ONLY_FILE_SYSTEM = EROFS,
  K_ATB_ERROR_GENERIC_RESOURCE_DEADLOCK_WOULD_OCCUR = EDEADLK,
  K_ATB_ERROR_GENERIC_RESOURCE_UNAVAILABLE_TRY_AGAIN = EAGAIN,
  K_ATB_ERROR_GENERIC_RESULT_OUT_OF_RANGE = ERANGE,
  K_ATB_ERROR_GENERIC_STATE_NOT_RECOVERABLE = ENOTRECOVERABLE,
  K_ATB_ERROR_GENERIC_TEXT_FILE_BUSY = ETXTBSY,
  K_ATB_ERROR_GENERIC_TIMED_OUT = ETIMEDOUT,
  K_ATB_ERROR_GENERIC_TOO_MANY_FILES_OPEN_IN_SYSTEM = ENFILE,
  K_ATB_ERROR_GENERIC_TOO_MANY_FILES_OPEN = EMFILE,
  K_ATB_ERROR_GENERIC_TOO_MANY_LINKS = EMLINK,
  K_ATB_ERROR_GENERIC_TOO_MANY_SYMBOLIC_LINK_LEVELS = ELOOP,
  K_ATB_ERROR_GENERIC_VALUE_TOO_LARGE = EOVERFLOW,
  K_ATB_ERROR_GENERIC_WRONG_PROTOCOL_TYPE = EPROTOTYPE,

  /* DEPRECATED */
  K_ATB_ERROR_GENERIC_NO_MSG_AVAILABLE = ENODATA,
  K_ATB_ERROR_GENERIC_NO_STREAM_RESOURCES = ENOSR,
  K_ATB_ERROR_GENERIC_NOT_A_STREAM = ENOSTR,
  K_ATB_ERROR_GENERIC_STREAM_TIMEOUT = ETIME,
} ATB_ERROR_GENERIC;

/// TODO
static inline void atb_GenericError_Set(struct atb_Error *const self,
                                        ATB_ERROR_GENERIC code);

/*****************************************************************************/
/*                         STATIC INLINE DEFINITIONS                         */
/*****************************************************************************/

static inline bool atb_Error_IsIgnored(struct atb_Error const *const self) {
  return self == K_ATB_ERROR_IGNORED;
}

static inline void atb_Error_Set(struct atb_Error *const self,
                                 atb_ErrorCategory_t cat,
                                 atb_ErrorCode_t code) {
  if (!atb_Error_IsIgnored(self)) {
    self->category = cat;
    self->code = code;
  }
}

static inline void atb_RawError_Set(struct atb_Error *const self,
                                    atb_ErrorCode_t code) {
  atb_Error_Set(self, K_ATB_ERROR_RAW, code);
}

static inline void atb_GenericError_Set(struct atb_Error *const self,
                                        ATB_ERROR_GENERIC code) {
  atb_Error_Set(self, K_ATB_ERROR_GENERIC, (atb_ErrorCode_t)code);
}

#if defined(__cplusplus)
} /* extern "C" */
#endif
