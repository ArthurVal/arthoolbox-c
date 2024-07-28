#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

/**
 *  \return size_t The size of the given static array
 */
#define atb_Array_GetSize(array) (sizeof((array)) / sizeof(array[0]))

/**
 *  \return True if the given idx is in range of the array [0, size), false
 *          otherwise
 */
#define atb_Array_IsInRangeOf(array, idx)                                      \
  ((0 <= (idx)) && ((idx) < atb_Array_GetSize(array)))

/**
 *  \return The begin iterator of a static array
 */
#define atb_Array_Begin(array) array

/**
 *  \return The end iterator of a static array (one past the las valid element)
 *
 *  \important This iterator is not dereferenceable
 */
#define atb_Array_End(array) atb_Array_Begin(array) + atb_Array_GetSize(array)

/**
 *  \brief Iterate over the array, using the provided iterator elem
 *
 *  \param[in] array The array we wish to iterate over
 *  \param[in] elem A pointer to the element contained within the array
 */
#define atb_Array_ForEach(elem, array)                                         \
  for (elem = atb_Array_Begin(array); elem != atb_Array_End(array); ++elem)

/**
 *  \return The r-begin iterator of a static array (last valid element)
 */
#define atb_Array_BeginR(array) atb_Array_End(array) - 1

/**
 *  \return The r-end iterator of a static array (one before the first element)
 *
 *  \important This iterator is not dereferenceable
 */
#define atb_Array_EndR(array) atb_Array_BeginR(array) - atb_Array_GetSize(array)

/**
 *  \brief Reverse iterate over the array, using the provided iterator elem
 *
 *  \param[in] array The array we wish to iterate over
 *  \param[in] elem A pointer to the element contained within the array
 */
#define atb_Array_ForEachR(elem, array)                                        \
  for (elem = atb_Array_BeginR(array); elem != atb_Array_EndR(array); --elem)

#if defined(__cplusplus)
} /* extern "C" */
#endif
