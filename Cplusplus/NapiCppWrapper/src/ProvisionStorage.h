#ifndef ProvisionStorage_h
#define ProvisionStorage_h

#include <string>
#include <iostream>

class ProvisionStorage {
public:
	ProvisionStorage(std::string filename);
	~ProvisionStorage();

	// methods
	std::string read();
	bool write(std::string provisions);

private:
	std::string m_filename;
};



#endif // ProvisionStorage_h