#include "gtest/gtest.h"

#include "atb/container/list.h"

namespace {

TEST(AtbList, StaticInit) {
  atb_List list = atb_List_INITIALIZER(list);
  EXPECT_EQ(list.next, &list);
  EXPECT_EQ(list.prev, &list);
}

TEST(AtbList, Init) {
  atb_List list;
  atb_List_Init(&list);
  EXPECT_EQ(list.next, &list);
  EXPECT_EQ(list.prev, &list);
}

TEST(AtbList, InsertAfter) {
  atb_List first = atb_List_INITIALIZER(first);

  atb_List second = atb_List_INITIALIZER(second);
  atb_List_InsertAfter(&(second), &(first));
  EXPECT_EQ(first.next, &(second));

  EXPECT_EQ(second.prev, &(first));
  EXPECT_EQ(second.next, &(first));

  EXPECT_EQ(first.prev, &(second));

  atb_List third = atb_List_INITIALIZER(third);
  atb_List_InsertAfter(&(third), &(second));
  EXPECT_EQ(first.next, &(second));

  EXPECT_EQ(second.prev, &(first));
  EXPECT_EQ(second.next, &(third));

  EXPECT_EQ(third.prev, &(second));
  EXPECT_EQ(third.next, &(first));

  EXPECT_EQ(first.prev, &(third));

  atb_List fourth = atb_List_INITIALIZER(third);
  atb_List_InsertAfter(&(fourth), &(third));

  EXPECT_EQ(first.next, &(second));

  EXPECT_EQ(second.prev, &(first));
  EXPECT_EQ(second.next, &(third));

  EXPECT_EQ(third.prev, &(second));
  EXPECT_EQ(third.next, &(fourth));

  EXPECT_EQ(fourth.prev, &(third));
  EXPECT_EQ(fourth.next, &(first));

  EXPECT_EQ(first.prev, &(fourth));
}

TEST(AtbList, InsertBefore) {
  atb_List first = atb_List_INITIALIZER(first);

  atb_List second = atb_List_INITIALIZER(second);
  atb_List_InsertBefore(&(second), &(first));
  EXPECT_EQ(first.next, &(second));

  EXPECT_EQ(second.prev, &(first));
  EXPECT_EQ(second.next, &(first));

  EXPECT_EQ(first.prev, &(second));

  atb_List third = atb_List_INITIALIZER(third);
  atb_List_InsertBefore(&(third), &(second));
  EXPECT_EQ(first.next, &(third));

  EXPECT_EQ(third.prev, &(first));
  EXPECT_EQ(third.next, &(second));

  EXPECT_EQ(second.prev, &(third));
  EXPECT_EQ(second.next, &(first));

  EXPECT_EQ(first.prev, &(second));

  atb_List fourth = atb_List_INITIALIZER(third);
  atb_List_InsertBefore(&(fourth), &(third));

  EXPECT_EQ(first.next, &(fourth));

  EXPECT_EQ(fourth.prev, &(first));
  EXPECT_EQ(fourth.next, &(third));

  EXPECT_EQ(third.prev, &(fourth));
  EXPECT_EQ(third.next, &(second));

  EXPECT_EQ(second.prev, &(third));
  EXPECT_EQ(second.next, &(first));

  EXPECT_EQ(first.prev, &(second));
}

TEST(AtbList, Pop) {
  atb_List first = atb_List_INITIALIZER(first);

  // Poping an empty list does nothing
  EXPECT_EQ(first.prev, &(first));
  EXPECT_EQ(first.next, &(first));
  atb_List_Pop(&(first));
  EXPECT_EQ(first.prev, &(first));
  EXPECT_EQ(first.next, &(first));

  atb_List second = atb_List_INITIALIZER(second);
  atb_List_Insert(&(second), After, &(first));

  atb_List third = atb_List_INITIALIZER(third);
  atb_List_Insert(&(third), After, &(second));

  atb_List_Pop(&(second));

  EXPECT_EQ(second.next, &(second));
  EXPECT_EQ(second.prev, &(second));

  EXPECT_EQ(first.next, &(third));

  EXPECT_EQ(third.prev, &(first));
  EXPECT_EQ(third.next, &(first));

  EXPECT_EQ(first.prev, &(third));
}

struct Toto {
  std::size_t useless_0;
  atb_List list;
  std::size_t useless_1;
};

TEST(AtbList, Entry) {
  Toto toto = {
      0,
      atb_List_INITIALIZER(toto.list),
      1,
  };

  atb_List *toto_list = &(toto.list);
  EXPECT_EQ(&toto, atb_List_Entry(toto_list, struct Toto, list));
}

TEST(AtbList, ForEach) {
  atb_List head = atb_List_INITIALIZER(head);

  Toto first{0, atb_List_INITIALIZER(first.list), 0};
  atb_List_Insert(&(first.list), After, &(head));

  Toto second{1, atb_List_INITIALIZER(second.list), 1};
  atb_List_Insert(&(second.list), After, &(first.list));

  Toto third{2, atb_List_INITIALIZER(third.list), 2};
  atb_List_Insert(&(third.list), After, &(second.list));

  const atb_List *elem = nullptr;
  std::size_t expected_id = 0;

  atb_List_ForEach(elem, &head) {
    EXPECT_EQ(atb_List_Entry(elem, struct Toto, list)->useless_0,
              expected_id++);
  }

  expected_id = third.useless_0;
  atb_List_ForEachR(elem, &head) {
    EXPECT_EQ(atb_List_Entry(elem, struct Toto, list)->useless_0,
              expected_id--);
  }
}

static int some_data = 3;

static bool IsEq0(void *data, atb_List const *node) {
  EXPECT_EQ(data, &some_data);
  return atb_List_Entry(node, Toto, list)->useless_0 == 0;
}

static bool IsEq3(void *data, atb_List const *node) {
  EXPECT_EQ(data, &some_data);
  return atb_List_Entry(node, Toto, list)->useless_0 == 3;
}

TEST(AtbList, FindIf) {
  atb_List head = atb_List_INITIALIZER(head);

  Toto first{0, atb_List_INITIALIZER(first.list), 0};
  atb_List_Insert(&(first.list), After, &(head));

  Toto second{1, atb_List_INITIALIZER(second.list), 1};
  atb_List_Insert(&(second.list), After, &(first.list));

  Toto third{2, atb_List_INITIALIZER(third.list), 2};
  atb_List_Insert(&(third.list), After, &(second.list));

  EXPECT_EQ(atb_List_FindIf(&head,
                            atb_List_UnaryOp{
                                .data = &some_data,
                                .op = IsEq3,
                            }),
            &head);
  EXPECT_EQ(atb_List_FindIf(&head,
                            atb_List_UnaryOp{
                                .data = &some_data,
                                .op = IsEq0,
                            }),
            &first.list);

  EXPECT_EQ(atb_List_FindIfR(&head,
                             atb_List_UnaryOp{
                                 .data = &some_data,
                                 .op = IsEq3,
                             }),
            &head);
  EXPECT_EQ(atb_List_FindIfR(&head,
                             atb_List_UnaryOp{
                                 .data = &some_data,
                                 .op = IsEq0,
                             }),
            &first.list);

  EXPECT_EQ(atb_List_FindIfExpr(&head, [](auto) { return false; }), &head);
  EXPECT_EQ(atb_List_FindIfExpr(
                &head,
                [](auto *node) {
                  return atb_List_Entry(node, Toto, list)->useless_0 == 1;
                }),
            &second.list);

  EXPECT_EQ(atb_List_FindIfRExpr(&head, [](auto) { return false; }), &head);
  EXPECT_EQ(atb_List_FindIfRExpr(
                &head,
                [](auto *node) {
                  return atb_List_Entry(node, Toto, list)->useless_0 == 2;
                }),
            &third.list);
}

} // namespace
