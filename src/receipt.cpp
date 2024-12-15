#include "receipt.hpp"

#include <algorithm>
#include <numeric>

bool Receipt::addItem(std::unique_ptr<ReceiptItem> receiptItem)
{
	const auto &new_name = receiptItem->getName();
	const auto it = std::find_if(items.begin(), items.end(), [new_name](const std::unique_ptr<ReceiptItem> &itemPtr) {
		return itemPtr->getName() == new_name;
	});
	if (it != items.end())
		return false;
	items.push_back(std::move(receiptItem));
	return true;
}

bool Receipt::addItem(const std::shared_ptr<ProductByPiece> product, int count)
{
	return addItem(std::make_unique<ReceiptItemByPiece>(ReceiptItemByPiece(product, count)));
}

bool Receipt::addItem(const std::shared_ptr<ProductByWeight> product, double weight)
{
	return addItem(std::make_unique<ReceiptItemByWeight>(ReceiptItemByWeight(product, weight)));
}

bool Receipt::removeItem(const std::string &name)
{
	const auto it = std::find_if(items.begin(), items.end(), [name](const std::unique_ptr<ReceiptItem> &itemPtr) {
		return itemPtr->getName() == name;
	});
	if (it == items.end())
		return false;

	items.erase(it);
	return true;
}

double Receipt::getTotalValue() const
{
	return std::accumulate(items.begin(), items.end(), 0.0, [](double sum, const std::unique_ptr<ReceiptItem> &item) {
		return sum + item->calculatePrice();
	});
}

std::vector<std::string> Receipt::getDetails() const
{
	std::vector<std::string> details(items.size());
	std::transform(items.cbegin(), items.cend(), details.begin(), [](const auto &item) { return item->getDetails(); });
	return details;
}
