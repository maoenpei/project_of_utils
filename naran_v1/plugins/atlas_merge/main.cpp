
#include "Naran.h"
#include "MergeStage.h"

using namespace Naran;

CLS_SHARE_MEMORY();

grab(MergeStage) g_merge;
Array<more(char)> input_paths;
more(char) image_path;
more(char) plist_path;

class ArguOp
{
public:
	int argu;
	const char *name;
	ArguOp(const char *_name, int _argu) : name(_name), argu(_argu) {}
	virtual ~ArguOp(){}
	int getArguCount(){return argu;}
	const char *getName(){return name;}
	virtual void dealWithString1(const char *str){}
	virtual void dealWithString2(const char *str, const char *str2){}
};

class ArguOpFile : public ArguOp
{
public:
	ArguOpFile() : ArguOp("", 1){}
	virtual void dealWithString1(const char *str){
		input_paths.insert(Utils::copyStr(str));
		g_merge->addImage(str);
	}
};

class ArguOpSort : public ArguOp
{
public:
	ArguOpSort() : ArguOp("--sort", 1){}
	virtual void dealWithString1(const char *str){
		if (0 == strcmp(str, "width")){
			g_merge->setSort(ImageSort_Width);
		}else if (0 == strcmp(str, "height")){
			g_merge->setSort(ImageSort_Height);
		}else if (0 == strcmp(str, "area")){
			g_merge->setSort(ImageSort_Area);
		}else if (0 == strcmp(str, "length")){
			g_merge->setSort(ImageSort_Length);
		}else{
			g_merge->setSort(ImageSort_None);
		}
	}
};

class ArguOpMaxSize : public ArguOp
{
public:
	ArguOpMaxSize() : ArguOp("--max-size", 2){}
	virtual void dealWithString2(const char *str, const char *str2){
		g_merge->setMaxSize(g2d::Sizei(atoi(str), atoi(str2)));
	}
};

class ArguOpMaxWidth : public ArguOp
{
public:
	ArguOpMaxWidth() : ArguOp("--max-width", 1){}
	virtual void dealWithString1(const char *str){
		g_merge->setMaxWidth(atoi(str));
	}
};

class ArguOpMaxHeight : public ArguOp
{
public:
	ArguOpMaxHeight() : ArguOp("--max-height", 1){}
	virtual void dealWithString1(const char *str){
		g_merge->setMaxHeight(atoi(str));
	}
};

class ArguOpAllowRotation : public ArguOp
{
public:
	ArguOpAllowRotation() : ArguOp("--allow-rotation", 1){}
	virtual void dealWithString1(const char *str){
		g_merge->setAllowRotation(0 == strcmp(str, "true"));
	}
};

class ArguOpForceSquare : public ArguOp
{
public:
	ArguOpForceSquare() : ArguOp("--force-square", 1){}
	virtual void dealWithString1(const char *str){
		g_merge->setSquare(0 == strcmp(str, "true"));
	}
};

class ArguOpPOT : public ArguOp
{
public:
	ArguOpPOT() : ArguOp("--pot", 1){}
	virtual void dealWithString1(const char *str){
		g_merge->setPOT(0 == strcmp(str, "true"));
	}
};

class ArguOpInnerPadding : public ArguOp
{
public:
	ArguOpInnerPadding() : ArguOp("--inner-padding", 1){}
	virtual void dealWithString1(const char *str){
		g_merge->setInnerPadding(atoi(str));
	}
};

class ArguOpBorderPadding : public ArguOp
{
public:
	ArguOpBorderPadding() : ArguOp("--border-padding", 1){}
	virtual void dealWithString1(const char *str){
		g_merge->setBorderPadding(atoi(str));
	}
};

class ArguOpTrim : public ArguOp
{
public:
	ArguOpTrim() : ArguOp("--trim", 1){}
	virtual void dealWithString(const char *str){
		g_merge->setTrim(0 == strcmp(str, "true"));
	}
};

class ArguOpOutputPath : public ArguOp
{
public:
	ArguOpOutputPath() : ArguOp("--output", 1){}
	virtual void dealWithString(const char *str){
		image_path = Utils::copyStr(str);
	}
};

class ArguOpOutputPlist : public ArguOp
{
public:
	ArguOpOutputPlist() : ArguOp("--data", 1){}
	virtual void dealWithString(const char *str){
		plist_path = Utils::copyStr(str);
	}
};

int main(int argc, char **argv)
{
	g_merge = MergeStage::create();
	image_path = Utils::copyStr("out.png");
	plist_path = Utils::copyStr("out.plist");

	grab(ArguOp) ops[] = {
		new ArguOpFile(),
		new ArguOpSort(),
		new ArguOpMaxSize(),
		new ArguOpMaxWidth(),
		new ArguOpMaxHeight(),
		new ArguOpAllowRotation(),
		new ArguOpForceSquare(),
		new ArguOpPOT(),
		new ArguOpInnerPadding(),
		new ArguOpBorderPadding(),
		new ArguOpTrim(),
		new ArguOpOutputPath(),
	};
	IntegerMap(grab(ArguOp)) opMap;
	grab(ArguOperation) ao = ArguOperation::create(0);
	opMap.set(0, ops[0]);
	for (int i = 1; i<(sizeof(ops)/sizeof(grab(ArguOp))); i++){
		ao->addOperator(ops[i]->getName(), ops[i]->getArguCount(), i);
		opMap.set(i, ops[0]);
	}
	
	arr(char *) args(new char *[argc], argc);
	ao->reset(args);
	
	interf(IArguOperator) op;
	while((op = ao->next())){
		arr(char *) params = op->getStrings();
		int t = op->type();
		grab(ArguOp) op = opMap.get(t);
		switch(params.count())
		{
		case 1:
			op->dealWithString1(params[0]);
			break;
		case 2:
			op->dealWithString2(params[0], params[1]);
			break;
		default:
			break;
		}
	}

	if (!g_merge->merge()){
		printf("args:\n"
			"--trim\n"
			"--border-padding\n"
			"--inner-padding\n"
			"--pot\n"
			"--force-square\n"
			"--allow-rotation\n"
			"--sort\n"
			"--max-size\n"
			"--max-width\n"
			"--max-height\n"
			"--output\n");
	}else{
		g_merge->makeImage(image_path.get());
	}
	return 0;
}

