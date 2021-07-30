#include "hufstr.h"

void scrivi_su_file_compression(std::string& s, std::vector<triplet> huff_code) {

	char c;
	uint8_t l = 0;
	uint32_t h_code = 0;

	std::ofstream os("compression.bin", std::ios::binary);
	if (!os) {
		std::cout << "Errore nella creazione di compression.bin";
		return;
	}

	bitwriter bw(os);

	for (size_t i = 0; i < s.size(); ++i) {

		c = s.at(i);

		for (auto& x : huff_code) {

			if (x.sym == c) {
				l = x.len;
				h_code = x.code;
				break;
			}

		}
		bw.write(h_code, l);
	}

	return;

}

void scrivi_su_file_decompression(std::vector<uint8_t> v) {

	std::ofstream os("decompression.bin", std::ios::binary);
	if (!os) {
		std::cout << "Errore nella creazione di decompression.bin";
		return;
	}

	for (auto& x : v) {
		raw_write(os, x);
	}

	return;
}

std::vector<uint8_t> hufstr::compress(const std::string& s) const {

	std::vector<uint8_t> comp;
	std::string str = s;

	//Controllo se la stringa s è vuota
	if (s.size() == 0)
		return comp;

	//Creo un file cui in scrivo il mio byte stream corretto;

	scrivi_su_file_compression(str, huff_code);

	// ORA LEGGO COMPRESSION.BIN e creo il vettore finale
	std::ifstream is("compression.bin", std::ios::binary);
	if (!is) {
		std::cout << "Errore nell'apertura compression.bin";
		return comp;
	}

	uint8_t byte;
	//Leggo un byte alla volta dal mio file e lo aggiungo al mio vettore
	while (raw_read(is, byte)) {
		comp.push_back(byte);
	}

	return comp;
}

std::string hufstr::decompress(const std::vector<uint8_t>& v) const {

	std::vector<uint8_t> vec = v;
	std::string decompress = "";

	//Controllo se il vettore v è vuoto
	if (v.size() == 0)
		return decompress;

	// CREO UN FILE SU CUI SCRIVERE IL MIO BYTE STREAM
	scrivi_su_file_decompression(v);

	// ORA LEGGO DECOMPRESSION.BIN e creo la stringa finale
	std::ifstream is("decompression.bin", std::ios::binary);
	if (!is) {
		std::cout << "Errore nell'apertura decompression.bin";
		return decompress;
	}

	uint32_t buffer = 0;
	uint32_t read_buffer = 0;
	uint8_t buffer_len = 0;
	bitreader br(is);

	//Leggo un bit alla volta dal mio file, e se ha la lunghezza corretta con la codifica, controllo se anche la codifica è
	//uguale, nel caso lo aggiungo al mio vettore finale
	while (br.read(read_buffer, 1)) {

		//Leggo un bit e lo aggiungo al mio buffer ed incremento la sua lunghezza
		buffer = (buffer << 1 ) | read_buffer;
		++buffer_len;
		for (auto& x : huff_code) {

			if (buffer_len == x.len) {
				if (x.code == buffer) {
					//Trovata la codifica aggiungo il carattere alla mia stringa e azzero buffer e buffer_len
					decompress.push_back(x.sym);
					buffer = 0;
					buffer_len = 0;
					break;
				}
			}
				
		}

	}
	return decompress;
}