
module lfant.game;

import std.path;
import std.file;
import std.stdio;
import std.typecons;
//import std.stream;

import lfant.gameobject;
import lfant.properties;
import lfant.time;
import lfant.window;
import lfant.scene;
import lfant.renderer;
import lfant.input;

public
{
	import lfant.subsystem;
}

class Game : GameObject
{
public:
	this()
	{	
		window = new Window(this);
		scene = new Scene(this);
		renderer = new Renderer(this);
		input = new Input(this);
		time = new Time(this);
	}
	~this()
	{
	//	this.log("game");
		writeln("game");
	}
	
	override void initialize()
	{
		// open log file
		// clear log file		
		time.initialize();
		
		logFile.open(logName, "w");
	//	logFile.clear();
		
		loadFile("settings/game.prop");
		
		if(standAlone) window.initialize();
		window.title = orgName~"."~gameName;
		renderer.initialize();
		input.initialize();
	//	physics.initialize();
	//	network.initialize();
	//	scriptSystem.initialize();
	//	if(standAlone) userInterface.initialize();
		
		scene.initialize();
		
		this.log("Game initialized.");
	}
	
	override void update(in float delta = 0.0f)
	{
		scene.update(time.deltaTime);
		time.update();
		input.update(time.deltaTimeFixed);
		if(standAlone)
		{
			window.update(time.deltaTimeFixed);
			renderer.render();
		}
	}
	
	override void deinit()
	{
		scene.deinit(); //scene.reset();
		this.log("Scene destroyed.");
	//	openCL.deinit();
	//	audio.deinit();
	//	userInterface.deinit();
	//	network.deinit();
	//	physics.deinit();
		input.deinit();
		this.log("Input deinit.");
		renderer.deinit();
		if(standAlone) window.deinit();
	//	systemInfo.deinit();
		time.deinit();
		
		this.log("game deinited");
		logFile.close();
	}
	
	override void serialize(Properties prop)
	{
		prop.value("orgName", orgName);
		prop.value("gameName", gameName);
		prop.value("defaultScene", defaultScene);
		prop.value("logFile", logName);
	}
	
	void exit()
	{
		exited = true;
	}

	void log(T...)(T args)
	{
		writeln(args);
	//	logFile.writeln(args);
	}

	string getAssetPath(string path)
	{
		string result = gameFolder~"/assets/"~path;
		if(!exists(result))
		{
			this.log("File "~result~" not found.");
		}
		return result;
	}
	
	@property
	{
		override Game game()
		{
			return this;
		}
	}

	bool standAlone = true;

	string orgName = "piecrust";
	string gameName = "lfant";
	string defaultScene;
	
/*	Unique!(Time) time;
	Unique!(Window) window;
	Unique!(Scene) scene;
	Unique!(Renderer) renderer;*/
//	Unique!(Input) input = new Input(null);
	Time time;
	Window window;
	Scene scene;
	Renderer renderer;
	Input input;
	
	bool exited = false;
	
private:
	string gameFolder = "../../..";
	string userFolder;
	
	string logName = "lfant.log";
	std.stdio.File logFile;
	
}