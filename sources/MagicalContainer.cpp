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
#include <cmath>
#include <memory>
#include <algorithm>
#include "MagicalContainer.hpp"

using namespace std;
using namespace ariel;

void MagicalContainer::addElement(int element) {
	auto in = _elements.insert(element);

	if (in.second)
	{
		// Handle prime order - O(n) in this case, as we need to find the correct place to insert the element.
		if (_isPrime(element))
		{
			auto index_to_insert = lower_bound(_elements_prime_order.begin(), _elements_prime_order.end(), &(*in.first), [](const int *a, const int *b) {
				return *a < *b;
			});

			_elements_prime_order.insert(index_to_insert, &(*in.first));
		}

		// Handle ascending order - O(n) in this case, as we need to find the correct place to insert the element.
		auto index_to_insert = lower_bound(_elements_ascending_order.begin(), _elements_ascending_order.end(), &(*in.first), [](const int *a, const int *b) {
			return *a < *b;
		});

		_elements_ascending_order.insert(index_to_insert, &(*in.first));

		// Handle sidecross order - O(n) in this case, as we need to rebuild the vector (Easier than reordering it).
		_elements_sidecross_order.clear();

		if (size() == 1)
			_elements_sidecross_order.push_back(_elements_ascending_order.front());

		else
		{
			size_t start = 0, end = size() - 1;

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
		throw runtime_error("Element not found");

	// Handle prime order - O(n) in this case, as we need to find the element in the vector to remove it.
	if (_isPrime(element))
	{
		auto it_prime = find(_elements_prime_order.begin(), _elements_prime_order.end(), &(*it));
		_elements_prime_order.erase(it_prime);
	}

	// Handle ascending order - O(n) in this case, as we need to find the element in the vector to remove it.
	auto it_ascending = find(_elements_ascending_order.begin(), _elements_ascending_order.end(), &(*it));
	_elements_ascending_order.erase(it_ascending);

	// Delete the element - O(logn)
	_elements.erase(element);

	// Handle sidecross order - O(n) in this case, as we need to rebuild the vector (Easier than reordering it).
	_elements_sidecross_order.clear();

	// Incase the main container is empty, we don't need to rebuild the vector.
	if (size() == 0)
		return;

	_elements_sidecross_order.reserve(_elements.size());

	if (size() == 1)
		_elements_sidecross_order.push_back(_elements_ascending_order.at(0));

	else
	{
		size_t start = 0, end = size() - 1;

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

bool MagicalContainer::_isPrime(int num) {
	if (num <= 1)
		return false;

	for (int i = 2; i <= sqrt(num); ++i)
	{
		if (num % i == 0)
			return false;
	}

	return true;
}

MagicalContainer::AscendingIterator::AscendingIterator(const AscendingIterator &other) {
	if (_container != other._container && _container != nullptr && other._container != nullptr)
		throw runtime_error("Cannot copy iterators from different containers");

	_container = other._container;
	_index = other._index;
}

MagicalContainer::AscendingIterator::AscendingIterator(AscendingIterator &&other) noexcept: _container(other._container), _index(other._index) {
	other._container = nullptr;
	other._index = 0;
}

MagicalContainer::AscendingIterator &MagicalContainer::AscendingIterator::operator=(const AscendingIterator &other) {
	if (this != &other)
	{
		if (&_container != &other._container && _container != nullptr && other._container != nullptr)
			throw runtime_error("Cannot assign iterators from different containers");

		_container = other._container;
		_index = other._index;
	}

	return *this;
}

MagicalContainer::AscendingIterator &MagicalContainer::AscendingIterator::operator=(AscendingIterator &&other) noexcept {
	if (this != &other)
	{
		_container = other._container;
		_index = other._index;

		other._container = nullptr;
		other._index = 0;
	}

	return *this;
}

bool MagicalContainer::AscendingIterator::operator==(const IIterator &other) const {
	const AscendingIterator *other_ptr = dynamic_cast<const AscendingIterator *>(&other);

	if (other_ptr == nullptr)
		throw runtime_error("Cannot compare iterators of different types");

	else if (_container == nullptr || other_ptr->_container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other_ptr->_container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index == other_ptr->_index;
}

bool MagicalContainer::AscendingIterator::operator!=(const IIterator &other) const {
	const AscendingIterator *other_ptr = dynamic_cast<const AscendingIterator *>(&other);

	if (other_ptr == nullptr)
		throw runtime_error("Cannot compare iterators of different types");

	else if (_container == nullptr || other_ptr->_container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other_ptr->_container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index != other_ptr->_index;
}

bool MagicalContainer::AscendingIterator::operator<(const IIterator &other) const {
	const AscendingIterator *other_ptr = dynamic_cast<const AscendingIterator *>(&other);

	if (other_ptr == nullptr)
		throw runtime_error("Cannot compare iterators of different types");

	else if (_container == nullptr || other_ptr->_container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other_ptr->_container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index < other_ptr->_index;
}

bool MagicalContainer::AscendingIterator::operator>(const IIterator &other) const {
	const AscendingIterator *other_ptr = dynamic_cast<const AscendingIterator *>(&other);

	if (other_ptr == nullptr)
		throw runtime_error("Cannot compare iterators of different types");

	else if (_container == nullptr || other_ptr->_container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other_ptr->_container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index > other_ptr->_index;
}

bool MagicalContainer::AscendingIterator::operator==(const AscendingIterator &other) const {
	if (_container == nullptr || other._container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other._container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index == other._index;
}

bool MagicalContainer::AscendingIterator::operator!=(const AscendingIterator &other) const {
	if (_container == nullptr || other._container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other._container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index != other._index;
}

bool MagicalContainer::AscendingIterator::operator<(const AscendingIterator &other) const {
	if (_container == nullptr || other._container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other._container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index < other._index;
}

bool MagicalContainer::AscendingIterator::operator>(const AscendingIterator &other) const {
	if (_container == nullptr || other._container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other._container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index > other._index;
}

int MagicalContainer::AscendingIterator::operator*() const {
	if (_container == nullptr)
		throw runtime_error("Iterator not initialized");

	else if (_index >= _container->_elements_ascending_order.size())
		throw runtime_error("Iterator out of range");

	return *(_container->_elements_ascending_order.at(_index));
}

MagicalContainer::AscendingIterator &MagicalContainer::AscendingIterator::operator++() {
	if (_container == nullptr)
		throw runtime_error("Iterator not initialized");

	else if (_index >= _container->_elements_ascending_order.size())
		throw runtime_error("Iterator out of range");

	++_index;
	return *this;
}

MagicalContainer::SideCrossIterator::SideCrossIterator(const SideCrossIterator &other) {
	if (_container != other._container && _container != nullptr && other._container != nullptr)
		throw runtime_error("Cannot copy iterators from different containers");

	_container = other._container;
	_index = other._index;
}

MagicalContainer::SideCrossIterator::SideCrossIterator(SideCrossIterator &&other) noexcept: _container(other._container), _index(other._index) {
	other._container = nullptr;
	other._index = 0;
}

MagicalContainer::SideCrossIterator &MagicalContainer::SideCrossIterator::operator=(const SideCrossIterator &other) {
	if (this != &other)
	{
		if (&_container != &other._container)
			throw runtime_error("Cannot assign iterators from different containers");
		
		_index = other._index;
	}

	return *this;
}

MagicalContainer::SideCrossIterator &MagicalContainer::SideCrossIterator::operator=(SideCrossIterator &&other) noexcept {
	if (this != &other)
	{
		_container = other._container;
		_index = other._index;
	}

	return *this;
}

bool MagicalContainer::SideCrossIterator::operator==(const IIterator &other) const {
	const SideCrossIterator *other_ptr = dynamic_cast<const SideCrossIterator *>(&other);

	if (other_ptr == nullptr)
		throw runtime_error("Cannot compare iterators of different types");

	else if (_container == nullptr || other_ptr->_container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other_ptr->_container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index == other_ptr->_index;
}

bool MagicalContainer::SideCrossIterator::operator!=(const IIterator &other) const {
	const SideCrossIterator *other_ptr = dynamic_cast<const SideCrossIterator *>(&other);

	if (other_ptr == nullptr)
		throw runtime_error("Cannot compare iterators of different types");

	else if (_container == nullptr || other_ptr->_container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other_ptr->_container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index != other_ptr->_index;
}

bool MagicalContainer::SideCrossIterator::operator<(const IIterator &other) const {
	const SideCrossIterator *other_ptr = dynamic_cast<const SideCrossIterator *>(&other);

	if (other_ptr == nullptr)
		throw runtime_error("Cannot compare iterators of different types");

	else if (_container == nullptr || other_ptr->_container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other_ptr->_container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index < other_ptr->_index;
}

bool MagicalContainer::SideCrossIterator::operator>(const IIterator &other) const {
	const SideCrossIterator *other_ptr = dynamic_cast<const SideCrossIterator *>(&other);

	if (other_ptr == nullptr)
		throw runtime_error("Cannot compare iterators of different types");

	else if (_container == nullptr || other_ptr->_container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other_ptr->_container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index > other_ptr->_index;
}

bool MagicalContainer::SideCrossIterator::operator==(const SideCrossIterator &other) const {
	if (_container == nullptr || other._container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other._container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index == other._index;
}

bool MagicalContainer::SideCrossIterator::operator!=(const SideCrossIterator &other) const {
	if (_container == nullptr || other._container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other._container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index != other._index;
}

bool MagicalContainer::SideCrossIterator::operator<(const SideCrossIterator &other) const {
	if (_container == nullptr || other._container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other._container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index < other._index;
}

bool MagicalContainer::SideCrossIterator::operator>(const SideCrossIterator &other) const {
	if (_container == nullptr || other._container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other._container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index > other._index;
}

int MagicalContainer::SideCrossIterator::operator*() const {
	if (_container == nullptr)
		throw runtime_error("Iterator not initialized");

	else if (_index >= _container->_elements_ascending_order.size())
		throw runtime_error("Iterator out of range");

	return *(_container->_elements_sidecross_order.at(_index));
}

MagicalContainer::SideCrossIterator &MagicalContainer::SideCrossIterator::operator++() {
	if (_container == nullptr)
		throw runtime_error("Iterator not initialized");

	else if (_index >= _container->_elements_sidecross_order.size())
		throw runtime_error("Iterator out of range");

	++_index;
	return *this;
}

MagicalContainer::PrimeIterator::PrimeIterator(const PrimeIterator &other) {
	if (_container != other._container && _container != nullptr && other._container != nullptr)
		throw runtime_error("Cannot copy iterators from different containers");

	_container = other._container;
	_index = other._index;
}

MagicalContainer::PrimeIterator::PrimeIterator(PrimeIterator &&other) noexcept: _container(other._container), _index(other._index) {
	other._container = nullptr;
	other._index = 0;
}

MagicalContainer::PrimeIterator &MagicalContainer::PrimeIterator::operator=(const PrimeIterator &other) {
	if (this != &other)
	{
		if (_container != other._container && _container != nullptr && other._container != nullptr)
		{
			throw runtime_error("Cannot assign iterators from different containers");
		}

		_container = other._container;
		_index = other._index;
	}

	return *this;
}

MagicalContainer::PrimeIterator &MagicalContainer::PrimeIterator::operator=(PrimeIterator &&other) noexcept {
	if (this != &other)
	{
		_container = other._container;
		_index = other._index;

		other._container = nullptr;
		other._index = 0;
	}

	return *this;
}

bool MagicalContainer::PrimeIterator::operator==(const IIterator &other) const {
	const PrimeIterator *other_ptr = dynamic_cast<const PrimeIterator *>(&other);

	if (other_ptr == nullptr)
		throw runtime_error("Cannot compare iterators of different types");

	else if (_container == nullptr || other_ptr->_container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other_ptr->_container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index == other_ptr->_index;
}

bool MagicalContainer::PrimeIterator::operator!=(const IIterator &other) const {
	const PrimeIterator *other_ptr = dynamic_cast<const PrimeIterator *>(&other);

	if (other_ptr == nullptr)
		throw runtime_error("Cannot compare iterators of different types");

	else if (_container == nullptr || other_ptr->_container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other_ptr->_container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index != other_ptr->_index;
}

bool MagicalContainer::PrimeIterator::operator<(const IIterator &other) const {
	const PrimeIterator *other_ptr = dynamic_cast<const PrimeIterator *>(&other);

	if (other_ptr == nullptr)
		throw runtime_error("Cannot compare iterators of different types");

	else if (_container == nullptr || other_ptr->_container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other_ptr->_container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index < other_ptr->_index;
}

bool MagicalContainer::PrimeIterator::operator>(const IIterator &other) const {
	const PrimeIterator *other_ptr = dynamic_cast<const PrimeIterator *>(&other);

	if (other_ptr == nullptr)
		throw runtime_error("Cannot compare iterators of different types");

	else if (_container == nullptr || other_ptr->_container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other_ptr->_container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index > other_ptr->_index;
}

bool MagicalContainer::PrimeIterator::operator==(const PrimeIterator &other) const {
	if (_container == nullptr)
		throw runtime_error("Iterator not initialized");

	else if (_container != other._container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index == other._index;
}

bool MagicalContainer::PrimeIterator::operator!=(const PrimeIterator &other) const {
	if (_container == nullptr || other._container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other._container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index != other._index;
}

bool MagicalContainer::PrimeIterator::operator<(const PrimeIterator &other) const {
	if (_container == nullptr || other._container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other._container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index < other._index;
}

bool MagicalContainer::PrimeIterator::operator>(const PrimeIterator &other) const {
	if (_container == nullptr || other._container == nullptr)
		throw runtime_error("One of the iterators is not initialized");

	else if (_container != other._container)
		throw runtime_error("Cannot compare iterators from different containers");

	return _index > other._index;
}

int MagicalContainer::PrimeIterator::operator*() const {
	if (_container == nullptr)
		throw runtime_error("Iterator not initialized");

	else if (_index >= _container->_elements_ascending_order.size())
		throw runtime_error("Iterator out of range");

	return *(_container->_elements_prime_order.at(_index));
}

MagicalContainer::PrimeIterator &MagicalContainer::PrimeIterator::operator++() {
	if (_container == nullptr)
		throw runtime_error("Iterator not initialized");

	else if (_index >= _container->_elements_prime_order.size())
		throw runtime_error("Iterator out of range");

	++_index;
	return *this;
}