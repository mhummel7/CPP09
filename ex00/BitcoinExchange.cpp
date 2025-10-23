/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhummel <mhummel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 09:56:16 by mhummel           #+#    #+#             */
/*   Updated: 2025/10/23 12:06:47 by mhummel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib> // for std::atof
#include <cctype>  // for std::isspace
#include <iomanip> // for std::setprecision for 0.90 output instead of 0.9
#include <limits> // for std::numeric_limits

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
			char* endptr;
			float value = static_cast<float>(strtod(valueStr.c_str(), &endptr));
			if (*endptr != '\0' || value < 0) continue; // Invalid value in DB, skip
			_db[date] = value;
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
	if (_db.empty()) return 0.0f;
	std::map<std::string, float>::iterator it = _db.lower_bound(date);
	if (it != _db.end() && it->first == date) return it->second;
	if (it == _db.begin()) return _db.begin()->second; // Use first if no lower
	--it;
	return it->second;
}

void BitcoinExchange::processInput(const std::string& inputFile) {
	std::ifstream inFile(inputFile.c_str());
	if (!inFile.is_open()) {
		std::cerr << "Error: Could not open file." << std::endl;
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
		char* endptr;
		double valueD = strtod(valueStr.c_str(), &endptr);
		if (*endptr != '\0' || valueD < 0) {
			if (valueD < 0) std::cerr << "Error: not a positive number." << std::endl;
			else std::cerr << "Error: bad input => " << original_line << std::endl;
			continue;
		}
		// Strengere Grenze: 10^15, um Präzisionsverlust zu vermeiden
		if (valueD > 1e15) {
			std::cerr << "Error: Number is too big => " << original_line << std::endl;
			continue;
		}
		// Kein Cast zu float, behalte double
		if (date.length() != 10 || !isValidDate(date)) {
			std::cerr << "Error: bad input => " << original_line << std::endl;
			continue;
		}
		float rate = getExchangeRate(date);
		double result = valueD * static_cast<double>(rate); // Berechnung in double
		std::cout << std::fixed << std::setprecision(2) << date << " => " << valueD << " = " << result << std::endl;
	}
	inFile.close();
}
