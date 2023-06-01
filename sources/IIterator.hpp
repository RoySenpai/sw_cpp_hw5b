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

#pragma once

namespace ariel
{
	/*
	 * @brief An interface for iterators.
	*/
	class IIterator
	{
		public:
			/*
			 * @brief Compare two iterators by their current index.
			 * @param other The iterator to compare to.
			 * @return true if the iterators are equal, false otherwise.
			 * @throw std::runtime_error if the iterators are not from the same container or not the same type.
			 * @note This method is pure virtual and must be implemented by the inheriting class.
			*/
			virtual bool operator==(const IIterator& other) const = 0;

			/*
			 * @brief Compare two iterators by their current index.
			 * @param other The iterator to compare to.
			 * @return true if the the iterators are not equal, false otherwise.
			 * @throw std::runtime_error if the iterators are not from the same container or not the same type.
			 * @note This method is pure virtual and must be implemented by the inheriting class.
			*/
			virtual bool operator!=(const IIterator& other) const = 0;

			/*
			 * @brief Compare two iterators by their current index.
			 * @param other The iterator to compare to.
			 * @return true if the first iterator is smaller than the second, false otherwise.
			 * @throw std::runtime_error if the iterators are not from the same container or not the same type.
			 * @note This method is pure virtual and must be implemented by the inheriting class.
			*/
			virtual bool operator<(const IIterator& other) const = 0;

			/*
			 * @brief Compare two iterators by their current index.
			 * @param other The iterator to compare to.
			 * @return true if the first iterator is bigger than the second, false otherwise.
			 * @throw std::runtime_error if the iterators are not from the same container or not the same type.
			 * @note This method is pure virtual and must be implemented by the inheriting class.
			*/
			virtual bool operator>(const IIterator& other) const = 0;

			/*
			 * @brief Creates a instance of the iterator.
			 * @note We use the default constructor here because the iterator does not allocate any memory.
			*/
			IIterator() = default;

			/*
			 * @brief A destructor for the iterator.
			 * @note We use the default destructor here because the iterator does not allocate any memory.
			*/
			virtual ~IIterator() = default;

			/*
			 * @brief A copy constructor for the iterator.
			 * @param other The iterator to copy.
			 * @note We use the default copy constructor here because the iterator does not allocate any memory.
			*/
			IIterator(const IIterator& other) = default;

			/*
			 * @brief A move constructor for the iterator.
			 * @param other The iterator to move.
			 * @note We use the default move constructor here because the iterator does not allocate any memory.
			*/
			IIterator(IIterator&& other) = default;

			/*
			 * @brief A copy assignment operator for the iterator.
			 * @param other The iterator to copy.
			 * @return a reference to the iterator after the copy.
			 * @note We use the default copy assignment operator here because the iterator does not allocate any memory.
			*/
			IIterator& operator=(const IIterator& other) = default;

			/*
			 * @brief A move assignment operator for the iterator.
			 * @param other The iterator to move.
			 * @return a reference to the iterator after the move.
			 * @note We use the default move assignment operator here because the iterator does not allocate any memory.
			*/
			IIterator& operator=(IIterator&& other) = default;
	};
}