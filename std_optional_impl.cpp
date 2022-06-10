#include <iostream>
#include <memory>
#include <cstring>

template<typename T>
class Optional
{
private:
	static constexpr size_t DataSize = sizeof(T);
	char data_[DataSize];
	bool valid_;

public:
	Optional() : valid_(false) {}

	Optional(const T& v) : valid_(true)
	{
		new (data_) T(v);
	}

	~Optional()
	{
		if(valid_)
		{
			reinterpret_cast<T*>(data_)->~T();
		}
	}

	Optional& operator=(const Optional& obj)
	{
		if(valid_)
		{
			reinterpret_cast<T*>(data_)->~T();
		}
		valid_ = false;
		if(obj.valid_)
		{
			valid_ = true;
			memcpy(data_, obj.data_, DataSize);
		}
		return *this;
	}

	void reset()
	{
		if(valid_)
		{
			valid_ = false;
			reinterpret_cast<T*>(data_)->~T();
		}
	}

	bool has_value() const
	{
		return valid_;
	}

	T& value()
	{
		if(valid_)
		{
			return *(reinterpret_cast<T*>(data_));
		}
		throw "Bad optional access";
	}

	T* operator->() 
	{
		if(valid_)
		{
			return reinterpret_cast<T*>(data_);
		}
		throw "Bad optional access";
	}

	T& operator*()
	{
		if(valid_)
		{
			return (reinterpret_cast<T&>(data_));
		}
		throw "Bad optional access";
	}
};

int main(int argc, char const *argv[])
{
	Optional<int> a(100);
	Optional<int> b;
	// std::cout << a.has_value() << std::endl;
	std::cout << a.value() << std::endl;

	b = a;
	// std::cout << b.has_value() << std::endl;
	std::cout << b.value() << std::endl;
	std::cout << *b << std::endl;
	
	return 0;
}