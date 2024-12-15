#include "product.hpp"

#include <stdexcept>

Product::Product(const std::string &name, double price) : _name(name), _price(price)
{
	if (name.empty())
	{
		throw std::invalid_argument("Product name cannot be empty");
	}
	if (price < 0)
	{
		throw std::invalid_argument("Product price cannot be negative");
	}
}
