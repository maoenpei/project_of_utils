

#include "Naran.h"
#include "plugins/atlas_merge/AtlasMerge.h"

CLS_SHARE_MEMORY();

NS_DEF_NARAN{

	class AtlasInputHandler : public IInputHandler
	{
	public:
		virtual void run(arr(char *) args)
		{
			if (args.size() > 0){
				grab(Path) path = Path::create(args[0]);
				grab(Directory) dirs = Directory::create(path);
				arr(DirProps) children = dirs->enumChildren();
				grab(AtlasMerge) am = AtlasMerge::create();
				for (int i = children.size()-1; i>=0; i--){
					if (! children[i].isDir){
						grab(Image) img = ImageFactory::shared()->loadFile(children[i].path->getChars().get());
						am->setImage(children[i].path->getChars().get(), img);
					}
				}
				am->merge();
			}
		}
	};

	class AddHandlerRun : public IRunnable
	{
	public:
		void run(){
			grab(AtlasInputHandler) object = new AtlasInputHandler();
			Runtime::shared()->addInputHandler(stablize_grab(IInputHandler, AtlasInputHandler, object));
		}
	};

	static StaticRun<AddHandlerRun> _var;

}


