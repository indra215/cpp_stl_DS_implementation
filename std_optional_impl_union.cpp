#include <iostream>

template<typename T>
class Optional
{
private:
	bool valid_;
	union
	{
		bool present_;
		T data_;
	};

public:
	Optional() : present_(false), valid_(false) {}
	Optional(const T& val) : valid_(true), data_(val) {}
	~Optional()
	{
		if(valid_)
		{
			data_.~T();
		}
	}
	Optional& operator=(const Optional& obj)
	{
		reset();
		if(obj.valid_)
		{
			valid_ = true;
			data_ = obj.data_;
		}
		return *this;
	}
	bool has_value() const
	{
		return valid_;
	}
	void reset()
	{
		if(valid_)
		{
			valid_ = false;
			data_.~T();
		}
	}
	T& value()
	{
		if(valid_)
		{
			return data_;
		}
		throw "Bad optional data";
	}
	template<typename U>
	T value_or(U&& def)
	{
		if(!valid_)
		{
			return def;
		}
		return *this;
	}

	T* operator->()
	{
		if(valid_)
		{
			return &data_;
		}
		throw "Bad optional data";
	}
};

int main(int argc, char const *argv[])
{
	Optional<int> a(100);
	std::cout << std::boolalpha << a.has_value() << std::endl;
	std::cout << a.value() << std::endl;

	Optional<int> b;
	b = a;
	std::cout << std::boolalpha << b.has_value() << std::endl;
	std::cout << b.value() << std::endl;
	std::cout << *b << std::endl;

	return 0;
}