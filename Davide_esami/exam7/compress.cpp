#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "mat.h"
#include "ppm.h"


void PackBitsEncode(const mat<uint8_t>& img, std::vector<uint8_t>& encoded) {

	uint8_t run=0;

	std::vector<uint8_t> copy;

	if (img.size() == 0) {
		encoded.push_back(128);
		return;
	}

	for (size_t i = 0; i < img.size(); ++i) {

		//Controllo Copy come ultimo elemento 
		if ((i + 1) >= img.size()) {
			encoded.push_back(0);
			encoded.push_back(img.rawdata()[i]);
			break;
		}

		//Siamo in RUN
		if (img.rawdata()[i] == img.rawdata()[i + 1]) {
				
			for (size_t j = i; j < img.size(); ++j) {

				if ((j + 1) >= img.size()) {
					++run;
					encoded.push_back(257 - run);
					encoded.push_back(img.rawdata()[i]);
					i = j;
					break;
				}

				if (img.rawdata()[j] == img.rawdata()[j + 1])
					++run;
				else{
					++run;
					encoded.push_back(257 - run);
					encoded.push_back(img.rawdata()[i]);
					run = 0;
					i = j;
					break;
				}

			}

		}
		else{

			//SIAMO IN COPY
			for (size_t j = i; j < img.size(); ++j) {

				if ((j + 1) >= img.size()) {

					copy.push_back(img.rawdata()[j]);
					encoded.push_back(copy.size() - 1);

					for(auto& x : copy)
						encoded.push_back(x);

					copy.clear();
					i = j;

					break;
				}

				if (img.rawdata()[j] != img.rawdata()[j + 1]) {
					copy.push_back(img.rawdata()[j]);
				}
				else{

					encoded.push_back(copy.size() - 1);
					for (auto& x : copy)
						encoded.push_back(x);

					copy.clear();
					i = j-1;

					break;
				}

			}

		}

	}

	encoded.push_back(128);

}



std::string Base64Encode(const std::vector<uint8_t>& v) {

	std::vector<uint8_t> vec = v;
	std::string encode = "";
	std::unordered_map<char, uint8_t> code;
	uint8_t codice = 0;
	char v1;
	char v2;
	char v3;
	char v4;

	if (v.empty())
		return "";

	//EFFETTUO PADDING
	while ((vec.size() % 3) != 0) 
		vec.push_back(128);
	
	for (char i = 'A'; i <= 'Z'; ++i) {
		code.insert(std::pair<char, uint8_t>(i, codice));
		++codice;
	}

	for (char i = 'a'; i <= 'z'; ++i) {
		code.insert(std::pair<char, uint8_t>(i, codice));
		++codice;
	}

	for (char i = '0'; i <= '9'; ++i) {
		code.insert(std::pair<char, uint8_t>(i, codice));
		++codice;
	}

	code.insert(std::pair<char, uint8_t>('+', 62));
	code.insert(std::pair<char, uint8_t>('/', 63));

	for (size_t i = 0; i < vec.size(); i = i + 3) {

		v1 = vec[i] >> 2;
		v2 = ((vec[i] << 4) & 48) | (vec[i+1] >>4);
		v3 = ((vec[i + 1] << 2) & 60) | (vec[i + 2] >> 6);
		v4 = vec[i + 2] & 63;

		for (auto& x : code) {
			if(x.second == v1)
				encode.push_back(x.first);
		}

		for (auto& x : code) {
			if (x.second == v2)
				encode.push_back(x.first);
		}

		for (auto& x : code) {
			if (x.second == v3)
				encode.push_back(x.first);
		}

		for (auto& x : code) {
			if (x.second == v4)
				encode.push_back(x.first);
		}

	}

	return encode;

}







