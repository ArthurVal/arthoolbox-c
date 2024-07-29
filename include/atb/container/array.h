#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

/**
 *  \return The size N of a static array a[N].
 *
 *  Example:
 *
 *  \code
 *  int main(int argc, char** argv) {
 *
 *    int some_ints[] = {1,2,3,4,5,6};
 *    float some_floats[10];
 *
 *    puts("- Begin");
 *    printf("Size ints   = %d\n", atb_Array_GetSize(some_ints));
 *    printf("Size floats = %d\n", atb_Array_GetSize(some_floats));
 *    puts("- End");
 *
 *    return 0;
 *  }
 *  \endcode
 *
 *  STDOUT Output:
 *  - Begin
 *  Size ints   = 6
 *  Size floats = 10
 *  - End
 *
 *  \param[in] array Static C array (size known at compile time, do not work on
 *                   raw pointers)
 */
#define atb_Array_GetSize(array) (sizeof((array)) / sizeof(array[0]))

/**
 *  \return True if idx is in the range of the given static array (i.e. 0 <= idx
 *          < N), false otherwise.
 *
 *  \note usefull for assert statements
 *
 *  Example:
 *  \code
 *  int main(int argc, char** argv) {
 *
 *    float my_array[20];
 *
 *    puts("- Begin");
 *    printf("idx = -20 -> %d\n", atb_Array_IsInRangeOf(my_array, -20));
 *    printf("idx =   0 -> %d\n", atb_Array_IsInRangeOf(my_array, 0));
 *    printf("idx =  10 -> %d\n", atb_Array_IsInRangeOf(my_array, 10));
 *    printf("idx =  20 -> %d\n", atb_Array_IsInRangeOf(my_array, 20));
 *    printf("idx =  40 -> %d\n", atb_Array_IsInRangeOf(my_array, 40));
 *    puts("- End");
 *
 *    return 0;
 *  }
 *  \endcode
 *
 *  STDOUT Output:
 *  - Begin
 *  idx = -20 -> 0
 *  idx =   0 -> 1
 *  idx =  10 -> 1
 *  idx =  20 -> 0
 *  idx =  40 -> 0
 *  - End
 *
 *  \param[in] array Static C array (size known at compile time, do not work on
 *                   raw pointers)
 *  \param[in] idx Index we wish to check for
 */
#define atb_Array_IsInRangeOf(array, idx)                                      \
  ((0 <= (idx)) && ((idx) < atb_Array_GetSize(array)))

/**
 *  \return The address of the FIRST element of a static array
 *
 *  \note Mainly used for iterating over the array
 *
 *  \param[in] array Static C array (size known at compile time, do not work on
 *                   raw pointers)
 */
#define atb_Array_Begin(array) array

/**
 *  \return The address of the ONE PAST THE LAST element of a static array
 *
 *  \note Mainly used for iterating over the array
 *
 *  \warning This address cannot be dereferenced
 *
 *  \param[in] array Static C array (size known at compile time, do not work on
 *                   raw pointers)
 */
#define atb_Array_End(array) atb_Array_Begin(array) + atb_Array_GetSize(array)

/**
 *  \brief Alias used to do a for loop on all elements of a static C array
 *
 *  Example:
 *
 *  \code
 *  int main(int argc, char** argv) {
 *
 *    int may_static_array[] = {1,2,3,4,5,6};
 *
 *    puts("- Begin");
 *
 *    int *elem = NULL;
 *    atb_Array_ForEach(elem, my_static_array) {
 *      printf("%d, ", *elem);
 *    }
 *
 *    puts("\n- End");
 *
 *    return 0;
 *  }
 *  \endcode
 *
 *  STDOUT Output:
 *  - Begin
 *  1, 2, 3, 4, 5, 6,
 *  - End
 *
 *  \param[out] elem Raw pointer of the an array elements used to iterate
 *  \param[in] array Static C array (size known at compile time, do not work on
 *                   raw pointers)
 */
#define atb_Array_ForEach(elem, array)                                         \
  for (elem = atb_Array_Begin(array); elem != atb_Array_End(array); ++elem)

/**
 *  \return The address of the LAST element of a static array
 *
 *  \note Mainly used for reverse iterating over the array
 *
 *  \param[in] array Static C array (size known at compile time, do not work on
 *                   raw pointers)
 */
#define atb_Array_BeginR(array) atb_Array_End(array) - 1

/**
 *  \return The address of the ONE BEFORE THE FIRST element of a static array
 *
 *  \note Mainly used for iterating over the array
 *
 *  \warning This address cannot be dereferenced
 *
 *  \param[in] array Static C array (size known at compile time, do not work on
 *                   raw pointers)
 */
#define atb_Array_EndR(array) atb_Array_BeginR(array) - atb_Array_GetSize(array)

/**
 *  \brief Same as _ForEach but in REVERSE order
 *
 *  Example:
 *
 *  \code
 *  int main(int argc, char** argv) {
 *
 *    int may_static_array[] = {1,2,3,4,5,6};
 *
 *    puts("- Begin");
 *
 *    int *elem = NULL;
 *    atb_Array_ForEachR(elem, my_static_array) {
 *      printf("%d, ", *elem);
 *    }
 *
 *    puts("\n- End");
 *
 *    return 0;
 *  }
 *  \endcode
 *
 *  STDOUT Output:
 *  - Begin
 *  6, 5, 4, 3, 2, 1,
 *  - End
 *
 *  \param[out] elem Raw pointer of the an array elements used to iterate
 *  \param[in] array Static C array (size known at compile time, do not work on
 *                   raw pointers)
 */
#define atb_Array_ForEachR(elem, array)                                        \
  for (elem = atb_Array_BeginR(array); elem != atb_Array_EndR(array); --elem)

#if defined(__cplusplus)
} /* extern "C" */
#endif
