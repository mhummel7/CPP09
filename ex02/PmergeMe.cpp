/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhummel <mhummel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 09:31:38 by mhummel           #+#    #+#             */
/*   Updated: 2025/11/26 14:20:42 by mhummel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

bool custom_cmp(const std::pair<int, int>& a, const std::pair<int, int>& b) {
	return a.second < b.second;
}

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other) : _unsorted(other._unsorted) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
	if (this != &other) {
		_unsorted = other._unsorted;
	}
	return *this;
}

PmergeMe::~PmergeMe() {}

void PmergeMe::parseInput(int argc, char** argv) {
	std::set<int> uniqueCheck;
	for (int i = 1; i < argc; ++i) {
		std::string input = argv[i];
		std::stringstream ss(input);
		int num;
		ss >> num;
		if (ss.fail() || num < 0 || uniqueCheck.count(num) || input.length() > 10 || num > INT_MAX) {
			throw std::runtime_error("Error");
		}
		uniqueCheck.insert(num);
		_unsorted.push_back(num);
	}
}

// Vector-spezifische Funktionen
void PmergeMe::createPairsVec(std::vector<std::pair<int, int>> *pairs, const std::vector<int>& input, int *additional_value) {
	*additional_value = -1;
	for (size_t i = 0; i < input.size(); i += 2) {
		if (i + 1 < input.size()) {
			pairs->push_back(std::make_pair(input[i], input[i + 1]));
		} else {
			*additional_value = input[i];
		}
	}
}

void PmergeMe::sortPairsVec(std::vector<std::pair<int, int>> *pairs) {
	for (std::vector<std::pair<int, int>>::iterator it = pairs->begin(); it != pairs->end(); ++it) {
		if (it->first > it->second) {
			std::swap(it->first, it->second);
		}
	}
}

void PmergeMe::mergeSortVec(std::vector<std::pair<int, int>> *pairs, std::vector<std::pair<int, int>>::iterator start, std::vector<std::pair<int, int>>::iterator end, size_t size) {
	if (size == 0 && start != end) size = std::distance(start, end);
	if (size <= 1) return;

	size_t firstHalf = size / 2;
	std::vector<std::pair<int, int>>::iterator center = start;
	std::advance(center, firstHalf);

	mergeSortVec(pairs, start, center, firstHalf);
	mergeSortVec(pairs, center, end, size - firstHalf);
	std::inplace_merge(start, center, end, custom_cmp);
}

void PmergeMe::initMainChainVec(std::vector<int> *main_chain, const std::vector<std::pair<int, int>>& pairs) {
	for (std::vector<std::pair<int, int>>::const_iterator it = pairs.begin(); it != pairs.end(); ++it) {
		main_chain->push_back(it->second);
	}
	if (!pairs.empty()) {
		main_chain->insert(main_chain->begin(), pairs.begin()->first);
	}
}

void PmergeMe::binarySearchInsertionVec(std::vector<int> *main_chain, std::vector<int>::iterator end, int val) {
	std::vector<int>::iterator place_to_insert = std::lower_bound(main_chain->begin(), end, val);
	main_chain->insert(place_to_insert, val);
}

void PmergeMe::insertIntoMainChainVec(const std::vector<std::pair<int, int>>& pairs, std::vector<int> *main_chain, int additional_value) {
	size_t jacobsthal[] = {1, 3, 5, 11, 21, 43, 85, 171, 341, 683, 1365, 2731, 5461, 10923, 21845, 43691, 87381, 174763, 349525, 699051, 1398101, 2796203, 5592405, 11184811, 22369621, 44739243, 89478485, 178956971, 357913941, 715827883, 1431655765};
	size_t jacobsthal_idx = 1;

	std::vector<std::pair<int, int>>::const_iterator pair_it;
	std::vector<std::pair<int, int>>::const_iterator last_jacob_it = pairs.begin();
	std::vector<int>::iterator slice_delim_it;

	while (jacobsthal[jacobsthal_idx] <= pairs.size()) {
		pair_it = pairs.begin();
		std::advance(pair_it, jacobsthal[jacobsthal_idx] - 1);
		last_jacob_it = pair_it;

		int insertion_counter = 0;
		while (jacobsthal[jacobsthal_idx] - insertion_counter > jacobsthal[jacobsthal_idx - 1]) {
			slice_delim_it = std::find(main_chain->begin(), main_chain->end(), pair_it->second);
			binarySearchInsertionVec(main_chain, slice_delim_it, pair_it->first);
			if (pair_it == pairs.begin()) break;
			--pair_it;
			insertion_counter++;
		}
		jacobsthal_idx++;
	}

	if (jacobsthal[jacobsthal_idx] != pairs.size()) {
		pair_it = pairs.end();
		if (pair_it != pairs.begin()) --pair_it;
		while (pair_it != last_jacob_it) {
			slice_delim_it = std::find(main_chain->begin(), main_chain->end(), pair_it->second);
			binarySearchInsertionVec(main_chain, slice_delim_it, pair_it->first);
			if (pair_it == pairs.begin()) break;
			--pair_it;
		}
	}

	if (additional_value != -1) {
		binarySearchInsertionVec(main_chain, main_chain->end(), additional_value);
	}
}

double PmergeMe::sortAndMeasureVec(std::vector<int>& main_chain, std::vector<std::pair<int, int>>& pairs, int additional_value) {
	auto start = std::chrono::high_resolution_clock::now();
	sortPairsVec(&pairs);
	mergeSortVec(&pairs, pairs.begin(), pairs.end(), 0);
	initMainChainVec(&main_chain, pairs);
	insertIntoMainChainVec(pairs, &main_chain, additional_value);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::micro> duration = end - start;
	return duration.count();
}

// Deque-spezifische Funktionen (dupliziert)
void PmergeMe::createPairsDeq(std::deque<std::pair<int, int>> *pairs, const std::vector<int>& input, int *additional_value) {
	*additional_value = -1;
	for (size_t i = 0; i < input.size(); i += 2) {
		if (i + 1 < input.size()) {
			pairs->push_back(std::make_pair(input[i], input[i + 1]));
		} else {
			*additional_value = input[i];
		}
	}
}

void PmergeMe::sortPairsDeq(std::deque<std::pair<int, int>> *pairs) {
	for (std::deque<std::pair<int, int>>::iterator it = pairs->begin(); it != pairs->end(); ++it) {
		if (it->first > it->second) {
			std::swap(it->first, it->second);
		}
	}
}

void PmergeMe::mergeSortDeq(std::deque<std::pair<int, int>> *pairs, std::deque<std::pair<int, int>>::iterator start, std::deque<std::pair<int, int>>::iterator end, size_t size) {
	if (size == 0 && start != end) size = std::distance(start, end);
	if (size <= 1) return;

	size_t firstHalf = size / 2;
	std::deque<std::pair<int, int>>::iterator center = start;
	std::advance(center, firstHalf);

	mergeSortDeq(pairs, start, center, firstHalf);
	mergeSortDeq(pairs, center, end, size - firstHalf);
	std::inplace_merge(start, center, end, custom_cmp);
}

void PmergeMe::initMainChainDeq(std::deque<int> *main_chain, const std::deque<std::pair<int, int>>& pairs) {
	for (std::deque<std::pair<int, int>>::const_iterator it = pairs.begin(); it != pairs.end(); ++it) {
		main_chain->push_back(it->second);
	}
	if (!pairs.empty()) {
		main_chain->insert(main_chain->begin(), pairs.begin()->first);
	}
}

void PmergeMe::binarySearchInsertionDeq(std::deque<int> *main_chain, std::deque<int>::iterator end, int val) {
	std::deque<int>::iterator place_to_insert = std::lower_bound(main_chain->begin(), end, val);
	main_chain->insert(place_to_insert, val);
}

void PmergeMe::insertIntoMainChainDeq(const std::deque<std::pair<int, int>>& pairs, std::deque<int> *main_chain, int additional_value) {
	size_t jacobsthal[] = {1, 3, 5, 11, 21, 43, 85, 171, 341, 683, 1365, 2731, 5461, 10923, 21845, 43691, 87381, 174763, 349525, 699051, 1398101, 2796203, 5592405, 11184811, 22369621, 44739243, 89478485, 178956971, 357913941, 715827883, 1431655765};
	size_t jacobsthal_idx = 1;

	std::deque<std::pair<int, int>>::const_iterator pair_it;
	std::deque<std::pair<int, int>>::const_iterator last_jacob_it = pairs.begin();
	std::deque<int>::iterator slice_delim_it;

	while (jacobsthal[jacobsthal_idx] <= pairs.size()) {
		pair_it = pairs.begin();
		std::advance(pair_it, jacobsthal[jacobsthal_idx] - 1);
		last_jacob_it = pair_it;

		int insertion_counter = 0;
		while (jacobsthal[jacobsthal_idx] - insertion_counter > jacobsthal[jacobsthal_idx - 1]) {
			slice_delim_it = std::find(main_chain->begin(), main_chain->end(), pair_it->second);
			binarySearchInsertionDeq(main_chain, slice_delim_it, pair_it->first);
			if (pair_it == pairs.begin()) break;
			--pair_it;
			insertion_counter++;
		}
		jacobsthal_idx++;
	}

	if (jacobsthal[jacobsthal_idx] != pairs.size()) {
		pair_it = pairs.end();
		if (pair_it != pairs.begin()) --pair_it;
		while (pair_it != last_jacob_it) {
			slice_delim_it = std::find(main_chain->begin(), main_chain->end(), pair_it->second);
			binarySearchInsertionDeq(main_chain, slice_delim_it, pair_it->first);
			if (pair_it == pairs.begin()) break;
			--pair_it;
		}
	}

	if (additional_value != -1) {
		binarySearchInsertionDeq(main_chain, main_chain->end(), additional_value);
	}
}

double PmergeMe::sortAndMeasureDeq(std::deque<int>& main_chain, std::deque<std::pair<int, int>>& pairs, int additional_value) {
	auto start = std::chrono::high_resolution_clock::now();
	sortPairsDeq(&pairs);
	mergeSortDeq(&pairs, pairs.begin(), pairs.end(), 0);
	initMainChainDeq(&main_chain, pairs);
	insertIntoMainChainDeq(pairs, &main_chain, additional_value);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::micro> duration = end - start;
	return duration.count();
}

void PmergeMe::printContainer(const std::vector<int>& arr) const {
	for (std::vector<int>::const_iterator it = arr.begin(); it != arr.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::execute() {
	std::cout << "Before: ";
	printContainer(_unsorted);

	std::vector<std::pair<int, int>> vecPairs;
	int vecAdditional = -1;
	createPairsVec(&vecPairs, _unsorted, &vecAdditional);
	std::vector<int> vecMain;
	double vecTime = sortAndMeasureVec(vecMain, vecPairs, vecAdditional);

	std::deque<std::pair<int, int>> deqPairs;
	int deqAdditional = -1;
	createPairsDeq(&deqPairs, _unsorted, &deqAdditional);
	std::deque<int> deqMain;
	double deqTime = sortAndMeasureDeq(deqMain, deqPairs, deqAdditional);

	std::cout << "After: ";
	printContainer(vecMain);

	std::cout << "Time to process a range of " << _unsorted.size() << " elements with std::vector : " << std::fixed << std::setprecision(6) << vecTime << " us" << std::endl;
	std::cout << "Time to process a range of " << _unsorted.size() << " elements with std::deque : " << std::fixed << std::setprecision(6) << deqTime << " us" << std::endl;
}

