#include "atb/span/ints.h"

#define _ATB_DEFINE_INT_SPANS(T, NAME, ...) \
  ATB_SPAN_VIEW_DEFINE(, atb_Span_##NAME, atb_View_##NAME, T);

ATB_INTS_X_FOREACH(_ATB_DEFINE_INT_SPANS)

#undef _ATB_DEFINE_INT_SPANS

struct atb_Span_i8 atb_Span_i8_From_i16(struct atb_Span_i16 other) {
  struct atb_Span_i8 span;
  span.data = (int8_t *)other.data;
  span.size = other.size * 2;
  return span;
}
struct atb_Span_i8 atb_Span_i8_From_i32(struct atb_Span_i32 other) {
  struct atb_Span_i8 span;
  span.data = (int8_t *)other.data;
  span.size = other.size * 4;
  return span;
}
struct atb_Span_i8 atb_Span_i8_From_i64(struct atb_Span_i64 other) {
  struct atb_Span_i8 span;
  span.data = (int8_t *)other.data;
  span.size = other.size * 8;
  return span;
}

struct atb_Span_i16 atb_Span_i16_From_i8(struct atb_Span_i8 other) {
  struct atb_Span_i16 span;
  span.data = (int16_t *)other.data;
  span.size = other.size / 2;
  return span;
}
struct atb_Span_i16 atb_Span_i16_From_i32(struct atb_Span_i32 other) {
  struct atb_Span_i16 span;
  span.data = (int16_t *)other.data;
  span.size = other.size * 2;
  return span;
}
struct atb_Span_i16 atb_Span_i16_From_i64(struct atb_Span_i64 other) {
  struct atb_Span_i16 span;
  span.data = (int16_t *)other.data;
  span.size = other.size * 4;
  return span;
}

struct atb_Span_i32 atb_Span_i32_From_i8(struct atb_Span_i8 other) {
  struct atb_Span_i32 span;
  span.data = (int32_t *)other.data;
  span.size = other.size / 4;
  return span;
}
struct atb_Span_i32 atb_Span_i32_From_i16(struct atb_Span_i16 other) {
  struct atb_Span_i32 span;
  span.data = (int32_t *)other.data;
  span.size = other.size / 2;
  return span;
}
struct atb_Span_i32 atb_Span_i32_From_i64(struct atb_Span_i64 other) {
  struct atb_Span_i32 span;
  span.data = (int32_t *)other.data;
  span.size = other.size * 2;
  return span;
}

struct atb_Span_i64 atb_Span_i64_From_i8(struct atb_Span_i8 other) {
  struct atb_Span_i64 span;
  span.data = (int64_t *)other.data;
  span.size = other.size / 8;
  return span;
}
struct atb_Span_i64 atb_Span_i64_From_i16(struct atb_Span_i16 other) {
  struct atb_Span_i64 span;
  span.data = (int64_t *)other.data;
  span.size = other.size / 4;
  return span;
}
struct atb_Span_i64 atb_Span_i64_From_i32(struct atb_Span_i32 other) {
  struct atb_Span_i64 span;
  span.data = (int64_t *)other.data;
  span.size = other.size / 2;
  return span;
}

struct atb_Span_u8 atb_Span_u8_From_u16(struct atb_Span_u16 other) {
  struct atb_Span_u8 span;
  span.data = (uint8_t *)other.data;
  span.size = other.size * 2;
  return span;
}
struct atb_Span_u8 atb_Span_u8_From_u32(struct atb_Span_u32 other) {
  struct atb_Span_u8 span;
  span.data = (uint8_t *)other.data;
  span.size = other.size * 4;
  return span;
}
struct atb_Span_u8 atb_Span_u8_From_u64(struct atb_Span_u64 other) {
  struct atb_Span_u8 span;
  span.data = (uint8_t *)other.data;
  span.size = other.size * 8;
  return span;
}

struct atb_Span_u16 atb_Span_u16_From_u8(struct atb_Span_u8 other) {
  struct atb_Span_u16 span;
  span.data = (uint16_t *)other.data;
  span.size = other.size / 2;
  return span;
}
struct atb_Span_u16 atb_Span_u16_From_u32(struct atb_Span_u32 other) {
  struct atb_Span_u16 span;
  span.data = (uint16_t *)other.data;
  span.size = other.size * 2;
  return span;
}
struct atb_Span_u16 atb_Span_u16_From_u64(struct atb_Span_u64 other) {
  struct atb_Span_u16 span;
  span.data = (uint16_t *)other.data;
  span.size = other.size * 4;
  return span;
}

struct atb_Span_u32 atb_Span_u32_From_u8(struct atb_Span_u8 other) {
  struct atb_Span_u32 span;
  span.data = (uint32_t *)other.data;
  span.size = other.size / 4;
  return span;
}
struct atb_Span_u32 atb_Span_u32_From_u16(struct atb_Span_u16 other) {
  struct atb_Span_u32 span;
  span.data = (uint32_t *)other.data;
  span.size = other.size / 2;
  return span;
}
struct atb_Span_u32 atb_Span_u32_From_u64(struct atb_Span_u64 other) {
  struct atb_Span_u32 span;
  span.data = (uint32_t *)other.data;
  span.size = other.size * 2;
  return span;
}

struct atb_Span_u64 atb_Span_u64_From_u8(struct atb_Span_u8 other) {
  struct atb_Span_u64 span;
  span.data = (uint64_t *)other.data;
  span.size = other.size / 8;
  return span;
}
struct atb_Span_u64 atb_Span_u64_From_u16(struct atb_Span_u16 other) {
  struct atb_Span_u64 span;
  span.data = (uint64_t *)other.data;
  span.size = other.size / 4;
  return span;
}
struct atb_Span_u64 atb_Span_u64_From_u32(struct atb_Span_u32 other) {
  struct atb_Span_u64 span;
  span.data = (uint64_t *)other.data;
  span.size = other.size / 2;
  return span;
}

struct atb_View_i8 atb_View_i8_From_i16(struct atb_View_i16 other) {
  struct atb_View_i8 view;
  view.data = (const int8_t *)other.data;
  view.size = other.size * 2;
  return view;
}
struct atb_View_i8 atb_View_i8_From_i32(struct atb_View_i32 other) {
  struct atb_View_i8 view;
  view.data = (const int8_t *)other.data;
  view.size = other.size * 4;
  return view;
}
struct atb_View_i8 atb_View_i8_From_i64(struct atb_View_i64 other) {
  struct atb_View_i8 view;
  view.data = (const int8_t *)other.data;
  view.size = other.size * 8;
  return view;
}

struct atb_View_i16 atb_View_i16_From_i8(struct atb_View_i8 other) {
  struct atb_View_i16 view;
  view.data = (const int16_t *)other.data;
  view.size = other.size / 2;
  return view;
}
struct atb_View_i16 atb_View_i16_From_i32(struct atb_View_i32 other) {
  struct atb_View_i16 view;
  view.data = (const int16_t *)other.data;
  view.size = other.size * 2;
  return view;
}
struct atb_View_i16 atb_View_i16_From_i64(struct atb_View_i64 other) {
  struct atb_View_i16 view;
  view.data = (const int16_t *)other.data;
  view.size = other.size * 4;
  return view;
}

struct atb_View_i32 atb_View_i32_From_i8(struct atb_View_i8 other) {
  struct atb_View_i32 view;
  view.data = (const int32_t *)other.data;
  view.size = other.size / 4;
  return view;
}
struct atb_View_i32 atb_View_i32_From_i16(struct atb_View_i16 other) {
  struct atb_View_i32 view;
  view.data = (const int32_t *)other.data;
  view.size = other.size / 2;
  return view;
}
struct atb_View_i32 atb_View_i32_From_i64(struct atb_View_i64 other) {
  struct atb_View_i32 view;
  view.data = (const int32_t *)other.data;
  view.size = other.size * 2;
  return view;
}

struct atb_View_i64 atb_View_i64_From_i8(struct atb_View_i8 other) {
  struct atb_View_i64 view;
  view.data = (const int64_t *)other.data;
  view.size = other.size / 8;
  return view;
}
struct atb_View_i64 atb_View_i64_From_i16(struct atb_View_i16 other) {
  struct atb_View_i64 view;
  view.data = (const int64_t *)other.data;
  view.size = other.size / 4;
  return view;
}
struct atb_View_i64 atb_View_i64_From_i32(struct atb_View_i32 other) {
  struct atb_View_i64 view;
  view.data = (const int64_t *)other.data;
  view.size = other.size / 2;
  return view;
}

struct atb_View_u8 atb_View_u8_From_u16(struct atb_View_u16 other) {
  struct atb_View_u8 view;
  view.data = (const uint8_t *)other.data;
  view.size = other.size * 2;
  return view;
}
struct atb_View_u8 atb_View_u8_From_u32(struct atb_View_u32 other) {
  struct atb_View_u8 view;
  view.data = (const uint8_t *)other.data;
  view.size = other.size * 4;
  return view;
}
struct atb_View_u8 atb_View_u8_From_u64(struct atb_View_u64 other) {
  struct atb_View_u8 view;
  view.data = (const uint8_t *)other.data;
  view.size = other.size * 8;
  return view;
}

struct atb_View_u16 atb_View_u16_From_u8(struct atb_View_u8 other) {
  struct atb_View_u16 view;
  view.data = (const uint16_t *)other.data;
  view.size = other.size / 2;
  return view;
}
struct atb_View_u16 atb_View_u16_From_u32(struct atb_View_u32 other) {
  struct atb_View_u16 view;
  view.data = (const uint16_t *)other.data;
  view.size = other.size * 2;
  return view;
}
struct atb_View_u16 atb_View_u16_From_u64(struct atb_View_u64 other) {
  struct atb_View_u16 view;
  view.data = (const uint16_t *)other.data;
  view.size = other.size * 4;
  return view;
}

struct atb_View_u32 atb_View_u32_From_u8(struct atb_View_u8 other) {
  struct atb_View_u32 view;
  view.data = (const uint32_t *)other.data;
  view.size = other.size / 4;
  return view;
}
struct atb_View_u32 atb_View_u32_From_u16(struct atb_View_u16 other) {
  struct atb_View_u32 view;
  view.data = (const uint32_t *)other.data;
  view.size = other.size / 2;
  return view;
}
struct atb_View_u32 atb_View_u32_From_u64(struct atb_View_u64 other) {
  struct atb_View_u32 view;
  view.data = (const uint32_t *)other.data;
  view.size = other.size * 2;
  return view;
}

struct atb_View_u64 atb_View_u64_From_u8(struct atb_View_u8 other) {
  struct atb_View_u64 view;
  view.data = (const uint64_t *)other.data;
  view.size = other.size / 8;
  return view;
}
struct atb_View_u64 atb_View_u64_From_u16(struct atb_View_u16 other) {
  struct atb_View_u64 view;
  view.data = (const uint64_t *)other.data;
  view.size = other.size / 4;
  return view;
}
struct atb_View_u64 atb_View_u64_From_u32(struct atb_View_u32 other) {
  struct atb_View_u64 view;
  view.data = (const uint64_t *)other.data;
  view.size = other.size / 2;
  return view;
}
