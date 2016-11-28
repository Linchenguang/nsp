#pragma once

#include <type_traits>
#include <functional>
#include <vector>

namespace jian {

#define JN_ENABLE(t) std::enable_if_t<t, int> = 42
#define JN_IS_SAME(A, B) std::is_same<A, B>::value

#define JN_DEFAULT_CONSTRUCTORS(type) \
	type() = default;\
	type(const type &) = default;\
	type(type &&) = default;\
	type &operator =(const type &) = default;\
	type &operator =(type &&) = default

	template<typename T>
	int size(T &&t) {
		return int(t.size());
	}

	template<typename _ValType>
	class refs : public std::vector<std::reference_wrapper<_ValType>> {
	public:
		using value_type = _ValType;

		_ValType &operator [](int i) const {
			return std::vector<std::reference_wrapper<_ValType>>::operator[](i);
		}

		_ValType &at(int i) const {
			return std::vector<std::reference_wrapper<_ValType>>::at(i);
		}

		refs<_ValType> &append(_ValType &val) {
			std::vector<std::reference_wrapper<_ValType>>::push_back(std::ref(val));
			return *this;
		}

		template<typename _Ls>
		refs<_ValType> &append(_Ls &&ls) {
			for (auto && item : ls) {
				append(item);
			}
			return *this;
		}

	};

	template<typename T, typename U>
	struct is_decay_same {
		enum { value = std::is_same<std::decay_t<T>, std::decay_t<U>>::value };
	};

	template<typename... T>
	struct is_all_decay_same;

	template<typename T>
	struct is_all_decay_same<T> {
		enum { value = true };
	};

	template<typename T, typename U, typename... F>
	struct is_all_decay_same<T, U, F...> {
		enum { value = is_decay_same<T, U>::value && is_all_decay_same<U, F...>::value };
	};

	template<typename T, template<typename...> class U>
	struct is_template_same {
	private:
		template<template<typename...> class F, typename K, typename... Pars>
		static K check(F<K, Pars...>);
	public:
		enum { value = std::is_same<U<decltype(check(std::declval<std::decay_t<T>>()))>, std::decay_t<T>>::value };
	};

	template<typename T, typename U> struct uniform_const { using type = std::remove_const_t<T>; };
	template<typename T, typename U> struct uniform_const<T, const U> { using type = const std::remove_const_t<T>; };
	template<typename T, typename U> struct uniform_const<T, const U &> { using type = const std::remove_const_t<T>; };
	template<typename T, typename U> struct uniform_const<T, const U &&> { using type = const std::remove_const_t<T>; };
	template<typename T, typename U> using uniform_const_t = typename uniform_const<T, U>::type;

	template<typename T>
	struct template_first_parameter {
	private:
		template<typename K> struct check { using type = std::false_type; };
		template<template<typename...> class F, typename K, typename... Pars> struct check<F<K, Pars...>> { using type = K; };
		template<template<typename, int...> class F, typename K, int... ints> struct check<F<K, ints...>> { using type = K; };
		template<typename K, typename U> struct modify { using type = K; };
		template<typename K, typename U> struct modify<K, const U> { using type = const K; };
		template<typename K, typename U> struct modify<K, const U &> { using type = const K &; };
		template<typename K, typename U> struct modify<K, const U &&> { using type = const K &&; };
		template<typename K, typename U> struct modify<K, U&> { using type = K&; };
		template<typename K, typename U> struct modify<K, U&&> { using type = K&&; };
	public:
		using type = typename modify<typename check<std::decay_t<T>>::type, T>::type;
	};

	template<typename T>
	using template_first_parameter_t = typename template_first_parameter<T>::type;

} // namespace jian

