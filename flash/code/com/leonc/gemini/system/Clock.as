package com.leonc.gemini.system
{
public class Clock
{
	private var _frameTime:Number;
	private var _deltaTime:Number;

	public function get frameTime():Number { return _frameTime; }
	public function get deltaTime():Number { return _deltaTime; }

	public void startup():void
	{
	}

	public void shutdown():void
	{
	}

	public void update():void
	{
	}
}
}