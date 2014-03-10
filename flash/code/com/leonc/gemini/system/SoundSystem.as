package com.leonc.gemini.system
{
	import flash.display.Stage;
    import flash.media.Sound;
    import flash.media.SoundChannel;
    import flash.media.SoundLoaderContext;
	import flash.media.SoundTransform;
    import flash.utils.Dictionary;
    import flash.net.URLRequest;
    import flash.media.SoundMixer;
    import flash.events.Event;
	
	public class SoundSystem 
    {
		public static const SOUND_MUSIC:uint = 0;
		public static const SOUND_FX:uint = 1;
		
		public static const MUSIC_VOLUME:Number = 1;
		public static const FX_VOLUME:Number = 1;
		
		public static var isMusicMuted:Boolean = false;
		public static var isSoundFxMuted:Boolean = false;
		
		protected static var _initialized:Boolean = false;
		protected static var _stage:Stage = null;
		protected static var _loadedSounds:Dictionary = null;
		protected static var _soundChannels:Dictionary = null;
		protected static var _loopSound:Sound = null;
		
		protected static var _bgVolume:SoundTransform = null;
		protected static var _fgVolume:SoundTransform = null;
		protected static var _muteVolume:SoundTransform = null;
		
		
		public function SoundSystem()
		{
			
		}
		
		public static function init(stageRef:Stage):void
		{
			if ( _initialized ) { return; }
            
            _stage = stageRef;
            
            _loadedSounds = new Dictionary( true );
            _soundChannels = new Dictionary(true);
            
			_bgVolume = new SoundTransform(MUSIC_VOLUME, 0);
			_fgVolume = new SoundTransform(FX_VOLUME, 0);
			_muteVolume = new SoundTransform(0, 0);
			
			_soundChannels[SOUND_MUSIC] = new SoundChannel;
			_soundChannels[SOUND_FX] = new SoundChannel;
			
            _initialized = true;
		}
		
		public static function destroy():void
		{
			for ( var sKey:Object  in _loadedSounds ) {
                _loadedSounds[ sKey ] = null;
            }
            for ( var i:uint = 0; i < _soundChannels.length; i++) {
                _soundChannels[ i ] = null;
            }
		}
		
		public static function loasSound(soundName:String):void
		{
			var sound:Sound = _loadedSounds[ soundName ];
            if ( sound == null ) {
                sound = new Sound();
                sound.load( new URLRequest( "audio/" + soundName ));
				trace("trying to load 'audio/" + soundName + "'");
                if ( sound.bytesLoaded == 0 ) { return; }
                _loadedSounds[ soundName ] = sound;
            }
		}
		
		public static function playSound(soundType:uint, soundName:String, loop:Boolean = false, completeEvtCallback:Function = null):void
		{
			var sound:Sound = _loadedSounds[ soundName ];
            if ( sound == null ) {
                sound = new Sound();
                sound.load( new URLRequest( "audio/" + soundName ) );
				trace("trying to play 'audio/" + soundName + "'");
                if ( sound.bytesLoaded == 0 ) { return; }
                _loadedSounds[ soundName ] = sound;
            }
            
			var channel:SoundChannel = _soundChannels[soundType];
			if (channel != null && soundType == SOUND_MUSIC) {
				channel.stop();
			}
			
			
			
			channel = sound.play();
			
			_soundChannels[soundName] = channel;
			
			_soundChannels[soundType] = channel;
			channel.soundTransform = _muteVolume;
			if (soundType == SOUND_MUSIC ) { 
				channel.soundTransform = !isMusicMuted ? _bgVolume : _muteVolume; 
			} else { 
				channel.soundTransform = !isSoundFxMuted ? _fgVolume : _muteVolume; 
			}
            
            if ( soundType == SOUND_MUSIC && loop ) {
                _loopSound = sound;
                channel.addEventListener( Event.SOUND_COMPLETE, loopBackground, false, 0, true );
            }
            
			if (completeEvtCallback != null) {
                channel.addEventListener( Event.SOUND_COMPLETE, completeEvtCallback, false, 0, true );
            }
		}
		
		public static function loopBackground():void
		{
			var channel:SoundChannel = _loopSound.play();
			_soundChannels[SOUND_MUSIC] = channel;
			channel.soundTransform = !isMusicMuted ? _bgVolume : _muteVolume;
            channel.addEventListener( Event.SOUND_COMPLETE, loopBackground, false, 0, true );
		}
		
		public static function stopSound(soundName:String):void
		{
			var sound:Sound = _loadedSounds[ soundName ];
			if (sound != null) {
				(_soundChannels[soundName] as SoundChannel).stop();
			}
		}
		
		public static function muteMusic():void
		{
			isMusicMuted = true;
            _soundChannels[SOUND_MUSIC].soundTransform = _muteVolume;
		}
		
		public static function muteSoundFx():void
		{
			isSoundFxMuted = true;
            _soundChannels[SOUND_FX].soundTransform = _muteVolume;
		}
		
		public static function unmuteMusic():void
		{
			isMusicMuted = false;
            _soundChannels[SOUND_MUSIC].soundTransform = _bgVolume;
		}
		
		public static function unmuteSoundFx():void
		{
			isSoundFxMuted = false;
            _soundChannels[SOUND_FX].soundTransform = _fgVolume;
		}
	}
}





