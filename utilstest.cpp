#include "expsolver.h"
#include "tokenizer.h"
#include "thread.h"
#include "urlparser.h"
#include "xmlparser.h"
#include <vector>
#include <functional>

#include <math.h>

using namespace gnilk;
using namespace std::placeholders;

extern "C" {
	static double functionCallBack(void *pUser, const char *data, int args, double *arg, int *bOk_out);
	static double varCallBack(void *pUser, const char *data, int *bOk_out);
}

static double varCallBack(void *pUser, const char *data, int *bOk_out) 
{
	printf("Var: %s\n",data);
	*bOk_out = 1;
	if (!strcmp(data,"t")) return 4;
	*bOk_out = 0;
	return 0;
}
static double functionCallBack(void *pUser, const char *data, int args, double *arg, int *bOk_out)
{
	double result = 0.0;
	*bOk_out = 0;

	printf("Func: %s\n", data);
	printf("Args: %d\n", args);

	for(int i=0;i<args;i++) {
		printf("%d:%f\n", i, arg[i]);
	}


	if (!strcmp(data,"inc")) {
	    *bOk_out = 1;
	    if (args > 0) {
	    	result = 0;
	    	for(int i=0;i<args;i++) {
	    		result += arg[i];
	    	}
	    }
	}


	return result;
}

static void testExpFunc() {
	static const char *expression = "inc(2+1, 2, inc(3+1), 5)*3";
	printf("Testing function calls: %s\n",expression);
	ExpSolver exp(expression);
	exp.RegisterUserFunctionCallback(functionCallBack, NULL);
	if (exp.Prepare()) {
		double val = exp.Evaluate();
		printf("Value: %f\n",val);
	}	
	printf("done\n");
}

static void testExpVar() {
	printf ("Testing variables\n");
	
	ExpSolver exp("t+1 inc(1)");
	exp.RegisterUserFunctionCallback(functionCallBack, NULL);
	exp.RegisterUserVariableCallback(varCallBack, NULL);

	if (exp.Prepare()) {
		double val = exp.Evaluate();
		printf("Value: %f\n",val);
	}		
}

static void testExpSolver() {

	printf("Testing Exp Solver\n");
	// this is testing nested functions
	testExpFunc();
	testExpVar();

	printf("Test simple expressions\n");

	double tmp;
	ExpSolver::Solve(&tmp, "-4+-1");
	printf("Value: %f\n",tmp);
	printf("boolean test\n");
	ExpSolver::Solve(&tmp, "4>1");
	printf("Bool: %f\n",tmp);
	printf("IF Operator\n");
	ExpSolver::Solve(&tmp, "4<1?4*2+1:3*2+1");
	printf("if: %f (7)\n",tmp);
	if (!ExpSolver::Solve(&tmp, "4<1?3*2+1")) {
		printf("Ok, should not work\n");
	}
	if (!ExpSolver::Solve(&tmp, "4<1?")) {
		printf("Ok, should not work\n");
	}
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

void testXMLParser() {
	printf("Testing XML Parser\n");
	printf("DEF\n%s\n",xmldef);
	Document *doc = Parser::loadXML(xmldef);
	doc->dumpTagTree(doc->getRoot(), 0);

	ITag *tagRoot = doc->getRoot();
	tagRoot = tagRoot->getFirstChild("service");
	ITag *tagInput = tagRoot->getFirstChild("input");
	if (tagInput != NULL) {
		printf("'%s' found\n", tagInput->getName().c_str());
		ITag *tagFrag = tagInput->getChildWithAttributeValue("property","name","frag");
		if (tagFrag != NULL) {
			printf("'%s' found\n", tagFrag->getName().c_str());
			printf("  %s\n",tagFrag->getAttributeValue("name","").c_str());
			printf("  %s\n",tagFrag->getAttributeValue("display","").c_str());
			printf("  %s\n",tagFrag->getAttributeValue("type","").c_str());
			printf("  %s\n",tagFrag->getAttributeValue("default","").c_str());
			printf("  %s\n",tagFrag->getAttributeValue("description","").c_str());
		}
	}
	printf("Test XML Doc traversal\n");
	doc->traverse(std::bind(xmlTagStartHandler, std::placeholders::_1, std::placeholders::_2),
				  std::bind(xmlTagEndHandler, std::placeholders::_1, std::placeholders::_2));
}

int main (int argc, char **argv) {
	printf("hello world\n");

	//printf("%f\n", fmod(5.5,1.0));

	testXMLParser();
	testExpSolver();

	// TODO: put test code here!
	return 0;
}
