/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhummel <mhummel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 10:40:24 by mhummel           #+#    #+#             */
/*   Updated: 2025/10/22 11:15:14 by mhummel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <iostream>

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << "Error: invalid arguments. Usage: ./btc <file>" << std::endl;
		return 1;
	}
	BitcoinExchange exchange;
	exchange.processInput(argv[1]);
	return 0;
}
