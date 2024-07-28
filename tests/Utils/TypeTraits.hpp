#pragma once

#include <cstddef>
#include <type_traits>
#include <utility> // integer_sequence

namespace tests::utils {

/// Type holding a list of type from a parameter pack
template <class... T> struct TypeSequence {};

/// Indicates if the type T is a TypeSequence
template <class T> struct IsTypeSequence : std::false_type {};
template <class... T>
struct IsTypeSequence<TypeSequence<T...>> : std::true_type {};

template <class T> constexpr bool IsTypeSequence_v = IsTypeSequence<T>::value;

/// Get the size of a TypeSequence
template <class Sequence> struct SizeOf;

template <class... T> struct SizeOf<TypeSequence<T...>> {
  static constexpr auto value = sizeof...(T);
};

template <class Sequence> constexpr auto SizeOf_v = SizeOf<Sequence>::value;

/// Extract the first type T of a parameter pack
template <class Sequence> struct Head;

template <class... T> struct Head<TypeSequence<T...>> {
  static_assert(SizeOf_v<TypeSequence<T...>> > 0,
                "Cannot get the Head type of an empty TypeSequence");

  template <class FirstType, class... U> struct Impl {
    using type = FirstType;
  };

  using type = typename Impl<T...>::type;
};

template <class Sequence> using Head_t = typename Head<Sequence>::type;
template <class Sequence> using First_t = Head_t<Sequence>;

/// Extract the list of type ...U AFTER the first type T of a parameter pack
template <class Sequence> struct Tail;

template <class T, class... U> struct Tail<TypeSequence<T, U...>> {
  using type = TypeSequence<U...>;
};

template <> struct Tail<TypeSequence<>> { using type = TypeSequence<>; };

template <class Sequence> using Tail_t = typename Tail<Sequence>::type;

/// Create a new type_sequence by adding T and U
/// If some types are already TypeSequence, it concat the type list together
template <class... T> struct Cons;

template <class T, class U, class W, class... Other>
struct Cons<T, U, W, Other...> : Cons<typename Cons<T, U>::type, W, Other...> {
};

template <class T, class U> struct Cons<T, U> {
  using type = TypeSequence<T, U>;
};

template <class T, class... U> struct Cons<T, TypeSequence<U...>> {
  using type = TypeSequence<T, U...>;
};

template <class... T, class U> struct Cons<TypeSequence<T...>, U> {
  using type = TypeSequence<T..., U>;
};

template <class... T, class... U>
struct Cons<TypeSequence<T...>, TypeSequence<U...>> {
  using type = TypeSequence<T..., U...>;
};

template <class... T> using Cons_t = typename Cons<T...>::type;

/// Extract the last type of TypeSequence
template <class Sequence> struct Last;

template <class... T>
struct Last<TypeSequence<T...>> : Last<Tail_t<TypeSequence<T...>>> {
  static_assert(SizeOf_v<TypeSequence<T...>> > 0,
                "Cannot get the Last type of an empty TypeSequence");
};

template <class T> struct Last<TypeSequence<T>> { using type = T; };

template <class Sequence> using Last_t = typename Last<Sequence>::type;

/// Returns true when Sequence contains T
template <class Sequence, class T> struct Contains : std::false_type {};

template <class T, class... Types>
struct Contains<TypeSequence<T, Types...>, T> : std::true_type {};

template <class T, class U, class... Types>
struct Contains<TypeSequence<U, Types...>, T>
    : Contains<TypeSequence<Types...>, T> {};

template <class T> struct Contains<TypeSequence<>, T> : std::false_type {};

template <class Sequence, class T>
constexpr bool Contains_v = Contains<Sequence, T>::value;

/// Get the Type T at index I in Sequence
// This reduce the Sequence until Idx == I
template <class Sequence, std::size_t I, std::size_t Idx>
struct AtImpl : AtImpl<Tail_t<Sequence>, I, Idx + 1> {
  static_assert(Idx < I);
};

// Idx == I -> We extract the Head
template <std::size_t I, class Sequence>
struct AtImpl<Sequence, I, I> : Head<Sequence> {};

template <class Sequence, std::size_t I> struct At : AtImpl<Sequence, I, 0> {
  static_assert(I < SizeOf_v<Sequence>);
  static_assert(IsTypeSequence_v<Sequence>);
};

template <class Sequence, std::size_t I>
using At_t = typename At<Sequence, I>::type;

/// Create a sub sequence from given indexes

// The implementation is in charge of pushing types coming from InSequence for
// each indexes, into OutSequence, until Indexes is empty
template <class InSequence, class OutSequence, std::size_t... Idx>
struct SubView;

// This push the N-th type from InSequence into OutSequence and the drop N
template <class InSequence, class OutSequence, std::size_t N,
          std::size_t... Others>
struct SubView<InSequence, OutSequence, N, Others...>
    : SubView<InSequence, Cons_t<OutSequence, At_t<InSequence, N>>, Others...> {
  static_assert(IsTypeSequence_v<InSequence>);
  static_assert(IsTypeSequence_v<OutSequence>);
};

// End of the loop, no more index -> return the constructed OutSequence
template <class InSequence, class OutSequence>
struct SubView<InSequence, OutSequence> {
  static_assert(IsTypeSequence_v<InSequence>);
  static_assert(IsTypeSequence_v<OutSequence>);
  using type = OutSequence;
};

// Special overloading in order to transform an integer_sequence into the right
// indexes, using the OutSequence
template <class InSequence, std::size_t... Idx>
struct SubView<InSequence, std::integer_sequence<std::size_t, Idx...>>
    : SubView<InSequence, TypeSequence<>, Idx...> {};

template <class Sequence, std::size_t... Indexes>
using SubView_t = typename SubView<Sequence, TypeSequence<>, Indexes...>::type;

template <class Sequence, class IdxSeq>
using SubViewIdx_t = typename SubView<Sequence, IdxSeq>::type;

/// Offset a std::index_sequence by N
template <std::size_t N, class IdxSeq> struct OffsetIndexes;

template <std::size_t N, std::size_t... Indexes>
struct OffsetIndexes<N, std::integer_sequence<std::size_t, Indexes...>> {
  using type = std::integer_sequence<std::size_t, (Indexes + N)...>;
};

template <std::size_t N, class IdxSeq>
using OffsetIndexes_t = typename OffsetIndexes<N, IdxSeq>::type;

/// Inside a Sequence, replace the type at index I, by NewT
template <class Sequence, std::size_t I, class NewT> class Replace {
  static_assert(IsTypeSequence_v<Sequence>);
  static_assert(I < SizeOf_v<Sequence>);

  // We extract ALL types BEFORE index I
  using Before_I = SubViewIdx_t<Sequence, std::make_index_sequence<I>>;

  // We extract ALL types AFTER index I
  using After_I = SubViewIdx_t<
      Sequence,
      OffsetIndexes_t<I + 1,
                      std::make_index_sequence<SizeOf_v<Sequence> - I - 1>>>;

public:
  using type = Cons_t<Before_I, NewT, After_I>;
};

template <class Sequence, class NewT>
struct Replace<Sequence, 0, NewT> : Cons<NewT, Tail_t<Sequence>> {};

template <class Sequence, std::size_t I, class NewT>
using Replace_t = typename Replace<Sequence, I, NewT>::type;

/// Create T using all types contained within the TypeSequence
template <class TypeSequence, template <class...> class T>
struct FromTypeSequence;

template <template <class...> class T, class... Types>
struct FromTypeSequence<TypeSequence<Types...>, T> {
  using type = T<Types...>;
};

template <class TypeSequence, template <class...> class T>
using FromTypeSequence_t = typename FromTypeSequence<TypeSequence, T>::type;

/// Create a TypeSequence from a templated type T<...>
template <class T> struct ToTypeSequence;

template <template <class...> class T, class... Types>
struct ToTypeSequence<T<Types...>> {
  using type = TypeSequence<Types...>;
};

template <class T> using ToTypeSequence_t = typename ToTypeSequence<T>::type;

} // namespace tests::utils
