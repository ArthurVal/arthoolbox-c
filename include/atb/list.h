#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h> /* NULL */

#if defined(__cplusplus)
extern "C" {
#endif

/**
 *  \brief Double linked list pointing towards next/prev elements
 *
 *  This comes from the linux kernel's list implementation.
 *
 *  This data structure is meant to be used in 2 ways:
 *  - As a list HEAD, representing the list entry point;
 *  - As a NODE, inside any other data structure that should be part of a double
 *    linked list.
 *
 *  Be aware that list HEADs or when a NODE is inside a list, it is expected to
 *  ALWAYS points towards next/prev elements. Having any of its attributs
 *  prev/next equal to NULL inside a list means that the node is 'Corrupted'.
 *
 *  Example:
 *  struct Toto {
 *    int a;
 *    double b;
 *    struct atb_List linked_list;
 *  };
 *
 *  // This is the list HEAD, used to points to the begin/end of the list
 *  struct atb_List my_list = atb_List_INITIALIZE(my_list);
 *
 *  struct Toto toto_0 = {};
 *  atb_List_Init(&(toto_0.linked_list));
 *
 *  // We can insert into the list, using the list head.
 *  // This is the only way (using the head), when the list is empty, to insert
 *  // anything into it
 *  atb_List_Insert(&(toto_0.linked_list), After, &(my_list));
 *
 *  //             *---> [HEAD] ----*
 *  //             |                |
 *  //             |                |
 *  //             *--- [toto_0] <--*
 *
 *  struct Toto toto_1 = {};
 *  atb_List_Init(&(toto_1.linked_list));
 *
 *  // We can insert into the list, using other nodes
 *  atb_List_Insert(&(toto_1.linked_list), After, &(toto_0.linked_list));
 *
 *  //             *---> [HEAD] ----*
 *  //             |                |
 *  //             |                |
 *  //          [toto_1] <----- [toto_0]
 *
 *  struct Toto toto_2 = {};
 *  atb_List_Init(&(toto_2.linked_list));
 *
 *  atb_List_Insert(&(toto_2.linked_list), Before, &(my_list));
 *
 *  //             *------> [HEAD] -------*
 *  //             |                      |
 *  //             |                      |
 *  //         [toto_2] <- [toto_1] <- [toto_0]
 *
 *
 *  // Traversing the list requires an iterator
 *  struct atb_List *node = NULL;
 *
 *  atb_DLinkedlist_ForEach(node, &(my_list)) {
 *    // atd_List data struct is of no use directly.
 *    // You can use _Entry in order to retreive the parent struct (here Toto)
 *    struct Toto* toto = atb_DLinkedlist_Entry(node, struct Toto, linked_list);
 *
 *    // toto should points to _0, _1 then _2.
 *  }
 *
 *  // Since this is a double linked list, you obviously can iterate backwards
 *  atb_DLinkedlist_ForEachR(node, &(my_list)) {
 *    struct Toto* toto = atb_DLinkedlist_Entry(node, struct Toto, linked_list);
 *    // toto should points to _2, _1 then _0.
 *  }
 *
 */
struct atb_List {
  struct atb_List *next;
  struct atb_List *prev;
};

typedef struct atb_List atb_List;

/**
 *  \brief Retreive the ptr of the data structure containing the give node_ptr,
 *  based on the parent struct type and the member name of the node
 *
 *  \param[in] node_ptr A atb_DLinkedlist ptr
 *  \param[in] type The parent struct type containing the node_ptr
 *  \param[in] member The name of the atb_DLinkedlist member inside type
 *
 *  \return type* The parent struct ptr containing node_ptr
 */
#define atb_List_Entry(node_ptr, type, member) \
  ((type *)((char *)(node_ptr) - offsetof(type, member)))

/* Init *********************************************************************/

/**
 *  \brief Statically initialize an atb_List (set prev & next to itself)
 *
 *  This should always be used as an rvalue with node being the lvalue
 *  variable's name like this:
 *
 *  struct atb_List foo = atb_List_INITIALIZE(foo);
 *  struct atb_List bar = atb_List_INITIALIZE(bar);
 *
 *  you can use it to initialize static variables like so:
 *
 *  struct Foo {
 *    int a;
 *    int b;
 *    atb_List l;
 *    double c;
 *  };
 *
 *  static struct Foo foo = {
 *    .a = 0,
 *    .b = 42,
 *    .l = atb_List_INITIALIZE(foo.l),
 *    .c = 3.141592,
 *  };
 *
 *  static struct Foo bar = {
 *    .a = 1,
 *    .b = -42,
 *    .l = atb_List_INITIALIZE(bar.l),
 *    .c = 2.718281,
 *  };
 */
#define atb_List_INITIALIZE(node) \
  { &(node), &(node) }

/**
 *  \brief Declare a new list HEAD variable with name 'n'
 */
#define atb_List_DECLARE_HEAD(n) struct atb_List n = atb_List_INITIALIZE(n)

/**
 *  \brief Initialize an atb_List (set prev & next to itself)
 *
 *  \pre node != NULL
 *  \post !atb_List_IsCorrupted(node)
 */
static inline void atb_List_Init(struct atb_List *const node);

/* Introspect **************************************************************/
/**
 *  \return true when one of node->prev or node->next are NULL, false otherwise
 *
 *  \pre node != NULL
 */
static inline bool atb_List_IsCorrupted(struct atb_List const *const node);

/**
 *  \return The size of the list
 *
 *  \pre list_head != NULL
 *  \pre The list is not corrupted
 *
 *  \note Complexity: O(n)
 */
static inline size_t atb_List_Size(struct atb_List const *const list_head);

/* Mutation *****************************************************************/
/**
 *  \brief Connects lhs as the prev node of rhs (and vice versa)
 *
 *  \pre lhs != NULL
 *  \pre rhs != NULL
 *
 *  \warning This is not meant to be used directly as an INSERTION into
 *           a list, use the _Insert* function for this
 */
static inline void atb_List_Connect(struct atb_List *const lhs,
                                    struct atb_List *const rhs);
/**
 *  \brief Insert new_node AFTER other (as other->next)
 *
 * Given the following list:
 * ... <==> A <==> other <==> B <==> ...
 *
 * The list will transform to:
 * ... <==> A <==> other <==> new_node <==> B <==> ...
 *
 *  \param[in] new_node New node we wish to insert
 *  \param[in] other Node part of a list where the insertion take place
 *
 *  \pre new_node != NULL
 *  \pre other != NULL
 *  \pre !atb_List_IsCorrupted(other)
 */
static inline void atb_List_InsertAfter(struct atb_List *const new_node,
                                        struct atb_List *const other);

/**
 *  \brief Insert new_node BEFORE other (other->prev)
 *
 * Given the following list:
 * ... <==> A <==> other <==> B <==> ...
 *
 * The list will transform to:
 * ... <==> A <==> new_node <==> node <==> B <==> ...
 *
 *  \param[in] new_node New node we wish to insert
 *  \param[in] other Node part of a list where the insertion take place
 *
 *  \pre new_node != NULL
 *  \pre other != NULL
 *  \pre !atb_List_IsCorrupted(other)
 */
static inline void atb_List_InsertBefore(struct atb_List *const new_node,
                                         struct atb_List *const other);

/**
 *  \brief Insert new_node Before/After other
 *
 *  \param[in] new_node New node we wish to insert
 *  \param[in] where One of [Before, After]
 *  \param[in] other Node part of a list where the insertion take place
 *
 *  \pre new_node != NULL
 *  \pre other != NULL
 *  \pre !atb_List_IsCorrupted(other)
 */
#define atb_List_Insert(new_node, where, other) \
  atb_List_Insert##where((new_node), (other))

/**
 *  \brief Pop node from a list
 *
 *  \pre node != NULL
 *  \pre !atb_List_IsCorrupted(node)
 */
static inline void atb_List_Pop(struct atb_List *const node);

/* Iterate *****************************************************************/
/**
 *  \brief Forward iterate (using ->next) over the double linked list
 *
 *  \param[in] node_it A atb_DLinkedlist ptr variable used as an iterator
 *  \param[in] list_head A atb_DLinkedlist* List head we wish to iterate over
 *
 *  \pre list_head != NULL
 *  \pre list_head doesn't contained any corrupted node
 *
 *  \note This ForEach function iterates over all nodes EXCEPT the list_head
 *  \note Complexity: O(n)
 */
#define atb_List_ForEach(node_it, list_head)                    \
  for ((node_it) = (list_head)->next; (node_it) != (list_head); \
       (node_it) = (node_it)->next)

/**
 *  \brief Forward iterate over a double linked list using parent struct
 * iterator
 *
 *  \param[in] entry_it An iterator to a struct containing a atb_DLinkedlist
 *  \param[in] member Name of the atb_DLinkedlist inside the type of entry_it
 *  \param[in] list_head A atb_DLinkedlist* List head we wish to iterate over
 *
 *  \pre list_head != NULL
 *  \pre list_head doesn't contained any corrupted node
 *
 *  \note This ForEach function iterates over all nodes EXCEPT the list_head
 *  \note Complexity: O(n)
 *  \note Use the parent data struct as iterator
 */
#define atb_List_ForEachEntry(entry_it, member, list_head)                 \
  for ((entry_it) =                                                        \
           atb_List_Entry((list_head)->next, typeof(*(entry_it)), member); \
       (&(entry_it)->member) != (list_head);                               \
       (entry_it) =                                                        \
           atb_List_Entry((list_head)->next, typeof(*(entry_it)), member))

/**
 *  \brief Backward iterate (using ->prev) over the double linked list
 *
 *  \param[in] node_it A atb_DLinkedlist ptr variable used as an iterator
 *  \param[in] list_head A atb_DLinkedlist* List head we wish to iterate over
 *
 *  \pre list_head != NULL
 *  \pre list_head doesn't contained any corrupted
 *
 *  \note This ForEach function iterates over all nodes EXCEPT the list_head
 *  \note Complexity: O(n)
 */
#define atb_List_ForEachR(node_it, list_head)                   \
  for ((node_it) = (list_head)->prev; (node_it) != (list_head); \
       (node_it) = (node_it)->prev)

/***************************************************************************/
/*                           Inline definitions                            */
/***************************************************************************/
static inline void atb_List_Init(struct atb_List *const node) {
  assert(node != NULL);

  node->next = node;
  node->prev = node;
}

static inline bool atb_List_IsCorrupted(struct atb_List const *const node) {
  assert(node != NULL);

  return (node->next == NULL) || (node->prev == NULL);
}

static inline size_t atb_List_Size(struct atb_List const *const list_head) {
  assert(list_head != NULL);

  size_t size = 0u;
  struct atb_List *_not_used = NULL;

  atb_List_ForEach(_not_used, list_head) { size += 1; }

  return size;
}

static inline void atb_List_Connect(struct atb_List *const lhs,
                                    struct atb_List *const rhs) {
  assert(lhs != NULL);
  assert(rhs != NULL);

  lhs->next = rhs;
  rhs->prev = lhs;
}

static inline void atb_List_InsertAfter(struct atb_List *const new_node,
                                        struct atb_List *const other) {
  atb_List_Connect(new_node, other->next);
  atb_List_Connect(other, new_node);
}

static inline void atb_List_InsertBefore(struct atb_List *const new_node,
                                         struct atb_List *const other) {
  atb_List_Connect(other->prev, new_node);
  atb_List_Connect(new_node, other);
  /* atb_List_InsertAfter(new_node, other->prev); */
}

static inline void atb_List_Pop(struct atb_List *const node) {
  assert(node != NULL);
  assert(!atb_List_IsCorrupted(node));

  atb_List_Connect(node->prev, node->next);
  atb_List_Init(node);
}

#if defined(__cplusplus)
} /* extern "C" */
#endif
