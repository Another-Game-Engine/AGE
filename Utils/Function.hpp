#ifndef    __FUNCTION_HPP__
# define   __FUNCTION_HPP__

template <typename F>
struct Function : public Function<decltype(&F::operator())>
{};

template <typename Type, typename Return, typename... Args>
struct Function<Return(Type::*)(Args...) const>
{
	typedef Return(*pointer)(Args...);
	typedef std::function<Return(Args...)> function;
};

template <typename F>
typename Function<F>::pointer
toFnPointer(F &lambda)
{
	return static_cast<typename Function<F>::pointer>(lambda);
}


template <typename F>
typename Function<F>::function
toFn(F &lambda)
{
	return static_cast<typename Function<F>::function>(lambda);
}

#endif     //__FUNCTION_HPP__