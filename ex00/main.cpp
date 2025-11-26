/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhummel <mhummel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 10:40:24 by mhummel           #+#    #+#             */
/*   Updated: 2025/11/26 15:15:52 by mhummel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

/*  CPP09 ex00 – Bitcoin Exchange
    Container: std::map<std::string, float>
    Reason   : ordered map → automatic sorting + O(log n) lower_bound for dates  */

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << "Error: invalid arguments. Usage: ./btc <file>" << std::endl;
		return 1;
	}
	BitcoinExchange exchange;
	exchange.processInput(argv[1]);
	return 0;
}
