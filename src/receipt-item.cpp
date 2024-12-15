#include "receipt-item.hpp"

#include <stdexcept>

ReceiptItemByWeight::ReceiptItemByWeight(const std::shared_ptr<ProductByWeight> product, double weight)
	: product(product), weight(weight)
{
	if (product == nullptr)
		throw std::invalid_argument("Product cannot be null!");
	if (weight <= 0)
		throw std::invalid_argument("Weight cannot be zero or negative");
}

ReceiptItemByPiece::ReceiptItemByPiece(const std::shared_ptr<ProductByPiece> product, int quantity)
	: product(product), quantity(quantity)
{
	if (product == nullptr)
		throw std::invalid_argument("Product cannot be null!");
	if (quantity <= 0)
		throw std::invalid_argument("Weight cannot be zero or negative");
}
