#pragma once

#include <iostream>

#include "atb/functional.h"
#include "gmock/gmock.h"

namespace atb {

namespace details {

template <class... T>
struct GetArgType;

template <class T, class U, class... Others>
struct GetArgType<T, U, Others...> {
  using type = std::tuple<T, U, Others...>;
};

template <class T>
struct GetArgType<T> {
  using type = T;
};

template <class... T>
using GetArgType_t = typename GetArgType<T...>::type;

} // namespace details

template <class R, class... Args>
struct MockCallable {
  using return_t = R;
  using arg_t = details::GetArgType_t<Args...>;

  MOCK_METHOD(return_t, Call, (arg_t), (const));

  constexpr auto operator()(Args... args) const -> return_t {
    if constexpr (sizeof...(Args) == 0) {
      return Call();
    } else {
      return Call(arg_t{std::move(args)...});
    }
  }

  ATB_CALLABLE_DECLARE(R, callable_t, Args...);

  MockCallable() : m_itf(ATB_BIND_AS(callable_t, DoCall, this)) {}

  auto Itf() const -> const callable_t * { return &(m_itf); }

 private:
  static auto DoCall(void *mock, Args... args) -> R {
    auto &m = *reinterpret_cast<MockCallable *>(mock);
    return m(std::move(args)...);
  }

  const callable_t m_itf;
};

template <class R>
struct MockCallable<R> {
  using return_t = R;

  MOCK_METHOD(return_t, Call, (), (const));

  constexpr auto operator()() const -> return_t { return Call(); }

  ATB_CALLABLE_DECLARE(R, callable_t);

  MockCallable() : m_itf(ATB_BIND_AS(callable_t, DoCall, this)) {}

  auto Itf() const -> const callable_t * { return &(m_itf); }

 private:
  static auto DoCall(void *mock) -> R {
    auto &m = *reinterpret_cast<MockCallable *>(mock);
    return m();
  }

  const callable_t m_itf;
};

template <class R, class... Args>
auto operator<<(std::ostream &os,
                const typename MockCallable<R, Args...>::callable_t &callable)
    -> std::ostream & {
  os << "Callable{";
  os << ".fn=" << (void *)callable.fn << ", ";
  os << ".data=" << callable.data << ", ";
  os << '}';
  return os;
}

} // namespace atb
