#include <iostream>

template<typename... Ts>
class Tuple	{};

template<typename T1,typename... Ts>
class Tuple<T1,Ts...>
{
public:
	T1 t1_;
	Tuple<Ts...> ts_;

	template<typename U,typename... Us>
	Tuple(U&& u1, Us&&... args) : t1_(std::forward<U>(u1)), 
			ts_(std::forward<Ts>(args)...)
	{

	}
};

template<size_t idx,template <typename...> class Tuple,typename... Args>
auto get(Tuple<Args...>& t)
{
	if constexpr (idx == 0)
	{
		return t.t1_;
	}
	else
	{
		return get<idx-1>(t.ts_);
	}
}

int main(int argc, char const *argv[])
{
	Tuple<int,double> t(1, 3.14);
	std::cout << get<1>(t) << std::endl;
	
	return 0;
}