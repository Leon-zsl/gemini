package com.leonc.gemini.entity
{
import com.leonc.gemini.scene.Entity;

public class EntityComponent
{
	private var _name:String;
	private var _owner:Entity;

	public function get owner():Entity { return _owner; }
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

	public function active():void
	{
	}

	public function deactive():void
	{
	}
}
}