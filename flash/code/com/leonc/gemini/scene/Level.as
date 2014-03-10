package com.leonc.gemini.level
{
	import flash.display.MovieClip;
    import flash.display.Sprite;
    import flash.display.DisplayObject;
    import flash.events.Event;
    import flash.events.MouseEvent
    import flash.events.TouchEvent;
    import flash.geom.Point;
    import flash.geom.Rectangle;
	import flash.net.URLLoader;
	import flash.media.Sound;
    import flash.media.SoundChannel;
    import flash.net.URLRequest;

	import fl.transitions.easing.*;

	import scaleform.clik.motion.Tween;
    import scaleform.gfx.Extensions;

	import com.leonc.gemini.util.LaneGenerator;
	import com.leonc.gemini.util.Constants;
    import com.leonc.gemini.util.Util;
    import com.leonc.gemini.util.Layer;
	
	public class Level extends MovieClip
	{
		protected var _xmlPath:String;
		public var paused:Boolean = true;
		
		public function Level(pathToXml:String)
		{
			super();
			_xmlPath = pathToXml;
			Util.resetIds();
			
			addEventListener( Event.ENTER_FRAME, configUI, false, 0, true );
		}
		
		public function configUI(e:Event):void
		{
			removeEventListener( Event.ENTER_FRAME, configUI, false );
		}
		
		protected function loadLevelData():void
		{
			var xmlLoader:URLLoader = new URLLoader();
			xmlLoader.addEventListener(Event.COMPLETE, processLevelData);
			xmlLoader.load(new URLRequest(_xmlPath));
		}
		
		protected function processLevelData(e:Event):void 
		{
			configLevel();
		}
		
		public function configLevel():void 
		{
			
		}
		
		protected function handleLevelComplete():void 
		{
			
		}
		
		public function tick():void
		{
			if ( paused ) { return; }
		}
	}
}