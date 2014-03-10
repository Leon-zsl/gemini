package com.leonc.gemini.subsystem
{
public class SubSystemMgr
{
	private var _subSystems:Dictionary = new Dictionary();

	public function startup():void
	{
		//init sub sys
	}

	public function shutdown():void
	{
		for each(var ss:SubSystem in _subSystems) {
			ss.shutdown();
		}
	}

	public function update():void
	{
		for each(var ss:SubSystem in _subSystems) {
			ss.update();
		}
	}

	public function addSubSystem(name:String, ss:SubSystem):void
	{
	}

	public function removeSubSystem(name:String):void
	{
	}

	public function getSubSystem(name:String):SubSystem
	{
	}

	public function clearSubSystem():void
	{
	}
}
}