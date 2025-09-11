#include "atb/list.h"
#include "gtest/gtest.h"

namespace {

TEST(AtbListTest, Initialize) {
  atb_List list = atb_List_INITIALIZE(list);
  EXPECT_EQ(list.next, &list);
  EXPECT_EQ(list.prev, &list);
}

TEST(AtbListTest, DeclareHead) {
  atb_List_DECLARE_HEAD(list);
  EXPECT_EQ(list.next, &list);
  EXPECT_EQ(list.prev, &list);
}

TEST(AtbListTest, Init) {
  atb_List list;
  atb_List_Init(&list);
  EXPECT_EQ(list.next, &list);
  EXPECT_EQ(list.prev, &list);
}

TEST(AtbListTest, IsCorrupted) {
  atb_List list;

  atb_List_Init(&list);
  EXPECT_FALSE(atb_List_IsCorrupted(&list));

  list.next = nullptr;
  EXPECT_TRUE(atb_List_IsCorrupted(&list));
  atb_List_Init(&list);

  list.prev = nullptr;
  EXPECT_TRUE(atb_List_IsCorrupted(&list));
  atb_List_Init(&list);

  list.prev = nullptr;
  list.next = nullptr;
  EXPECT_TRUE(atb_List_IsCorrupted(&list));
}

TEST(AtbListTest, InsertAfter) {
  atb_List first = atb_List_INITIALIZE(first);

  atb_List second = atb_List_INITIALIZE(second);
  atb_List_InsertAfter(&(second), &(first));
  EXPECT_EQ(first.next, &(second));

  EXPECT_EQ(second.prev, &(first));
  EXPECT_EQ(second.next, &(first));

  EXPECT_EQ(first.prev, &(second));

  atb_List third = atb_List_INITIALIZE(third);
  atb_List_InsertAfter(&(third), &(second));
  EXPECT_EQ(first.next, &(second));

  EXPECT_EQ(second.prev, &(first));
  EXPECT_EQ(second.next, &(third));

  EXPECT_EQ(third.prev, &(second));
  EXPECT_EQ(third.next, &(first));

  EXPECT_EQ(first.prev, &(third));

  atb_List fourth = atb_List_INITIALIZE(fourth);
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

TEST(AtbListTest, InsertBefore) {
  atb_List first = atb_List_INITIALIZE(first);

  atb_List second = atb_List_INITIALIZE(second);
  atb_List_InsertBefore(&(second), &(first));
  EXPECT_EQ(first.next, &(second));

  EXPECT_EQ(second.prev, &(first));
  EXPECT_EQ(second.next, &(first));

  EXPECT_EQ(first.prev, &(second));

  atb_List third = atb_List_INITIALIZE(third);
  atb_List_InsertBefore(&(third), &(second));
  EXPECT_EQ(first.next, &(third));

  EXPECT_EQ(third.prev, &(first));
  EXPECT_EQ(third.next, &(second));

  EXPECT_EQ(second.prev, &(third));
  EXPECT_EQ(second.next, &(first));

  EXPECT_EQ(first.prev, &(second));

  atb_List fourth = atb_List_INITIALIZE(fourth);
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

TEST(AtbListTest, Pop) {
  atb_List first = atb_List_INITIALIZE(first);

  // Poping an empty list does nothing
  EXPECT_EQ(first.prev, &(first));
  EXPECT_EQ(first.next, &(first));
  atb_List_Pop(&(first));
  EXPECT_EQ(first.prev, &(first));
  EXPECT_EQ(first.next, &(first));

  atb_List second = atb_List_INITIALIZE(second);
  atb_List_Insert(&(second), After, &(first));

  atb_List third = atb_List_INITIALIZE(third);
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

TEST(AtbListTest, Entry) {
  Toto toto = {
      0,
      atb_List_INITIALIZE(toto.list),
      1,
  };

  atb_List *toto_list = &(toto.list);
  EXPECT_EQ(&toto, atb_List_Entry(toto_list, struct Toto, list));
}

TEST(AtbListTest, ForEach) {
  atb_List head = atb_List_INITIALIZE(head);

  Toto first{0, atb_List_INITIALIZE(first.list), 0};
  atb_List_Insert(&(first.list), After, &(head));

  Toto second{1, atb_List_INITIALIZE(second.list), 1};
  atb_List_Insert(&(second.list), After, &(first.list));

  Toto third{2, atb_List_INITIALIZE(third.list), 2};
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

TEST(AtbListTest, Size) {
  atb_List_DECLARE_HEAD(head);

  EXPECT_EQ(atb_List_Size(&head), 0);

  atb_List a;
  atb_List_Insert(&a, After, &head);
  EXPECT_EQ(atb_List_Size(&head), 1);

  atb_List b;
  atb_List_Insert(&b, After, &head);
  EXPECT_EQ(atb_List_Size(&head), 2);

  atb_List_Pop(&a);
  EXPECT_EQ(atb_List_Size(&head), 1);

  atb_List_Pop(&b);
  EXPECT_EQ(atb_List_Size(&head), 0);
}

} // namespace
