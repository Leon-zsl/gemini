package com.leonc.gemini.scene
{
import com.leonc.gemini.entity.EntityComponent;

public class Entity 
{
    public var name:String = "";

	public function get name():String { return _name; }
	
	public function startup():void
	{
	}

	public function shutdown():void
	{
	}

	public function update():void
	{
	}

	public function addComponent(name:String, comp:Component):void
	{
	}

	public function removeComponent(name:String):void
	{
	}

	public function getComponent(name:String):void
	{
	}

	public function clearComponent():void
	{
	}

	public function activeCompoent(name:String):void
	{
	}

	public function deactiveComponent(name:String):void
	{
	}
}
}