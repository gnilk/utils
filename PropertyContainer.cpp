/*-------------------------------------------------------------------------
 File    : $Archive: $
 Author  : $Author: $
 Version : $Revision: $
 Orginal : 
 Descr   : Native INI file handling (no comments supported - need to extend Tokenizer)
 
 
 Modified: $Date: $ by $Author: $
 ---------------------------------------------------------------------------
 TODO: [ -:Not done, +:In progress, !:Completed]
 <pre>
 	- Remove duplicated code by hold a "default" section in the PropertyContainer
 	- Support for comments (need to modify Tokenizer)
 </pre>
 
 
 \History
 - 23.03.2014, FKling, Implementation 
 
 ---------------------------------------------------------------------------*/
#include <map>
#include <string>
#include "tokenizer.h"
#include "PropertyContainer.h"

using namespace gnilk;
using namespace std;

PropertyContainer::PropertyContainer()
{

}
PropertyContainer::~PropertyContainer()
{

}
int PropertyContainer::AddFromBuffer(char *buffer) {
	return AddFromBuffer(buffer, DEFAULT_PROP_SEPARATORS);
}
int PropertyContainer::AddFromBuffer(char *buffer, char *separator)
{
	int parsedItems = 0;
	char *currentSection = NULL;
    Tokenizer tok(buffer, separator);
    while(tok.HasMore()) {
    	// Section handling
    	if (!strcmp(tok.Peek(),(char *)"[")) {
    		tok.Next();	// '['
    		currentSection = (char *)tok.Next();
    		tok.Next(); // ']'
			continue;	// Jump back to top
    	}
    	// Read in key
        char *cmd = (char *)tok.Next();
        // Next must be '='
        if (strcmp(tok.Peek(),(char *)"=")) {
            printf("Syntax error, expected '=', found '%s'\n", tok.Peek());
            return -1;
        } else {
            tok.Next(); // swallow '='
        }
        // Read in '='
        char *param = (char *)tok.Next();

        // Add to 'section' if we have one, otherwise to the default
        if (currentSection == NULL) {
        	AddItem(cmd, param);
        	parsedItems++;
        } else {
        	AddItem(currentSection, cmd, param);
        	parsedItems++;
        }
    }
    return parsedItems;
}

// functions to get stuff from a specific section
bool PropertyContainer::AddItem(char *section, char *key, char *value)
{
	// have section already - add to it
	map<string,Section *>::iterator item = sections.find(key);
	if (item != sections.end()) {
		return item->second->AddItem(key, value);
	} 
	// No section - create one
	Section *s = new Section(std::string(section));
	sections.insert(pair<string, Section *>(section, s));
	return s->AddItem(key, value);

}
int PropertyContainer::NumSections()
{
	return sections.size();
}

bool PropertyContainer::HasItem(char *section, char *key)
{
	map<string,Section *>::iterator item = sections.find(key);
	if (item != sections.end()) {
		return item->second->HasItem(key);
	} 
	return false;
}

const char *PropertyContainer::GetItem(char *secton, char *key)
{
	map<string,Section *>::iterator item = sections.find(key);
	if (item != sections.end()) {
		return item->second->GetItem(key);
	} 
	return NULL;

}

// Default - non-section - related items
bool PropertyContainer::AddItem(char *key, char *value)
{
	items.insert(pair<string, string>(key, value));
	return true;
}

bool PropertyContainer::HasItem(char *key)
{
	return false;	// not implemented
}

int PropertyContainer::NumItems()
{
	return items.size();
}

// THIS IS A STUPID FUNCTION - USE ONLY FOR DEBUGGING!!!
const char *PropertyContainer::GetItemAt(int index)
{
	map<string, string>::iterator it = GetItForIndex(index);
	if (it != items.end()) {
		return it->second.c_str();
	}
	return NULL;
}

const char *PropertyContainer::GetKeyAt(int index) {
	map<string, string>::iterator it = GetItForIndex(index);
	if (it != items.end()) {
		return it->first.c_str();
	}
	return NULL;	
}

std::map<std::string, std::string>::iterator PropertyContainer::GetItForIndex(int index)
{
	int i = 0;
	map<string, string>::iterator it;
	for(it = items.begin(); it != items.end(); it++) {
		if (i == index) return it;
		i++;
	}
	return items.end();
}


const char *PropertyContainer::GetItem(char *key)
{
	map<string,string>::iterator item = items.find(key);
	if (item != items.end()) {
		return item->second.c_str();
	}
	return NULL;
}

void PropertyContainer::Dump(std::string &out)
{
	map<string,string>::iterator it = items.begin();
	for(;it != items.end(); it++) {
		out.append(it->first + std::string("=") + it->second + std::string("\n"));
	}
	map<string,Section *>::iterator sit = sections.begin();
	for(;sit != sections.end(); sit++) {
		out.append(std::string("[") + sit->first + std::string("]") + std::string("\n"));
		sit->second->Dump(out);
	}
}

// Section
Section::Section(std::string name)
{
	this->name = name;
}
Section::~Section()
{

}

bool Section::AddItem(char *key, char *value)
{
	items.insert(pair<string, string>(key, value));
	return true;
}

bool Section::HasItem(char *key)
{
	return false;	// not implemented
}

int Section::NumItems()
{
	return items.size();
}

// THIS IS A STUPID FUNCTION - USE ONLY FOR DEBUGGING!!!
const char *Section::GetItemAt(int index)
{
	map<string, string>::iterator it = GetItForIndex(index);
	if (it != items.end()) {
		return it->second.c_str();
	}
	return NULL;
}
const char *Section::GetKeyAt(int index) {
	map<string, string>::iterator it = GetItForIndex(index);
	if (it != items.end()) {
		return it->first.c_str();
	}
	return NULL;	
}
std::map<std::string, std::string>::iterator Section::GetItForIndex(int index)
{
	int i = 0;
	map<string, string>::iterator it;
	for(it = items.begin(); it != items.end(); it++) {
		if (i == index) return it;
		i++;
	}
	return items.end();
}

const char *Section::GetItem(char *key)
{
	map<string,string>::iterator item = items.find(key);
	if (item != items.end()) {
		return item->second.c_str();
	}
	return NULL;
}

void Section::Dump(std::string &out)
{
	map<string,string>::iterator it = items.begin();
	for(;it != items.end(); it++) {
		out.append(it->first + std::string("=") + it->second + std::string("\n"));
	}
}
