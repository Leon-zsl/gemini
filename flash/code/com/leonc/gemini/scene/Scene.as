package com.leonc.gemini.scene
{
import com.leonc.gemini.entity.Entity;	

public class Scene
{
	private var _entities:Dictionary = new Dictionary();

	public function startup():void
	{
		//init entities
	}

	public function shutdown():void
	{
		for each(var ent:Entity in _entities) {
			ent.shutdown();
		}
	}

	public function update():void
	{
		for each(var ent:Entity in _entities) {
			ent.update();
		}
	}

	public function getEntity(name:String):Entity
	{
		return _entities[name];
	}

	public function addEntity(name:String, ent:Entity):void
	{
		_entities[name] = ent;
	}

	public function removeEntity(name:String):void
	{
		delete _entities[name];
	}

	public function clearEntity():void
	{
		//clear dic
	}
}
}