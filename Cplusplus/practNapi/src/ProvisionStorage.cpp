#include "ProvisionStorage.h"

#include <fstream>

using namespace std;

ProvisionStorage::ProvisionStorage(std::string filename): m_filename(filename) {
	// Nothing else
}

ProvisionStorage::~ProvisionStorage() {
}

std::string ProvisionStorage::read() {
	std::ifstream fs(m_filename);
	if (!fs.is_open()) {
		return "{}";
	}

	std::string jsonOut = "";
	std::string linein;

	while (getline(fs, linein))
		jsonOut.append(linein);

	fs.close();
	return jsonOut;
}

bool ProvisionStorage::write(std::string provisions) {
	std::ofstream fs(m_filename);
	if (!fs.is_open()) {
		std::cout << "ERROR: Unable to open provision file " << m_filename << " to write provisions" << endl;
		return false;
	}

	fs << provisions << endl;

	fs.close();

	return true;
}
