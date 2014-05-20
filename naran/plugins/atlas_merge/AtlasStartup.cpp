

#include "Naran.h"
#include "plugins/atlas_merge/AtlasMerge.h"

CLS_SHARE_MEMORY();

NS_DEF_NARAN{

	class AtlasArguOperator : public IArguOperator
	{
	public:
		grab(AtlasMerge) mAtlasMerge;
		AtlasArguOperator(grab(AtlasMerge) am) : mAtlasMerge(am) {}
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
						mAtlasMerge->setImage(path->getChars().get(), img);
						return true;
					}
				} traverse(mAtlasMerge);
				grab(Directory) dirs = Directory::create(path);
				dirs->enumTraverse(stablize_nop(IDirectoryTraverse, AtlasTraverse, &traverse));
			}else{
				grab(Image) img = ImageFactory::shared()->loadFile(path->getChars().get());
				mAtlasMerge->setImage(path->getChars().get(), img);
			}
		}
	};

	class AtlasTrimOperator : public AtlasArguOperator
	{
	public:
		AtlasTrimOperator(grab(AtlasMerge) am) : AtlasArguOperator(am) {}
		void run(const char *argValue){
			if (0 == strcmp(argValue, "true")){
				mAtlasMerge->setTrim(true);
			}else if (0 == strcmp(argValue, "false")){
				mAtlasMerge->setTrim(false);
			}else{
				printf("trim param: true/false allowed!\n");
			}
		}
	};

	class AtlasInputHandler : public IInputHandler
	{
	public:
		virtual void run(arr(char *) args)
		{
			grab(AtlasMerge) am = AtlasMerge::create();
			
			// default operator
			grab(AtlasArguOperator) object = new AtlasArguOperator(am);
			ArguOperation argOp(stablize_grab(IArguOperator, AtlasArguOperator, object));
			
			// trim operator
			grab(AtlasTrimOperator) object1 = new AtlasTrimOperator(am);
			argOp.addOperator("trim", stablize_grab(IArguOperator, AtlasTrimOperator, object1));
			
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


