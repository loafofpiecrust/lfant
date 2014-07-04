module lfant.properties;

import std.container;
import std.variant;
import std.conv;
import std.string;
import std.algorithm;
import std.array;
import std.stdio;

class Properties
{
	enum Mode : ubyte
	{
		Input,
		Output
	}

	enum SaveMode : ubyte
	{
		Value,
		Pointer
	}

	this()
	{
	}

	this(string path)
	{

	}

	this(Properties parent, string type, string name)
	{

	}

	~this()
	{
	}

	void saveFile(string path)
	{

	}
	void loadFile(string path)
	{

	}

	Properties getChild(int idx)
	{
		return null;
	}

//	void LoadFileYaml(string path);

	void LoadStream(File stream)
	{
		bool isOperator(in char c)
		{
			return c == '=' || c == ':';
		}
		
		bool isWhitespace(in char c)
		{
			return c == ' ' || c == '\t';
		}
		
		string trimSpace(in string str, bool onlyIndent)
		{
			if(str.empty) return "";
			
			string result;
			uint firstLetter = 0;
			for(uint i = 0; i < str.length; ++i)
			{
				if(!isWhitespace(str[i]) && str[i] != '\n')
				{
					firstLetter = i;
					break;
				}
			}
			result = str[firstLetter .. $];
			
			if(onlyIndent)
			{
				return result;
			}
			
			uint lastLetter = 0;
			for(uint i = 0; i < result.length; ++i)
			{
				if(isWhitespace(str[i]) || str[i] == '\n')
				{
					lastLetter = i;
					break;
				}
			}
			return result[0 .. lastLetter];
		}
		
		mode = Mode.Output;

		char curr = '\0';
		Properties obj = this;
	
		string token = ""; // current token
		string stype = "";
		string sname = "";
		string value = "";
		while(!stream.eof())
		{
			curr = stream.getc();
			if(curr == '/')
			{
				curr = stream.getc();
				if(curr == '/')
				{
					while(curr != '\n')
					{
						curr = stream.getc();
					}
				}
				else if(curr == '*')
				{
					while(!stream.eof())
					{
						curr = stream.getc();
						if(curr == '*')
						{
							curr = stream.getc();
							if(curr == '/')
							{
								curr = stream.getc();
								break;
							}
						}
					}
				}
				else
				{
					stream.ungetc(curr);
				}
			}
			if(isWhitespace(curr) || curr == '\n' || isOperator(curr))
			{
				// process token
				if(!token.empty())
				{
					if(token == "{")
					{
						stype = stype.toLower();
						stype.removechars(" ");
						sname.removechars(" ");
					//	writeln(GetIndent(this)~"new structure named '"~stype~" "~sname~"' in ", obj);
						obj = new Properties(obj, stype, sname);
						obj.mode = mode;
						obj.parent.children ~= obj;
						stype = "";
						sname = "";
					}
					else if(token == "}")
					{
						if(obj.parent) obj = obj.parent;
					}
					else if(stype.empty)
					{
						stype = token;
					}
					else if(sname.empty)
					{
						sname = token;
					}
	
					// clear token
					token = "";
	
				}
				if(curr == '=' || curr == ':')
				{
				//	oper = curr;
					value = "";
	
					bool lined = false;
					curr = stream.getc();
					while(!stream.eof())
					{
						if(curr == '{')
							lined = true;
						else if(curr == '}')
							lined = false;
	
						if(!lined && (curr == ';' || curr == '\n'))
						{
							break;
						}
	
						value ~= curr;
						curr = stream.getc();
					}
					value = trimSpace(value, false);
					if(!value.empty) obj.value(stype, value);
	
					stype = "";
					value = "";
					sname = "";
				}
			}
			else
			{
				token ~= curr;
			}
		}
		return;
	}
//	void SaveStream(std::ostream& stream);

	void value(T)(in string name, ref T value)
	{
		if(mode == Mode.Output)
		{
			if(saveMode == SaveMode.Value)
				values[name] = value;
			else
				values[name] = &value;
		}
		else
		{
			auto iter = (name in values);
			if(iter !is null)
			{
				if(iter.convertsTo!(string))
				{
					value = to!(T)(iter.get!(string));
				}
				else
				{
					if(saveMode == SaveMode.Value)
						value = iter.get!(T);
					else
						value = *(iter.get!(T*));
				}
			}
		}
	}
	
	T value(T)(in string name)
	{
		auto iter = (name in values);
		if(iter !is null)
		{
			if(iter.convertsTo!(string))
			{
				return to!(T)(iter.get!(string));
			}
			else
			{
				return iter.get!(T);
			}
		}
	}

	Properties child(string type, in string name)
	{
		if(mode == Mode.Input)
		{
			type = type.toLower();
			foreach(child; children)
			{
				if(child.type == type && (name.length == 0 || child.name == name))
				{
					return child;
				}
			}
			return null;
		}
		else
		{
			Properties prop = new Properties(this, type, name);
			children ~= prop;
			return prop;
		}
	}

	@property
	{
		Mode mode() const { return _mode; }
		void mode(in Mode m)
		{
			_mode = m;
			foreach(child; children)
			{
				child.mode = m;
			}
		}
	
		SaveMode saveMode() const { return _saveMode; }
		void saveMode(in SaveMode m)
		{
			_saveMode = m;
			foreach(child; children)
			{
				child.saveMode = m;
			}
		}
	}

	string type;
	string name;
	Variant[string] values;
//	SList!(Properties) children;
//	Array!(Properties*) children;
	Properties[] children;

private:
	Properties parent;

	// props
	Mode _mode = Mode.Input;
	SaveMode _saveMode = SaveMode.Value;
}