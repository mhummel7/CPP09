/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhummel <mhummel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 08:36:20 by mhummel           #+#    #+#             */
/*   Updated: 2025/10/23 12:31:02 by mhummel          ###   ########.fr       */
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
#include <climits> // Für INT_MAX
#include <sstream> // Für std::stringstream

class PmergeMe {
private:
	std::vector<int> _unsorted;

	template <typename PairContainer>
	void createPairs(PairContainer *pairs, const std::vector<int>& input, int *additional_value);

	template <typename PairContainer>
	void sortPairs(PairContainer *pairs);

	template <typename PairContainer>
	void mergeSort(PairContainer *pairs, typename PairContainer::iterator start, typename PairContainer::iterator end, size_t size);

	// Spezialisierte Versionen für initMainChain
	template <typename PairContainer>
	void initMainChain(std::vector<int> *main_chain, const PairContainer& pairs);

	template <typename PairContainer>
	void initMainChain(std::deque<int> *main_chain, const PairContainer& pairs);

	template <typename Container>
	void binarySearchInsertion(Container *main_chain, typename Container::iterator end, int val);

	template <typename Container, typename PairContainer>
	void insertIntoMainChain(const PairContainer& pairs, Container *main_chain, int additional_value);

	template <typename Container, typename PairContainer>
	double sortAndMeasure(Container& main_chain, const PairContainer& pairs, int additional_value);

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
