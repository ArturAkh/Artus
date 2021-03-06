/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include <Artus/Core/interface/ProductBase.h>

struct TestProduct : ProductBase {
	TestProduct() : iGlobalProduct(0), iGlobalProduct2(0), iLocalProduct(0) {}

	int iGlobalProduct;
	int iGlobalProduct2;
	int iLocalProduct;
};
