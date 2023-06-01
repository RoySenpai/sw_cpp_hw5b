/*
 *  Software Systems CPP Course Assignment 5
 *  Copyright (C) 2023  Roy Simanovich
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <cstdlib>
#include <memory>
#include <algorithm>
#include "MagicalContainer.hpp"

using namespace std;
using namespace ariel;

void MagicalContainer::addElement(int element) {
	auto in = _elements.insert(element);

	if (in.second)
	{
		const int* address = &(*in.first);

		_elements_ascending_order.push_back(address);
		
		if (_isPrime(element))
		{
			_elements_prime_order.push_back(address);
			sort(_elements_prime_order.begin(), _elements_prime_order.end(), [](const int* a, const int* b) { return *a < *b; });
		}

		sort(_elements_ascending_order.begin(), _elements_ascending_order.end(), [](const int* a, const int* b) { return *a < *b; });

		_elements_sidecross_order.clear();
		_elements_sidecross_order.reserve(_elements_ascending_order.size());

		size_t start = 0, end = _elements_ascending_order.size() - 1;

		if (_elements_ascending_order.size() == 1)
			_elements_sidecross_order.push_back(_elements_ascending_order.at(0));

		else
		{
			while (start <= end && end != 0)
			{
				_elements_sidecross_order.push_back(_elements_ascending_order.at(start));

				if (start != end)
					_elements_sidecross_order.push_back(_elements_ascending_order.at(end));

				start++;
				end--;
			}
		}
	}
}

void MagicalContainer::removeElement(int element) {
	auto it = _elements.find(element);

	if (it == _elements.end())
		throw std::runtime_error("Error: element not found");
	
	_elements.erase(element);

	if (_isPrime(element))
	{
		auto it_prime = find(_elements_prime_order.begin(), _elements_prime_order.end(), &(*it));
		_elements_prime_order.erase(it_prime);
	}

	auto it_ascending = find(_elements_ascending_order.begin(), _elements_ascending_order.end(), &(*it));
	_elements_ascending_order.erase(it_ascending);

	auto it_sidecross = find(_elements_sidecross_order.begin(), _elements_sidecross_order.end(), &(*it));
	_elements_sidecross_order.erase(it_sidecross);
}

bool MagicalContainer::_isPrime(int num) {
	if (abs(num) <= 1)
		return false;
	
	for (int i = 2; i < num; ++i)
	{
		if (num % i == 0)
			return false;
	}
	
	return true;
}

MagicalContainer::AscendingIterator::AscendingIterator(MagicalContainer& container): _container(container), _index(0) {}

MagicalContainer::AscendingIterator::AscendingIterator(MagicalContainer& container, size_t index): _container(container), _index(index) {}

MagicalContainer::AscendingIterator::AscendingIterator(const AscendingIterator &other): _container(other._container), _index(other._index) {}

MagicalContainer::SideCrossIterator::SideCrossIterator(MagicalContainer& container): _container(container), _index(0) {}

MagicalContainer::SideCrossIterator::SideCrossIterator(MagicalContainer& container, size_t index): _container(container), _index(index) {}

MagicalContainer::SideCrossIterator::SideCrossIterator(const SideCrossIterator &other): _container(other._container), _index(other._index) {}

MagicalContainer::PrimeIterator::PrimeIterator(MagicalContainer& container): _container(container), _index(0) {}

MagicalContainer::PrimeIterator::PrimeIterator(MagicalContainer& container, size_t index): _container(container), _index(index) {}

MagicalContainer::PrimeIterator::PrimeIterator(const PrimeIterator &other): _container(other._container), _index(other._index) {}