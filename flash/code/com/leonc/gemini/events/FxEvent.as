//特效所需的事件

package com.scaleform.std.events 
{
    
    import flash.events.Event;
    
    public class FxEvent extends Event 
	{

        public static const CREATED:String  = "fxCreated";
        public static const COMPLETE:String = "fxComplete";
        public static const LAYER_FX:uint = 0;
        public static const LAYER_BOTTOM:uint = 1;

        /** An override in case this Fx is on a layer other than the Fx layer. */
        public var layer:uint = 0;

        public function FxEvent(type:String, bubbles:Boolean = true, cancelable:Boolean = true,
                                 layer:uint = 0) 
		{
            super( type, bubbles, cancelable );
            this.layer = layer;
        }

        override public function clone():Event
		{
            return new FxEvent( type, bubbles, cancelable );
        }
        
        override public function toString():String 
		{
            return formatToString( "FxEvent", "type", "bubbles", "cancelable" );
        }

    }
    
}