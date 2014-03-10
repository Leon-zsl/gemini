package com.leonc.gemini.event
{
	import flash.events.Event;
	
	public class GameEvent extends Event 
	{
		public static const INIT_GAME:String = "initGame";
        public static const PAUSE_GAME:String = "systemPauseGame";
        public static const RESUME_GAME:String = "systemResumeGame";
        
        public static const LAUNCH_MISSION:String = "gameLaunchMission";
        public static const OPEN_MAIN_MENU:String = "gameOpenMainMenu";
        
        public static const SHOW_MENU_TUTORIAL:String = "showMenuTutorial";
        public static const SHOW_GAME_TUTORIAL:String = "showGameTutorial";
        public static const SHOW_ABOUT_DIALOG:String = "showAboutDialog";
		
		public function GameEvent(type:String, bubbles:Boolean = true, cancelable:Boolean = true)
		{
			super( type, bubbles, cancelable );
		}
		
		override public function clone():Event {
            return new GameEvent( type, bubbles, cancelable );
        }
        
        override public function toString():String {
            return formatToString( "GameEvent", "type", "bubbles", "cancelable" );
        }
		
	}
}