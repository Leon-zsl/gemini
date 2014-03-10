package com.leonc.gemini.event
{
    import flash.events.Event;
    import flash.geom.Point;
    
    public class LoadingEvent extends Event 
	{
        public static const LOAD_START:String = "loadStart";
        public static const LOAD_COMPLETE:String = "loadComplete";
        public static const LOAD_OPEN_COMPLETE:String = "loadOpenComplete";
        public static const LOAD_CLOSE_COMPLETE:String = "loadCloseComplete";

        public function LoadingEvent(  type:String, bubbles:Boolean = true, cancelable:Boolean = true) 
        {
            super(type, bubbles, cancelable);
        }

        override public function clone():Event 
		{
            return new LoadingEvent(type, bubbles, cancelable);
        }
        
        override public function toString():String 
		{
            return formatToString("LoadingEvent", "type", "bubbles", "cancelable");
        }

    }
    
}