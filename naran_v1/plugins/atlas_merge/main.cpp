
#include "Naran.h"

using namespace Naran;

CLS_SHARE_MEMORY();

grab(Configuration) global_config;

enum{
	Op_Files = 0,
	Op_Sort,
};

int main(int argc, char **argv)
{
	arr(char *) args(new char *[argc], argc);
	grab(ArguOperation) ops = ArguOperation::create(Op_Files);
	ops->addOperator("--sort", 1, Op_Sort);
	ops->reset(args);
	interf(IArguOperator) op;
	while((op = ops->next())){
		arr(char *) params = op->getStrings();
		switch(op->type()){
			case Op_Files:
				
				break;
			case Op_Sort:
				break;
			default:
				// printf("not valid operator");
				break;
		}
	}
	return 0;
}

