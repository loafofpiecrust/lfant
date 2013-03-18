/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2013-03-16 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License at
*
*		http://www.apache.org/licenses/LICENSE-2.0
*
*	Unless required by applicable law or agreed to in writing, software
*	distributed under the License is distributed on an "AS IS" BASIS,
*	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*	See the License for the specific language governing permissions and
*	limitations under the License.
*
******************************************************************************/

#include <lfant/Properties.h>

// Internal
#include <lfant/Console.h>
#include <lfant/FileSystem.h>
#include <lfant/Range.h>
#include <lfant/util/String.h>

// External

namespace lfant
{

Properties::Properties()
{
}

Properties::Properties(string path)
{
	LoadFile(path);
}

Properties::Properties(ifstream &stream, string type, string id) :
	type(type), id(id)
{
	LoadStream(stream);
}

Properties::~Properties()
{
}

void Properties::LoadFile(string path)
{
	ifstream stream(game->fileSystem->GetGamePath(path).string());
	LoadStream(stream);
}

void Properties::LoadStream(ifstream& stream)
{
	char line[1024];
	int c;
	char* name = "";
	char* value = "";
	char* parentID = "";
	char* rc = "";
	char* rcc = "";
	char* rccc = "";

	while (true)
	{
		SkipWhiteSpace(stream);

		// Stop when we have reached the end of the file.
		if (stream.eof())
		{
			break;
		}

		// Read the next line.
		stream.getline(line, 1024);
		/*
		if (rc == NULL)
		{
			Log("Error reading line from file.");
			return;
		}
		*/

		// Ignore comment, skip line.
		if (strncmp(line, "//", 2) != 0)
		{
			// If an '=' appears on this line, parse it as a name/value pair.
			// Note: strchr() has to be called before strtok(), or a backup of line has to be kept.
			rc = strchr(line, '=');
			if (rc != NULL)
			{
				// There could be a '}' at the end of the line, ending a namespace.
				rc = strchr(line, '}');

				// First token should be the property name.
				name = strtok(line, "=");
				if (name == NULL)
				{
					Log("Error parsing properties file: attribute without name.");
					return;
				}

				// Remove white-space from name.
				name = TrimWhiteSpace(name);

				// Scan for next token, the property's value.
				value = strtok(NULL, "=");
				if (value == NULL)
				{
					Log("Error parsing properties file: attribute with name ('", name, "') but no value.");
					return;
				}

				// Remove white-space from value.
				value = TrimWhiteSpace(value);

				// Store name/value pair.
				values[name] = value;

				if (rc != NULL)
				{
					// End of namespace.
					return;
				}
			}
			else
			{
				parentID = NULL;

				// Get the last character on the line (ignoring whitespace).
				const char* lineEnd = TrimWhiteSpace(line) + (strlen(TrimWhiteSpace(line)) - 1);

				// This line might begin or end a namespace,
				// or it might be a key/value pair without '='.

				// Check for '{' on same line.
				rc = strchr(line, '{');

				// Check for inheritance: ':'
				rcc = strchr(line, ':');

				// Check for '}' on same line.
				rccc = strchr(line, '}');

				// Get the name of the namespace.
				name = strtok(line, " \t\n{");
				name = TrimWhiteSpace(name);
				if (name == NULL)
				{
					Log("Error parsing properties file: failed to determine a valid token for line '", line, "'.");
					return;
				}
				else if (name[0] == '}')
				{
					// End of namespace.
					return;
				}

				// Get its ID if it has one.
				value = strtok(NULL, ":{");
				value = TrimWhiteSpace(value);

				// Get its parent ID if it has one.
				if (rcc != NULL)
				{
					parentID = strtok(NULL, "{");
					parentID = TrimWhiteSpace(parentID);
				}

				if (value != NULL && value[0] == '{')
				{
					// If the namespace ends on this line, seek back to right before the '}' character.
					if (rccc && rccc == lineEnd)
					{
						stream.seekg(-1, ios_base::cur);
						while(stream.get() != '}')
						{
							stream.seekg(-2, ios_base::cur);
						}
						stream.seekg(-1, ios_base::cur);
					}

					// New namespace without an ID.
					Properties* space = new Properties(stream, name);
					children.push_back(ptr<Properties>(space));

					// If the namespace ends on this line, seek to right after the '}' character.
					if (rccc && rccc == lineEnd)
					{
						stream.seekg(1, ios_base::cur);
					}
				}
				else
				{
					// If '{' appears on the same line.
					if (rc != NULL)
					{
						// If the namespace ends on this line, seek back to right before the '}' character.
						if (rccc && rccc == lineEnd)
						{
							stream.seekg(-1, ios_base::cur);
							while(stream.get() != '}')
							{
								stream.seekg(-2, ios_base::cur);
							}
							stream.seekg(-1, ios_base::cur);
						}

						// Create new namespace.
						Properties* space = new Properties(stream, name, value);
						children.push_back(ptr<Properties>(space));

						// If the namespace ends on this line, seek to right after the '}' character.
						if (rccc && rccc == lineEnd)
						{
							stream.seekg(1, ios_base::cur);
						}
					}
					else
					{
						// Find out if the next line starts with "{"
						SkipWhiteSpace(stream);
						c = stream.get();
						if (c == '{')
						{
							// Create new namespace.
							string nv = "";
							if(value) nv = value;
							Properties* space = new Properties(stream, name, nv);
							children.push_back(ptr<Properties>(space));
						}
						else
						{
							// Back up from fgetc()
							stream.seekg(-1, ios_base::cur);

							// Store "name value" as a name/value pair, or even just "name".
							if (value != NULL)
							{
								values[name] = value;
							}
							else
							{
								values[name] = "";
							}
						}
					}
				}
			}
		}
	}
}

void Properties::SaveFile(string path)
{
	path = game->fileSystem->GetGamePath(path).string();
}

Properties* Properties::GetChild(string type)
{
	for(auto& child : children)
	{
		if(child->type == type)
		{
			Log("Properties::GetChild: Got child namespace '"+child->type+"'.");
			return child;
		}
	}
	return nullptr;
}

Properties* Properties::GetChildById(string id)
{
	for(auto& child : children)
	{
		if(child->id == id)
		{
			return child;
		}
	}
	return nullptr;
}

vector<Properties*> Properties::GetChildren(string type)
{
	vector<Properties*> props;
	for(auto& child : children)
	{
		if(child->type == type)
		{
			props.push_back(child);
		}
	}
	return props;
}

void Properties::SkipWhiteSpace(ifstream &stream)
{
	signed char c = stream.get();
	while (isspace(c) && c != EOF)
	{
		c = stream.get();
	}

	// If we are not at the end of the file, then since we found a
	// non-whitespace character, we put the cursor back in front of it.
	if (c != EOF)
	{
		/*
		if (stream.seekg(-1) == false)
		{
			Log("Failed to seek backwards one character after skipping whitespace.");
		}
		*/
		//stream.putback(c);
	//	cout << "Putting back '" << c << "'.\n";
		stream.seekg(-1, ios_base::cur);
	}
}

char* Properties::TrimWhiteSpace(char* str)
{
	if (str == NULL)
	{
		return str;
	}

	char *end;

	// Trim leading space.
	while (isspace(*str))
		++str;

	// All spaces?
	if (*str == 0)
	{
		return str;
	}

	// Trim trailing space.
	end = str + strlen(str) - 1;
	while (end > str && isspace(*end))
	{
		--end;
	}

	// Write new null terminator.
	*(end+1) = 0;

	return str;
}

}
