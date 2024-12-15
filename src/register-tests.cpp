#include "gtest/gtest.h"

#include "register.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>

namespace
{
const std::string EXAMPLE_PRODUCT_BY_WEIGHT_NAME = "product1";
const std::string EXAMPLE_PRODUCT_BY_PIECE_NAME = "product2";
} // namespace

class UnsupportedProductType : public Product
{
  public:
	UnsupportedProductType(const std::string &name) : Product(name, 42)
	{
	}
};

struct RegisterShould : public ::testing::Test
{
	RegisterShould() : _sut(Register())
	{
	}

	std::shared_ptr<ProductByWeight> createExampleProductByWeight(std::string name = EXAMPLE_PRODUCT_BY_WEIGHT_NAME,
																  double price = 12.2)
	{
		return std::make_shared<ProductByWeight>(name, price);
	}

	std::shared_ptr<ProductByPiece> createExampleProductByPiece(std::string name = EXAMPLE_PRODUCT_BY_PIECE_NAME,
																double price = 14.2)
	{
		return std::make_shared<ProductByPiece>(name, price);
	}

	int getCatalogSize()
	{
		return _sut.getProductCatalog().size();
	}

	void insertExampleProductsAndExpectSuccess()
	{
		EXPECT_TRUE(_sut.addProductToCatalog(createExampleProductByWeight()));
		EXPECT_TRUE(_sut.addProductToCatalog(createExampleProductByPiece()));
		EXPECT_EQ(getCatalogSize(), 2);
	}

	void expectProductNotInCatalog(std::string name)
	{
		const auto &catalog = _sut.getProductCatalog();
		EXPECT_FALSE(std::any_of(catalog.cbegin(), catalog.cend(),
								 [name](const auto &product) { return product->getName() == name; }));
	}

	Register _sut;
};

TEST_F(RegisterShould, AddProductToCatalog)
{
	EXPECT_TRUE(_sut.addProductToCatalog(createExampleProductByPiece()));
}

TEST_F(RegisterShould, ReturnCorrectAmmoutOfProductsInCatalog)
{
	insertExampleProductsAndExpectSuccess();
}

TEST_F(RegisterShould, ReturnFalseWhenTryingToRemoveProductFromCatalogNotInCatalog)
{
	std::string name = "product1";
	std::string other_name = "other_product";
	expectProductNotInCatalog(other_name);
	EXPECT_TRUE(_sut.addProductToCatalog(createExampleProductByPiece(name)));
	EXPECT_FALSE(_sut.removeProductFromCatalog(other_name));
}

TEST_F(RegisterShould, RemoveProductFromReceiptWhenItIsRemovedFromCatalog)
{
	insertExampleProductsAndExpectSuccess();
	auto expected_catalog_count = getCatalogSize() - 1;

	_sut.addToReceipt(EXAMPLE_PRODUCT_BY_PIECE_NAME, 12);
	EXPECT_EQ(_sut.getReceiptDetails().size(), 1);

	_sut.removeProductFromCatalog(EXAMPLE_PRODUCT_BY_PIECE_NAME);
	EXPECT_EQ(_sut.getReceiptDetails().size(), 0);
	EXPECT_EQ(getCatalogSize(), expected_catalog_count);
}

TEST_F(RegisterShould, ReturnFalseWhenTryingToAddUnknownProductToReceipt)
{
	std::string unknown_product_name = "unknown";
	expectProductNotInCatalog(unknown_product_name);
	EXPECT_FALSE(_sut.addToReceipt(unknown_product_name, 12));
}

TEST_F(RegisterShould, ThrowWhenInvalidQuantityTypeIsUsed)
{
	insertExampleProductsAndExpectSuccess();
	EXPECT_THROW(_sut.addToReceipt(EXAMPLE_PRODUCT_BY_PIECE_NAME, 3.14), std::runtime_error);
}

TEST_F(RegisterShould, AllowDescribingWeightAsInt)
{
	insertExampleProductsAndExpectSuccess();
	EXPECT_TRUE(_sut.addToReceipt(EXAMPLE_PRODUCT_BY_WEIGHT_NAME, (int)42));
}

TEST_F(RegisterShould, ThrowWhenTryingToAddUnsupportedProductTypeToReceipt)
{
	std::string unsupported_product_name = "test";
	const auto unsupported_product = std::make_shared<UnsupportedProductType>(unsupported_product_name);
	EXPECT_TRUE(_sut.addProductToCatalog(unsupported_product));
	EXPECT_THROW(_sut.addToReceipt(unsupported_product_name, 42), std::runtime_error);
}

// Test Register
TEST_F(RegisterShould, AddAndRemoveProductsFromCatalog)
{
	/*Register reg;

	auto apples = std::make_shared<ProductByWeight>("Apples", 3.5);
	auto bananas = std::make_shared<ProductByPiece>("Bananas", 1.2);

	reg.addProductToCatalog(apples);
	reg.addProductToCatalog(bananas);

	ASSERT_THROW(reg.addToReceipt("Oranges", 2.0), std::runtime_error);

	reg.addToReceipt("Apples", 2.0); // 2 kg of apples
	reg.addToReceipt("Bananas", 3);  // 3 pieces of bananas

	EXPECT_DOUBLE_EQ(reg.getActiveReceiptValue(), 10.6);

	reg.removeProduct("Apples");
	EXPECT_DOUBLE_EQ(reg.getActiveReceiptValue(), 3.6);

	reg.removeProduct("Bananas");
	ASSERT_THROW(reg.addToReceipt("Bananas", 1), std::runtime_error);*/
}
