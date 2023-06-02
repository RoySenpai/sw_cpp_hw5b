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
		const int *address = &(*in.first);

		_elements_ascending_order.clear();
		_elements_sidecross_order.clear();

		_elements_ascending_order.reserve(_elements.size());
		_elements_sidecross_order.reserve(_elements.size());

		// Time complexity is O(n) in this case.
		if (_isPrime(element))
		{
			_elements_prime_order.clear();
			_elements_prime_order.reserve(_elements.size());

			for (const int &element : _elements)
			{
				if (_isPrime(element))
					_elements_prime_order.push_back(&element);
			}
		}

		// Reorder ascending order by clearing and inserting all elements again. Time complexity is O(n),
		// as opposed to O(nlogn) if we were to sort the vector.
		for (const int &element : _elements)
			_elements_ascending_order.push_back(&element);

		size_t start = 0, end = size() - 1;

		// Time complexity is O(1) in this case.
		if (size() == 1)
			_elements_sidecross_order.push_back(_elements_ascending_order.at(0));

		// Time complexity is O(n) in this case.
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
		throw runtime_error("Element not found");

	_elements.erase(element);

	// Handle prime order
	if (_isPrime(element))
	{
		auto it_prime = find(_elements_prime_order.begin(), _elements_prime_order.end(), &(*it));
		_elements_prime_order.erase(it_prime);
	}

	// Handle ascending order
	auto it_ascending = find(_elements_ascending_order.begin(), _elements_ascending_order.end(), &(*it));
	_elements_ascending_order.erase(it_ascending);

	// Handle sidecross order
	_elements_sidecross_order.clear();
	_elements_sidecross_order.reserve(_elements.size());

	size_t start = 0, end = size() - 1;

	if (size() == 1)
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

MagicalContainer::AscendingIterator::AscendingIterator(MagicalContainer &container) : _container(container), _index(0) {}

MagicalContainer::AscendingIterator::AscendingIterator(MagicalContainer &container, size_t index) : _container(container), _index(index) {}

MagicalContainer::AscendingIterator::AscendingIterator(const AscendingIterator &other) : _container(other._container), _index(other._index) {
	if (&_container != &other._container)
		throw std::runtime_error("Cannot copy iterators from different containers");
}

MagicalContainer::AscendingIterator::AscendingIterator(AscendingIterator &&other) noexcept: _container(other._container), _index(other._index) {}

MagicalContainer::AscendingIterator &MagicalContainer::AscendingIterator::operator=(const AscendingIterator &other) {
	if (this != &other)
	{
		if (&_container != &other._container)
			throw std::runtime_error("Cannot assign iterators from different containers");

		_index = other._index;
	}

	return *this;
}

MagicalContainer::AscendingIterator &MagicalContainer::AscendingIterator::operator=(AscendingIterator &&other) noexcept {
	if (this != &other)
	{
		_container = other._container;
		_index = other._index;
	}

	return *this;
}

bool MagicalContainer::AscendingIterator::operator==(const IIterator &other) const {
	const AscendingIterator *other_ptr = dynamic_cast<const AscendingIterator *>(&other);

	if (other_ptr == nullptr)
		throw std::runtime_error("Cannot compare iterators of different types");

	else if (&_container != &other_ptr->_container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index == other_ptr->_index;
}

bool MagicalContainer::AscendingIterator::operator!=(const IIterator &other) const {
	const AscendingIterator *other_ptr = dynamic_cast<const AscendingIterator *>(&other);

	if (other_ptr == nullptr)
		throw std::runtime_error("Cannot compare iterators of different types");

	if (&_container != &other_ptr->_container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index != other_ptr->_index;
}

bool MagicalContainer::AscendingIterator::operator<(const IIterator &other) const {
	const AscendingIterator *other_ptr = dynamic_cast<const AscendingIterator *>(&other);

	if (other_ptr == nullptr)
		throw std::runtime_error("Cannot compare iterators of different types");

	if (&_container != &other_ptr->_container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index < other_ptr->_index;
}

bool MagicalContainer::AscendingIterator::operator>(const IIterator &other) const {
	const AscendingIterator *other_ptr = dynamic_cast<const AscendingIterator *>(&other);

	if (other_ptr == nullptr)
		throw std::runtime_error("Cannot compare iterators of different types");

	if (&_container != &other_ptr->_container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index > other_ptr->_index;
}

bool MagicalContainer::AscendingIterator::operator==(const AscendingIterator &other) const {
	if (&_container != &other._container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index == other._index;
}

bool MagicalContainer::AscendingIterator::operator!=(const AscendingIterator &other) const {
	if (&_container != &other._container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index != other._index;
}

bool MagicalContainer::AscendingIterator::operator<(const AscendingIterator &other) const {
	if (&_container != &other._container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index < other._index;
}

bool MagicalContainer::AscendingIterator::operator>(const AscendingIterator &other) const {
	if (&_container != &other._container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index > other._index;
}

int MagicalContainer::AscendingIterator::operator*() const {
	if (_index >= _container._elements_ascending_order.size())
		throw std::out_of_range("Iterator out of range");

	return *(_container._elements_ascending_order.at(_index));
}

MagicalContainer::AscendingIterator &MagicalContainer::AscendingIterator::operator++() {
	if (_index >= _container._elements_ascending_order.size())
		throw std::runtime_error("Iterator out of range");

	++_index;
	return *this;
}

MagicalContainer::SideCrossIterator::SideCrossIterator(MagicalContainer &container) : _container(container), _index(0) {}

MagicalContainer::SideCrossIterator::SideCrossIterator(MagicalContainer &container, size_t index) : _container(container), _index(index) {}

MagicalContainer::SideCrossIterator::SideCrossIterator(const SideCrossIterator &other) : _container(other._container), _index(other._index) {
	if (&_container != &other._container)
		throw std::runtime_error("Cannot copy iterators from different containers");
}

MagicalContainer::SideCrossIterator::SideCrossIterator(SideCrossIterator &&other) noexcept: _container(other._container), _index(other._index) {}

MagicalContainer::SideCrossIterator &MagicalContainer::SideCrossIterator::operator=(const SideCrossIterator &other) {
	if (this != &other)
	{
		if (&_container != &other._container)
			throw std::runtime_error("Cannot assign iterators from different containers");
		
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
		throw std::runtime_error("Cannot compare iterators of different types");

	if (&_container != &other_ptr->_container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index == other_ptr->_index;
}

bool MagicalContainer::SideCrossIterator::operator!=(const IIterator &other) const {
	const SideCrossIterator *other_ptr = dynamic_cast<const SideCrossIterator *>(&other);

	if (other_ptr == nullptr)
		throw std::runtime_error("Cannot compare iterators of different types");

	if (&_container != &other_ptr->_container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index != other_ptr->_index;
}

bool MagicalContainer::SideCrossIterator::operator<(const IIterator &other) const {
	const SideCrossIterator *other_ptr = dynamic_cast<const SideCrossIterator *>(&other);

	if (other_ptr == nullptr)
		throw std::runtime_error("Cannot compare iterators of different types");

	if (&_container != &other_ptr->_container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index < other_ptr->_index;
}

bool MagicalContainer::SideCrossIterator::operator>(const IIterator &other) const {
	const SideCrossIterator *other_ptr = dynamic_cast<const SideCrossIterator *>(&other);

	if (other_ptr == nullptr)
		throw std::runtime_error("Cannot compare iterators of different types");

	if (&_container != &other_ptr->_container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index > other_ptr->_index;
}

bool MagicalContainer::SideCrossIterator::operator==(const SideCrossIterator &other) const {
	if (&_container != &other._container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index == other._index;
}

bool MagicalContainer::SideCrossIterator::operator!=(const SideCrossIterator &other) const {
	if (&_container != &other._container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index != other._index;
}

bool MagicalContainer::SideCrossIterator::operator<(const SideCrossIterator &other) const {
	if (&_container != &other._container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index < other._index;
}

bool MagicalContainer::SideCrossIterator::operator>(const SideCrossIterator &other) const {
	if (&_container != &other._container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index > other._index;
}

int MagicalContainer::SideCrossIterator::operator*() const {
	if (_index >= _container._elements_ascending_order.size())
		throw std::out_of_range("Iterator out of range");

	return *(_container._elements_sidecross_order.at(_index));
}

MagicalContainer::SideCrossIterator &MagicalContainer::SideCrossIterator::operator++() {
	if (_index >= _container._elements_sidecross_order.size())
		throw std::runtime_error("Iterator out of range");

	++_index;
	return *this;
}

MagicalContainer::PrimeIterator::PrimeIterator(MagicalContainer &container) : _container(container), _index(0) {}

MagicalContainer::PrimeIterator::PrimeIterator(MagicalContainer &container, size_t index) : _container(container), _index(index) {}

MagicalContainer::PrimeIterator::PrimeIterator(const PrimeIterator &other) : _container(other._container), _index(other._index) {
	if (&_container != &other._container)
		throw std::runtime_error("Cannot copy iterators from different containers");
}

MagicalContainer::PrimeIterator::PrimeIterator(PrimeIterator &&other) noexcept: _container(other._container), _index(other._index) {}

MagicalContainer::PrimeIterator &MagicalContainer::PrimeIterator::operator=(const PrimeIterator &other) {
	if (this != &other)
	{
		if (&_container != &other._container)
		{
			throw std::runtime_error("Cannot assign iterators from different containers");
		}

		_index = other._index;
	}

	return *this;
}

MagicalContainer::PrimeIterator &MagicalContainer::PrimeIterator::operator=(PrimeIterator &&other) noexcept {
	if (this != &other)
	{
		_container = other._container;
		_index = other._index;
	}

	return *this;
}

bool MagicalContainer::PrimeIterator::operator==(const IIterator &other) const {
	const PrimeIterator *other_ptr = dynamic_cast<const PrimeIterator *>(&other);

	if (other_ptr == nullptr)
		throw std::runtime_error("Cannot compare iterators of different types");

	if (&_container != &other_ptr->_container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index == other_ptr->_index;
}

bool MagicalContainer::PrimeIterator::operator!=(const IIterator &other) const {
	const PrimeIterator *other_ptr = dynamic_cast<const PrimeIterator *>(&other);

	if (other_ptr == nullptr)
		throw std::runtime_error("Cannot compare iterators of different types");

	if (&_container != &other_ptr->_container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index != other_ptr->_index;
}

bool MagicalContainer::PrimeIterator::operator<(const IIterator &other) const {
	const PrimeIterator *other_ptr = dynamic_cast<const PrimeIterator *>(&other);

	if (other_ptr == nullptr)
		throw std::runtime_error("Cannot compare iterators of different types");

	if (&_container != &other_ptr->_container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index < other_ptr->_index;
}

bool MagicalContainer::PrimeIterator::operator>(const IIterator &other) const {
	const PrimeIterator *other_ptr = dynamic_cast<const PrimeIterator *>(&other);

	if (other_ptr == nullptr)
		throw std::runtime_error("Cannot compare iterators of different types");

	if (&_container != &other_ptr->_container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index > other_ptr->_index;
}

bool MagicalContainer::PrimeIterator::operator==(const PrimeIterator &other) const {
	if (&_container != &other._container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index == other._index;
}

bool MagicalContainer::PrimeIterator::operator!=(const PrimeIterator &other) const {
	if (&_container != &other._container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index != other._index;
}

bool MagicalContainer::PrimeIterator::operator<(const PrimeIterator &other) const {
	if (&_container != &other._container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index < other._index;
}

bool MagicalContainer::PrimeIterator::operator>(const PrimeIterator &other) const {
	if (&_container != &other._container)
		throw std::runtime_error("Cannot compare iterators from different containers");

	return _index > other._index;
}

int MagicalContainer::PrimeIterator::operator*() const {
	if (_index >= _container._elements_ascending_order.size())
		throw std::out_of_range("Iterator out of range");

	return *(_container._elements_prime_order.at(_index));
}

MagicalContainer::PrimeIterator &MagicalContainer::PrimeIterator::operator++() {
	if (_index >= _container._elements_prime_order.size())
		throw std::runtime_error("Iterator out of range");

	++_index;
	return *this;
}