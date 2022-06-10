/* simple tuple implementation in c++ */
#include <iostream>

template<typename... T>
struct Tuple
{

};

template<typename First, typename... Rest>
struct Tuple<First, Rest...>
{
	First first_;
	Tuple<Rest...> rest_;

	Tuple(const First& first, const Rest&... rest) : first_(first), rest_(rest...)
	{

	}
};

template<size_t idx,typename T>
struct GetHelper;

template<size_t idx, typename T, typename... Rest>
struct GetHelper<idx,Tuple<T,Rest...>>
{
	static auto get(Tuple<T,Rest...>& t)
	{
		return GetHelper<idx-1,Tuple<Rest...>>::get(t.rest_);
	}
};

template<typename T,typename... Rest>
struct GetHelper<0,Tuple<T,Rest...>>
{
	static T get(Tuple<T,Rest...>& t)
	{
		return t.first_;
	}
};

// get method that takes index to retrieve the elements
template<size_t idx, template <typename...> class Tuple, typename... Args>
auto get(Tuple<Args...>& t)
{
	return GetHelper<idx,Tuple<Args...>>::get(t);
}

int main(int argc, char const *argv[])
{
	Tuple<bool> t1(true);
	Tuple<int,double,std::string> t2(1, 3.14, "indra");

	std::cout << get<0>(t1) << std::endl;

	std::cout << get<0>(t2) << std::endl;
	std::cout << get<1>(t2) << std::endl;
	std::cout << get<2>(t2) << std::endl;

	return 0;
}