#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "receipt.hpp"

#include <algorithm>
#include <memory>
#include <numeric>

#include "receipt-item.hpp"

struct ReceiptShould : public ::testing::Test
{
	ReceiptShould() : _sut(Receipt())
	{
	}

	std::shared_ptr<ProductByPiece> createExampleProductByPiece(std::string name = "Bananas", double price = 1.2)
	{
		return std::make_shared<ProductByPiece>(name, price);
	}

	std::shared_ptr<ProductByWeight> createExampleProductByWeight(std::string name = "Apples", double price = 3.5)
	{
		return std::make_shared<ProductByWeight>(name, price);
	}

	std::vector<std::unique_ptr<ReceiptItem>> createExampleReceiptItems()
	{
		std::vector<std::unique_ptr<ReceiptItem>> items;
		items.push_back(std::make_unique<ReceiptItemByPiece>(createExampleProductByPiece("P1"), 22));
		items.push_back(std::make_unique<ReceiptItemByWeight>(createExampleProductByWeight("P2"), 12.2));
		items.push_back(std::make_unique<ReceiptItemByPiece>(createExampleProductByPiece("P3"), 10));
		return items;
	}

	Receipt _sut;
};

TEST_F(ReceiptShould, ReturnCorrectSize)
{
	int expected_item_count = 5;
	for (int i = 0; i < expected_item_count; i++)
	{
		_sut.addItem(createExampleProductByPiece("Product" + std::to_string(i)), 1);
	}

	EXPECT_EQ(_sut.itemCount(), expected_item_count);
}

TEST_F(ReceiptShould, ReturnDetailsForAllProducts)
{
	EXPECT_TRUE(_sut.addItem(createExampleProductByPiece(), 2));
	EXPECT_TRUE(_sut.addItem(createExampleProductByWeight(), 1.2));

	EXPECT_EQ(_sut.getDetails().size(), _sut.itemCount());
}

TEST_F(ReceiptShould, AddProductByPieceCorrectly)
{
	const auto item = createExampleProductByPiece();
	int pcs = 3;

	EXPECT_TRUE(_sut.addItem(item, pcs));

	EXPECT_EQ(_sut.itemCount(), 1);
	EXPECT_DOUBLE_EQ(_sut.getTotalValue(), item->getPrice() * pcs);
}

TEST_F(ReceiptShould, AddProductByWeightCorrectly)
{
	const auto item = createExampleProductByWeight();
	double kg = 3.2;

	EXPECT_TRUE(_sut.addItem(item, kg));

	EXPECT_EQ(_sut.itemCount(), 1);
	EXPECT_DOUBLE_EQ(_sut.getTotalValue(), item->getPrice() * kg);
}

TEST_F(ReceiptShould, NotAddProductWithTheSameName)
{
	std::string name = "Example";
	EXPECT_TRUE(_sut.addItem(createExampleProductByPiece(name = name), 2));
	EXPECT_FALSE(_sut.addItem(createExampleProductByPiece(name = name), 3));
	EXPECT_FALSE(_sut.addItem(createExampleProductByWeight(name = name), 1.3));

	EXPECT_EQ(_sut.itemCount(), 1);
}

TEST_F(ReceiptShould, RemoveItemFromReceipt)
{
	const std::string name1 = "Example";
	const std::string name2 = "Name2";

	EXPECT_TRUE(_sut.addItem(createExampleProductByPiece(name1), 2));
	EXPECT_TRUE(_sut.addItem(createExampleProductByPiece(name2), 3));

	EXPECT_EQ(_sut.itemCount(), 2);

	EXPECT_TRUE(_sut.removeItem(name1));

	EXPECT_EQ(_sut.itemCount(), 1);
}

TEST_F(ReceiptShould, NotRemoveItemFromReceiptWhenItDoesNotExist)
{
	const std::string name1 = "Example";
	const std::string name2 = "Name2";

	EXPECT_TRUE(_sut.addItem(createExampleProductByPiece(name1), 2));

	EXPECT_EQ(_sut.itemCount(), 1);

	EXPECT_FALSE(_sut.removeItem(name2));

	EXPECT_EQ(_sut.itemCount(), 1);
}

TEST_F(ReceiptShould, ReturnCorrectDetails)
{
	auto items = createExampleReceiptItems();
	std::vector<std::string> expected_details(items.size());
	std::transform(items.cbegin(), items.cend(), expected_details.begin(),
				   [](auto const &receiptItem) { return receiptItem->getDetails(); });

	for (auto &item : items)
	{
		_sut.addItem(std::move(item));
	}

	const auto details = _sut.getDetails();
	EXPECT_THAT(details, ::testing::ContainerEq(expected_details));
}

TEST_F(ReceiptShould, ReturnZeroTotalItItIsEmpty)
{
	EXPECT_EQ(_sut.getTotalValue(), 0);
}

TEST_F(ReceiptShould, ReturnCorrectTotalValue)
{
	auto items = createExampleReceiptItems();
	std::vector<std::string> expected_details(items.size());
	const auto expected_total =
		std::accumulate(items.cbegin(), items.cend(), 0.0, [](double sum, const std::unique_ptr<ReceiptItem> &item) {
			return sum + item->calculatePrice();
		});

	for (auto &item : items)
	{
		_sut.addItem(std::move(item));
	}

	const auto totalValue = _sut.getTotalValue();
	EXPECT_EQ(totalValue, expected_total);
}
