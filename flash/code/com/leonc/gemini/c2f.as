package com.leonc.gemini {
	
	import flash.display.Stage;

	import com.leonc.gemini.Main;

	public class C2F {
		private var _root:Main;

		public function init(m:Main):void {
			_root = m;
		}

		public function get root():Main { return _root; }
		public function get app():App { return _root.app; }

		public function get packetDispathcer():PacketDispatcher
		{ return _app.packetDispatcher; }
		
		public function get assetLoader():AssetManager
		{ return _assetLoader; }	
	}
}