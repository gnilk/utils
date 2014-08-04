#include "expsolver.h"
#include "tokenizer.h"
#include "thread.h"
#include "urlparser.h"

using namespace gnilk;

extern "C" {
	static double functionCallBack(void *pUser, const char *data, int args, double *arg, int *bOk_out);
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

static void testExpSolver() {

	printf("Testing Exp Solver\n");
	// this is testing nested functions
	ExpSolver exp("inc(2+1, 2, inc(3+1), 5)*3");
	exp.RegisterUserFunctionCallback(functionCallBack, NULL);

	if (exp.Prepare()) {
		double val = exp.Evaluate();
		printf("Value: %f\n",val);
	}	

	double tmp;
	ExpSolver::Solve(&tmp, "-4+-1");
	printf("Value: %f\n",tmp);
}

int main (int argc, char **argv) {
	printf("hello world\n");


	testExpSolver();

	// TODO: put test code here!
	return 0;
}
