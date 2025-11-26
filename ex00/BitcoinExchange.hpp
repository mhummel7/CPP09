/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhummel <mhummel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 09:25:41 by mhummel           #+#    #+#             */
/*   Updated: 2025/11/26 15:13:47 by mhummel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <fstream> // for std::ifstream
#include <iostream> // for std::cerr, std::endl
#include <sstream> // for std::stringstream
#include <cctype>  // for std::isspace
#include <limits>  // for std::numeric_limits
#include <string>  // for std::stod, std::stof
#include <string> // for std::string
#include <map> // for std::map
//	std::map because keys (dates) are automatically sorted and we need
//	lower_bound / upper_bound to find the nearest lower or equal date in O(log n)

class BitcoinExchange {
	private:
		std::map<std::string, float> _db;

		bool isValidDate(const std::string& date);
		float getExchangeRate(const std::string& date);
		std::string trim(const std::string& str);

	public:
		BitcoinExchange(); // Loads the DB from data.csv
		BitcoinExchange(const BitcoinExchange& other);
		BitcoinExchange& operator=(const BitcoinExchange& other);
		~BitcoinExchange();

		void processInput(const std::string& inputFile);
};

#endif
