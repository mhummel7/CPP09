/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhummel <mhummel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 09:25:41 by mhummel           #+#    #+#             */
/*   Updated: 2025/10/22 11:13:06 by mhummel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>

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
