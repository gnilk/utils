#ifndef __PROPERTY_CONTAINER_H__
#define __PROPERTY_CONTAINER_H__

#include <map>
#include <string>

namespace gnilk {

class Section
{
public:
	Section(std::string name);
	virtual ~Section();


	bool AddItem(char *key, char *value);
	int NumItems();
	bool HasItem(char *key);
	const char *GetKeyAt(int index);
	const char *GetItemAt(int index);
	const char *GetItem(char *key);
	void Dump(std::string &out);
private: 
	std::string name;
	std::map<std::string, std::string> items;	
	std::map<std::string, std::string>::iterator GetItForIndex(int index);
};

#define DEFAULT_PROP_SEPARATORS ((char*)("= [ ]"))

class PropertyContainer
{
public:
	PropertyContainer();
	virtual ~PropertyContainer();
	int AddFromBuffer(char *buffer);
	int AddFromBuffer(char *buffer, char *separator);

	bool AddItem(char *section, char *key, char *value);
	int NumSections();
	bool HasItem(char *section, char *key);
	const char *GetItem(char *secton, char *key);

	// Default section access
	bool AddItem(char *key, char *value);
	int NumItems();
	bool HasItem(char *key);
	const char *GetKeyAt(int index);
	const char *GetItemAt(int index);
	const char *GetItem(char *key);
	void Dump(std::string &out);

private:
	std::map<std::string, Section *> sections;
	std::map<std::string, std::string>::iterator GetItForIndex(int index);
	// This is the default section - for all items without a section
	std::map<std::string, std::string> items;
};
}

#endif