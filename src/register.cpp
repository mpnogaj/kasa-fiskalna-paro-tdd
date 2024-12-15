#include "register.hpp"

#include <algorithm>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include "product.hpp"
#include "receipt-item.hpp"

bool Register::addProductToCatalog(const std::shared_ptr<Product> product)
{
	if (!product)
		return false;
	productCatalog[product->getName()] = product;
	return true;
}

bool Register::removeProductFromCatalog(const std::string &name)
{
	const auto it = productCatalog.find(name);
	if (it == productCatalog.end())
		return false;

	activeReceipt.removeItem(name);
	productCatalog.erase(it);
	return true;
}

bool Register::addToReceipt(const std::string &name, std::variant<int, double> quantity)
{
	const auto productIt = productCatalog.find(name);
	if (productIt == productCatalog.end())
	{
		return false;
	}

	const auto &[_, product] = *productIt;
	if (const auto weightProduct = std::dynamic_pointer_cast<ProductByWeight>(product))
	{
		return std::visit(
			[this, weightProduct](const auto &quantity) {
				return activeReceipt.addItem(weightProduct, static_cast<double>(quantity));
			},
			quantity);
	}
	else if (const auto pieceProduct = std::dynamic_pointer_cast<ProductByPiece>(product))
	{
		return std::visit(
			[this, pieceProduct](const auto &quantity) {
				if constexpr (std::is_same_v<std::decay_t<decltype(quantity)>, double>)
				{
					throw std::runtime_error("Quantity cannot be expressed by double");
				}
				return activeReceipt.addItem(pieceProduct, static_cast<int>(quantity));
			},
			quantity);
	}
	else
	{
		throw std::runtime_error("Unknown product type!");
	}
}

bool Register::removeFromReceipt(const std::string &name)
{
	return activeReceipt.removeItem(name);
}

double Register::getActiveReceiptValue() const
{
	return activeReceipt.getTotalValue();
}

std::vector<std::string> Register::getReceiptDetails() const
{
	return activeReceipt.getDetails();
}

std::vector<std::shared_ptr<const Product>> Register::getProductCatalog() const
{
	std::vector<std::shared_ptr<const Product>> result(productCatalog.size());
	std::transform(productCatalog.cbegin(), productCatalog.cend(), result.begin(), [](const auto &keyValuePair) {
		const auto &[_, product] = keyValuePair;
		return product;
	});

	return result;
}
