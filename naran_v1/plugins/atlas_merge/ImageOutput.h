#ifndef __NARAN_IMAGE_OUTPUT_H__
#define __NARAN_IMAGE_OUTPUT_H__

#include "Naran.h"
#include "MergeStage.h"

NS_DEF_NARAN{

	class ImageOutput
	{
	public:
		grab(MergeStage) getMergeState();

		void addImage(const char *path);
		void setImagePath(const char *path);
		void setPlistPath(const char *path);

		bool executeMerge();

		static grab(ImageOutput) shared();

	private:
		CLS_HIDE(ImageOutput);
		
		grab(MergeStage) mMerge;
		Array<more(char)> mPaths;
		more(char) mImagePath;
		more(char) mPlistPath;
	};

}

#endif
