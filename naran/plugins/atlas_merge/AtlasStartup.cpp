

#include "Naran.h"
#include "plugins/atlas_merge/AtlasMerge.h"

CLS_SHARE_MEMORY();

NS_DEF_NARAN{

	class AtlasArguOperator : public IArguOperator
	{
	public:
		grab(AtlasMerge) mAtlasMerge;
		AtlasArguOperator(grab(AtlasMerge) am) : mAtlasMerge(am) {}
		void run(){}
		void run(const char *argValue)
		{
			grab(Path) path = Path::create(argValue);
			if (path->isFolder()){
				class AtlasTraverse : public IDirectoryTraverse
				{
				public:
					grab(AtlasMerge) mAtlasMerge;
					AtlasTraverse(grab(AtlasMerge) am) : mAtlasMerge(am){}
					bool dealWithDirectory(grab(Path) path)
					{
						grab(Image) img = ImageFactory::shared()->loadFile(path->getChars().get());
						if (img){
							mAtlasMerge->setImage(path->getChars().get(), img);
						}
						return true;
					}
				} traverse(mAtlasMerge);
				grab(Directory) dirs = Directory::create(path);
				dirs->enumTraverse(stablize_nop(IDirectoryTraverse, AtlasTraverse, &traverse));
			}else{
				grab(Image) img = ImageFactory::shared()->loadFile(path->getChars().get());
				if (img){
					mAtlasMerge->setImage(path->getChars().get(), img);
				}
			}
		}
		void run(const char *argValue, const char *argValue2){}
		void run(const char *argValue, const char *argValue2, const char *argValue3){}
	};

	class AtlasTrimOperator : public AtlasArguOperator
	{
	public:
		AtlasTrimOperator(grab(AtlasMerge) am) : AtlasArguOperator(am) {}
		void run(){
			mAtlasMerge->setTrim(true);
		}
	};

	class AtlasNotrimOperator : public AtlasArguOperator
	{
	public:
		AtlasNotrimOperator(grab(AtlasMerge) am) : AtlasArguOperator(am) {}
		void run(){
			mAtlasMerge->setTrim(false);
		}
	};

	class AtlasInnerpaddingOperator : public AtlasArguOperator
	{
	public:
		AtlasInnerpaddingOperator(grab(AtlasMerge) am) : AtlasArguOperator(am) {}
		void run(const char *argValue){
			mAtlasMerge->setInnerPadding(atoi(argValue));
		}
	};

	class AtlasOrdertypeOperator : public AtlasArguOperator
	{
	public:
		AtlasOrdertypeOperator(grab(AtlasMerge) am) : AtlasArguOperator(am) {}
		void run(const char *argValue){
			u32 orderType;
			if (0 == strcmp(argValue, "name")){
				orderType = AtlasOrder_Name;
			}else if (0 == strcmp(argValue, "area")){
				orderType = AtlasOrder_Area;
			}else if (0 == strcmp(argValue, "width")){
				orderType = AtlasOrder_Width;
			}else if (0 == strcmp(argValue, "height")){
				orderType = AtlasOrder_Height;
			}else{
				printf("sort param: name, area, width, height allowed!\n");
				return ;
			}
			mAtlasMerge->setOrderType(orderType);
		}
	};

	class AtlasOrderrevertOperator : public AtlasArguOperator
	{
	public:
		AtlasOrderrevertOperator(grab(AtlasMerge) am) : AtlasArguOperator(am) {}
		void run(){
			mAtlasMerge->setOrderRevert(true);
		}
	};

	class AtlasInputHandler : public IInputHandler
	{
	public:
		virtual void run(arr(char *) args)
		{
			grab(AtlasMerge) am = AtlasMerge::create();
			
			// default operator
			grab(AtlasArguOperator) common = new AtlasArguOperator(am);
			ArguOperation argOp(stablize_grab(IArguOperator, AtlasArguOperator, common));
			
			// trim operator
			grab(AtlasTrimOperator) trim = new AtlasTrimOperator(am);
			argOp.addOperator("--trim", stablize_grab(IArguOperator, AtlasTrimOperator, trim), 0);

			// notrim operator
			grab(AtlasNotrimOperator) notrim = new AtlasNotrimOperator(am);
			argOp.addOperator("--notrim", stablize_grab(IArguOperator, AtlasNotrimOperator, notrim), 0);

			// inner padding operator
			grab(AtlasInnerpaddingOperator) innerpadding = new AtlasInnerpaddingOperator(am);
			argOp.addOperator("--innerpadding", stablize_grab(IArguOperator, AtlasInnerpaddingOperator, innerpadding));

			// order type operator
			grab(AtlasOrdertypeOperator) sort = new AtlasOrdertypeOperator(am);
			argOp.addOperator("--sort", stablize_grab(IArguOperator, AtlasOrdertypeOperator, sort));

			// notrim operator
			grab(AtlasOrderrevertOperator) revert = new AtlasOrderrevertOperator(am);
			argOp.addOperator("--revert", stablize_grab(IArguOperator, AtlasOrderrevertOperator, revert), 0);
			
			argOp.run(args);
			am->merge();
			
			grab(Image) atlas = am->genAtlas();
			ImageFactory::shared()->saveFile("resource/aaa.png", ImageFormat_PNG, atlas);
		}
	};

	CLS_HANDLER_RUN(AtlasInputHandler);

	/*
	class AddHandlerRun : public IRunnable
	{
	public:
		void run(){
			grab(AtlasInputHandler) object = new AtlasInputHandler();
			Runtime::shared()->addInputHandler(stablize_grab(IInputHandler, AtlasInputHandler, object));
		}
	};

	static StaticRun<AddHandlerRun> _var;
	*/
	
}


