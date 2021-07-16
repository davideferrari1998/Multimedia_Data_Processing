#include "lz78encode.h"

#include <iostream>
#include <fstream>
#include <map>


template<typename T>
std::istream& raw_read(std::istream& is, T& num, size_t size = sizeof(T))
{
	return is.read(reinterpret_cast<char*>(&num), size);
}
template<typename T>
std::ostream& raw_write(std::ostream& os, const T& num, size_t size = sizeof(T))
{
	return os.write(reinterpret_cast<const char*>(&num), size);
}

class bitreader {
	std::istream& is_;
	uint8_t buffer_;
	size_t nbits_;

	uint32_t read_bit() {
		if (nbits_ == 0) {
			raw_read(is_, buffer_);
			nbits_ = 8;
		}
		--nbits_;
		return (buffer_ >> nbits_) & 1;
	}

public:
	bitreader(std::istream& is) : is_(is), nbits_(0) {}

	std::istream& read(uint32_t& u, size_t n) {
		u = 0;
		while (n-- > 0) {
			u = (u << 1) | read_bit();
		}
		return is_;
	}

	std::istream& read(int32_t& i, size_t n) {
		uint32_t u;
		read(u, n);
		i = static_cast<int32_t>(u);
		return is_;
	}
};

class bitwriter {
	std::ostream& os_;
	uint8_t buffer_;
	size_t nbits_;

	std::ostream& write_bit(uint32_t u) {
		// buffer_ = buffer_ * 2 + u % 2;
		buffer_ = (buffer_ << 1) | (u & 1);
		++nbits_;
		if (nbits_ == 8) {
			raw_write(os_, buffer_);
			nbits_ = 0;
		}
		return os_;
	}

public:
	bitwriter(std::ostream& os) : os_(os), nbits_(0) {}

	~bitwriter() {
		flush();
	}

	std::ostream& write(uint32_t u, size_t n) {
		while (n-- > 0) {
			write_bit(u >> n);
		}
		return os_;
	}

	std::ostream& operator()(uint32_t u, size_t n) {
		return write(u, n);
	}

	void flush(uint32_t u = 0) {
		while (nbits_ > 0) {
			write_bit(u);
		}
	}
};


bool lz78encode(const std::string& input_filename, const std::string& output_filename, int maxbits) {

	std::ifstream is(input_filename, std::ios::binary);
	if (!is) {
		return false;
	}

	std::ofstream os(output_filename, std::ios::binary);
	if (!os) {
		return false;
	}

	if (maxbits < 1 || maxbits > 30) {
		printf("ERROR: 1 < MAXBITS < 30");
		return false;
	}

	
	bitreader br(is);
	bitwriter bw(os);
	std::map<std::string, uint32_t> dict;
	int nbit_dict = 1;
	uint32_t index_dict = 1;
	char c;
	std::string str = "";
	uint32_t val_dict;
	bool avoid_first_zero = true;
	bool dict_just_clear = false;


	// scrivo il MAGIC NUMBER
	os << "LZ78";

	// scrivo maxbits con 5 bits
	bw.write(maxbits, 5);

	//inizio la lettura dal file
	while (raw_read(is, c)) {

		str = str + c;
		
		//Aggiungo un nuovo elemento nel dizionario se non è già presente
		if (dict.find(str) == dict.end()) {

			dict.insert(std::pair<std::string, uint32_t>(str, index_dict));
			index_dict++;
			val_dict = 0;
			// se supero maxbits svuoto il dizionario
			if (index_dict > (std::pow(2, maxbits))) {
				dict.clear();
				index_dict = 1;
				dict_just_clear = true;
			}
				
		}
		else{

			while (raw_read(is, c)) {

				val_dict = dict[str];

				str = str + c;

				if (dict.find(str) == dict.end()) {

					dict.insert(std::pair<std::string, uint32_t>(str, index_dict));
					index_dict++;
					// se supero maxbits svuoto il dizionario
					if (index_dict > (std::pow(2, maxbits))) {
						dict.clear();
						index_dict = 1;
						dict_just_clear = true;
					}
					break;

				}

			}

		}

		//scrivo il val del dict con i giusti bit e NON scrivo il primo 0 come da specifiche
		if (avoid_first_zero) 
			avoid_first_zero = false;
		else
			bw.write(val_dict, nbit_dict);
		

		//scrivo il prossimo carattere da scrivere
		bw.write(c, 8);

		//svuoto la stringa
		str = "";


		// se il dizionario è stato resettato ripristino le sue variabili
		if (dict_just_clear) {
			nbit_dict = 1;
			avoid_first_zero = true;
			dict_just_clear = false;
		}

		//gestisco il numero di bit del dict
		if(dict.size() >= 2 && dict.size() < 4)
			nbit_dict = 2;

		if (dict.size() >= 4 && dict.size() < 8)
			nbit_dict = 3;

		if (dict.size() >= 8 && dict.size() < 16)
			nbit_dict = 4;

		if (dict.size() >= 16)
			nbit_dict = 5;

	}

	//Effettuo il padding in automatico con flush


	return true;
}