package com.leonc.gemini.state
{
import com.leon.gemini.state.GameState;

public class GameStateMgr
{
	private var _activeState:GameState;
	private var _states:Dictionary = new Dictionary();

	public function get activeState():GameState { return _activeState; }

	public function startup():void
	{
		//init game states
	}

	public function shutdown():void
	{
		_activeState.onExit();

		for each(var ss:GameState in _states) {
			ss.shutdown();
		}
	}

	public function update():void
	{
		_activeState.update();
	}

	public function addState(name:String, st:GameState):void
	{
	}

	public function removeState(name:String):void
	{
	}

	public function getState(name:String):void
	{
	}

	public function clearState():void
	{
	}
	
	public function enterState(name:String):void
	{
		_activeState.onExit();
		_activeState = _states[name];
		_activeState.onEnter();
	}
}
}