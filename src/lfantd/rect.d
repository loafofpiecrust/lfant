module lfant.rect;

struct Rect(T)
{	
	this(T x, T y, T w, T h)
	{
		this.x = x;
		this.y = y;
		this.w = w;
		this.h = h;
	}
	
	union {T x, left;}
	union {T y, top;}
	union {T w, right;}
	union {T h, bottom;}
}

alias Rect!int IRect;
alias Rect!uint URect;