package com.leonc.gemini 
{
import com.leonc.gemini.scene.Scene;
import com.leonc.gemini.gui.GUIFrameMgr;
import com.leonc.gemini.state.GameStateMgr;
import com.leonc.gemini.subsystem.SubSystemMgr;

public class Game 
{
	private var _scene:Scene = new Scene();
	private var _frameMgr:GUIFrameMgr = new GUIFrameMgr();
	private var _subSystemMgr:SubSystemMgr = new SubSystemMgr();
	private var _stateMgr:GameStateMgr = new GameStateMgr();

	public function get scene():Scene { return _scene; }
	public function get frameMgr():GUIFrameMgr { return _frameMgr; }
	public function get subSystemMgr():SubSystemMgr { return _subSystemMgr; }
	public function get stateMgr():GameStateMgr { return _stateMgr; }

	public function startup():void
	{
		_scene.startup();
		_subSystemMgr.startup();
		_frameMgr.startup();
		_stateMgr.startup();

		enterDefaultState();
	}

	public function shutdown():void
	{
		_stateMgr.shutdown();
		_frameMgr.shutdown();
		_subSystemMgr.shutdown();
		_scene.shutdown();
	}

	public function update():void
	{
		_scene.update();
		_subSystemMgr.update();
		_frameMgr.update();
		_stateMgr.update();
	}

	private function enterDefaultState():void
	{
		//todo:
	}
}
}