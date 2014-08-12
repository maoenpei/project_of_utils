

#include "MergeAlgorithm.h"
#include "FillTesting.h"

NS_DEF_NARAN{

#define MAX_CACHE		4
#define MAX_CALCU		(65535)

	static inline int getCacheCountOfIndex(int index, int count)
	{
		if (index == 0){
			return 1;
		}
		if (count <= 7){
			return 4;
		}else if (count <= 14){
			return (index <= 1 ? 4 : (index <= 3 ? 3 : (index <= 7 ? 2 : 1)));
		}else if (count <= 23){
			return (index <= 2 ? 3 : (index <= 4 ? 2 : 1));
		}else{
			return (index <= 2 ? 2 : 1);
		}
	}

	struct ConfigData{
		g2d::Sizei maxsize;
		int borderpadding;
		int innerpadding;
		bool pot;
		bool trim;
		bool square;
		bool allowrotation;
	};

	struct PutPosition
	{
		g2d::Sizei base_size;
		g2d::Recti valid_rect;
		g2d::Sizei last;
		int area;
		int index;
		PutPosition() : area(0){}
	};

	struct FillResult
	{
		int count;
		int area;
		g2d::Sizei last;
		FillResult() : count(0), area(0) {}
	};

#define NEXT_POT(X)		\
	{\
		X = (X) - 1;\
		X = (X) | ((X) >> 1);\
		X = (X) | ((X) >> 2);\
		X = (X) | ((X) >> 4);\
		X = (X) | ((X) >> 8);\
		X = (X) | ((X) >> 16);\
		X = (X) + 1;\
	}

	static inline void dataFromConfig(ConfigData *cdata, grab(Configuration) conf)
	{
		conf->get("maxsize", cdata->maxsize);
		conf->get("borderpadding", cdata->borderpadding);
		conf->get("innerpadding", cdata->innerpadding);
		conf->get("pot", cdata->pot);
		conf->get("trim", cdata->trim);
		conf->get("square", cdata->square);
		conf->get("allowrotation", cdata->allowrotation);
	}

	static inline bool checkNextPoint(grab(FillTesting) fill, const g2d::Pointi &pt, ConfigData *cdata)
	{
		const g2d::Sizei &size = fill->getSize();
		if (pt.x >= size.w - cdata->borderpadding || pt.y >= size.h - cdata->borderpadding){
			return false;
		}
		u32 cross = fill->testCross(pt);
		return ((pt.x == cdata->borderpadding || (cross & AtlasTestingCross_Left)) 
			&& (pt.y == cdata->borderpadding || (cross & AtlasTestingCross_Down)));
	}

	static inline void insertPossibleRect(PutPosition possibles[], int count, 
		const g2d::Sizei &size, const g2d::Recti &rect, const g2d::Sizei &last, int index, ConfigData *cdata)
	{
		g2d::Pointi far = rect.getFar();
		int lastW = (far.x > last.w ? far.x : last.w);
		int lastH = (far.y > last.h ? far.y : last.h);
		if (cdata->pot){
			NEXT_POT(lastW);
			NEXT_POT(lastH);
		}
		if (cdata->square){
			lastW = lastH = (lastW > lastH ? lastW : lastH);
		}
		int area = lastW * lastH;
		for (int i = 0; i<count; i++){
			if (possibles[i].area == 0 || possibles[i].area > area){
				memmove(possibles + i + 1, possibles + i, (MAX_CACHE - i - 1) * sizeof(PutPosition));
				possibles[i].area = area;
				possibles[i].base_size = size;
				possibles[i].index = index;
				possibles[i].last = g2d::Sizei(lastW, lastH);
				possibles[i].valid_rect = rect;
				break;
			}
		}
	}

	static inline void checkValidResult(int count, arr(ImageRectMerge *) objects, 
		Array<g2d::Recti> &rects, const g2d::Sizei &last, FillResult &fr, ConfigData *cdata)
	{
		if (count < fr.count)
			return;
		int area = last.w * last.h;
		if (count == fr.count && fr.area != 0 && area > fr.area)
			return;
		fr.count = count;
		fr.area = area;
		fr.last = last;
		for (int i = 0; i<count; i++){
			objects[i]->rect = rects[i];
			objects[i]->markUsed = true;
		}
	}

	static inline bool fillObjects(int index, arr(ImageRectMerge *) objects, 
		Array<g2d::Pointi> &candidates, grab(FillTesting) fill, Array<g2d::Recti> &rects, 
		const g2d::Sizei &last, FillResult &fr, ConfigData *cdata)
	{
		if (index == objects.count()){
			checkValidResult(index, objects, rects, last, fr, cdata);
			return true;
		}
		
		int count = getCacheCountOfIndex(index, objects.count());
		
		// find all possible positions
		// greed the former MAX_CACHE ones
		PutPosition possibles[MAX_CACHE] = {};
		const g2d::Sizei &size = objects[index]->size;
		for (int i = 0; i<candidates.count(); i++){
			g2d::Recti rect(candidates[i].x, candidates[i].y, 
				size.w + cdata->borderpadding, size.h + cdata->borderpadding);
			if (fill->testRect(rect)){
				insertPossibleRect(possibles, count, size, rect, last, i, cdata);
			}
			g2d::Recti r_rect(candidates[i].x, candidates[i].y, 
				size.h + cdata->borderpadding, size.w + cdata->borderpadding);
			if (size.w != size.h && cdata->allowrotation && fill->testRect(r_rect)){
				insertPossibleRect(possibles, count, g2d::Sizei(size.h, size.w), r_rect, last, i, cdata);
			}
		}
		
		// if no position can put this object, generate result
		if (possibles[0].area == 0){
			checkValidResult(index, objects, rects, last, fr, cdata);
			return false;
		}
		
		// put all possible objects in to the list
		bool ret = false;
		for (int i = 0; i<count; i++){
			if (possibles[i].area == 0){
				break;
			}
			int old_count = candidates.count();
			g2d::Pointi pt = candidates[possibles[i].index];
			candidates.remove(possibles[i].index);
			g2d::Recti max_rect(
				possibles[i].valid_rect.pt.x, possibles[i].valid_rect.pt.y, 
				possibles[i].base_size.w + cdata->innerpadding, possibles[i].base_size.h + cdata->innerpadding);
			fill->cutRect(max_rect);
			fill->fillRect(max_rect);
			g2d::Pointi pt1(max_rect.pt.x + max_rect.size.w, max_rect.pt.y);
			g2d::Pointi pt2(max_rect.pt.x, max_rect.pt.y + max_rect.size.h);
			if (checkNextPoint(fill, pt1, cdata)){
				candidates.insert(pt1);
			}
			if (checkNextPoint(fill, pt2, cdata)){
				candidates.insert(pt2);
			}
			rects.insert(g2d::Recti(possibles[i].valid_rect.pt, possibles[i].base_size));
			if (fillObjects(index + 1, objects, candidates, fill, rects, possibles[i].last, fr, cdata)){
				ret = true;
			}
			rects.remove(-1);
			fill->revert();
			while(candidates.count() >= old_count)
				candidates.remove(-1);
			candidates.insert(pt, possibles[i].index);
		}
		return ret;
	}

	int MergeAlgorithm::merge(arr(ImageRectMerge *) objects, grab(Configuration) conf, g2d::Sizei &size)
	{
		ConfigData cdata = {};
		dataFromConfig(&cdata, conf);
		grab(FillTesting) fill = FillTesting::create(cdata.maxsize);
		
		Array<g2d::Pointi> candidates;
		g2d::Pointi left_top(cdata.borderpadding, cdata.borderpadding);

		if (checkNextPoint(fill, left_top, &cdata)){
			candidates.insert(left_top);
			Array<g2d::Recti> rects;
			FillResult fr;
			bool ret = fillObjects(0, objects, candidates, fill, 
				rects, g2d::Sizei(1+2*cdata.borderpadding, 1+2*cdata.borderpadding), fr, &cdata);
			size = fr.last;
			return fr.count;
		}
		
		return 0;
	}


}

