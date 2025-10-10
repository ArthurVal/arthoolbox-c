#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

/**
 *  \return The size N of a static \a array[N].
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
 *    printf("Size ints   = %d\n", atb_Array_Size(some_ints));
 *    printf("Size floats = %d\n", atb_Array_Size(some_floats));
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
#define atb_Array_Size(array) (sizeof((array)) / sizeof(*(array)))

/**
 *  \return True if idx is out or range of the given static array[N] (i.e. not
 *  in [0, N)), false otherwise.
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
 *    printf("idx = -20 -> %d\n", atb_Array_IsOutOfRange(my_array, -20));
 *    printf("idx =   0 -> %d\n", atb_Array_IsOutOfRange(my_array, 0));
 *    printf("idx =  10 -> %d\n", atb_Array_IsOutOfRange(my_array, 10));
 *    printf("idx =  20 -> %d\n", atb_Array_IsOutOfRange(my_array, 20));
 *    printf("idx =  40 -> %d\n", atb_Array_IsOutOfRange(my_array, 40));
 *    puts("- End");
 *
 *    return 0;
 *  }
 *  \endcode
 *
 *  STDOUT Output:
 *  - Begin
 *  idx = -20 -> 1
 *  idx =   0 -> 0
 *  idx =  10 -> 0
 *  idx =  20 -> 1
 *  idx =  40 -> 1
 *  - End
 *
 *  \param[in] array Static C array (size known at compile time, do not work on
 *                   raw pointers)
 *  \param[in] idx Index we wish to check for
 */
#define atb_Array_IsOutOfRange(array, idx) \
  (((idx) < 0) || (atb_Array_Size(array) <= (idx)))

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
#define atb_Array_End(array) atb_Array_Begin(array) + atb_Array_Size(array)

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
 *    atb_Array_ForEach(elem, my_static_array, *elem == 4) {
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
 *  1, 2, 3,
 *  - End
 *
 *  \param[out] elem Raw pointer of the an array elements used to iterate
 *  \param[in] array Static C array (size known at compile time, do not work on
 *                   raw pointers)
 *  \param[in] predicate Safe predicate used to quits the loop prematurely when
 *                       it evaluates to TRUE
 */
#define atb_Array_ForEach(elem, array, predicate) \
  for (elem = atb_Array_Begin(array);             \
       (elem != atb_Array_End(array)) && (!(predicate)); ++elem)

/**
 *  \brief Same as _ForEach without the safe predicate
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
 *    atb_Array_ForEachUnsafe(elem, my_static_array) {
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
#define atb_Array_ForEachUnsafe(elem, array) \
  atb_Array_ForEach(elem, array, false)

/**
 *  \return The address of the LAST element of a static array
 *
 *  \note Mainly used for reverse iterating over the array
 *
 *  \param[in] array Static C array (size known at compile time, do not work on
 *                   raw pointers)
 */
#define atb_Array_RBegin(array) atb_Array_End(array) - 1

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
#define atb_Array_REnd(array) atb_Array_RBegin(array) - atb_Array_Size(array)

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
 *    atb_Array_ForEachR(elem, my_static_array, *elem == 4) {
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
 *  6, 5,
 *  - End
 *
 *  \param[out] elem Raw pointer of the an array elements used to iterate
 *  \param[in] array Static C array (size known at compile time, do not work on
 *                   raw pointers)
 *  \param[in] predicate Safe predicate used to quits the loop prematurely when
 *                       it evaluates to TRUE
 */
#define atb_Array_RForEach(elem, array, predicate) \
  for (elem = atb_Array_RBegin(array);             \
       (elem != atb_Array_REnd(array)) && (!(predicate)); --elem)

/**
 *  \brief Same as _RForEach without the safe predicate
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
 *    atb_Array_RForEachUnsafe(elem, my_static_array) {
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
#define atb_Array_RForEachUnsafe(elem, array) \
  atb_Array_RForEach(elem, array, false)

#if defined(__cplusplus)
} /* extern "C" */
#endif
