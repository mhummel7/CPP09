/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhummel <mhummel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 09:22:26 by mhummel           #+#    #+#             */
/*   Updated: 2025/11/26 15:15:32 by mhummel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

/*  CPP09 ex02 – PmergeMe (Ford-Johnson / merge-insertion sort)
    Containers: std::vector and std::deque
    Reason    : subject requires two different STL containers + time comparison
                → vector (best cache locality) + deque (fast push_front/back)    */

// test with this:
// ./PmergeMe $(shuf -i 1-100000 -n 3000 | tr "\n" " ")

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cerr << "Error: invalid arguments. Usage: ./pmergeme numbers..." << std::endl;
		return 1;
	}
	PmergeMe pmerge;
	try {
		pmerge.parseInput(argc, argv);
		pmerge.execute();
	} catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
