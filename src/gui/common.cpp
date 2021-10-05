#include "common.h"
#include "../jwin.h"

namespace GUI
{

int scrollProc(int msg, DIALOG* d, int c);

void findScrollingPane(DIALOG* d, DIALOG** sp, int* index)
{
	int i=0;
	do
	{
		--d;
		++i;
	} while(d->proc!=scrollProc);
	*sp=d;
	*index=i;
}

int newGUIProcImpl(int msg, DIALOG* d, int c, int (*base)(int, DIALOG*, int))
{
	if(d->flags&D_SCROLLING)
	{
		// This widget is in a scrolling pane and needs some special handling.
		DIALOG* sp;
		int index;

		switch(msg)
		{
		case MSG_WHEEL:
			if(base(MSG_WANTWHEEL, d, 0))
				break;
			findScrollingPane(d, &sp, &index);
			return sp->proc(msg, sp, c);

		case MSG_GOTFOCUS:
			// This will be followed by MSG_DRAW...
			d->flags |= D_NEEDSCLIPPED;
			[[fallthrough]];

		case MSG_CLICK:
		{
			findScrollingPane(d, &sp, &index);
			int ret=sp->proc(MSG_CHILDFOCUSED, sp, index); // MSG_CHILDFOCUSED
			return base(msg, d, c)|ret;                    // should be first
		}

		case MSG_LOSTFOCUS:
			d->flags |= D_NEEDSCLIPPED;
			break;

		case MSG_DRAW:
			if(d->flags&D_NEEDSCLIPPED)
			{
				findScrollingPane(d, &sp, &index);
				return sp->proc(MSG_DRAWCLIPPED, sp, index);
			}
			break;
		}
	}

	return base(msg, d, c);
}

int getAccelKey(const std::string_view text)
{
	bool lastWasAmpersand = false;
	for(auto& c: text)
	{
		if(c == '&')
		{
			if(lastWasAmpersand)
				lastWasAmpersand = false;
			else
				lastWasAmpersand = true;
		}
		else if(lastWasAmpersand)
			return c;
	}
	return 0;
}

}
