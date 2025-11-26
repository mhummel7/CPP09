/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhummel <mhummel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 09:56:16 by mhummel           #+#    #+#             */
/*   Updated: 2025/11/26 15:09:24 by mhummel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {
	std::ifstream dbFile("data.csv");
	if (!dbFile.is_open()) {
		std::cerr << "Error: Could not open database file." << std::endl;
		return;
	}
	std::string line;
	std::getline(dbFile, line); // Skip header
	while (std::getline(dbFile, line)) {
		std::stringstream ss(line);
		std::string date, valueStr;
		if (std::getline(ss, date, ',') && std::getline(ss, valueStr)) {
			date = trim(date);
			valueStr = trim(valueStr);
			try {
				float value = std::stof(valueStr);
				if (value < 0) continue; // Invalid value in DB, skip
				_db[date] = value;
			} catch (...) {
				continue; // Invalid conversion, skip
			}
		}
	}
	if (_db.empty()) {
		std::cerr << "Error: Database is empty or invalid." << std::endl;
	}
	dbFile.close();
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _db(other._db) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
	if (this != &other) {
		_db = other._db;
	}
	return *this;
}

BitcoinExchange::~BitcoinExchange() {}

std::string BitcoinExchange::trim(const std::string& str) {
	size_t start = 0;
	while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start]))) ++start;
	size_t end = str.size();
	while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1]))) --end;
	return str.substr(start, end - start);
}

bool BitcoinExchange::isValidDate(const std::string& date) {
	if (date.length() != 10 || date[4] != '-' || date[7] != '-') return false;
	std::stringstream ss(date);
	int year, month, day;
	char dash1, dash2;
	ss >> year >> dash1 >> month >> dash2 >> day;
	if (dash1 != '-' || dash2 != '-' || ss.fail() || year < 0) return false;
	if (month < 1 || month > 12 || day < 1 || day > 31) return false;
	bool is_leap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
	int max_day;
	if (month == 2) {
		max_day = is_leap ? 29 : 28;
	} else if (month == 4 || month == 6 || month == 9 || month == 11) {
		max_day = 30;
	} else {
		max_day = 31;
	}
	if (day > max_day) return false;
	return true;
}

float BitcoinExchange::getExchangeRate(const std::string& date) {
	if (_db.empty()) return -1.0f;
	std::map<std::string, float>::iterator it = _db.lower_bound(date);
	if (it != _db.end() && it->first == date) return it->second;
	if (it == _db.begin()) {
		// No lower rate available (date too early)
		return -1.0f;
	}
	--it;
	return it->second;
}

void BitcoinExchange::processInput(const std::string& inputFile) {
	std::ifstream inFile(inputFile.c_str());
	if (!inFile.is_open()) {
		std::cerr << "Error: could not open file." << std::endl;
		return;
	}
	// Check if file is empty
	inFile.seekg(0, std::ios::end);
	if (inFile.tellg() == 0) {
		std::cerr << "Error: empty input file." << std::endl;
		inFile.close();
		return;
	}
	inFile.seekg(0, std::ios::beg);

	// Skip the header line
	std::string header;
	std::getline(inFile, header);

	std::string line;
	while (std::getline(inFile, line)) {
		std::string original_line = line;  // For error messages
		line = trim(line);
		if (line.empty()) continue;
		std::stringstream ss(line);
		std::string date, valueStr;
		if (!(std::getline(ss, date, '|') && std::getline(ss, valueStr))) {
			std::cerr << "Error: bad input => " << original_line << std::endl;
			continue;
		}
		date = trim(date);
		valueStr = trim(valueStr);
		double valueD;
		try {
			valueD = std::stod(valueStr);
		} catch (const std::invalid_argument&) {
			std::cerr << "Error: bad input => " << original_line << std::endl;
			continue;
		} catch (const std::out_of_range&) {
			std::cerr << "Error: too large a number." << std::endl;
			continue;
		}
		if (valueD < 0) {
			std::cerr << "Error: not a positive number." << std::endl;
			continue;
		}
		if (valueD > 1000.0) {
			std::cerr << "Error: too large a number." << std::endl;
			continue;
		}
		if (date.length() != 10 || !isValidDate(date)) {
			std::cerr << "Error: bad input => " << original_line << std::endl;
			continue;
		}
		float rate = getExchangeRate(date);
		if (rate < 0) {
			std::cerr << "Error: no exchange rate available for date " << date << std::endl;
			continue;
		}
		double result = valueD * static_cast<double>(rate);
		std::cout << date << " => " << valueD << " = " << result << std::endl;
	}
	inFile.close();
}
