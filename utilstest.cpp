#include "expsolver.h"
#include "tokenizer.h"
#include "thread.h"
#include "urlparser.h"
#include <vector>

#include <math.h>

using namespace gnilk;

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
	printf("if: %f\n",tmp);
	ExpSolver::Solve(&tmp, "4<1?3*2+1");
	printf("if: %f\n",tmp);
	ExpSolver::Solve(&tmp, "4<1?");
	printf("if: %f\n",tmp);
}

int main (int argc, char **argv) {
	printf("hello world\n");

	printf("%f\n", fmod(5.5,1.0));

	testExpSolver();

	// TODO: put test code here!
	return 0;
}
