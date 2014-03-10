//游戏资源加载系统

package com.scaleform.std.system
{
	import flash.display.Stage;
	import flash.display.MovieClip;
    import flash.events.Event;
    import flash.events.MouseEvent;
    import flash.events.TouchEvent;
    import flash.display.StageScaleMode;
    import flash.display.Loader;
    import flash.net.URLRequest;
    import flash.ui.Multitouch;
    import flash.ui.MultitouchInputMode;
	
	import com.scaleform.std.events.LoadingEvent;
	import com.scaleform.std.loading.LoadingView;
	
	public class LoadingSystem
	{

		public static var _loadScreenLoader:Loader = null;
		protected static var _stage:Stage = null;
		
		public function LoadingSystem()
		{
			
		}
		
		public static function init(stageRef:Stage):void
		{
			_stage = stageRef;
			_stage.addEventListener( LoadingEvent.LOAD_COMPLETE, handleLoadComplete, false, 0, true );
            //_stage.addEventListener( LoadingEvent.LOAD_OPEN_COMPLETE, handleLoadOpenComplete, false, 0, true );
            _stage.addEventListener( LoadingEvent.LOAD_CLOSE_COMPLETE, handleLoadCloseComplete, false, 0, true );
		}
		
		public static function destroy():void
		{
			
		}
		
		public static function showLoadingScreen( animate:Boolean = true ):void 
		{
			if ( _loadScreenLoader == null ) { 
                _loadScreenLoader = new Loader();
                _loadScreenLoader.load( new URLRequest( "LoadingView.swf" ) );
            }
            
            if ( animate ) {
                _stage.addChild( _loadScreenLoader );    
            } else {
                _loadScreenLoader.contentLoaderInfo.addEventListener( Event.COMPLETE, handleLoadInit, false, 0, true );
            }
		}
		
		public static function closeLoadingScreen( animate:Boolean = true ):void 
		{
			_stage.dispatchEvent( new LoadingEvent( LoadingEvent.LOAD_COMPLETE ) );
		}
		
		public static function handleLoadComplete( le:LoadingEvent ):void 
		{
            //
        }
		
		// @TODO: Add a Start State at the first launch. Showing the loading screen didn't work too well.
        // @TODO: Probably need a Splash.
        public static function handleLoadInit(e:Event):void 
		{
            _loadScreenLoader.contentLoaderInfo.removeEventListener( Event.COMPLETE, handleLoadInit, false );
            _loadScreenLoader.content["Loading"].gotoAndPlay("on");
            _stage.addChild( _loadScreenLoader );
            _stage.dispatchEvent( new LoadingEvent( LoadingEvent.LOAD_COMPLETE, true, true ) );
        }
		
		public static function handleLoadCloseComplete( le:LoadingEvent ):void 
		{
            _stage.removeChild( _loadScreenLoader );
            _loadScreenLoader.unloadAndStop( true );
            _loadScreenLoader = null;
        }
		
		public static function handleLoadOpenComplete( le:LoadingEvent ):void
		{
			
		}
	}
}