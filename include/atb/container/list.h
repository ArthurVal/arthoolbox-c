#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include <assert.h>
#include <stdbool.h>
#include <stddef.h> /* NULL */

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
 *  Example:
 *  struct Toto {
 *    int a;
 *    double b;
 *    struct atb_List linked_list;
 *  };
 *
 *  // This is the list HEAD, used to points to the begin/end of the list
 *  struct atb_List my_list = atb_List_INITIALIZER(my_list);
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
 */
#define atb_List_INITIALIZER(node) \
  { &(node), &(node) }

/**
 *  \brief Initialize an atb_List (set prev & next to itself)
 *
 *  \warning A node should never be pointing towards NULL prev/next nodes
 *
 *  \pre self != NULL
 */
static inline void atb_List_Init(struct atb_List *const self);

/**
 *  \return True when one of self->prev or self->next are NULL
 *  \pre self != NULL
 */
static inline bool atb_List_IsCorrupted(struct atb_List const *const self);

/**
 *  \return The size of the list
 *  \pre self != NULL
 *  \pre The list is not corrupted
 *
 *  \note Complexity: O(n)
 */
static inline size_t atb_List_Size(struct atb_List const *const self);

/* Mutation *****************************************************************/
/**
 *  \brief Connects lhs as the prev node of rhs (and vice versa)
 *  \pre lhs != NULL
 *  \pre rhs != NULL
 *
 *  \warning This is not meant to be used directly as an INSERTION into
 *           a list, use the _Insert* function for this
 */
static inline void atb_List_Connect(struct atb_List *const lhs,
                                    struct atb_List *const rhs);
/**
 *  \brief Insert self AFTER other (as other->next)
 *
 * Given the following list:
 * ... <==> A <==> other <==> B <==> ...
 *
 * The list will transform to:
 * ... <==> A <==> other <==> self <==> B <==> ...
 *
 *  \param[in] self New node we wish to insert
 *  \param[in] other Node part of a list where the insertion take place
 *
 *  \pre self != NULL
 *  \pre other != NULL
 *  \pre other->next != NULL
 */
static inline void atb_List_InsertAfter(struct atb_List *const self,
                                        struct atb_List *const other);

/**
 *  \brief Insert self BEFORE other (other->prev)
 *
 * Given the following list:
 * ... <==> A <==> other <==> B <==> ...
 *
 * The list will transform to:
 * ... <==> A <==> self <==> node <==> B <==> ...
 *
 *  \param[in] self New node we wish to insert
 *  \param[in] other Node part of a list where the insertion take place
 *
 *  \pre self != NULL
 *  \pre other != NULL
 *  \pre other->prev != NULL
 */
static inline void atb_List_InsertBefore(struct atb_List *const self,
                                         struct atb_List *const other);

/**
 *  \brief Insert self Before/After other
 *
 *  \param[in] self New node we wish to insert
 *  \param[in] where One of [Before, After]
 *  \param[in] other Node part of a list where the insertion take place
 *
 *  \pre self != NULL
 *  \pre other != NULL
 *  \pre other->next != NULL when using After
 *  \pre other->prev != NULL when using Before
 */
#define atb_List_Insert(self, where, other) \
  atb_List_Insert##where((self), (other))

/**
 *  \brief Pop self from a list
 *
 *  \pre self != NULL
 *  \pre !atb_List_IsCorrupted(self)
 */
static inline void atb_List_Pop(struct atb_List *const self);

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
 *  \brief List Unary Operator
 */
struct atb_List_UnaryOp {
  bool (*f)(void *, struct atb_List const *const); /*!< Function predicate */
  void *data; /*!< Opaque internal data forwarded to each fn call */
};

/**
 *  \brief Find the first node for which the predicate returns true
 *
 *  \param[in] list_head A atb_DLinkedlist* List head we wish to iterate over
 *  \param[in] predicate An Unary operator use to stop the iteration over the
 *                       list
 *
 *  \pre list_head != NULL
 *  \pre list_head doesn't contained any corrupted node *
 *  \pre predicate.f != NULL
 *
 *  \note Complexity: O(n), forward iterates
 *
 *  \return struct atb_List * corresponding to the first node for which
 *          the predicate returns true, list_head otherwise
 */
static inline struct atb_List *atb_List_FindIf(
    struct atb_List const *const list_head, struct atb_List_UnaryOp predicate);

/**
 *  \return node when predicate(node) returns true, for each node in list_head,
 *          otherwise returns list_head
 *
 *  \param[in] list_head A atb_DLinkedlist* List head we wish to iterate over
 *  \param[in] predicate An Unary operator with the following signature:
 *                       bool (struct atb_List const * const);
 *
 *  \pre list_head != NULL
 *  \pre list_head doesn't contained any corrupted node *
 *  \pre predicate is callable
 *
 *  \note Complexity: O(n), forward iterates
 *  \warning Use of non-standard GNU statement expression
 */
#define atb_List_FindIfExpr(list_head, predicate)           \
  ({                                                        \
    struct atb_List const *const __list_head = (list_head); \
    struct atb_List *__node = NULL;                         \
    atb_List_ForEach(__node, (__list_head)) {               \
      if (predicate(__node)) {                              \
        break;                                              \
      }                                                     \
    }                                                       \
    __node;                                                 \
  })

/**
 *  \brief Backward iterate (using ->prev) over the double linked list
 *
 *  \param[in] node_it A atb_DLinkedlist ptr variable used as an iterator
 *  \param[in] list_head A atb_DLinkedlist* List head we wish to iterate over
 *
 *  \pre list_head != NULL
 *  \pre list_head doesn't contained any corrupted node *
 *
 *  \note This ForEach function iterates over all nodes EXCEPT the list_head
 *  \note Complexity: O(n)
 */
#define atb_List_ForEachR(node_it, list_head)                   \
  for ((node_it) = (list_head)->prev; (node_it) != (list_head); \
       (node_it) = (node_it)->prev)

/**
 *  \brief Find the first node for which the predicate returns true
 *
 *  \param[in] list_head A atb_DLinkedlist* List head we wish to iterate over
 *  \param[in] predicate An Unary operator use to stop the iteration over the
 *                       list
 *
 *  \pre list_head != NULL
 *  \pre list_head doesn't contained any corrupted node *
 *  \pre predicate.f != NULL
 *
 *  \note Complexity: O(n), backward iterates
 *
 *  \return struct atb_List * corresponding to the first node for which
 *          the predicate returns true, list_head otherwise
 */
static inline struct atb_List *atb_List_FindIfR(
    struct atb_List const *const list_head, struct atb_List_UnaryOp predicate);

/**
 *  \brief Find the first node for which the predicate returns true
 *
 *  \param[in] list_head A atb_DLinkedlist* List head we wish to iterate over
 *  \param[in] predicate An Unary operator with the following signature:
 *                       bool (struct atb_List const * const);
 *
 *  \pre list_head != NULL
 *  \pre list_head doesn't contained any corrupted node *
 *  \pre predicate is callable
 *
 *  \return struct atb_List * corresponding to the first node for which
 *          the predicate returns true, list_head otherwise
 */
#define atb_List_FindIfRExpr(list_head, predicate)          \
  ({                                                        \
    struct atb_List const *const __list_head = (list_head); \
    struct atb_List *__node = NULL;                         \
    atb_List_ForEachR(__node, (__list_head)) {              \
      if (predicate(__node)) {                              \
        break;                                              \
      }                                                     \
    }                                                       \
    __node;                                                 \
  })

/***************************************************************************/
/*                           Inline definitions                            */
/***************************************************************************/
static inline void atb_List_Init(struct atb_List *const self) {
  assert(self != NULL);

  self->next = self;
  self->prev = self;
}

static inline bool atb_List_IsCorrupted(struct atb_List const *const self) {
  assert(self != NULL);

  return (self->next == NULL) || (self->prev == NULL);
}

static inline size_t atb_List_Size(struct atb_List const *const self) {
  assert(self != NULL);

  size_t size = 0u;
  struct atb_List *_not_used = NULL;

  atb_List_ForEach(_not_used, self) { size += 1; }

  return size;
}

static inline void atb_List_Connect(struct atb_List *const lhs,
                                    struct atb_List *const rhs) {
  assert(lhs != NULL);
  assert(rhs != NULL);

  lhs->next = rhs;
  rhs->prev = lhs;
}

static inline void atb_List_InsertAfter(struct atb_List *const self,
                                        struct atb_List *const other) {
  atb_List_Connect(self, other->next);
  atb_List_Connect(other, self);
}

static inline void atb_List_InsertBefore(struct atb_List *const self,
                                         struct atb_List *const other) {
  atb_List_Connect(other->prev, self);
  atb_List_Connect(self, other);
  /* atb_List_InsertAfter(self, other->prev); */
}

static inline void atb_List_Pop(struct atb_List *const self) {
  assert(self != NULL);
  assert(!atb_List_IsCorrupted(self));

  atb_List_Connect(self->prev, self->next);
  atb_List_Init(self);
}

static inline struct atb_List *atb_List_FindIf(
    struct atb_List const *const list_head, struct atb_List_UnaryOp predicate) {
  assert(list_head != NULL);
  assert(predicate.f != NULL);

  struct atb_List *node = NULL;
  atb_List_ForEach(node, list_head) {
    if (predicate.f(predicate.data, node)) {
      break;
    }
  }
  return node;
}

static inline struct atb_List *atb_List_FindIfR(
    struct atb_List const *const list_head, struct atb_List_UnaryOp predicate) {
  assert(list_head != NULL);
  assert(predicate.f != NULL);

  struct atb_List *node = NULL;
  atb_List_ForEachR(node, list_head) {
    if (predicate.f(predicate.data, node)) {
      break;
    }
  }
  return node;
}

#if defined(__cplusplus)
} /* extern "C" */
#endif
