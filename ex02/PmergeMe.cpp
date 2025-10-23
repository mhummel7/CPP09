/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhummel <mhummel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 09:31:38 by mhummel           #+#    #+#             */
/*   Updated: 2025/10/23 10:48:11 by mhummel          ###   ########.fr       */
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
            throw std::runtime_error("Error: input is invalid");
        }
        uniqueCheck.insert(num);
        _unsorted.push_back(num);
    }
}

template <typename PairContainer>
void PmergeMe::createPairs(PairContainer *pairs, const std::vector<int>& input, int *additional_value) {
    *additional_value = -1;
    for (size_t i = 0; i < input.size(); i += 2) {
        if (i + 1 < input.size()) {
            pairs->push_back(std::make_pair(input[i], input[i + 1]));
        } else {
            *additional_value = input[i];
        }
    }
}

template <typename PairContainer>
void PmergeMe::sortPairs(PairContainer *pairs) {
    for (typename PairContainer::iterator it = pairs->begin(); it != pairs->end(); ++it) {
        if (it->first > it->second) {
            std::swap(it->first, it->second);
        }
    }
}

template <typename PairContainer>
void PmergeMe::mergeSort(PairContainer *pairs, typename PairContainer::iterator start, typename PairContainer::iterator end, size_t size) {
    if (size == 0 && start != end) size = std::distance(start, end);
    if (size <= 1) return;

    size_t firstHalf = size / 2;
    typename PairContainer::iterator center = start;
    std::advance(center, firstHalf);

    mergeSort(pairs, start, center, firstHalf);
    mergeSort(pairs, center, end, size - firstHalf);
    std::inplace_merge(start, center, end, custom_cmp);
}

template <typename PairContainer>
void PmergeMe::initMainChain(std::vector<int> *main_chain, const PairContainer& pairs) {
    for (typename PairContainer::const_iterator it = pairs.begin(); it != pairs.end(); ++it) {
        main_chain->push_back(it->second);
    }
    if (!pairs.empty()) {
        main_chain->insert(main_chain->begin(), pairs.begin()->first);
    }
}

template <typename PairContainer>
void PmergeMe::initMainChain(std::deque<int> *main_chain, const PairContainer& pairs) {
    for (typename PairContainer::const_iterator it = pairs.begin(); it != pairs.end(); ++it) {
        main_chain->push_back(it->second);
    }
    if (!pairs.empty()) {
        main_chain->push_front(pairs.begin()->first);
    }
}

template <typename Container>
void PmergeMe::binarySearchInsertion(Container *main_chain, typename Container::iterator end, int val) {
    typename Container::iterator place_to_insert = std::lower_bound(main_chain->begin(), end, val);
    main_chain->insert(place_to_insert, val);
}

template <typename Container, typename PairContainer>
void PmergeMe::insertIntoMainChain(const PairContainer& pairs, Container *main_chain, int additional_value) {
    size_t jacobsthal[] = {1, 3, 5, 11, 21, 43, 85, 171, 341, 683, 1365, 2731, 5461, 10923, 21845, 43691, 87381, 174763, 349525, 699051, 1398101, 2796203, 5592405, 11184811, 22369621, 44739243, 89478485, 178956971, 357913941, 715827883, 1431655765};
    size_t jacobsthal_idx = 1;

    typename Container::iterator slice_delim_it;
    typename PairContainer::const_iterator pair_it;
    typename PairContainer::const_iterator last_jacob_it = pairs.begin();

    while (jacobsthal[jacobsthal_idx] <= pairs.size()) {
        pair_it = pairs.begin();
        std::advance(pair_it, jacobsthal[jacobsthal_idx] - 1);
        last_jacob_it = pair_it;

        int insertion_counter = 0;
        while (jacobsthal[jacobsthal_idx] - insertion_counter > jacobsthal[jacobsthal_idx - 1]) {
            slice_delim_it = std::find(main_chain->begin(), main_chain->end(), pair_it->second);
            binarySearchInsertion(main_chain, slice_delim_it, pair_it->first);
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
            binarySearchInsertion(main_chain, slice_delim_it, pair_it->first);
            if (pair_it == pairs.begin()) break;
            --pair_it;
        }
    }

    if (additional_value != -1) {
        binarySearchInsertion(main_chain, main_chain->end(), additional_value);
    }
}

template <typename Container, typename PairContainer>
double PmergeMe::sortAndMeasure(Container& main_chain, const PairContainer& pairs, int additional_value) {
    auto start = std::chrono::high_resolution_clock::now();
    insertIntoMainChain(pairs, &main_chain, additional_value);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
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
    createPairs(&vecPairs, _unsorted, &vecAdditional);
    sortPairs(&vecPairs);
    mergeSort(&vecPairs, vecPairs.begin(), vecPairs.end(), 0);

    std::vector<int> vecMain;
    initMainChain(&vecMain, vecPairs);
    double vecTime = sortAndMeasure(vecMain, vecPairs, vecAdditional);

    std::deque<std::pair<int, int>> deqPairs;
    int deqAdditional = -1;
    createPairs(&deqPairs, _unsorted, &deqAdditional);
    sortPairs(&deqPairs);
    mergeSort(&deqPairs, deqPairs.begin(), deqPairs.end(), 0);

    std::deque<int> deqMain;
    initMainChain(&deqMain, deqPairs);
    double deqTime = sortAndMeasure(deqMain, deqPairs, deqAdditional);

    std::cout << "After: ";
    printContainer(vecMain);

    std::cout << "Time to process a range of " << _unsorted.size() << " elements with std::vector : " << vecTime << " us" << std::endl;
    std::cout << "Time to process a range of " << _unsorted.size() << " elements with std::deque : " << deqTime << " us" << std::endl;
}
