#include <testinterface.h>

#include "xmlparser.h"
#include <vector>
#include <functional>

#include <math.h>

using namespace gnilk;
using namespace std::placeholders;

extern "C" {
    int test_xmlparser_general(ITesting *t);
}

// --  
//
// XML Test
//
static const char * xmldef = \
"	<service name=\"ping\">\n"\
"		<description>sends icmp packets on the network</description>\n"\
"		<input>\n"\
"			<property name=\"host\" display=\"Host\" type=\"string\" default=\"127.0.0.1\" description=\"name or ip for host to ping\" />\n"\
"			<property name=\"packetsize\" display=\"Packet Size\" type=\"int\" default=\"128\" description=\"number of bytes per packet\" />\n"\
"			<property name=\"frag\" display=\"Allow fragmentation\" type=\"bool\" default=\"false\" description=\"allow IP fragmentation\" />\n"\
"			<property name=\"packets\" display=\"Packets\" type=\"int\" default=\"3\" description=\"Number of packets\" />\n"\
"			<property name=\"timeout\" display=\"Timeout per packet\" type=\"float\" default=\"5\" description=\"Maxmimum time (in seconds) to wait for a response per ping packet\" />\n"\
"			<property name=\"ttl\" display=\"Packet Time To Live\" type=\"int\" default=\"128\" description=\"Maxmimum time (in seconds) to wait for a response per ping packet\" />\n"\
"		</input>\n"\
"		<output>\n"\
"		</output>\n"\
"	</service>\n";


using namespace gnilk::xml;
void xmlTagStartHandler(ITag *tag, std::list<IAttribute *> &attributes) {
	printf("%s - %s\n",tag->getName().c_str(), tag->getAttributeValue("name","").c_str());
}
void xmlTagEndHandler(ITag *tag, std::list<IAttribute *> &attributes) {
	printf("%s - %s\n",tag->getName().c_str(), tag->getAttributeValue("name","").c_str());
}

int test_xmlparser_general(ITesting *t) {

	// printf("Testing XML Parser\n");
	// printf("DEF\n%s\n",xmldef);
	Document *doc = Parser::loadXML(xmldef);
    if (doc == NULL) {
        return kTR_Fail;
    }
	//doc->dumpTagTree(doc->getRoot(), 0);

	ITag *tagRoot = doc->getRoot();
	tagRoot = tagRoot->getFirstChild("service");
    if (tagRoot == NULL) {
        return kTR_Fail;
    }
	ITag *tagInput = tagRoot->getFirstChild("input");
    if (tagInput == NULL) {
        return kTR_Fail;
    }
//    printf("'%s' found\n", tagInput->getName().c_str());
    ITag *tagFrag = tagInput->getChildWithAttributeValue("property","name","frag");
    if (tagFrag == NULL) {
        return kTR_Fail;
    }

    printf("'%s' found\n", tagFrag->getName().c_str());
    printf("  %s\n",tagFrag->getAttributeValue("name","").c_str());
    printf("  %s\n",tagFrag->getAttributeValue("display","").c_str());
    printf("  %s\n",tagFrag->getAttributeValue("type","").c_str());
    printf("  %s\n",tagFrag->getAttributeValue("default","").c_str());
    printf("  %s\n",tagFrag->getAttributeValue("description","").c_str());
    

	printf("Test XML Doc traversal\n");
	doc->traverse(std::bind(xmlTagStartHandler, std::placeholders::_1, std::placeholders::_2),
				  std::bind(xmlTagEndHandler, std::placeholders::_1, std::placeholders::_2));
    
    return kTR_Pass;
}
