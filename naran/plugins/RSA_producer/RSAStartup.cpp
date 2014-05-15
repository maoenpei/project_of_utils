

#include "Naran.h"

CLS_SHARE_MEMORY();

NS_DEF_NARAN{

	class RSAInputHandler : public IInputHandler
	{
	public:
		virtual void run(arr(char *) args)
		{
			if (0 == strcmp(args[0], "-Key")){
				if (args.size() < 4){
					printf("please input 2 prime number and 1 private key!\n");
					return;
				}
				int prime1 = atoi(args[1]);
				int prime2 = atoi(args[2]);
				int key = atoi(args[3]);
				grab(RSAProvider) provider = RSAProvider::create(prime1, prime2, key);
				int mod = provider->getMod();
				int okey = provider->getOKey();
				printf("RSA big number: %d, public key: %d\n", mod, okey);
			}
		}
	};

	class AddHandlerRun : public IRunnable
	{
	public:
		void run(){
			grab(RSAInputHandler) object = new RSAInputHandler();
			Runtime::shared()->addInputHandler(stablize_grab(IInputHandler, RSAInputHandler, object));
		}
	};

	static StaticRun<AddHandlerRun> _var;

}

