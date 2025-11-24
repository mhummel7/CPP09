/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhummel <mhummel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 08:36:20 by mhummel           #+#    #+#             */
/*   Updated: 2025/11/24 10:24:15 by mhummel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <iostream>
#include <chrono>
#include <set>
#include <utility>
#include <algorithm>
#include <climits> // for INT_MAX
#include <sstream> // for std::stringstream
#include <iomanip> // for std::setprecision

class PmergeMe {
private:
	std::vector<int> _unsorted;

	// Vector
	void createPairsVec(std::vector<std::pair<int, int>> *pairs, const std::vector<int>& input, int *additional_value);
	void sortPairsVec(std::vector<std::pair<int, int>> *pairs);
	void mergeSortVec(std::vector<std::pair<int, int>> *pairs, std::vector<std::pair<int, int>>::iterator start, std::vector<std::pair<int, int>>::iterator end, size_t size);
	void initMainChainVec(std::vector<int> *main_chain, const std::vector<std::pair<int, int>>& pairs);
	void binarySearchInsertionVec(std::vector<int> *main_chain, std::vector<int>::iterator end, int val);
	void insertIntoMainChainVec(const std::vector<std::pair<int, int>>& pairs, std::vector<int> *main_chain, int additional_value);
	double sortAndMeasureVec(std::vector<int>& main_chain, std::vector<std::pair<int, int>>& pairs, int additional_value);

	// Deque
	void createPairsDeq(std::deque<std::pair<int, int>> *pairs, const std::vector<int>& input, int *additional_value);
	void sortPairsDeq(std::deque<std::pair<int, int>> *pairs);
	void mergeSortDeq(std::deque<std::pair<int, int>> *pairs, std::deque<std::pair<int, int>>::iterator start, std::deque<std::pair<int, int>>::iterator end, size_t size);
	void initMainChainDeq(std::deque<int> *main_chain, const std::deque<std::pair<int, int>>& pairs);
	void binarySearchInsertionDeq(std::deque<int> *main_chain, std::deque<int>::iterator end, int val);
	void insertIntoMainChainDeq(const std::deque<std::pair<int, int>>& pairs, std::deque<int> *main_chain, int additional_value);
	double sortAndMeasureDeq(std::deque<int>& main_chain, std::deque<std::pair<int, int>>& pairs, int additional_value);

	void printContainer(const std::vector<int>& arr) const;

public:
	PmergeMe();
	PmergeMe(const PmergeMe& other);
	PmergeMe& operator=(const PmergeMe& other);
	~PmergeMe();

	void parseInput(int argc, char** argv);
	void execute();
};

bool custom_cmp(const std::pair<int, int>& a, const std::pair<int, int>& b);

#endif
